/*
 * ExpressionGenerator.h
 *
 *  Created on: Mar 21, 2015
 *      Author: wangli
 */

#ifndef EXPRESSIONGENERATOR_H_
#define EXPRESSIONGENERATOR_H_
#include "../common/Expression/qnode.h"
#include "../common/Schema/Schema.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IR/DerivedTypes.h"

using namespace std;
typedef void (*expr_func_prototype)(void*,void*);

expr_func_prototype getExprFunc(QNode* qnode, Schema* schema);

llvm::Value* codegen_binary_cal(llvm::Value* lvalue, llvm::Value* rvalue, QExpr_binary* node);
llvm::Value* codegen_column(QColcumns* node, Schema* schema,llvm::Value* tuple_addr);
llvm::Value* codegen(QNode* qnode, Schema* schema, llvm::Value* tuple_addr);
llvm::FunctionType* createFunctionPrototype();
void storeTheReturnValue(llvm::Value* value, llvm::Value* dest_prt,QNode* node);


#endif /* EXPRESSIONGENERATOR_H_ */
