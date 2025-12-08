#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include <llvm/IR/Analysis.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>

using namespace llvm;

struct CheckPass : public PassInfoMixin<CheckPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    errs() << "Running pass on " << F.getName() << "\n";
    return PreservedAnalyses::none();
  }
};

extern "C" ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
  errs() << "Plugin loaded\n";
  return {LLVM_PLUGIN_API_VERSION, "CheckPass", "1.0", [](PassBuilder &PB) {
            // Register pipeline callback
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "CheckPass") {
                    FPM.addPass(CheckPass());
                    return true;
                  }
                  return false;
                });
          }};
}
