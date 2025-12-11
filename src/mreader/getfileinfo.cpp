#include "getfileinfo.h"
#include "logging.h"

#include <llvm/DebugInfo/DWARF/DWARFContext.h>
#include <llvm/DebugInfo/DWARF/DWARFDebugLine.h>
#include <llvm/Object/Binary.h>
#include <llvm/Object/ObjectFile.h>
#include <llvm/Support/Error.h>

using namespace llvm;
using namespace llvm::object;

extern "C" void *getfileinfo(const char *file) {
  Expected<OwningBinary<Binary>> BinOrErr = object::createBinary(file);

  if (!BinOrErr) {
    ERRORF("Failed to load binary file '%s': %s\n", file,
           toString(BinOrErr.takeError()).c_str());
    return nullptr;
  }

  object::ObjectFile *Bin =
      llvm::dyn_cast<object::ObjectFile>(BinOrErr->getBinary());

  std::unique_ptr<llvm::DWARFContext> DWARF_File =
      llvm::DWARFContext::create(*Bin);

  for (const std::unique_ptr<llvm::DWARFUnit> &CU :
       DWARF_File->compile_units()) {
    DWARFDie DD = CU->getUnitDIE();
    DD.dump(llvm::outs());

    const DWARFDebugLine::LineTable *line_table =
        DWARF_File->getLineTableForUnit(CU.get());

    if (line_table == NULL) {
      ERRORF("Failed to get line table for Compile Unit '%s'\n", "AHHHH");
      return NULL;
    }

    for (const llvm::DWARFDebugLine::Row &line : line_table->Rows) {
      DEBUGF("Line: %#X: %d\n", line.Line, line.Column);
    }
  }

  return NULL;
}
