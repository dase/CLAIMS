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
#include "llvm/IR/IntrinsicInst.h"
#include "CompareFunctonGenerator.h"
#include "../common/TypePromotionMap.h"

using llvm::IRBuilderBase;
using llvm::ConstantInt;
using llvm::InitializeNativeTarget;

filter_process_func getFilterProcessFunc(QNode* qnode, Schema* schema) {

  /*
   *    int process_func(char* b_start, int & b_cur_addr, int b_tuple_count, char* c_start,int & c_cur_addr, int c_tuple_count){
   *        int b_cur=*b_cur_addr;
   *        int c_cur=*c_cur_addr;
   *        while(c_cur<c_tuple_count){
   *            bool ret*=alloc;
   *            char* c_tuple_addr= c_start+length*c_cur;
   *            f(c_tuple_addr,ret);
   *            bool pass=*ret;
   *            if(pass){
   *                if(b_cur<b_tuple_count){
   *                    char* b_tuple_addr=b_start+length*b_cur;
   *                    b_cur=b_cur+1;
   *                    memcpy(b_typle_addr,c_tuple_addr,length);
   *                }
   *                else{
   *                    break;
   *                }
   *            }
   *            c_cur=c_cur+1;
   *        }
   *        *b_cur_addr=b_cur;
   *        *c_cur_addr=c_cur;
   *    }
   *
   */

  llvm::LLVMContext& context = llvm::getGlobalContext();
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  CodeGenerator::getInstance()->lock();
  llvm::Function* expression_function = getExprLLVMFucn(qnode, schema);
  if (expression_function == NULL) return NULL;

  /* create function prototype */
  std::vector<llvm::Type *> parameter_types;
  parameter_types.push_back(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
  parameter_types.push_back(
      llvm::PointerType::getUnqual(llvm::Type::getInt32Ty(context)));
  parameter_types.push_back(llvm::Type::getInt32Ty(llvm::getGlobalContext()));
  parameter_types.push_back(
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
  parameter_types.push_back(
      llvm::PointerType::getUnqual(llvm::Type::getInt32Ty(context)));
  parameter_types.push_back(llvm::Type::getInt32Ty(llvm::getGlobalContext()));
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getVoidTy(context), parameter_types, false);

  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "a",
      CodeGenerator::getInstance()->getModule());

  /* create function entry */
  llvm::BasicBlock *entry_block = llvm::BasicBlock::Create(
      llvm::getGlobalContext(), "entry_block", F);
  llvm::BasicBlock *while_cond = llvm::BasicBlock::Create(
      llvm::getGlobalContext(), "while_cond", F);
  llvm::BasicBlock *while_body = llvm::BasicBlock::Create(
      llvm::getGlobalContext(), "while_body", F);
  llvm::BasicBlock *while_end = llvm::BasicBlock::Create(
      llvm::getGlobalContext(), "while_end", F);
  llvm::BasicBlock *if_passed = llvm::BasicBlock::Create(
      llvm::getGlobalContext(), "if_passed", F);
  llvm::BasicBlock *if_has_space = llvm::BasicBlock::Create(
      llvm::getGlobalContext(), "if_has_space", F);
  llvm::BasicBlock* exit_block = llvm::BasicBlock::Create(context, "exit_block",
                                                          F);

  builder->SetInsertPoint(entry_block);

  /* get the parameters of the function */
  llvm::Function::arg_iterator AI = F->arg_begin();
  llvm::Value* b_start = AI++;
  b_start->setName("b_start");
  llvm::Value* b_cur_addr = AI++;
  b_cur_addr->setName("b_cur_addr");
  llvm::Value* b_tuple_count = AI++;
  b_tuple_count->setName("b_tuple_count");
  llvm::Value* c_start = AI++;
  c_start->setName("c_start");
  llvm::Value* c_cur_addr = AI++;
  c_cur_addr->setName("c_cur_addr");
  llvm::Value* c_tuple_count = AI++;
  c_tuple_count->setName("c_tuple_count");

  // construct two constants
  llvm::Value* tuple_size = llvm::ConstantInt::get(
      llvm::Type::getInt32Ty(context), schema->getTupleMaxSize());
  llvm::Value* const_int_32_1 = llvm::ConstantInt::get(
      llvm::Type::getInt32Ty(context), 1);
  llvm::Value* const_int_32_3 = llvm::ConstantInt::get(
      llvm::Type::getInt32Ty(context), 3);
  llvm::Value* const_true = llvm::ConstantInt::get(
      llvm::Type::getInt1Ty(context), 1);

  // store two values from pointers

  llvm::Value* b_cur = builder->CreateLoad(b_cur_addr);
  llvm::Value* c_cur = builder->CreateLoad(c_cur_addr);
  builder->CreateBr(while_cond);

  // where condition
  builder->SetInsertPoint(while_cond);
  c_cur = builder->CreateLoad(c_cur_addr);
  llvm::Value* while_cond_result = builder->CreateICmpSLT(c_cur, c_tuple_count);
  builder->CreateCondBr(while_cond_result, while_body, exit_block);

  // where body
  {
    /*
     *            bool ret*=alloc;
     *            char* c_tuple_addr= c_start+length*c_cur;
     *            f(c_tuple_addr,ret);
     *            bool pass=*ret;
     */
    builder->SetInsertPoint(while_body);
    llvm::Value* pass_addr = builder->CreateAlloca(
        llvm::Type::getInt1Ty(context));
    c_cur = builder->CreateLoad(c_cur_addr);
    llvm::Value* c_offset = builder->CreateMul(c_cur, tuple_size);
    llvm::Value* c_offset_64 = builder->CreateIntCast(
        c_offset, llvm::Type::getInt64Ty(context), true);
    llvm::Value* c_start_64 = builder->CreatePtrToInt(
        c_start, llvm::Type::getInt64Ty(context));
    llvm::Value* c_tuple_addr = builder->CreateAdd(c_start_64, c_offset_64);
    c_tuple_addr = builder->CreateIntToPtr(
        c_tuple_addr,
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
    llvm::Value* pass_addr_int8 = builder->CreatePointerCast(
        pass_addr,
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
    std::vector<llvm::Value*> paras;
    paras.push_back(c_tuple_addr);
    paras.push_back(pass_addr_int8);
//  builder->CreateStore(const_true,pass_addr);
    builder->CreateCall(expression_function, paras);
    // if condition
    {
      llvm::Value* pass = builder->CreateLoad(pass_addr);
      builder->CreateCondBr(pass, if_passed, while_end);
    }
    // if body
    {
      // if condition
      {
        builder->SetInsertPoint(if_passed);
        b_cur = builder->CreateLoad(b_cur_addr);
        llvm::Value* has_space = builder->CreateICmpSLT(b_cur, b_tuple_count);
        builder->CreateCondBr(has_space, if_has_space, exit_block);
      }
      // if body
      {
        builder->SetInsertPoint(if_has_space);
        b_cur = builder->CreateLoad(b_cur_addr);
        llvm::Value* b_offset = builder->CreateMul(b_cur, tuple_size);
        llvm::Value* b_offset_64 = builder->CreateIntCast(
            b_offset, llvm::Type::getInt64Ty(context), true);
        llvm::Value* b_start_64 = builder->CreatePtrToInt(
            b_start, llvm::Type::getInt64Ty(context));
        llvm::Value* b_tuple_addr = builder->CreateAdd(b_start_64, b_offset_64);
        b_tuple_addr = builder->CreateIntToPtr(
            b_tuple_addr,
            llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context)));
        b_cur = builder->CreateAdd(b_cur, const_int_32_1);
        builder->CreateStore(b_cur, b_cur_addr);
        builder->CreateMemCpy(b_tuple_addr, c_tuple_addr, tuple_size, 4);
        builder->CreateBr(while_end);
      }
    }
    builder->SetInsertPoint(while_end);
    c_cur = builder->CreateLoad(c_cur_addr);
    c_cur = builder->CreateAdd(c_cur, const_int_32_1);
    builder->CreateStore(c_cur, c_cur_addr);
    builder->CreateBr(while_cond);
  }
  // exit block
  builder->SetInsertPoint(exit_block);
  builder->CreateRetVoid();

  llvm::verifyFunction(*F);
//    F->dump();
  CodeGenerator::getInstance()->getFunctionPassManager()->run(*F);
  filter_process_func ret = (filter_process_func) CodeGenerator::getInstance()
      ->getExecutionEngine()->getPointerToFunction(F);
  CodeGenerator::getInstance()->release();
  return ret;
}

