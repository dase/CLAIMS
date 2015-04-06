/*
 * NValueExpressionGenerator.cpp
 *
 *  Created on: 2015-4-1
 *      Author: imdb
 */

#include "NValueExpressionGenerator.h"
#include "llvm/IR/Module.h"

using decimal::NValue;
using llvm::Function;

extern "C"
bool NValueLess(void* l, void* r) {
	/*
	 *  bool* ret_bool;
	 *  NValue* lnv = l;
	 *  NValue* rnv = r;
	 *  *ret_bool = lnv->op_less(rnv);
	 *  return ret_bool;
	 */
	NValue* lvalue=(NValue*)l;
	NValue* rvalue=(NValue*)r;

	return lvalue->op_less(*rvalue);
}

bool NValueEqual(void *l, void *r) {
	NValue* lvalue=(NValue*)l;
	NValue* rvalue=(NValue*)r;

	return lvalue->op_equals(*rvalue);
}

bool NValueGreat(void *l, void *r) {
	NValue* lvalue=(NValue*)l;
	NValue* rvalue=(NValue*)r;

	return lvalue->op_great(*rvalue);
}

bool NValueLessEqual(void *l, void *r) {
	NValue* lvalue=(NValue*)l;
	NValue* rvalue=(NValue*)r;

	return lvalue->op_less_equals(*rvalue);
}

bool NValueGreatEqual(void *l, void *r) {
	NValue* lvalue=(NValue*)l;
	NValue* rvalue=(NValue*)r;

	return lvalue->op_great_equals(*rvalue);
}

llvm::Function* CreateNValueCompareFunc(enum NValueCompareType compare_type) {
	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	//Type Definitions
	std::vector<llvm::Type *> para_types;
//	parameter_types.push_back(llvm::PointerType::getInt8PtrTy(llvm::getGlobalContext()));
	para_types.push_back(llvm::Type::getInt8PtrTy(llvm::getGlobalContext()));
	para_types.push_back(llvm::PointerType::getInt8PtrTy(llvm::getGlobalContext()));
	llvm::FunctionType* ft =
			llvm::FunctionType::get(
					llvm::Type::getInt1Ty(llvm::getGlobalContext()),
					para_types,
					false);

	string function_name = "";
	switch(compare_type) {
	case LESS:{
		function_name = "NValueLess";
		break;
	}
	case GREAT:{
		function_name = "NValueGreat";
		break;
	}
	case EQUAL:{
		function_name = "NValueEqual";
		break;
	}
	case LESS_EQUAL:{
		function_name = "NValueLessEqual";
		break;
	}
	case GREAT_EQUAL:{
		function_name = "NValueGreatEqual";
		break;
	}
	default :
		cout<<"no matched function";
		return NULL;
	}

	llvm::Function* ff =
			CodeGenerator::getInstance()->getModule()->getFunction(function_name);
	if (!ff) {
		cout<<"get function from module fail"<<endl;
		ff = llvm::Function::Create(
				ft,
				llvm::Function::ExternalLinkage,
				function_name,
				CodeGenerator::getInstance()->getModule());
		ff->setCallingConv(llvm::CallingConv::C);
	}

	return ff;
}



