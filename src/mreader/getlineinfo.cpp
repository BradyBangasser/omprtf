#include "getfileinfo.h"
#include "logging.h"

#include <llvm/DebugInfo/DWARF/DWARFContext.h>
#include <llvm/DebugInfo/DWARF/DWARFDebugLine.h>
#include <llvm/DebugInfo/Symbolize/Symbolize.h>
#include <llvm/Object/Binary.h>
#include <llvm/Object/ObjectFile.h>
#include <llvm/Support/Error.h>

using namespace llvm;
using namespace llvm::object;

extern "C" void *getlineinfo(const char *file, uint64_t addr) {
  llvm::symbolize::LLVMSymbolizer::Options opts;
  opts.Demangle = true;
  opts.RelativeAddresses = true;

  llvm::symbolize::LLVMSymbolizer Symbolizer(opts);

  llvm::Expected<llvm::DILineInfo> LI = Symbolizer.symbolizeCode(
      file, {addr, llvm::object::SectionedAddress::UndefSection});

  if (!LI) {
    ERRORF("Failed to fetch address %#lX, error: %s\n", addr,
           toString(LI.takeError()).data());
    return NULL;
  }

  LI->dump(outs());

  SUCCESSF("Success (line %d)\n", LI->Line);
  return NULL;
}