llvm::Function* getExprLLVMFucn(QNode* qnode, Schema* schema) {
  /* create a function prototype:
   * void Function(void* tuple_addr, void* return)
   * */

  std::vector<llvm::Type *> parameter_types;
  parameter_types.push_back(
      llvm::PointerType::getUnqual(
          llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
  parameter_types.push_back(
      llvm::PointerType::getUnqual(
          llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getVoidTy(llvm::getGlobalContext()), parameter_types, false);
  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "a",
      CodeGenerator::getInstance()->getModule());

  /* create function entry */
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(),
                                                  "entry", F);
  CodeGenerator::getInstance()->getBuilder()->SetInsertPoint(BB);

  /* get the parameter value of the function */
  llvm::Function::arg_iterator AI = F->arg_begin();
  llvm::Value* tuple_addr = AI++;  //@ assume we have got the tuple addr.
  tuple_addr->setName("tuple_addr");
  llvm::Value* return_addr = AI++;
  return_addr->setName("return_addr");

  /* try to generate the code and get the return value
   * If we cannot generate the code, return_value is NULL*/
  llvm::Value* return_value = codegen(qnode, schema, tuple_addr);

  if (!return_value) {
    CodeGenerator::getInstance()->release();
    return NULL;
  }

  /* store the value to the return address */
  if (!storeTheReturnValue(return_value, return_addr, qnode)) {
    CodeGenerator::getInstance()->release();
    return NULL;
  }

//  llvm::raw_ostream* rstream = new llvm::raw_fd_ostream(2, false);

  /* create return block for the function */
  CodeGenerator::getInstance()->getBuilder()->CreateRetVoid();
//  verifyFunction(*F, rstream);
  verifyFunction(*F);
//  F->dump();
//  if(verifyModule(*CodeGenerator::getInstance()->getModule())){
//    llvm::outs()<<"errors!";
//  }
  CodeGenerator::getInstance()->getFunctionPassManager()->run(*F);
  return F;
}

expr_func getExprFunc(QNode* qnode, Schema* schema) {

  CodeGenerator::getInstance()->lock();
  llvm::Function* fun = getExprLLVMFucn(qnode, schema);
  if (fun == 0) {
    CodeGenerator::getInstance()->release();
    return NULL;
  }
  else {
    expr_func ret =
        (expr_func) CodeGenerator::getInstance()->getExecutionEngine()
            ->getPointerToFunction(fun);
    CodeGenerator::getInstance()->release();
    return ret;
  }
}
ExprFuncTwoTuples getExprFuncTwoTuples(QNode* qnode, Schema* l_schema,
                                          Schema* r_schema) {
  CodeGenerator::getInstance()->lock();
  llvm::Function* fun = getExprLLVMFuncForTwoTuples(qnode, l_schema, r_schema);
  if (fun == 0) {
    CodeGenerator::getInstance()->release();
    return NULL;
  }
  else {
    ExprFuncTwoTuples ret =
        (ExprFuncTwoTuples) CodeGenerator::getInstance()->getExecutionEngine()
            ->getPointerToFunction(fun);
    CodeGenerator::getInstance()->release();
    return ret;
  }
}

llvm::Value* codegen(QNode* qnode, Schema* schema, llvm::Value* tuple_addr,
                     Schema* r_schema, llvm::Value* r_tuple_addr) {

  llvm::Value* value;
  data_type actual_type = qnode->actual_type, return_type = qnode->return_type;
  switch (qnode->type) {
    case t_qexpr_cmp: {
      QExpr_binary* node = (QExpr_binary*) qnode;
      llvm::Value* lvalue = codegen(node->lnext, schema, tuple_addr, r_schema,
                                    r_tuple_addr);
      llvm::Value* rvalue = codegen(node->rnext, schema, tuple_addr, r_schema,
                                    r_tuple_addr);
      value = codegen_binary_op(lvalue, rvalue, node);
      actual_type = t_boolean;  // for boolean compression, the actual type is always boolean.
      break;
    }
    case t_qexpr_cal: {
      QExpr_binary* node = (QExpr_binary*) qnode;
      llvm::Value* lvalue = codegen(node->lnext, schema, tuple_addr, r_schema,
                                    r_tuple_addr);
      llvm::Value* rvalue = codegen(node->rnext, schema, tuple_addr, r_schema,
                                    r_tuple_addr);
      value = codegen_binary_op(lvalue, rvalue, node);
      break;
    }
    case t_qcolcumns: {
      QColcumns* node = (QColcumns*) qnode;
      if (node->tab_index == 0)
        value = codegen_column(node, schema, tuple_addr);
      else
        value = codegen_column(node, r_schema, r_tuple_addr);
      break;
    }
    case t_qexpr: {
      QExpr * node = (QExpr*) qnode;
      value = codegen_const(node);
      break;
    }
    default:
      printf("no supported\n");
      return 0;
  }
  // conduct the type promotion if needed
  if (actual_type != return_type) {
    value = typePromotion(value, actual_type, return_type);
  }
  return value;
}

llvm::Value* codegen_binary_op(llvm::Value* l, llvm::Value* r,
                               QExpr_binary* node) {
  if (l == 0 || r == 0) return NULL;
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (node->op_type) {
    case oper_add:
      return createAdd(l, r, node->actual_type);
    case oper_minus:
      return createMinus(l, r, node->actual_type);
    case oper_multiply:
      return createMultiply(l, r, node->actual_type);
    case oper_divide:
      return createDivide(l, r, node->actual_type);
//    case oper_mod:
//      return createMod(l,r,node->actual_type);

    case oper_less:
      return createLess(l, r, node->actual_type);
    case oper_less_equal:
      return createLessEqual(l, r, node->actual_type);
    case oper_great:
      return createGreat(l, r, node->actual_type);
    case oper_great_equal:
      return createGreatEqual(l, r, node->actual_type);
    case oper_equal:
      return createEqual(l, r, node->actual_type);
    case oper_not_equal:
      return createNotEqual(l, r, node->actual_type);
    case oper_and:
      return builder->CreateAnd(l, r);
    default:
      printf("no supported\n");
      return NULL;
  }
}

llvm::Value* codegen_column(QColcumns* node, Schema* schema,
                            llvm::Value* tuple_addr) {
  /* If the following assert does not hold, we should conduct
   * the type promotion.
   */

  llvm::Value* value;

  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  unsigned offset = schema->getColumnOffset(node->id);

  // cast offset from unsigned to LLVM::Int64
  llvm::Value* value_off = ConstantInt::get(
      llvm::Type::getInt64Ty(llvm::getGlobalContext()), (int64_t) offset,
      false);

  // cast tuple_addr from LLVM::PtrInt1 to LLVM::Int64
  tuple_addr = builder->CreatePtrToInt(
      tuple_addr, llvm::Type::getInt64Ty(llvm::getGlobalContext()));

  // add LLVM::Int64 with LLVM::Int64
  llvm::Value* column_addr = builder->CreateAdd(value_off, tuple_addr);

  switch (node->actual_type) {
    case t_int:
      // cast from LLVM::Int64 to LLVM::PtrInt32
      column_addr = builder->CreateIntToPtr(
          column_addr,
          llvm::PointerType::getUnqual(
              llvm::Type::getInt32Ty(llvm::getGlobalContext())));

      // create a LLVM::Int32 and return
      value = builder->CreateLoad(column_addr);
      break;
    case t_float:
      // cast LLVM::Int64 to LLVM::PtrFloat
      column_addr = builder->CreateIntToPtr(
          column_addr,
          llvm::PointerType::getUnqual(
              llvm::Type::getFloatTy(llvm::getGlobalContext())));
      // create a LLVM::Float and return
      value = builder->CreateLoad(column_addr);
      break;
    case t_u_long:
      //cast LLVM:LINT64 to LLVM::int64Ptr
      column_addr = builder->CreateIntToPtr(
          column_addr,
          llvm::PointerType::getUnqual(
              llvm::Type::getInt64Ty(llvm::getGlobalContext())));
      // create a LLVM::Int64 and return
      value = builder->CreateLoad(column_addr);
      break;
    case t_double:
      //cast LLVM:INT64 to LLVM::DoublePtr
      column_addr = builder->CreateIntToPtr(
          column_addr,
          llvm::PointerType::getUnqual(
              llvm::Type::getDoubleTy(llvm::getGlobalContext())));
      //create a LLVM::Double and return
      value = builder->CreateLoad(column_addr);
      break;
    case t_boolean:
      //cast LLVM:INT64 to LLVM::Int1Ptr
      column_addr = builder->CreateIntToPtr(
          column_addr,
          llvm::PointerType::getUnqual(
              llvm::Type::getInt1Ty(llvm::getGlobalContext())));
      //create a LLVM::Int1 and return
      value = builder->CreateLoad(column_addr);
      break;
    case t_decimal:
    case t_date:
      //cast LLVM:INT64 to LLVM::INT8Ptr
      column_addr = builder->CreateIntToPtr(
          column_addr,
          llvm::PointerType::getUnqual(
              llvm::Type::getInt8Ty(llvm::getGlobalContext())));
      value = column_addr;
      break;

    default:
      printf("no supported\n");
      return 0;

  }

  return value;
}

bool storeTheReturnValue(llvm::Value* value, llvm::Value* dest_ptr,
                         QNode* node) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (node->return_type) {
    case t_int:
      //cast return_addr to LLVM::Int32Ptr
      dest_ptr = builder->CreatePointerCast(
          dest_ptr,
          llvm::PointerType::getUnqual(
              llvm::Type::getInt32Ty(llvm::getGlobalContext())));

      //flush the return value
      builder->CreateStore(value, dest_ptr);
      return true;
    case t_float:
      //cast return_addr to LLVM::Int32Ptr
      dest_ptr = builder->CreatePointerCast(
          dest_ptr,
          llvm::PointerType::getUnqual(
              llvm::Type::getFloatTy(llvm::getGlobalContext())));

      //flush the return value
      builder->CreateStore(value, dest_ptr);
      return true;
    case t_u_long:
      //cast return_addr to LLVM::int64Ptr
      dest_ptr = builder->CreatePointerCast(
          dest_ptr,
          llvm::PointerType::getUnqual(
              llvm::Type::getInt64Ty(llvm::getGlobalContext())));

      //fluash the return value
      builder->CreateStore(value, dest_ptr);
      return true;
    case t_double:
      //cast return_addr to LLVM::DoublePtr
      dest_ptr = builder->CreatePointerCast(
          dest_ptr,
          llvm::PointerType::getUnqual(
              llvm::Type::getDoubleTy(llvm::getGlobalContext())));

      //flush the result
      builder->CreateStore(value, dest_ptr);
      return true;
    case t_boolean:
      //cast return_addr to LLVM::Int1
      dest_ptr = builder->CreatePointerCast(
          dest_ptr,
          llvm::PointerType::getUnqual(
              llvm::Type::getInt1Ty(llvm::getGlobalContext())));

      //flush the result
      builder->CreateStore(value, dest_ptr);
      return true;
    default:
      printf("no supported\n");
      return false;
  }
}

