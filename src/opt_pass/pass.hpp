#ifndef OMPRTF_PASS
#define OMPRTF_PASS

#include <llvm/DebugInfo/DIContext.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <map>
#include <set>
#include <string>
#include <vector>

void replaceLinesWithFiveFunctions(
    const char *file,
    const std::vector<std::unique_ptr<llvm::DILineInfo>> &LineInfos);

#endif
