#include "cmeta.h"
#include "logging.h"

#include <fcntl.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Support/raw_ostream.h>
#include <stdio.h>

#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>

using namespace llvm;

extern "C" int cmeta(const char *file) {
  int fd = -1;
  size_t n = 0;
  char *line = NULL, *curs = NULL;
  size_t line_s = 0;
  FILE *f = NULL;

  std::unordered_map<std::string_view, size_t> line_map;

  f = fopen(file, "r");

  while (getline(&line, &line_s, f) != -1) {
    curs = line;
    while (isspace(*curs))
      curs++;
    if (*curs == '%') {
      n++;
      line_map[std::string_view(line)] = n;
    }
  }

  if (line != NULL)
    free(line);
  fclose(f);

  LLVMContext Ctx;
  SMDiagnostic Err;

  std::unique_ptr<Module> M = parseIRFile(file, Err, Ctx);

  if (M == nullptr) {
    ERRORF("Failed to parse '%s', Error: %s\n", file, Err.getMessage().data());
    return 2;
  }

  DIBuilder DIB(*M);

  DIFile *File = DIB.createFile(file, ".");
  DICompileUnit *CU =
      DIB.createCompileUnit(dwarf::DW_LANG_C, File, "cmeta", false, "", 1);

  for (Function &F : *M) {
    if (F.isDeclaration())
      continue;

    DISubprogram *SP =
        DIB.createFunction(File, F.getName(), F.getName(), File, 1, nullptr, 1);

    F.setSubprogram(SP);

    n = 1;
    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        DILocation *Loc = DILocation::get(Ctx, n++, 0, SP);
        I.setDebugLoc(DebugLoc(Loc));
      }
    }
  }

  DIB.finalize();

  std::error_code EC;
  raw_fd_ostream Out(file, EC);

  if (EC) {
    ERRORF("An error occured writing the file: %s\n", EC.message().data());
    return 3;
  }

  M->print(Out, nullptr);

  return 0;
}