llvm::Value* createAdd(llvm::Value* l, llvm::Value* r, data_type type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (type) {
    case t_int:
    case t_u_long:
      return builder->CreateAdd(l, r);
    case t_float:
    case t_double:
      return builder->CreateFAdd(l, r);
    default:
      printf("no supported\n");
      return NULL;
  }
}

llvm::Value* createMinus(llvm::Value* l, llvm::Value* r, data_type type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (type) {
    case t_int:
    case t_u_long:
      return builder->CreateSub(l, r);
    case t_float:
    case t_double:
      return builder->CreateFSub(l, r);
    default:
      printf("no supported\n");
      return NULL;
  }
}

llvm::Value* createMultiply(llvm::Value* l, llvm::Value* r, data_type type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (type) {
    case t_int:
    case t_u_long:
      return builder->CreateMul(l, r);
    case t_float:
    case t_double:
      return builder->CreateFMul(l, r);
    default:
      printf("no supported\n");
      return NULL;
  }
}

llvm::Value* createDivide(llvm::Value* l, llvm::Value* r, data_type type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (type) {
    case t_int:
    case t_u_long:
      return builder->CreateUDiv(l, r);
    case t_float:
    case t_double:
      return builder->CreateFDiv(l, r);
    default: {
      printf("no supported\n");
      return NULL;
    }
  }
}

