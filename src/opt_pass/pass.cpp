#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include <llvm/IR/Analysis.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>

using namespace llvm;

class CheckPass : public PassInfoMixin<CheckPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    errs() << "Running pass on " << F.getName() << "\n";

    for (BasicBlock &BB : F) {
      errs() << BB.getName() << " block\n";

      for (Instruction &I : BB) {
        errs() << I.getOpcodeName() << "\n";
      }
    }

    return PreservedAnalyses::none();
  }

  static bool isRequired() { return true; }
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
