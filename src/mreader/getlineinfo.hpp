#ifndef OMPRTF_GETLINEINFO_H
#define OMPRTF_GETLINEINFO_H

#include <inttypes.h>
#include <llvm/DebugInfo/DWARF/DWARFDebugLine.h>

std::unique_ptr<llvm::DILineInfo> getlineinfo(const char *file, uint64_t addr);

#endif