llvm::Value* createMod(llvm::Value* l, llvm::Value* r, data_type type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (type) {
    default: {
      printf("no supported\n");
      return NULL;
    }
  }
}

llvm::Value* createLess(llvm::Value* l, llvm::Value* r, data_type type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (type) {
    case t_int:
      return builder->CreateICmpSLT(l, r);
    case t_u_long:
      return builder->CreateICmpULT(l, r);
    case t_float:
    case t_double:
      return builder->CreateFCmpOLT(l, r, "<");
    case t_decimal: {
      llvm::Function* ff = CreateNValueCompareFunc(oper_less);

      std::vector<lv*> args;
      args.push_back(l);
      args.push_back(r);
      return builder->CreateCall(ff, args);
    }
    case t_date: {
      llvm::Function* ff = CreateDateCompareFunc(oper_less);
      std::vector<lv*> args;
      args.push_back(l);
      args.push_back(r);
      return builder->CreateCall(ff, args);
    }
    default: {
      printf("no supported\n");
      return NULL;
    }
  }
}

llvm::Value* createLessEqual(llvm::Value* l, llvm::Value* r, data_type type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (type) {
    case t_int:
      return builder->CreateICmpSLE(l, r);
    case t_u_long:
      return builder->CreateICmpULE(l, r);
    case t_float:
    case t_double:
      return builder->CreateFCmpOLE(l, r, "<=");
    case t_decimal: {
      llvm::Function* ff = CreateNValueCompareFunc(oper_less_equal);

      std::vector<lv*> args;
      args.push_back(l);
      args.push_back(r);
      return builder->CreateCall(ff, args);
    }
    case t_date: {
      llvm::Function* ff = CreateDateCompareFunc(oper_less_equal);
      std::vector<lv*> args;
      args.push_back(l);
      args.push_back(r);
      return builder->CreateCall(ff, args);
    }
    default: {
      printf("no supported\n");
      return NULL;
    }
  }
}

llvm::Value *createGreat(llvm::Value *l, llvm::Value *r, data_type type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (type) {
    case t_int:
      return builder->CreateICmpSGT(l, r, ">");
    case t_u_long:
      return builder->CreateICmpUGT(l, r, "u>");
    case t_float:
    case t_double:
      return builder->CreateFCmpOGT(l, r, ">");
    case t_decimal: {
      llvm::Function* ff = CreateNValueCompareFunc(oper_great);

      std::vector<lv*> args;
      args.push_back(l);
      args.push_back(r);
      return builder->CreateCall(ff, args);
    }
    case t_date: {
      llvm::Function* ff = CreateDateCompareFunc(oper_great);

      std::vector<lv*> args;
      args.push_back(l);
      args.push_back(r);
      return builder->CreateCall(ff, args);
    }
    default: {
      printf("no supported\n");
      return NULL;
    }
  }
}

llvm::Value *createGreatEqual(llvm::Value *l, llvm::Value *r, data_type type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (type) {
    case t_int:
      return builder->CreateICmpSGT(l, r, "s>=");
    case t_u_long:
      return builder->CreateICmpUGT(l, r, "u>=");
    case t_float:
    case t_double:
      return builder->CreateFCmpOGT(l, r, "f>=");
    case t_decimal: {
      llvm::Function* ff = CreateNValueCompareFunc(oper_great_equal);

      std::vector<lv*> args;
      args.push_back(l);
      args.push_back(r);
      return builder->CreateCall(ff, args);
    }
    case t_date: {
      llvm::Function* ff = CreateDateCompareFunc(oper_great_equal);

      std::vector<lv*> args;
      args.push_back(l);
      args.push_back(r);
      return builder->CreateCall(ff, args);
    }
    default: {
      printf("no supported\n");
      return NULL;
    }
  }
}

llvm::Value *createEqual(llvm::Value *l, llvm::Value *r, data_type type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (type) {
    case t_int:
    case t_u_long: {
      return builder->CreateICmpEQ(l, r, "=");
    }
    case t_float:
    case t_double:
      return builder->CreateFCmpOEQ(l, r, "=");
    case t_decimal: {
      llvm::Function* ff = CreateNValueCompareFunc(oper_equal);

      std::vector<lv*> args;
      args.push_back(l);
      args.push_back(r);
      return builder->CreateCall(ff, args);
    }
    default: {
      printf("no supported\n");
      return NULL;
    }
  }
}

llvm::Value *createNotEqual(llvm::Value *l, llvm::Value *r, data_type type) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (type) {
    case t_int:
    case t_u_long:
      return builder->CreateICmpNE(l, r, "!=");
    case t_float:
    case t_double:
      // FCMPUNE means if unordered or unequal return true
      // What's ordered? Unordered must lead to unequal, right?
      return builder->CreateFCmpUNE(l, r, "!=");
    case t_decimal: {
      llvm::Function* ff = CreateNValueCompareFunc(oper_equal);

      std::vector<lv*> args;
      args.push_back(l);
      args.push_back(r);
      return builder->CreateCall(ff, args);
    }
    default: {
      printf("no supported\n");
      return NULL;
    }
  }
}

llvm::Value* codegen_const(QExpr* node) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (node->actual_type) {
    case t_int:
      return llvm::ConstantInt::get(
          llvm::Type::getInt32Ty(llvm::getGlobalContext()),
          atoi(node->const_value.c_str()), true);
    case t_u_long:
      return llvm::ConstantInt::get(
          llvm::Type::getInt64Ty(llvm::getGlobalContext()),
          atol(node->const_value.c_str()), true);
    case t_float:
      return llvm::ConstantFP::get(
          llvm::Type::getFloatTy(llvm::getGlobalContext()),
          atof(node->const_value.c_str()));
    case t_double:
      return llvm::ConstantFP::get(
          llvm::Type::getDoubleTy(llvm::getGlobalContext()),
          atof(node->const_value.c_str()));
    default: {
      printf("no supported\n");
      return NULL;
    }
  }
}

