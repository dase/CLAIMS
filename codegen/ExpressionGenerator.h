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
llvm::Value* codegen_const(QExpr* node);
llvm::Value* codegen(QNode* qnode, Schema* schema, llvm::Value* tuple_addr);
llvm::FunctionType* createFunctionPrototype();
bool storeTheReturnValue(llvm::Value* value, llvm::Value* dest_prt,QNode* node);

/* create add instruction
 * Note: l and r  must be in the same type. */
llvm::Value* createAdd(llvm::Value* l,llvm::Value* r, data_type type);

/* create minus instruction
 * Note: l and r must be in the same type .
 */
llvm::Value* createMinus(llvm::Value* l,llvm::Value* r,data_type type);

/* create multiply instruction
 * Note: l and r should be in the same type.
 */
llvm::Value* createMultiply(llvm::Value* l, llvm::Value* r, data_type type);

/* create divide instruction
 * Note: l and r should be in the same type.
 */
llvm::Value* createDivide(llvm::Value* l, llvm::Value* r ,data_type type);

/* create less instruction
 * Note: l and r should be in the same type.
 */
llvm::Value* createLess(llvm::Value* l, llvm::Value* r ,data_type type);

/* conduct the type promotion and return the promoted value */
llvm::Value* typePromotion(llvm::Value* v,data_type old_ty, data_type target_ty);



#endif /* EXPRESSIONGENERATOR_H_ */
