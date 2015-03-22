#include <function.h>
namespace test{
 int test_llvm(){
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  InitializeNativeTargetAsmParser();
  LLVMContext &Context = getGlobalContext();
  JITHelper = new MCJITHelper(Context);

  // Install standard binary operators.
  // 1 is lowest precedence.
  BinopPrecedence['<'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40; // highest.

  // Prime the first token.
  fprintf(stderr, "ready> ");
  getNextToken();

  // Run the main "interpreter loop" now.
  MainLoop();

  // Print out all of the generated code.
  JITHelper->dump();

  return 0;
}
 uint64_t HelpingMemoryManager::getSymbolAddress(const std::string &Name) {
   uint64_t FnAddr = SectionMemoryManager::getSymbolAddress(Name);
   if (FnAddr)
     return FnAddr;

   uint64_t HelperFun = (uint64_t)MasterHelper->getSymbolAddress(Name);
   if (!HelperFun)
     report_fatal_error("Program used extern function '" + Name +
                        "' which could not be resolved!");

   return HelperFun;
 }



 MCJITHelper::~MCJITHelper() {
   if (OpenModule)
     delete OpenModule;
   EngineVector::iterator begin = Engines.begin();
   EngineVector::iterator end = Engines.end();
   EngineVector::iterator it;
   for (it = begin; it != end; ++it)
     delete *it;
 }

 Function *MCJITHelper::getFunction(const std::string FnName) {
   ModuleVector::iterator begin = Modules.begin();
   ModuleVector::iterator end = Modules.end();
   ModuleVector::iterator it;
   for (it = begin; it != end; ++it) {
     Function *F = (*it)->getFunction(FnName);
     if (F) {
       if (*it == OpenModule)
         return F;

       assert(OpenModule != NULL);

       // This function is in a module that has already been JITed.
       // We need to generate a new prototype for external linkage.
       Function *PF = OpenModule->getFunction(FnName);
       if (PF && !PF->empty()) {
         ErrorF("redefinition of function across modules");
         return 0;
       }

       // If we don't have a prototype yet, create one.
       if (!PF)
         PF = Function::Create(F->getFunctionType(), Function::ExternalLinkage,
                               FnName, OpenModule);
       return PF;
     }
   }
   return NULL;
 }

 Module *MCJITHelper::getModuleForNewFunction() {
   // If we have a Module that hasn't been JITed, use that.
   if (OpenModule)
     return OpenModule;

   // Otherwise create a new Module.
   std::string ModName = GenerateUniqueName("mcjit_module_");
   Module *M = new Module(ModName, Context);
   Modules.push_back(M);
   OpenModule = M;
   return M;
 }

 void *MCJITHelper::getPointerToFunction(Function *F) {
   // See if an existing instance of MCJIT has this function.
   EngineVector::iterator begin = Engines.begin();
   EngineVector::iterator end = Engines.end();
   EngineVector::iterator it;
   for (it = begin; it != end; ++it) {
     void *P = (*it)->getPointerToFunction(F);
     if (P)
       return P;
   }

   // If we didn't find the function, see if we can generate it.
   if (OpenModule) {
     std::string ErrStr;
     ExecutionEngine *NewEngine =
         EngineBuilder((Module*)(OpenModule))
             .setErrorStr(&ErrStr)
             .setMCJITMemoryManager((HelpingMemoryManager*)(
                 new HelpingMemoryManager(this)))
             .create();
     if (!NewEngine) {
       fprintf(stderr, "Could not create ExecutionEngine: %s\n", ErrStr.c_str());
       exit(1);
     }

     // Create a function pass manager for this engine
     auto *FPM = new legacy::FunctionPassManager(OpenModule);

     // Set up the optimizer pipeline.  Start with registering info about how the
     // target lays out data structures.
     OpenModule->setDataLayout(NewEngine->getDataLayout());
     // Provide basic AliasAnalysis support for GVN.
     FPM->add(createBasicAliasAnalysisPass());
     // Promote allocas to registers.
     FPM->add(createPromoteMemoryToRegisterPass());
     // Do simple "peephole" optimizations and bit-twiddling optzns.
     FPM->add(createInstructionCombiningPass());
     // Reassociate expressions.
     FPM->add(createReassociatePass());
     // Eliminate Common SubExpressions.
     FPM->add(createGVNPass());
     // Simplify the control flow graph (deleting unreachable blocks, etc).
     FPM->add(createCFGSimplificationPass());
     FPM->doInitialization();

     // For each function in the module
     Module::iterator it;
     Module::iterator end = OpenModule->end();
     for (it = OpenModule->begin(); it != end; ++it) {
       // Run the FPM on this function
       FPM->run(*it);
     }

     // We don't need this anymore
     delete FPM;

     OpenModule = NULL;
     Engines.push_back(NewEngine);
     NewEngine->finalizeObject();
     return NewEngine->getPointerToFunction(F);
   }
   return NULL;
 }

 void *MCJITHelper::getSymbolAddress(const std::string &Name) {
   // Look for the symbol in each of our execution engines.
   EngineVector::iterator begin = Engines.begin();
   EngineVector::iterator end = Engines.end();
   EngineVector::iterator it;
   for (it = begin; it != end; ++it) {
     uint64_t FAddr = (*it)->getFunctionAddress(Name);
     if (FAddr) {
       return (void *)FAddr;
     }
   }
   return NULL;
 }

 void MCJITHelper::dump() {
   ModuleVector::iterator begin = Modules.begin();
   ModuleVector::iterator end = Modules.end();
   ModuleVector::iterator it;
   for (it = begin; it != end; ++it)
     (*it)->dump();
 }
 //===----------------------------------------------------------------------===//
 // Code Generation
 //===----------------------------------------------------------------------===//

// MCJITHelper *JITHelper;
// IRBuilder<> Builder(getGlobalContext());
// std::map<std::string, Value *> NamedValues;

 inline Value *ErrorV(const char *Str) {
   Error(Str);
   return 0;
 }

 Value *NumberExprAST::Codegen() {
   return ConstantFP::get(getGlobalContext(), APFloat(Val));
 }

 Value *VariableExprAST::Codegen() {
   // Look this variable up in the function.
   Value *V = NamedValues[Name];
   return V ? V : ErrorV("Unknown variable name");
 }

 Value *BinaryExprAST::Codegen() {
   Value *L = LHS->Codegen();
   Value *R = RHS->Codegen();
   if (L == 0 || R == 0)
     return 0;

   switch (Op) {
   case '+':
     return Builder.CreateFAdd(L, R, "addtmp");
   case '-':
     return Builder.CreateFSub(L, R, "subtmp");
   case '*':
     return Builder.CreateFMul(L, R, "multmp");
   case '<':
     L = Builder.CreateFCmpULT(L, R, "cmptmp");
     // Convert bool 0/1 to double 0.0 or 1.0
     return Builder.CreateUIToFP(L, Type::getDoubleTy(getGlobalContext()),
                                 "booltmp");
   default:
     return ErrorV("invalid binary operator");
   }
 }

 Value *CallExprAST::Codegen() {
   // Look up the name in the global module table.
   Function *CalleeF = JITHelper->getFunction(Callee);
   if (CalleeF == 0)
     return ErrorV("Unknown function referenced");

   // If argument mismatch error.
   if (CalleeF->arg_size() != Args.size())
     return ErrorV("Incorrect # arguments passed");

   std::vector<Value *> ArgsV;
   for (unsigned i = 0, e = Args.size(); i != e; ++i) {
     ArgsV.push_back(Args[i]->Codegen());
     if (ArgsV.back() == 0)
       return 0;
   }

   return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
 }

 Function *PrototypeAST::Codegen() {
   // Make the function type:  double(double,double) etc.
   std::vector<Type *> Doubles(Args.size(),
                               Type::getDoubleTy(getGlobalContext()));
   FunctionType *FT =
       FunctionType::get(Type::getDoubleTy(getGlobalContext()), Doubles, false);

   std::string FnName = MakeLegalFunctionName(Name);

   Module *M = JITHelper->getModuleForNewFunction();

   Function *F = Function::Create(FT, Function::ExternalLinkage, FnName, M);

   // If F conflicted, there was already something named 'Name'.  If it has a
   // body, don't allow redefinition or reextern.
   if (F->getName() != FnName) {
     // Delete the one we just made and get the existing one.
     F->eraseFromParent();
     F = JITHelper->getFunction(Name);
     // If F already has a body, reject this.
     if (!F->empty()) {
       ErrorF("redefinition of function");
       return 0;
     }

     // If F took a different number of args, reject.
     if (F->arg_size() != Args.size()) {
       ErrorF("redefinition of function with different # args");
       return 0;
     }
   }

   // Set names for all arguments.
   unsigned Idx = 0;
   for (Function::arg_iterator AI = F->arg_begin(); Idx != Args.size();
        ++AI, ++Idx) {
     AI->setName(Args[Idx]);

     // Add arguments to variable symbol table.
     NamedValues[Args[Idx]] = AI;
   }

   return F;
 }

 Function *FunctionAST::Codegen() {
   NamedValues.clear();

   Function *TheFunction = Proto->Codegen();
   if (TheFunction == 0)
     return 0;

   // Create a new basic block to start insertion into.
   BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", TheFunction);
   Builder.SetInsertPoint(BB);

   if (Value *RetVal = Body->Codegen()) {
     // Finish off the function.
     Builder.CreateRet(RetVal);

     // Validate the generated code, checking for consistency.
     verifyFunction(*TheFunction);

     return TheFunction;
   }

   // Error reading body, remove function.
   TheFunction->eraseFromParent();
   return 0;
 }

}