llvm::Value* typePromotion(llvm::Value* v, data_type old_ty,
                           data_type target_ty) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  switch (old_ty) {
    case t_int: {
      switch (target_ty) {
        case t_float:
          return builder->CreateSIToFP(
              v, llvm::Type::getFloatTy(llvm::getGlobalContext()));
        case t_u_long:
          return builder->CreateIntCast(
              v, llvm::Type::getInt64Ty(llvm::getGlobalContext()), true, "long");
        case t_double:
          return builder->CreateSIToFP(
              v, llvm::Type::getDoubleTy(llvm::getGlobalContext()));
        default:
          printf("no supported\n");
          return NULL;
      }
      break;
    }
    case t_u_long: {
      switch (target_ty) {
        case t_float:
          return builder->CreateSIToFP(
              v, llvm::Type::getFloatTy(llvm::getGlobalContext()));
        case t_double:
          return builder->CreateSIToFP(
              v, llvm::Type::getDoubleTy(llvm::getGlobalContext()));
        default:
          printf("no supported\n");
          return NULL;
      }
      break;
    }
    case t_float: {
      switch (target_ty) {
        case t_double:
          return builder->CreateFPCast(
              v, llvm::Type::getDoubleTy(llvm::getGlobalContext()));
        default:
          printf("no supported\n");
          return NULL;
      }
      break;
    }
    default:
      printf("no supported\n");
      return NULL;
  }
}

using namespace llvm;

