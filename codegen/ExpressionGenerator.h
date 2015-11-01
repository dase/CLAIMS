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
typedef void (*expr_func)(void*, void*);
typedef void (*ExprFuncTwoTuples)(void*, void*, void*);
typedef void (*LLVMMemcpy)(void* desc, void* src);
typedef void (*LLVMMemcat)(void* desc, void* src1, void* src2);

typedef void (*filter_process_func)(void*, int*, int, void*, int*, int);
namespace myllvm {
using namespace llvm;
void test();
void test1();
}
static void process_func(char* b_start, int * b_cur_addr, int b_tuple_count,
                         char* c_start, int * c_cur_addr, int c_tuple_count,
                         const int length, expr_func ff) {
  int b_cur = *b_cur_addr;
  int c_cur = *c_cur_addr;
  while (c_cur < c_tuple_count) {
    bool ret;
    char* c_tuple_addr = c_start + length * c_cur;
    ff(c_tuple_addr, &ret);
    bool pass = ret;
    if (pass) {
      if (b_cur < b_tuple_count) {
        char* b_tuple_addr = b_start + length * b_cur;
        b_cur = b_cur + 1;
        memcpy(b_tuple_addr, c_tuple_addr, length);
      } else {
        break;
      }
    }
    c_cur = c_cur + 1;
  }
  *b_cur_addr = b_cur;
  *c_cur_addr = c_cur;
}

LLVMMemcpy getMemcpy(unsigned length);

LLVMMemcat getMemcat(unsigned length1, unsigned length2);

expr_func getExprFunc(QNode* qnode, Schema* schema);

ExprFuncTwoTuples getExprFuncTwoTuples(QNode* qnode, Schema* l_schema,
                                          Schema* r_schema);

filter_process_func getFilterProcessFunc(QNode* qnode, Schema* schema);

llvm::Function* getExprLLVMFucn(QNode* qnode, Schema* schema);

llvm::Function* getExprLLVMFuncForTwoTuples(QNode* qnode, Schema* l_schema,
                                            Schema* r_schema);

llvm::Value* codegen_binary_op(llvm::Value* lvalue, llvm::Value* rvalue,
                               QExpr_binary* node);
llvm::Value* codegen_column(QColcumns* node, Schema* schema,
                            llvm::Value* tuple_addr);
llvm::Value* codegen_const(QExpr* node);
llvm::Value* codegen(QNode* qnode, Schema* schema, llvm::Value* tuple_addr,
                     Schema* r_schema = 0, llvm::Value* r_tuple_addr = 0);
bool storeTheReturnValue(llvm::Value* value, llvm::Value* dest_prt,
                         QNode* node);

/* create add instruction
 * Note: l and r  must be in the same type. */
llvm::Value* createAdd(llvm::Value* l, llvm::Value* r, data_type type);

/* create minus instruction
 * Note: l and r must be in the same type .
 */
llvm::Value* createMinus(llvm::Value* l, llvm::Value* r, data_type type);

/* create multiply instruction
 * Note: l and r should be in the same type.
 */
llvm::Value* createMultiply(llvm::Value* l, llvm::Value* r, data_type type);

/* create divide instruction
 * Note: l and r should be in the same type.
 */
llvm::Value* createDivide(llvm::Value* l, llvm::Value* r, data_type type);

/* create less instruction
 * Note: l and r should be in the same type.
 */
llvm::Value* createLess(llvm::Value* l, llvm::Value* r, data_type type);

/* create less equal instruction
 * Note: l and r should be in the same type.
 */
llvm::Value* createLessEqual(llvm::Value* l, llvm::Value* r, data_type type);

/* create great instruction
 * Note: l and r should be in the same type.
 */
llvm::Value* createGreat(llvm::Value* l, llvm::Value* r, data_type type);

/* create great equal instruction
 * Note: l and r should be in the same type.
 */
llvm::Value* createGreatEqual(llvm::Value* l, llvm::Value* r, data_type type);

/* create equal instruction
 * Note: l and r should be in the same type.
 */
llvm::Value* createEqual(llvm::Value* l, llvm::Value* r, data_type type);

/* create not equal instruction
 * Note: l and r should be in the same type.
 */
llvm::Value* createNotEqual(llvm::Value* l, llvm::Value* r, data_type type);

/* conduct the type promotion and return the promoted value */
llvm::Value* typePromotion(llvm::Value* v, data_type old_ty,
                           data_type target_ty);

/* create expression representing the equal join logic*/
QNode* createEqualJoinExpression(Schema* l_s, Schema* r_s,
                                 std::vector<unsigned>& l_join_index,
                                 std::vector<unsigned>& r_join_index);

void test_reference();

#endif /* EXPRESSIONGENERATOR_H_ */
