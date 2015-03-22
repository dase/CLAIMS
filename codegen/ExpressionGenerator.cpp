/*
 * ExpressionGenerator.cpp
 *
 *  Created on: Mar 21, 2015
 *      Author: wangli
 */

#include <assert.h>
#include "ExpressionGenerator.h"
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

using llvm::IRBuilderBase;
using llvm::ConstantInt;
using llvm::InitializeNativeTarget;



expr_func_prototype getExprFunc(QNode* qnode,Schema* schema) {
	/*	TODO: create a function prototype first
	 * 	The function prototype is very likely to be "void FunctionName(void* tuple, void* ret_value)"
	 */

	std::vector<llvm::Type *> parameter_types;
	parameter_types.push_back(llvm::PointerType::getUnqual(llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
	parameter_types.push_back(llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(),32)));
    llvm::FunctionType *FT =
    llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()), parameter_types,false);
    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "a", CodeGenerator::getInstance()->getModule());

    llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", F);
    CodeGenerator::getInstance()->getBuilder()->SetInsertPoint(BB);


    llvm::Function::arg_iterator AI = F->arg_begin();



	llvm::Value* tuple_addr=AI++;//@ assume we have got the tuple addr.
	tuple_addr->setName("tuple_addr");
	llvm::Value* return_addr=AI++;
	tuple_addr->setName("return_addr");

	llvm::Value* return_value=codegen(qnode,schema,tuple_addr);

	// switch case is needed here. assume return int for now!

//	llvm::PointerType::getUnqual(llvm::IntegerType::getInt32Ty(llvm::getGlobalContext()));
	CodeGenerator::getInstance()->getBuilder()->CreateStore(return_value,return_addr);

	CodeGenerator::getInstance()->getBuilder()->CreateRetVoid();
	verifyFunction(*F);
	F->dump();
	 if(verifyModule(*CodeGenerator::getInstance()->getModule())){
		llvm::outs()<<"errors!";
	 }
	 CodeGenerator::getInstance()->getFunctionPassManager()->run(*F);
	 llvm::outs()<<*CodeGenerator::getInstance()->getModule()<<"\n";
	 return CodeGenerator::getInstance()->getExecutionEngine()->getPointerToFunction(F);
}

llvm::Value* codegen(QNode* qnode, Schema* schema,llvm::Value* tuple_addr) {
	switch(qnode->type){
	case t_qexpr_cal:{
		QExpr_binary* node=(QExpr_binary*)qnode;
		assert(node->lnext->return_type==node->rnext->return_type);
		llvm::Value* lvalue=codegen(node->lnext,schema,tuple_addr);
		llvm::Value* rvalue=codegen(node->rnext,schema,tuple_addr);
		return codegen_binary_cal(lvalue,rvalue,node);
	}
	case t_qcolcumns:{
		QColcumns* node=(QColcumns*)qnode;
		return codegen_column(node,schema,tuple_addr);
	}
	default:
		return 0;
	}
}

llvm::Value* codegen_binary_cal(llvm::Value* l, llvm::Value* r,
		QExpr_binary* node) {
	switch(node->op_type){
	case oper_add:
		assert(node->return_type==t_int);
		return CodeGenerator::getInstance()->getBuilder()->CreateAdd(l, r, "+");
	default:
		return 0;
	}
}

llvm::Value* codegen_column(QColcumns* node, Schema* schema,llvm::Value* tuple_addr) {
	/* If the following assert does not hold, we should conduct
	 * the type promotion.
	 */
	assert(schema->getcolumn(node->id).type==node->actual_type);
	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	unsigned offset = schema->getColumnOffset(node->id);

	// cast offset from unsigned to LLVM::Int64
	llvm::Value* value_off=ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), (int64_t)offset,"generate LLVM::Int64Const");

	// cast tuple_addr from LLVM::PtrInt1 to LLVM::Int64
	tuple_addr=tuple_addr=builder->CreatePtrToInt(tuple_addr,llvm::Type::getInt64Ty(llvm::getGlobalContext()),"Cast LLVM::PtrInt1 to LLVM::Int64");

	// add LLVM::Int64 with LLVM::Int64
	llvm::Value* column_addr=builder->CreateAdd(value_off,tuple_addr,"Calculate the offset");
	switch(node->actual_type){
	case t_int:
		// cast from LLVM::Int64 to LLVM::PtrInt32
		column_addr=builder->CreateIntToPtr(column_addr,llvm::PointerType::getUnqual(llvm::Type::getInt32Ty(llvm::getGlobalContext())));

		// create a LLVM::Int32 and return
		return builder->CreateLoad(column_addr);

	default:{
		assert(false);
		return 0;
	}

	}
}
