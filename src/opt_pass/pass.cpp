#include "pass.hpp"
#include "logging.h"

using namespace llvm;

const std::vector<std::string> FuncNames = {"__omprtf_alloc", "__omprtf_h2d",
                                            "__omprtf_kernel", "__omprtf_d2h",
                                            "__omprtf_free"};

void defineOmprtfReplacementFunctions(std::unique_ptr<Module> &M) {
  LLVMContext &Ctx = M->getContext();
  Type *VoidTy = Type::getVoidTy(Ctx);
  Type *Int32Ty = Type::getInt32Ty(Ctx);
  Type *PtrTy = PointerType::getUnqual(Ctx);

  FunctionType *PrintfTy = FunctionType::get(Int32Ty, {PtrTy}, true);
  FunctionCallee PrintfFunc = M->getOrInsertFunction("printf", PrintfTy);

  for (const std::string &Name : FuncNames) {
    FunctionType *FTy = FunctionType::get(VoidTy, false);
    Function *F = Function::Create(FTy, Function::ExternalLinkage, Name, *M);

    BasicBlock *BB = BasicBlock::Create(Ctx, "entry", F);
    IRBuilder<> Builder(BB);

    std::string Msg = Name;
    Constant *MsgConst = ConstantDataArray::getString(Ctx, Msg);
    GlobalVariable *MsgVar = new GlobalVariable(*M, MsgConst->getType(), true,
                                                GlobalValue::PrivateLinkage,
                                                MsgConst, ".str." + Name);

    Value *MsgPtr =
        Builder.CreateInBoundsGEP(MsgConst->getType(), MsgVar,
                                  {Builder.getInt64(0), Builder.getInt64(0)});
    Builder.CreateCall(PrintfFunc, {MsgPtr});

    // Return
    Builder.CreateRetVoid();
  }
}

void replaceLinesWithFiveFunctions(
    const char *file,
    const std::vector<std::unique_ptr<llvm::DILineInfo>> &LineInfos) {
  std::map<std::string, std::set<uint32_t>> TargetLines;
  for (const auto &Info : LineInfos) {
    if (Info)
      TargetLines[Info->FunctionName].insert(Info->Line);
  }

  LLVMContext Ctx;
  SMDiagnostic Err;

  std::unique_ptr<Module> M = parseIRFile(file, Err, Ctx);

  defineOmprtfReplacementFunctions(M);

  std::vector<Instruction *> toRemove;

  for (Function &F : *M) {
    if (F.isDeclaration() || TargetLines[F.getName().str()].empty())
      continue;

    for (BasicBlock &BB : F) {
      for (auto It = BB.begin(); It != BB.end();) {
        Instruction *I = &*It++; // Advance iterator immediately

        const DebugLoc &DL = I->getDebugLoc();
        if (!DL)
          continue;

        if (CallInst *CI = dyn_cast<CallInst>(I)) {
          Function *CalledF = CI->getCalledFunction();
          if (CalledF && CalledF->getName().contains("__tgt_target_kernel")) {
            IRBuilder<> Builder(CI);

            for (const std::string &Name : FuncNames) {
              Function *Func = M->getFunction(Name);
              Builder.CreateCall(Func);
            }

            toRemove.push_back(CI);

            errs() << "Replaced OpenMP call at " << F.getName() << ":"
                   << DL.getLine() << "\n";
          }
        }
      }
    }
  }

  for (Instruction *I : toRemove) {
    I->eraseFromParent();
  }
  std::error_code EC;
  raw_fd_ostream Out(file, EC);

  if (EC) {
    ERRORF("An error occured writing the file: %s\n", EC.message().data());
  }

  M->print(Out, nullptr);
}