llvm::Function* getExprLLVMFuncForTwoTuples(QNode* qnode, Schema* l_schema,
                                            Schema* r_schema) {
  /* create a function prototype:
   * void Function(void* l_tuple_addr, void* r_tuple_addr, void* return)
   * */
  std::vector<llvm::Type *> parameter_types;
  parameter_types.push_back(
      llvm::PointerType::getUnqual(
          llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
  parameter_types.push_back(
      llvm::PointerType::getUnqual(
          llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
  parameter_types.push_back(
      llvm::PointerType::getUnqual(
          llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getVoidTy(llvm::getGlobalContext()), parameter_types, false);
  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "a",
      CodeGenerator::getInstance()->getModule());

  /* create function entry */
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(),
                                                  "entry", F);
  CodeGenerator::getInstance()->getBuilder()->SetInsertPoint(BB);

  /* get the parameter value of the function */
  llvm::Function::arg_iterator AI = F->arg_begin();
  llvm::Value* l_tuple_addr = AI++;
  l_tuple_addr->setName("l_tuple_addr");
  llvm::Value* r_tuple_addr = AI++;
  r_tuple_addr->setName("r_tuple_addr");
  llvm::Value* return_addr = AI++;
  return_addr->setName("return_addr");

  /* try to generate the code and get the return value
   * If we cannot generate the code, return_value is NULL*/
  llvm::Value* return_value = codegen(qnode, l_schema, l_tuple_addr, r_schema,
                                      r_tuple_addr);

  if (!return_value) {
    CodeGenerator::getInstance()->release();
    return NULL;
  }

  /* store the value to the return address */
  if (!storeTheReturnValue(return_value, return_addr, qnode)) {
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
  return F;
}

QNode* createEqualJoinExpression(Schema* l_s, Schema* r_s,
                                 std::vector<unsigned>& l_join_index,
                                 std::vector<unsigned>& r_join_index) {
  assert(l_join_index.size() == r_join_index.size() && l_join_index.size() > 0);
  QNode* ret = 0;
  for (int i = 0; i < l_join_index.size(); i++) {
    QColcumns* l_column = new QColcumns("L", "L.A",
                                        l_s->getcolumn(l_join_index[i]).type,
                                        "L.A");
    l_column->id = l_join_index[i];
    l_column->tab_index = 0;
    QColcumns* r_column = new QColcumns("R", "R.A",
                                        r_s->getcolumn(r_join_index[i]).type,
                                        "R.A");
    r_column->id = r_join_index[i];
    r_column->tab_index = 1;
    data_type return_type = TypePromotion::arith_type_promotion_map
        [l_s->getcolumn(l_join_index[i]).type]
         [r_s->getcolumn(r_join_index[i]).type];
    l_column->return_type = return_type;
    r_column->return_type = return_type;

    QExpr_binary* AND = new QExpr_binary(l_column, r_column, return_type,
                                         oper_equal, t_qexpr_cmp, "==");
    AND->return_type = t_boolean;
    if (ret == 0) {
      ret = AND;
    } else {
      QExpr_binary* Upper_AND = new QExpr_binary(AND, ret, t_boolean, oper_and,
                                                 t_qexpr_cmp, "AND");
      Upper_AND->return_type = t_boolean;
      ret = Upper_AND;
    }
  }
  return ret;
}

LLVMMemcpy getMemcpy(unsigned length) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  CodeGenerator::getInstance()->lock();
  std::vector<llvm::Type *> parameter_types;
  parameter_types.push_back(
      llvm::PointerType::getUnqual(
          llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
  parameter_types.push_back(
      llvm::PointerType::getUnqual(
          llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getVoidTy(llvm::getGlobalContext()), parameter_types, false);
  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "a",
      CodeGenerator::getInstance()->getModule());

  /* create function entry */
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(),
                                                  "entry", F);
  CodeGenerator::getInstance()->getBuilder()->SetInsertPoint(BB);

  /* get the parameter value of the function */
  llvm::Function::arg_iterator AI = F->arg_begin();
  llvm::Value* desc = AI++;  //@ assume we have got the tuple addr.
  desc->setName("desc");
  llvm::Value* src = AI++;
  src->setName("src");

  builder->CreateMemCpy(desc, src, length, 4);

  builder->CreateRetVoid();

  verifyFunction(*F);
//	F->dump();
  CodeGenerator::getInstance()->getFunctionPassManager()->run(*F);
  LLVMMemcpy ret = (LLVMMemcpy) CodeGenerator::getInstance()
      ->getExecutionEngine()->getPointerToFunction(F);
  CodeGenerator::getInstance()->release();
  return ret;

}

LLVMMemcat getMemcat(unsigned length1, unsigned length2) {
  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  CodeGenerator::getInstance()->lock();
  std::vector<llvm::Type *> parameter_types;
  parameter_types.push_back(
      llvm::PointerType::getUnqual(
          llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
  parameter_types.push_back(
      llvm::PointerType::getUnqual(
          llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
  parameter_types.push_back(
      llvm::PointerType::getUnqual(
          llvm::IntegerType::getInt8Ty(llvm::getGlobalContext())));
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getVoidTy(llvm::getGlobalContext()), parameter_types, false);
  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "a",
      CodeGenerator::getInstance()->getModule());

  /* create function entry */
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(),
                                                  "entry", F);
  CodeGenerator::getInstance()->getBuilder()->SetInsertPoint(BB);

  /* get the parameter value of the function */
  llvm::Function::arg_iterator AI = F->arg_begin();
  llvm::Value* desc = AI++;  //@ assume we have got the tuple addr.
  desc->setName("desc");
  llvm::Value* src1 = AI++;
  src1->setName("src1");
  llvm::Value* src2 = AI++;
  src2->setName("src2");

  builder->CreateMemCpy(desc, src1, length1, 4);

  desc = builder->CreatePtrToInt(
      desc, llvm::Type::getInt64Ty(llvm::getGlobalContext()));
  llvm::Value* const_length2 = llvm::ConstantInt::get(
      llvm::Type::getInt64Ty(llvm::getGlobalContext()), length1);
  desc = builder->CreateAdd(desc, const_length2);
  desc = builder->CreateIntToPtr(
      desc,
      llvm::PointerType::getUnqual(
          llvm::Type::getInt8Ty(llvm::getGlobalContext())));
  builder->CreateMemCpy(desc, src2, length2, 4);
  builder->CreateRetVoid();

  verifyFunction(*F);
//	F->dump();
  CodeGenerator::getInstance()->getFunctionPassManager()->run(*F);
  LLVMMemcat ret = (LLVMMemcat) CodeGenerator::getInstance()
      ->getExecutionEngine()->getPointerToFunction(F);
  CodeGenerator::getInstance()->release();
  return ret;
}

//Module* makeLLVMModule() {
// // Module Construction
// Module* mod = new Module("/tmp/webcompile/_31804_0.bc", getGlobalContext());
// mod->setDataLayout("0x3f54b40");
// mod->setTargetTriple("x86_64-ellcc-linux");
//
// // Type Definitions
// std::vector<Type*>FuncTy_0_args;
// PointerType* Int32PtrType = PointerType::get(IntegerType::get(mod->getContext(), 32), 0);
//
// FuncTy_0_args.push_back(Int32PtrType);
// FunctionType* FuncTy_0 = FunctionType::get(
//  /*Result=*/Type::getVoidTy(mod->getContext()),
//  /*Params=*/FuncTy_0_args,
//  /*isVarArg=*/false);
//
//
// // Function Declarations
//
// Function* func__Z4fuckRi = mod->getFunction("_Z4fuckRi");
// if (!func__Z4fuckRi) {
// func__Z4fuckRi = Function::Create(
//  /*Type=*/FuncTy_0,
//  /*Linkage=*/GlobalValue::ExternalLinkage,
//  /*Name=*/"_Z4fuckRi", mod);
// func__Z4fuckRi->setCallingConv(CallingConv::C);
// }
// AttributeSet func__Z4fuckRi_PAL;
// {
//  SmallVector<AttributeSet, 4> Attrs;
//  AttributeSet PAS;
//   {
//    AttrBuilder B;
//    PAS = AttributeSet::get(mod->getContext(), 1U, B);
//   }
//
//  Attrs.push_back(PAS);
//  {
//   AttrBuilder B;
//   B.addAttribute(Attribute::NoUnwind);
//   PAS = AttributeSet::get(mod->getContext(), ~0U, B);
//  }
//
// Attrs.push_back(PAS);
// func__Z4fuckRi_PAL = AttributeSet::get(mod->getContext(), Attrs);
//
//}
//func__Z4fuckRi->setAttributes(func__Z4fuckRi_PAL);
//
//// Global Variable Declarations
//
//
//// Constant Definitions
//ConstantInt* const10 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("1"), 10));
//
//// Global Variable Definitions
//
//// Function Definitions
//
//// Function: _Z4fuckRi (func__Z4fuckRi)
//{
// Function::arg_iterator args = func__Z4fuckRi->arg_begin();
// Value* ptr_a = args++;
// ptr_a->setName("a");
//
// BasicBlock* label_entry = BasicBlock::Create(mod->getContext(), "entry",func__Z4fuckRi,0);
//
// // Block entry (label_entry)
// AllocaInst* ptr_a_addr = new AllocaInst(Int32PtrType, "a.addr", label_entry);
// ptr_a_addr->setAlignment(8);
// StoreInst* void_4 = new StoreInst(ptr_a, ptr_a_addr, false, label_entry);
// void_4->setAlignment(8);
// LoadInst* ptr_5 = new LoadInst(ptr_a_addr, "", false, label_entry);
// ptr_5->setAlignment(8);
// StoreInst* void_6 = new StoreInst(const10, ptr_5, false, label_entry);
// void_6->setAlignment(4);
// ReturnInst::Create(mod->getContext(), label_entry);
//
//}
//
//return mod;
//}
void test_reference() {
  llvm::IRBuilder<> * builder = CodeGenerator::getInstance()->getBuilder();

  llvm::FunctionType *FT = FunctionType::get(
      /*Result=*/Type::getVoidTy(llvm::getGlobalContext()),
      /*Params=*/PointerType::get(
          IntegerType::get(llvm::getGlobalContext(), 32), 0),
      /*isVarArg=*/false);
  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, "a",
      CodeGenerator::getInstance()->getModule());

  ConstantInt* const10 = ConstantInt::get(llvm::getGlobalContext(),
                                          APInt(32, StringRef("1"), 10));

  BasicBlock* label_entry = BasicBlock::Create(llvm::getGlobalContext(),
                                               "entry", F, 0);
  builder->SetInsertPoint(label_entry);

  llvm::Function::arg_iterator args = F->arg_begin();
  Value* ptr_a = args++;
  ptr_a->setName("a");

  llvm::AllocaInst* ptr_a_addr = builder->CreateAlloca(
      llvm::PointerType::getUnqual(
          llvm::Type::getInt32Ty(llvm::getGlobalContext())));

  builder->CreateStore(ptr_a, ptr_a_addr);

  Value* ptr_5 = builder->CreateLoad(ptr_a_addr, "");

  builder->CreateStore(const10, ptr_5);

  builder->CreateRetVoid();

  verifyFunction(*F);
//		F->dump();
//	 if(verifyModule(*CodeGenerator::getInstance()->getModule())){
//		llvm::outs()<<"errors!";
//	 }
  CodeGenerator::getInstance()->getFunctionPassManager()->run(*F);
//	 llvm::outs()<<*CodeGenerator::getInstance()->getModule()<<"\n";

  typedef void (*func)(int &);

  func ret = (func) CodeGenerator::getInstance()->getExecutionEngine()
      ->getPointerToFunction(F);
  int a = 1024;
  ret(a);
  printf("result %d!\n", a);

}
void test_while() {
//	llvm::IRBuilder<> * builder=CodeGenerator::getInstance()->getBuilder();
//
//    llvm::FunctionType *FT =FunctionType::get(
//    		  /*Result=*/Type::getInt32Ty(llvm::getGlobalContext()),
//    		  	  	  	 Type::getInt32Ty(llvm::getGlobalContext()),
//    		  /*isVarArg=*/false);
//    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "a", CodeGenerator::getInstance()->getModule());
//
//
//    BasicBlock* label_entry = BasicBlock::Create(llvm::getGlobalContext(), "entry",F,0);
//    builder->SetInsertPoint(label_entry);
//
//
//    llvm::Function::arg_iterator args = F->arg_begin();
//    llvm::Value* a=args++;
//    a->setName("a");
//
//    llvm::Value* tmp=ConstantInt::get(llvm::getGlobalContext(), APInt(32, StringRef("2"), 10));
//
//
//
//
//    builder->CreateRet(tmp);
//
//	verifyFunction(*F);
//		F->dump();
////	 if(verifyModule(*CodeGenerator::getInstance()->getModule())){
////		llvm::outs()<<"errors!";
////	 }
//	 CodeGenerator::getInstance()->getFunctionPassManager()->run(*F);
////	 llvm::outs()<<*CodeGenerator::getInstance()->getModule()<<"\n";
//
//	 typedef void (*func)(int &);
//
//	 func ret=CodeGenerator::getInstance()->getExecutionEngine()->getPointerToFunction(F);
//	 int a=1024;
//	 ret(a);
//	 printf("result %d!\n",a);

}

void myllvm::test() {
  // Type Definitions
  Module* mod = CodeGenerator::getInstance()->getModule();
  std::vector<Type*> FuncTy_0_args;
  FuncTy_0_args.push_back(IntegerType::get(mod->getContext(), 32));
  FunctionType* FuncTy_0 = FunctionType::get(
  /*Result=*/IntegerType::get(mod->getContext(), 32),
                                             /*Params=*/FuncTy_0_args,
                                             /*isVarArg=*/false);

  PointerType* PointerTy_1 = PointerType::get(
      IntegerType::get(mod->getContext(), 32), 0);

  // Function Declarations

  Function* func__Z4funci = mod->getFunction("_Z4funci");
  if (!func__Z4funci) {
    func__Z4funci = Function::Create(
    /*Type=*/FuncTy_0,
                                     /*Linkage=*/GlobalValue::ExternalLinkage,
                                     /*Name=*/"_Z4funci",
                                     mod);
    func__Z4funci->setCallingConv(CallingConv::C);
  }
  AttributeSet func__Z4funci_PAL;
  {
    SmallVector<AttributeSet, 4> Attrs;
    AttributeSet PAS;
    {
      AttrBuilder B;
      B.addAttribute(llvm::Attribute::NoUnwind);
      PAS = AttributeSet::get(mod->getContext(), ~0U, B);
    }

    Attrs.push_back(PAS);
    func__Z4funci_PAL = AttributeSet::get(mod->getContext(), Attrs);

  }
  func__Z4funci->setAttributes(func__Z4funci_PAL);

  // Global Variable Declarations

  // Constant Definitions
  ConstantInt* const_int32_2 = ConstantInt::get(mod->getContext(),
                                                APInt(32, StringRef("1"), 10));
  ConstantInt* const_int32_3 = ConstantInt::get(mod->getContext(),
                                                APInt(32, StringRef("10"), 10));

  // Global Variable Definitions

  // Function Definitions

  // Function: _Z4funci (func__Z4funci)
  {
    Function::arg_iterator args = func__Z4funci->arg_begin();
    Value* int32_c = args++;
    int32_c->setName("c");

    BasicBlock* label_entry = BasicBlock::Create(mod->getContext(), "entry",
                                                 func__Z4funci, 0);
    BasicBlock* label_while_cond = BasicBlock::Create(mod->getContext(),
                                                      "while.cond",
                                                      func__Z4funci, 0);
    BasicBlock* label_while_body = BasicBlock::Create(mod->getContext(),
                                                      "while.body",
                                                      func__Z4funci, 0);
    BasicBlock* label_while_end = BasicBlock::Create(mod->getContext(),
                                                     "while.end", func__Z4funci,
                                                     0);

    // Block entry (label_entry)
    AllocaInst* ptr_c_addr = new AllocaInst(
        IntegerType::get(mod->getContext(), 32), "c.addr", label_entry);
    ptr_c_addr->setAlignment(4);
    AllocaInst* ptr_b = new AllocaInst(IntegerType::get(mod->getContext(), 32),
                                       "b", label_entry);
    ptr_b->setAlignment(4);
    StoreInst* void_4 = new StoreInst(int32_c, ptr_c_addr, false, label_entry);
    void_4->setAlignment(4);
    StoreInst* void_5 = new StoreInst(const_int32_3, ptr_b, false, label_entry);
    void_5->setAlignment(4);
    BranchInst::Create(label_while_cond, label_entry);

    // Block while.cond (label_while_cond)
    LoadInst* int32_7 = new LoadInst(ptr_b, "", false, label_while_cond);
    int32_7->setAlignment(4);
    LoadInst* int32_8 = new LoadInst(ptr_c_addr, "", false, label_while_cond);
    int32_8->setAlignment(4);
    ICmpInst* int1_cmp = new ICmpInst(*label_while_cond, ICmpInst::ICMP_SLT,
                                      int32_7, int32_8, "cmp");
    BranchInst::Create(label_while_body, label_while_end, int1_cmp,
                       label_while_cond);

    // Block while.body (label_while_body)
    LoadInst* int32_10 = new LoadInst(ptr_b, "", false, label_while_body);
    int32_10->setAlignment(4);
    LoadInst* int32_11 = new LoadInst(ptr_b, "", false, label_while_body);
    int32_11->setAlignment(4);
    BinaryOperator* int32_mul = BinaryOperator::Create(Instruction::Mul,
                                                       int32_10, int32_11,
                                                       "mul", label_while_body);
    StoreInst* void_12 = new StoreInst(int32_mul, ptr_b, false,
                                       label_while_body);
    void_12->setAlignment(4);
    BranchInst::Create(label_while_cond, label_while_body);

    // Block while.end (label_while_end)
    LoadInst* int32_14 = new LoadInst(ptr_b, "", false, label_while_end);
    int32_14->setAlignment(4);
    ReturnInst::Create(mod->getContext(), int32_14, label_while_end);

  }
  verifyFunction(*func__Z4funci);
  typedef int (*func)(int);

  func ret = (func) CodeGenerator::getInstance()->getExecutionEngine()
      ->getPointerToFunction(func__Z4funci);

  printf("f(%d)=%d\n", 50, ret(50));

}

void myllvm::test1() {
  // Type Definitions
  Module* mod = CodeGenerator::getInstance()->getModule();
  IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  std::vector<Type*> FuncTy_0_args;
  FuncTy_0_args.push_back(IntegerType::get(mod->getContext(), 32));
  FunctionType* FuncTy_0 = FunctionType::get(
  /*Result=*/IntegerType::get(mod->getContext(), 32),
                                             /*Params=*/FuncTy_0_args,
                                             /*isVarArg=*/false);

  PointerType* PointerTy_1 = PointerType::get(
      IntegerType::get(mod->getContext(), 32), 0);

  // Function Declarations

  Function* func__Z4funci = mod->getFunction("_Z4funci");
  if (!func__Z4funci) {
    func__Z4funci = Function::Create(
    /*Type=*/FuncTy_0,
                                     /*Linkage=*/GlobalValue::ExternalLinkage,
                                     /*Name=*/"_Z4funci",
                                     mod);
    func__Z4funci->setCallingConv(CallingConv::C);
  }
  AttributeSet func__Z4funci_PAL;
  {
    SmallVector<AttributeSet, 4> Attrs;
    AttributeSet PAS;
    {
      AttrBuilder B;
      B.addAttribute(llvm::Attribute::NoUnwind);
      PAS = AttributeSet::get(mod->getContext(), ~0U, B);
    }

    Attrs.push_back(PAS);
    func__Z4funci_PAL = AttributeSet::get(mod->getContext(), Attrs);

  }
  func__Z4funci->setAttributes(func__Z4funci_PAL);

  // Global Variable Declarations

  // Constant Definitions
  ConstantInt* const_int32_2 = ConstantInt::get(mod->getContext(),
                                                APInt(32, StringRef("1"), 10));
  ConstantInt* const_int32_3 = ConstantInt::get(mod->getContext(),
                                                APInt(32, StringRef("10"), 10));

  // Global Variable Definitions

  // Function Definitions

  // Function: _Z4funci (func__Z4funci)
  {
    Function::arg_iterator args = func__Z4funci->arg_begin();
    Value* int32_c = args++;
    int32_c->setName("c");

    BasicBlock* label_entry = BasicBlock::Create(mod->getContext(), "entry",
                                                 func__Z4funci, 0);
    BasicBlock* label_while_cond = BasicBlock::Create(mod->getContext(),
                                                      "while.cond",
                                                      func__Z4funci, 0);
    BasicBlock* label_while_body = BasicBlock::Create(mod->getContext(),
                                                      "while.body",
                                                      func__Z4funci, 0);
    BasicBlock* label_while_end = BasicBlock::Create(mod->getContext(),
                                                     "while.end", func__Z4funci,
                                                     0);
    builder->SetInsertPoint(label_entry);

    // Block entry (label_entry)

    AllocaInst* ptr_c_addr = builder->CreateAlloca(
        IntegerType::get(mod->getContext(), 32));
    ptr_c_addr->setAlignment(4);
    AllocaInst* ptr_b = builder->CreateAlloca(
        IntegerType::get(mod->getContext(), 32));
    ptr_b->setAlignment(4);
    builder->CreateStore(int32_c, ptr_c_addr);
    builder->CreateStore(const_int32_3, ptr_b);
//	  StoreInst* void_4 = new StoreInst(int32_c, ptr_c_addr, false, label_entry);
//	  void_4->setAlignment(4);
//	  StoreInst* void_5 = new StoreInst(const_int32_3, ptr_b, false, label_entry);
//	  void_5->setAlignment(4);
    builder->CreateBr(label_while_cond);
//	  BranchInst::Create(label_while_cond, label_entry);

    // Block while.cond (label_while_cond)
    builder->SetInsertPoint(label_while_cond);
    LoadInst* int32_7 = builder->CreateLoad(ptr_b);
    LoadInst* int32_8 = builder->CreateLoad(ptr_c_addr);
    Value* int1_cmp = builder->CreateICmpSLT(int32_7, int32_8);
    builder->CreateCondBr(int1_cmp, label_while_body, label_while_end);
//	  LoadInst* int32_7 = new LoadInst(ptr_b, "", false, label_while_cond);
//	  int32_7->setAlignment(4);
//	  LoadInst* int32_8 = new LoadInst(ptr_c_addr, "", false, label_while_cond);
//	  int32_8->setAlignment(4);
//	  ICmpInst* int1_cmp = new ICmpInst(*label_while_cond, ICmpInst::ICMP_SLT, int32_7, int32_8, "cmp");
//	  BranchInst::Create(label_while_body, label_while_end, int1_cmp, label_while_cond);

    // Block while.body (label_while_body)

    builder->SetInsertPoint(label_while_body);
    LoadInst* int32_10 = builder->CreateLoad(ptr_b);
    LoadInst* int32_11 = builder->CreateLoad(ptr_b);
    Value* int32_mul = builder->CreateMul(int32_10, int32_11);
    builder->CreateStore(int32_mul, ptr_b);
    builder->CreateBr(label_while_cond);
//	  LoadInst* int32_10 = new LoadInst(ptr_b, "", false, label_while_body);
//	  int32_10->setAlignment(4);
//	  LoadInst* int32_11 = new LoadInst(ptr_b, "", false, label_while_body);
//	  int32_11->setAlignment(4);
//	  BinaryOperator* int32_mul = BinaryOperator::Create(Instruction::Mul, int32_10, int32_11, "mul", label_while_body);
//	  StoreInst* void_12 = new StoreInst(int32_mul, ptr_b, false, label_while_body);
//	  void_12->setAlignment(4);
//	  BranchInst::Create(label_while_cond, label_while_body);

    // Block while.end (label_while_end)
    builder->SetInsertPoint(label_while_end);
    LoadInst* int32_14 = builder->CreateLoad(ptr_b);
    builder->CreateRet(int32_14);
//	  LoadInst* int32_14 = new LoadInst(ptr_b, "", false, label_while_end);
//	  int32_14->setAlignment(4);
//	  ReturnInst::Create(mod->getContext(), int32_14, label_while_end);

  }
  verifyFunction(*func__Z4funci);
  typedef int (*func)(int);

  func ret = (func) CodeGenerator::getInstance()->getExecutionEngine()
      ->getPointerToFunction(func__Z4funci);

  printf("f(%d)=%d\n", 50, ret(50));

}

