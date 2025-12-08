#include <llvm/IR/Analysis.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>

using namespace llvm;

struct CheckPass : public PassInfoMixin<CheckPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    return PreservedAnalyses::none();
  }
};
