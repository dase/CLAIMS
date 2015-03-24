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

	CodeGenerator::getInstance()->lock();
	/* create a function prototype:
	 * void Function(void* tuple_addr, void* return)
	 * */
    llvm::FunctionType *FT =createFunctionPrototype();
    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "a", CodeGenerator::getInstance()->getModule());



    /* create function entry */
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", F);
    CodeGenerator::getInstance()->getBuilder()->SetInsertPoint(BB);


    /* get the parameter value of the function */
    llvm::Function::arg_iterator AI = F->arg_begin();
	llvm::Value* tuple_addr=AI++;//@ assume we have got the tuple addr.
	tuple_addr->setName("tuple_addr");
	llvm::Value* return_addr=AI++;
	return_addr->setName("return_addr");


    /* try to generate the code and get the return value
     * If we cannot generate the code, return_value is NULL*/
	llvm::Value* return_value=codegen(qnode,schema,tuple_addr);

	if(!return_value){
		CodeGenerator::getInstance()->release();
		return NULL;
	}

	/* store the value to the return address */
	if(!storeTheReturnValue(return_value,return_addr,qnode)){
		CodeGenerator::getInstance()->release();
		return NULL;
	}

	/* create return block for the function */
	CodeGenerator::getInstance()->getBuilder()->CreateRetVoid();
	verifyFunction(*F);
//		F->dump();
//	 if(verifyModule(*CodeGenerator::getInstance()->getModule())){
//		llvm::outs()<<"errors!";
//	 }
	 CodeGenerator::getInstance()->getFunctionPassManager()->run(*F);
//	 llvm::outs()<<*CodeGenerator::getInstance()->getModule()<<"\n";
	 expr_func_prototype ret=CodeGenerator::getInstance()->getExecutionEngine()->getPointerToFunction(F);
	 CodeGenerator::getInstance()->release();
	 return ret;
}

llvm::Value* codegen(QNode* qnode, Schema* schema,llvm::Value* tuple_addr) {
	llvm::Value* value;
	data_type actual_type=qnode->actual_type,return_type=qnode->return_type;
	switch(qnode->type){
	case t_qexpr_cmp:{
		QExpr_binary* node=(QExpr_binary*)qnode;
		llvm::Value* lvalue=codegen(node->lnext,schema,tuple_addr);
		llvm::Value* rvalue=codegen(node->rnext,schema,tuple_addr);
		value= codegen_binary_op(lvalue,rvalue,node);
		actual_type=t_boolean; // for boolean compression, the actual type is always boolean.
		break;
	}
	case t_qexpr_cal:{
		QExpr_binary* node=(QExpr_binary*)qnode;
		llvm::Value* lvalue=codegen(node->lnext,schema,tuple_addr);
		llvm::Value* rvalue=codegen(node->rnext,schema,tuple_addr);
		value= codegen_binary_op(lvalue,rvalue,node);
		break;
	}
	case t_qcolcumns:{
		QColcumns* node=(QColcumns*)qnode;
		value= codegen_column(node,schema,tuple_addr);
		break;
	}
	case t_qexpr:{
		QExpr * node=(QExpr*)qnode;
		value=codegen_const(node);
		break;
	}
	default:
		return 0;
	}
	// conduct the type promotion if needed
	if(actual_type!=return_type){
		value=typePromotion(value,actual_type,return_type);
	}
	return value;
}

llvm::Value* codegen_binary_op(llvm::Value* l, llvm::Value* r,
		QExpr_binary* node) {
	if(l==0||r==0)
		return NULL;
	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	switch(node->op_type){
	case oper_add:
		return createAdd(l,r,node->actual_type);
	case oper_minus:
		return createMinus(l,r,node->actual_type);
	case oper_multiply:
		return createMultiply(l,r,node->actual_type);
	case oper_divide:
		return createDivide(l,r,node->actual_type);
	case oper_less:
		return createLess(l,r,node->actual_type);
	default:
		return NULL;
	}
}

