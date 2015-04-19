#ifndef __LLVM_MAIN__
#define __LLVM_MAIN__
#include <iostream>
using namespace std;
#include "generating_expr.cpp"
#include "../Test/gtest.h"
#include "codegen_test.h"
#include "ExpressionGenerator.h"
#include "CompareFunctonGenerator.h"
#include "CodeGenerator.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/IR/IntrinsicInst.h"
#include "../common/types/NValue.hpp"

extern "C"
struct bit128{
	int64_t a;
	int64_t b;
};
float myprint(int a){
	printf("OK--->%d\n",a);
	float ret;
	int* ret_addr=(int*)&ret;
	*ret_addr=a*a;
	return ret;
}
void test_call(){
	llvm::IRBuilder<> * builder=CodeGenerator::getInstance()->getBuilder();

	llvm::FunctionType *FT =llvm::FunctionType::get(
			  /*Result=*/llvm::Type::getInt32Ty(llvm::getGlobalContext()),
			  /*Params=*/llvm::Type::getInt32Ty(llvm::getGlobalContext()),
			  /*isVarArg=*/false);
	llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "a", CodeGenerator::getInstance()->getModule());


	llvm::BasicBlock* label_entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry",F,0);
	builder->SetInsertPoint(label_entry);


	llvm::Function::arg_iterator AI = F->arg_begin();
	llvm::Value* v=AI++;

	llvm::FunctionType *ft_for_myprint =llvm::FunctionType::get(
				  /*Result=*/llvm::Type::getInt32Ty(llvm::getGlobalContext()),
				  /*Params=*/llvm::Type::getInt32Ty(llvm::getGlobalContext()),
				  /*isVarArg=*/false);
	llvm::Function* func_print = CodeGenerator::getInstance()->getModule()->getFunction("myprint");
	if(func_print==0){
		printf("func_print does not exist, we get it.\n");
		func_print = llvm::Function::Create(
		  /*Type=*/ft_for_myprint,
		  /*Linkage=*/llvm::GlobalValue::ExternalLinkage,
		  /*Name=*/"myprint", CodeGenerator::getInstance()->getModule());
		func_print->setCallingConv(llvm::CallingConv::C);
	}
	llvm::Value* return_value = builder->CreateCall(func_print,v);
	builder->CreateRet(return_value);
	verifyFunction(*F);
	typedef int (*myfunc)(int);
	myfunc generated_func = CodeGenerator::getInstance()->getExecutionEngine()->getPointerToFunction(F);
	printf("2*2=%d\n",generated_func(2));
	printf("3*3=%d\n",generated_func(3));
//	llvm::Type::getIntNTy(llvm::getGlobalContext(),128);
}

//extern "C"
//int64_t decimal_less(void* a, void* b){
//	((NValue*)a)->op_add((*(NValue*)b);
//}

void test_decimal_comparison(){

}

int main(int argc, char** argv){
//	return test::test_llvm();
	return g_test_main(argc, argv);
//	get_one();
//	get_another_one();
//	test_reference();
//	myllvm::test();
//	myllvm::test1();
//	myprint(1000);
//	test_call();
}
#endif
