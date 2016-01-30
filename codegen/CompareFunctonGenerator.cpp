/*
 * NValueExpressionGenerator.cpp
 *
 *  Created on: 2015-4-1
 *      Author: imdb
 */

#include "CompareFunctonGenerator.h"
#include "llvm/IR/Module.h"
#include <boost/date_time/gregorian/gregorian.hpp>

using decimal::NValue;
using llvm::Function;

extern "C" bool NValueLess(void* l, void* r) {
  /*
   *  bool* ret_bool;
   *  NValue* lnv = l;
   *  NValue* rnv = r;
   *  *ret_bool = lnv->op_less(rnv);
   *  return ret_bool;
   */
  Decimal* lvalue = (Decimal*) l;
  Decimal* rvalue = (Decimal*) r;

  return lvalue->op_less(*rvalue);
}

bool NValueEqual(void *l, void *r) {
  Decimal* lvalue = (Decimal*) l;
  Decimal* rvalue = (Decimal*) r;

  return lvalue->op_equals(*rvalue);
}

bool NValueNotEqual(void *l, void *r) {
  Decimal* lvalue = (Decimal*) l;
  Decimal* rvalue = (Decimal*) r;

  return lvalue->op_not_equals(*rvalue);
}

bool NValueGreat(void *l, void *r) {
  Decimal* lvalue = (Decimal*) l;
  Decimal* rvalue = (Decimal*) r;

  return lvalue->op_great(*rvalue);
}

bool NValueLessEqual(void *l, void *r) {
  Decimal* lvalue = (Decimal*) l;
  Decimal* rvalue = (Decimal*) r;

  return lvalue->op_less_equals(*rvalue);
}

bool NValueGreatEqual(void *l, void *r) {
  Decimal* lvalue = (Decimal*) l;
  Decimal* rvalue = (Decimal*) r;

  return lvalue->op_great_equals(*rvalue);
}

llvm::Function* CreateNValueCompareFunc(enum oper_type compare_type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  // Type Definitions
  std::vector<llvm::Type *> para_types;
//  parameter_types.push_back(llvm::PointerType::getInt8PtrTy(llvm::getGlobalContext()));
  para_types.push_back(llvm::Type::getInt8PtrTy(llvm::getGlobalContext()));
  para_types.push_back(
      llvm::PointerType::getInt8PtrTy(llvm::getGlobalContext()));
  llvm::FunctionType* ft = llvm::FunctionType::get(
      llvm::Type::getInt1Ty(llvm::getGlobalContext()), para_types, false);

  string function_name = "";
  switch (compare_type) {
    case oper_less: {
      function_name = "NValueLess";
      break;
    }
    case oper_great: {
      function_name = "NValueGreat";
      break;
    }
    case oper_equal: {
      function_name = "NValueEqual";
      break;
    }
    case oper_less_equal: {
      function_name = "NValueLessEqual";
      break;
    }
    case oper_great_equal: {
      function_name = "NValueGreatEqual";
      break;
    }
    case oper_not_equal: {
      function_name = "NValueNotEqual";
      break;
    }
    default:
      cout << "no matched function";
      return NULL;
  }

  llvm::Function* ff = CodeGenerator::getInstance()->getModule()->getFunction(
      function_name);
  if (!ff) {
    ff = llvm::Function::Create(ft, llvm::Function::ExternalLinkage,
                                function_name,
                                CodeGenerator::getInstance()->getModule());
    ff->setCallingConv(llvm::CallingConv::C);
  }

  return ff;
}

bool DateGreat(void* l, void* r) {
  return *(date*) l > *(date*) r;
}

bool DateLess(void* l, void* r) {
  return *(date*) l < *(date*) r;
}

llvm::Function* CreateDateCompareFunc(enum oper_type compare_type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  //Type Definitions
  std::vector<llvm::Type *> para_types;
  para_types.push_back(llvm::Type::getInt8PtrTy(llvm::getGlobalContext()));
  para_types.push_back(
      llvm::PointerType::getInt8PtrTy(llvm::getGlobalContext()));
  llvm::FunctionType* ft = llvm::FunctionType::get(
      llvm::Type::getInt1Ty(llvm::getGlobalContext()), para_types, false);

  string function_name = "";
  switch (compare_type) {
    case oper_less: {
      function_name = "DateLess";
      break;
    }
    case oper_great: {
      function_name = "DateGreat";
      break;
    }
    default:
      cout << "no matched function";
      return NULL;
  }

  llvm::Function* ff = CodeGenerator::getInstance()->getModule()->getFunction(
      function_name);
  if (!ff) {
    ff = llvm::Function::Create(ft, llvm::Function::ExternalLinkage,
                                function_name,
                                CodeGenerator::getInstance()->getModule());
    ff->setCallingConv(llvm::CallingConv::C);
  }

  return ff;
}