llvm::Value* codegen_column(QColcumns* node, Schema* schema,llvm::Value* tuple_addr) {
	/* If the following assert does not hold, we should conduct
	 * the type promotion.
	 */

	llvm::Value* value;

	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	unsigned offset = schema->getColumnOffset(node->id);

	// cast offset from unsigned to LLVM::Int64
	llvm::Value* value_off=ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()), (int64_t)offset,"generate LLVM::Int64Const");

	// cast tuple_addr from LLVM::PtrInt1 to LLVM::Int64
	tuple_addr=builder->CreatePtrToInt(tuple_addr,llvm::Type::getInt64Ty(llvm::getGlobalContext()),"Cast LLVM::PtrInt1 to LLVM::Int64");

	// add LLVM::Int64 with LLVM::Int64
	llvm::Value* column_addr=builder->CreateAdd(value_off,tuple_addr,"Calculate the offset");

	switch(node->actual_type){
	case t_int:
		// cast from LLVM::Int64 to LLVM::PtrInt32
		column_addr=builder->CreateIntToPtr(column_addr,llvm::PointerType::getUnqual(llvm::Type::getInt32Ty(llvm::getGlobalContext())));

		// create a LLVM::Int32 and return
		value= builder->CreateLoad(column_addr);
		break;
	case t_float:
		// cast LLVM::Int64 to LLVM::PtrFloat
		column_addr=builder->CreateIntToPtr(column_addr,llvm::PointerType::getUnqual(llvm::Type::getFloatTy(llvm::getGlobalContext())));
		// create a LLVM::Float and return
		value= builder->CreateLoad(column_addr);
		break;
	case t_u_long:
		//cast LLVM:LINT64 to LLVM::int64Ptr
		column_addr=builder->CreateIntToPtr(column_addr,llvm::PointerType::getUnqual(llvm::Type::getInt64Ty(llvm::getGlobalContext())));
		// create a LLVM::Int64 and return
		value=builder->CreateLoad(column_addr);
		break;
	case t_double:
		//cast LLVM:INT64 to LLVM::DoublePtr
		column_addr=builder->CreateIntToPtr(column_addr,llvm::PointerType::getUnqual(llvm::Type::getDoubleTy(llvm::getGlobalContext())));
		//create a LLVM::Double and return
		value=builder->CreateLoad(column_addr);
		break;
	default:
		return 0;

	}

	return value;
}

llvm::FunctionType* createFunctionPrototype() {
	std::vector<llvm::Type *> parameter_types;
	parameter_types.push_back(llvm::PointerType::getUnqual(llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
	parameter_types.push_back(llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(),32)));
    llvm::FunctionType *FT =
    llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()), parameter_types,false);
    return FT;
}

bool storeTheReturnValue(llvm::Value* value, llvm::Value* dest_ptr,
		QNode* node) {
	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	switch(node->return_type){
	case t_int:
		//cast return_addr to LLVM::Int32Ptr
		dest_ptr=builder->CreatePointerCast(dest_ptr,llvm::PointerType::getUnqual(llvm::Type::getInt32Ty(llvm::getGlobalContext())));

		//flush the return value
		builder->CreateStore(value,dest_ptr);
		return true;
	case t_float:
		//cast return_addr to LLVM::Int32Ptr
		dest_ptr=builder->CreatePointerCast(dest_ptr,llvm::PointerType::getUnqual(llvm::Type::getFloatTy(llvm::getGlobalContext())));

		//flush the return value
		builder->CreateStore(value,dest_ptr);
		return true;
	case t_u_long:
		//cast return_addr to LLVM::int64Ptr
		dest_ptr=builder->CreatePointerCast(dest_ptr,llvm::PointerType::getUnqual(llvm::Type::getInt64Ty(llvm::getGlobalContext())));

		//fluash the return value
		builder->CreateStore(value,dest_ptr);
		return true;
	case t_double:
		//cast return_addr to LLVM::DoublePtr
		dest_ptr=builder->CreatePointerCast(dest_ptr,llvm::PointerType::getUnqual(llvm::Type::getDoubleTy(llvm::getGlobalContext())));

		//flush the result
		builder->CreateStore(value,dest_ptr);
		return true;
	case t_boolean:
		//cast return_addr to LLVM::Int1
		dest_ptr=builder->CreatePointerCast(dest_ptr,llvm::PointerType::getUnqual(llvm::Type::getInt1Ty(llvm::getGlobalContext())));

		//flush the result
		builder->CreateStore(value,dest_ptr);
		return true;
	default:
		return false;
	}
}

llvm::Value* createAdd(llvm::Value* l, llvm::Value* r, data_type type) {
	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	switch(type){
	case t_int:
	case t_u_long:
		return builder->CreateAdd(l,r,"+");
	case t_float:
	case t_double:
		return builder->CreateFAdd(l,r,"+");
	default:
		return NULL;
	}
}

llvm::Value* createMinus(llvm::Value* l, llvm::Value* r, data_type type) {
	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	switch(type){
	case t_int:
	case t_u_long:
		return builder->CreateSub(l,r,"-");
	case t_float:
	case t_double:
		return builder->CreateFSub(l,r,"-");
	default:
		return NULL;
	}
}

llvm::Value* createMultiply(llvm::Value* l, llvm::Value* r, data_type type) {
	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	switch(type){
	case t_int:
	case t_u_long:
		return builder->CreateMul(l,r);
	case t_float:
	case t_double:
		return builder->CreateFMul(l,r);
	default:
		return NULL;
	}
}

llvm::Value* createDivide(llvm::Value* l, llvm::Value* r, data_type type) {
	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	switch(type){
	case t_int:
	case t_u_long:
		return builder->CreateUDiv(l,r);
	case t_float:
	case t_double:
		return builder->CreateFDiv(l,r);
	}
}

llvm::Value* createLess(llvm::Value* l, llvm::Value* r, data_type type) {
	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	switch(type){
	case t_int:
	case t_u_long:
		return builder->CreateICmpSLT(l,r,"<");
	case t_float:
	case t_double:
		return builder->CreateFCmpOLT(l,r,"<");
	default:
		return NULL;
	}
}
llvm::Value* codegen_const(QExpr* node) {
	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	switch(node->actual_type){
	case t_int:
		return llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()),atoi(node->const_value.c_str()),true);
	case t_u_long:
		return llvm::ConstantInt::get(llvm::Type::getInt64Ty(llvm::getGlobalContext()),atoi(node->const_value.c_str()),true);
	case t_float:
		return llvm::ConstantFP::get(llvm::Type::getFloatTy(llvm::getGlobalContext()),atof(node->const_value.c_str()));
	case t_double:
		return llvm::ConstantFP::get(llvm::Type::getDoubleTy(llvm::getGlobalContext()),atof(node->const_value.c_str()));
	default:
		return NULL;
	}
}


llvm::Value* typePromotion(llvm::Value* v, data_type old_ty,
		data_type target_ty) {
	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	switch(old_ty){
	case t_int:{
		switch(target_ty){
		case t_float:
			return builder->CreateSIToFP(v,llvm::Type::getFloatTy(llvm::getGlobalContext()));
		case t_u_long:
			return builder->CreateIntCast(v,llvm::Type::getInt64Ty(llvm::getGlobalContext()),true,"long");
		case t_double:
			return builder->CreateSIToFP(v,llvm::Type::getDoubleTy(llvm::getGlobalContext()));
		default:
			return NULL;
		}
		break;
	}
	case t_u_long:{
		switch(target_ty){
		case t_float:
			return builder->CreateSIToFP(v,llvm::Type::getFloatTy(llvm::getGlobalContext()));
		case t_double:
			return builder->CreateSIToFP(v,llvm::Type::getDoubleTy(llvm::getGlobalContext()));
		default:
			return NULL;
		}
		break;
	}
	case t_float:{
		switch(target_ty){
		case t_double:
			return builder->CreateFPCast(v,llvm::Type::getDoubleTy(llvm::getGlobalContext()));
		default:
			return NULL;
		}
		break;
	}
	default:
		return NULL;
	}
}
