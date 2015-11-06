/*
 * codegen_test.h
 *
 *  Created on: Mar 22, 2015
 *      Author: wangli
 */

#ifndef CODEGEN_TEST_H_
#define CODEGEN_TEST_H_
#include <gtest/gtest.h>
#include <map>
#include "CodeGenerator.h"
#include "../common/data_type.h"
#include "../common/Schema/Schema.h"
#include "../common/Expression/qnode.h"
#include "../common/Expression/initquery.h"
#include "ExpressionGenerator.h"
#include "../Environment.h"
#include "CompareFunctonGenerator.h"
#include <llvm/ExecutionEngine/GenericValue.h>
#include "boost/date_time/gregorian/parsers.hpp"

using std::map;

// class CodeGenTestEnvironment : public testing::Environment
//{
// public:
//	virtual void SetUp() {
//		CodeGenerator::getInstance();
//		initialize_arithmetic_type_promotion_matrix();
//		initialize_type_cast_functions();
//		initialize_operator_function();
//	}
//	virtual void TearDown() {
//		delete CodeGenerator::getInstance();
//	}
//};

class CodeGenerationTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    CodeGenerator::getInstance();
    initialize_arithmetic_type_promotion_matrix();
    initialize_type_cast_functions();
    initialize_operator_function();
  }
  static void TearDownTestCase() { delete CodeGenerator::getInstance(); }
};

TEST_F(CodeGenerationTest, AddInt) {
  /* #      1# |2#
   * Tuple: int|int
   *         1   2
   * Express: #1 + #2
   */

  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["a"] = 0;
  column_index["b"] = 1;

  QColcumns* a = new QColcumns("T", "a", t_int, "a");
  QColcumns* b = new QColcumns("T", "b", t_int, "b");

  QExpr_binary* op =
      new QExpr_binary(a, b, t_int, oper_add, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op, t_int, column_index, s);
  CodeGenerator::getInstance();

  expr_func f = getExprFunc(op, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *(int*)tuple = 1;
  *((int*)tuple + 1) = 2;
  int ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op;
  EXPECT_EQ(ret, 3);
}

TEST_F(CodeGenerationTest, AddInt2) {
  /* #      #1 |#2 |#3
   * Tuple: int|int|int
   *         -1   2   3
   * Express: #1 + #2 + #3
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  column_index["#3"] = 2;
  QColcumns* a = new QColcumns("T", "#1", t_int, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_int, "#2");
  QColcumns* c = new QColcumns("T", "#3", t_int, "#3");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_int, oper_add, t_qexpr_cal, "result");
  QExpr_binary* op2 =
      new QExpr_binary(op1, c, t_int, oper_add, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op2, t_int, column_index, s);
  CodeGenerator::getInstance();

  expr_func f = getExprFunc(op2, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *(int*)tuple = -1;
  *((int*)tuple + 1) = 2;
  *((int*)tuple + 2) = 3;
  int ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op2;
  EXPECT_EQ(ret, 4);
}
TEST_F(CodeGenerationTest, AddFloat) {
  /* #      #1    |#2    |#3
   * Tuple: float |float |float
   *         0     -1.2    3.8
   * Express: #1 + #2 + #3
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_float));
  columns.push_back(data_type(t_float));
  columns.push_back(data_type(t_float));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  column_index["#3"] = 2;
  QColcumns* a = new QColcumns("T", "#1", t_float, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_float, "#2");
  QColcumns* c = new QColcumns("T", "#3", t_float, "#3");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_float, oper_add, t_qexpr_cal, "result");
  QExpr_binary* op2 =
      new QExpr_binary(op1, c, t_float, oper_add, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op2, t_float, column_index, s);

  expr_func f = getExprFunc(op2, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *(float*)tuple = 0;
  *((float*)tuple + 1) = -1.2;
  *((float*)tuple + 2) = 3.8;
  float ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op2;
  EXPECT_LE(abs(ret - 2.6), 0.00001);
}
TEST_F(CodeGenerationTest, AddFloatPromote) {
  /* #      #1    |#2    |#3
   * Tuple: int |float |float
   *         1     -1.2    3.8
   * Express: #1 + #2 + #3
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_float));
  columns.push_back(data_type(t_float));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  column_index["#3"] = 2;
  QColcumns* a = new QColcumns("T", "#1", t_int, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_float, "#2");
  QColcumns* c = new QColcumns("T", "#3", t_float, "#3");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_float, oper_add, t_qexpr_cal, "result");
  QExpr_binary* op2 =
      new QExpr_binary(op1, c, t_float, oper_add, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op2, t_float, column_index, s);

  expr_func f = getExprFunc(op2, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *(int*)tuple = 1;
  *((float*)tuple + 1) = -1.2;
  *((float*)tuple + 2) = 3.8;
  float ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op2;
  EXPECT_LE(abs(ret - 3.6), 0.00001);
}
TEST_F(CodeGenerationTest, AddFloatPromote2) {
  /* #      #1    |#2    |#3
   * Tuple: int |float |float
   *         1     -1.2    3.8
   * Express: #1 + #2 + #3
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_float));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  column_index["#3"] = 2;
  QColcumns* a = new QColcumns("T", "#1", t_int, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_int, "#2");
  QColcumns* c = new QColcumns("T", "#3", t_float, "#3");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_int, oper_add, t_qexpr_cal, "result");
  QExpr_binary* op2 =
      new QExpr_binary(op1, c, t_float, oper_add, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op2, t_float, column_index, s);

  expr_func f = getExprFunc(op2, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *(int*)tuple = 1;
  *((int*)tuple + 1) = -2;
  *((float*)tuple + 2) = 3.8;
  float ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op2;
  EXPECT_LE(abs(ret - 2.8), 0.00001);
}
TEST_F(CodeGenerationTest, AddLongPromote) {
  /* #      #1    |#2    |#3
   * Tuple: int   |int   |long
   *         1     -1     200
   * Express: #1 + #2 + #3
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_u_long));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  column_index["#3"] = 2;
  QColcumns* a = new QColcumns("T", "#1", t_int, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_int, "#2");
  QColcumns* c = new QColcumns("T", "#3", t_u_long, "#3");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_int, oper_add, t_qexpr_cal, "result");
  QExpr_binary* op2 =
      new QExpr_binary(op1, c, t_u_long, oper_add, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op2, t_u_long, column_index, s);

  expr_func f = getExprFunc(op2, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *(int*)tuple = 1;
  *((int*)s->getColumnAddess(1, tuple)) = -1;
  *((long*)s->getColumnAddess(2, tuple)) = 200;
  long ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op2;
  EXPECT_LE(ret, 200);
}
TEST_F(CodeGenerationTest, SUB) {
  /* #      #1    |#2    |#3
   * Tuple: int   |int   |long
   *         1     -1     200
   * Express: #1 + #2 + #3
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_u_long));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  column_index["#3"] = 2;
  QColcumns* a = new QColcumns("T", "#1", t_int, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_int, "#2");
  QColcumns* c = new QColcumns("T", "#3", t_u_long, "#3");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_int, oper_minus, t_qexpr_cal, "result");
  QExpr_binary* op2 =
      new QExpr_binary(op1, c, t_u_long, oper_minus, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op2, t_u_long, column_index, s);

  expr_func f = getExprFunc(op2, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *(int*)tuple = 1;
  *((int*)s->getColumnAddess(1, tuple)) = -1;
  *((long*)s->getColumnAddess(2, tuple)) = 200;
  long ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op2;
  EXPECT_LE(ret, -198);
}
TEST_F(CodeGenerationTest, Multiply) {
  /* #      #1    |#2    |#3
   * Tuple: int   |int   |long
   *         1     -1     200
   * Express: #1 + #2 + #3
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_u_long));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  column_index["#3"] = 2;
  QColcumns* a = new QColcumns("T", "#1", t_int, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_int, "#2");
  QColcumns* c = new QColcumns("T", "#3", t_u_long, "#3");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_int, oper_multiply, t_qexpr_cal, "result");
  QExpr_binary* op2 =
      new QExpr_binary(op1, c, t_u_long, oper_multiply, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op2, t_u_long, column_index, s);

  expr_func f = getExprFunc(op2, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *(int*)tuple = 1;
  *((int*)s->getColumnAddess(1, tuple)) = -1;
  *((long*)s->getColumnAddess(2, tuple)) = 200;
  long ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op2;
  EXPECT_LE(ret, -200);
}
TEST_F(CodeGenerationTest, Multiply1) {
  /* #      #1    |#2    |#3
   * Tuple: int   |float |long
   *         2     0.5    3
   * Express: #1 x #2 x #3 = 3
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_float));
  columns.push_back(data_type(t_u_long));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  column_index["#3"] = 2;
  QColcumns* a = new QColcumns("T", "#1", t_int, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_float, "#2");
  QColcumns* c = new QColcumns("T", "#3", t_u_long, "#3");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_float, oper_multiply, t_qexpr_cal, "result");
  QExpr_binary* op2 =
      new QExpr_binary(op1, c, t_float, oper_multiply, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op2, t_float, column_index, s);

  expr_func f = getExprFunc(op2, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *(int*)tuple = 2;
  *((float*)s->getColumnAddess(1, tuple)) = 0.5;
  *((long*)s->getColumnAddess(2, tuple)) = 3;
  float ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op2;
  EXPECT_LE(ret, 3);
}
TEST_F(CodeGenerationTest, Divide) {
  /* #      #1    |#2    |#3
   * Tuple: int   |int   |float
   *         4     3     0.5
   * Express: #1 / #2 / #3 = 2
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_float));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  column_index["#3"] = 2;
  QColcumns* a = new QColcumns("T", "#1", t_int, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_int, "#2");
  QColcumns* c = new QColcumns("T", "#3", t_float, "#3");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_int, oper_divide, t_qexpr_cal, "result");
  QExpr_binary* op2 =
      new QExpr_binary(op1, c, t_float, oper_divide, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op2, t_float, column_index, s);

  expr_func f = getExprFunc(op2, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *(int*)tuple = 4;
  *((int*)s->getColumnAddess(1, tuple)) = 3;
  *((float*)s->getColumnAddess(2, tuple)) = 0.5;
  float ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op2;
  EXPECT_LE(abs(ret - 2), 0.0001);
}
TEST_F(CodeGenerationTest, Divide2) {
  /* #      #1    |#2    |#3
   * Tuple: int   |float |double
   *         4     0.5     -0.5
   * Express: #1 / #2 / #3 = -16
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_float));
  columns.push_back(data_type(t_double));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  column_index["#3"] = 2;
  QColcumns* a = new QColcumns("T", "#1", t_int, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_float, "#2");
  QColcumns* c = new QColcumns("T", "#3", t_double, "#3");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_float, oper_divide, t_qexpr_cal, "result");
  QExpr_binary* op2 =
      new QExpr_binary(op1, c, t_double, oper_divide, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op2, t_double, column_index, s);

  expr_func f = getExprFunc(op2, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *(int*)tuple = 4;
  *((float*)s->getColumnAddess(1, tuple)) = 0.5;
  *((double*)s->getColumnAddess(2, tuple)) = -0.5;
  double ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op2;
  EXPECT_LE(abs(ret + 16), 0.0001);
}
TEST_F(CodeGenerationTest, Const) {
  /* #      #1
   * Tuple: double
   *        -0.5
   * Express: (int)4*(float)0.5*#1 = -1.0
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_double));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  QExpr* a = new QExpr("4", t_int, "#1");
  QExpr* b = new QExpr("0.5", t_float, "#2");
  QColcumns* c = new QColcumns("T", "#3", t_double, "#3");

  QExpr_binary* op1 =
      new QExpr_binary(a, c, t_double, oper_multiply, t_qexpr_cal, "result");
  QExpr_binary* op2 =
      new QExpr_binary(op1, b, t_double, oper_multiply, t_qexpr_cal, "result");

  InitExprAtLogicalPlan(op2, t_double, column_index, s);

  expr_func f = getExprFunc(op2, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *((double*)s->getColumnAddess(0, tuple)) = -0.5;
  double ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op2;
  EXPECT_LE(abs(ret + 1), 0.0001);
}

TEST_F(CodeGenerationTest, CompareLT) {
  /* #      #1    | #2
   * Tuple: long  | long
   *        3     | 4
   * Express: #1 < #2 = true
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_u_long));
  columns.push_back(data_type(t_u_long));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_u_long, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_u_long, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_u_long, oper_less, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *((long*)s->getColumnAddess(0, tuple)) = 3;
  *((long*)s->getColumnAddess(1, tuple)) = 4;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_TRUE(ret);
}

TEST_F(CodeGenerationTest, GreatCompare) {
  /* #      #1    | #2
   * Tuple: long  | long
   *        3     | 4
   * Express: #1 < #2 = true
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_u_long));
  columns.push_back(data_type(t_u_long));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_u_long, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_u_long, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_u_long, oper_great, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *((long*)s->getColumnAddess(0, tuple)) = 3;
  *((long*)s->getColumnAddess(1, tuple)) = 4;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_FALSE(ret);
}

TEST_F(CodeGenerationTest, AND) {
  /* #      #1    | #2
   * Tuple: bool  | bool
   *        1     | 0
   * Express: #1 AND #2 = false
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_boolean));
  columns.push_back(data_type(t_boolean));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_boolean, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_boolean, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_boolean, oper_and, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *((bool*)s->getColumnAddess(0, tuple)) = true;
  *((bool*)s->getColumnAddess(1, tuple)) = false;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_FALSE(ret);
}

TEST_F(CodeGenerationTest, CompareEQ) {
  /* #      #1    | #2
   * Tuple: long  | long
   *        3     | 4
   * Express: #1 = #2 = false
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_u_long));
  columns.push_back(data_type(t_u_long));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_u_long, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_u_long, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_u_long, oper_equal, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *((long*)s->getColumnAddess(0, tuple)) = 3;
  *((long*)s->getColumnAddess(1, tuple)) = 4;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_FALSE(ret);
}
TEST_F(CodeGenerationTest, CompareEQ1) {
  /* #      #1    | #2
   * Tuple: long  | long
   *        4     | 4
   * Express: #1 = #2 = true
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_u_long));
  columns.push_back(data_type(t_u_long));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_u_long, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_u_long, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_u_long, oper_equal, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *((long*)s->getColumnAddess(0, tuple)) = 2011012161127060000;
  *((long*)s->getColumnAddess(1, tuple)) = 2111106231004740;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_FALSE(ret);
}
TEST_F(CodeGenerationTest, CompareEQ2) {
  /* #      #1    | #2
   * Tuple: long  | long
   *        4     | 4
   * Express: #1 = #2 = true
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_u_long));
  columns.push_back(data_type(t_u_long));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_u_long, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_u_long, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_u_long, oper_equal, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *((long*)s->getColumnAddess(0, tuple)) = 2011012161127060000;
  *((long*)s->getColumnAddess(1, tuple)) = 2011012161127060000;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_TRUE(ret);
}
TEST_F(CodeGenerationTest, CompareEQ3) {
  /* #        #1    | #2
   * Tuple1: long  | long
   *         6     | 4
   * Tuple2: int   | double
   *         3     | 6.3
   * Express: #1 < #2 = true
   */
  std::vector<column_type> columns1;
  columns1.push_back(data_type(t_u_long));
  columns1.push_back(data_type(t_u_long));
  Schema* s1 = new SchemaFix(columns1);

  std::vector<column_type> columns2;
  columns2.push_back(data_type(t_int));
  columns2.push_back(data_type(t_double));
  Schema* s2 = new SchemaFix(columns2);

  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_u_long, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_u_long, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_u_long, oper_equal, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s1);

  ExprFuncTwoTuples f = getExprFuncTwoTuples(op1, s1, s2);

  void* tuple1 = malloc(s1->getTupleMaxSize());
  *((long*)s1->getColumnAddess(0, tuple1)) = 6;
  *((long*)s1->getColumnAddess(1, tuple1)) = 4;

  void* tuple2 = malloc(s2->getTupleMaxSize());
  *((int*)s2->getColumnAddess(0, tuple2)) = 3;
  *((double*)s2->getColumnAddess(1, tuple2)) = 6.3;

  bool ret;
  f(tuple1, tuple2, &ret);
  delete tuple1;
  delete s1;
  delete op1;
  EXPECT_FALSE(ret);
}

TEST_F(CodeGenerationTest, EqualJoinCompare) {
  /* #        #1    | #2
   * Tuple1: long  | long
   *         3     | 4
   * Tuple2: long  | long
   *         4     | 3
   * Expression Tuple1.#1 == Tuple2.#2 AND Tuple1.#2 == Tuple1.#1 = true
   */
  std::vector<column_type> columns1;
  columns1.push_back(data_type(t_u_long));
  columns1.push_back(data_type(t_u_long));
  Schema* s1 = new SchemaFix(columns1);

  std::vector<column_type> columns2;
  columns2.push_back(data_type(t_u_long));
  columns2.push_back(data_type(t_u_long));
  Schema* s2 = new SchemaFix(columns2);

  std::vector<unsigned> l_join_index, r_join_index;
  l_join_index.push_back(0);
  l_join_index.push_back(1);
  r_join_index.push_back(1);
  r_join_index.push_back(0);

  QNode* expr = createEqualJoinExpression(s1, s2, l_join_index, r_join_index);

  //	InitExprAtLogicalPlan(expr,t_boolean,column_index,s1);

  ExprFuncTwoTuples f = getExprFuncTwoTuples(expr, s1, s2);

  void* tuple1 = malloc(s1->getTupleMaxSize());
  *((long*)s1->getColumnAddess(0, tuple1)) = 3;
  *((long*)s1->getColumnAddess(1, tuple1)) = 4;

  void* tuple2 = malloc(s2->getTupleMaxSize());
  *((long*)s2->getColumnAddess(0, tuple2)) = 4;
  *((long*)s2->getColumnAddess(1, tuple2)) = 3;

  bool ret;
  f(tuple1, tuple2, &ret);
  delete tuple1;
  delete s1;
  delete expr;
  EXPECT_TRUE(ret);
}

TEST_F(CodeGenerationTest, FilterLogic) {
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  Schema* s = new SchemaFix(columns);
  map<string, int> column_index;
  column_index["a"] = 0;
  column_index["b"] = 1;

  QColcumns* a = new QColcumns("T", "a", t_int, "a");
  QColcumns* b = new QColcumns("T", "b", t_int, "b");

  QExpr_binary* op =
      new QExpr_binary(a, b, t_int, oper_less, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op, t_boolean, column_index, s);
  CodeGenerator::getInstance();

  filter_process_func gen_func = getFilterProcessFunc(op, s);

  const unsigned b_tuple_count = 10;
  int b_cur = 0;
  int c_cur = 0;
  void* b_start;
  void* c_start;

  b_start = malloc(s->getTupleMaxSize() * b_tuple_count);
  memset(b_start, 255, s->getTupleMaxSize() * b_tuple_count);

  const unsigned c_tuple_count = 10;

  /* populate the block beginning at c_start with tuples such that only
   * the first tuple can pass the filter
   */
  c_start = malloc(s->getTupleMaxSize() * c_tuple_count);
  char* c = (char*)c_start;
  for (int i = 0; i < c_tuple_count; i++) {
    *(int*)s->getColumnAddess(0, c) = 5 + i * 5;
    *(int*)s->getColumnAddess(1, c) = 10;
    c += s->getTupleMaxSize();
  }

  gen_func(b_start, &b_cur, b_tuple_count, c_start, &c_cur, c_tuple_count);

  EXPECT_TRUE(c_cur == c_tuple_count && b_cur == 1 &&
              *(int*)s->getColumnAddess(1, b_start) == 10);
}

TEST_F(CodeGenerationTest, FilterLogic1) {
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  Schema* s = new SchemaFix(columns);
  map<string, int> column_index;
  column_index["a"] = 0;
  column_index["b"] = 1;

  QColcumns* a = new QColcumns("T", "a", t_int, "a");
  QColcumns* b = new QColcumns("T", "b", t_int, "b");

  QExpr_binary* op =
      new QExpr_binary(a, b, t_int, oper_less, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op, t_boolean, column_index, s);
  CodeGenerator::getInstance();

  filter_process_func gen_func = getFilterProcessFunc(op, s);

  const unsigned b_tuple_count = 10;
  int b_cur = 0;
  int c_cur = 0;
  void* b_start;
  void* c_start;

  b_start = malloc(s->getTupleMaxSize() * b_tuple_count);
  memset(b_start, 255, s->getTupleMaxSize() * b_tuple_count);

  const unsigned c_tuple_count = 10;

  /* populate the block beginning at c_start with tuples such that only
   * the first four tuples can pass the filter
   */
  c_start = malloc(s->getTupleMaxSize() * c_tuple_count);
  char* c = (char*)c_start;
  for (int i = 0; i < c_tuple_count; i++) {
    *(int*)s->getColumnAddess(0, c) = i * 5;
    *(int*)s->getColumnAddess(1, c) = 20;
    c += s->getTupleMaxSize();
  }

  gen_func(b_start, &b_cur, b_tuple_count, c_start, &c_cur, c_tuple_count);

  EXPECT_TRUE(c_cur == c_tuple_count && b_cur == 4);
}

TEST_F(CodeGenerationTest, FilterLogic2) {
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  Schema* s = new SchemaFix(columns);
  map<string, int> column_index;
  column_index["a"] = 0;
  column_index["b"] = 1;

  QColcumns* a = new QColcumns("T", "a", t_int, "a");
  QColcumns* b = new QColcumns("T", "b", t_int, "b");

  QExpr_binary* op =
      new QExpr_binary(a, b, t_int, oper_less, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op, t_boolean, column_index, s);
  CodeGenerator::getInstance();

  filter_process_func gen_func = getFilterProcessFunc(op, s);

  const unsigned b_tuple_count = 2;
  const unsigned c_tuple_count = 20;
  int b_cur = 0;
  int c_cur = 0;
  void* b_start;
  void* c_start;

  b_start = malloc(s->getTupleMaxSize() * b_tuple_count);
  memset(b_start, 255, s->getTupleMaxSize() * b_tuple_count);

  /* populate the block beginning at c_start with tuples such that only
   * the first tuple can pass the filter
   */
  c_start = malloc(s->getTupleMaxSize() * c_tuple_count);
  char* c = (char*)c_start;
  for (int i = 0; i < c_tuple_count; i++) {
    *(int*)s->getColumnAddess(0, c) = i * 5;
    *(int*)s->getColumnAddess(1, c) = 20;
    c += s->getTupleMaxSize();
  }

  gen_func(b_start, &b_cur, b_tuple_count, c_start, &c_cur, c_tuple_count);

  EXPECT_TRUE(c_cur == b_tuple_count && b_cur == b_tuple_count);
}

TEST_F(CodeGenerationTest, FilterLogic3) {
  std::vector<column_type> columns;
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  Schema* s = new SchemaFix(columns);
  map<string, int> column_index;
  column_index["a"] = 0;
  column_index["b"] = 1;

  QColcumns* a = new QColcumns("T", "a", t_int, "a");
  QColcumns* b = new QColcumns("T", "b", t_int, "b");

  QExpr_binary* op =
      new QExpr_binary(a, b, t_int, oper_less, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op, t_boolean, column_index, s);
  CodeGenerator::getInstance();

  filter_process_func gen_func = getFilterProcessFunc(op, s);

  const unsigned b_tuple_count = 5;
  const unsigned c_tuple_count = 3;
  int b_cur = 0;
  int c_cur = 0;
  void* b_start;
  void* c_start;

  b_start = malloc(s->getTupleMaxSize() * b_tuple_count);
  memset(b_start, 255, s->getTupleMaxSize() * b_tuple_count);

  /* populate the block beginning at c_start with tuples such that only
   * the first four tuples can pass the filter
   */
  c_start = malloc(s->getTupleMaxSize() * c_tuple_count);
  char* c = (char*)c_start;
  for (int i = 0; i < c_tuple_count; i++) {
    *(int*)s->getColumnAddess(0, c) = i * 5;
    *(int*)s->getColumnAddess(1, c) = 20;
    c += s->getTupleMaxSize();
  }

  gen_func(b_start, &b_cur, b_tuple_count, c_start, &c_cur, c_tuple_count);

  EXPECT_TRUE(c_cur == c_tuple_count && b_cur == c_tuple_count);
}

TEST_F(CodeGenerationTest, FilterLogic4) {
  std::vector<column_type> columns;
  columns.push_back(data_type(t_u_long));
  columns.push_back(data_type(t_u_long));
  Schema* s = new SchemaFix(columns);
  map<string, int> column_index;
  column_index["a"] = 0;
  column_index["b"] = 1;

  QColcumns* a = new QColcumns("T", "a", t_u_long, "a");
  QColcumns* b = new QColcumns("T", "b", t_u_long, "b");

  QExpr_binary* op =
      new QExpr_binary(a, b, t_u_long, oper_equal, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op, t_boolean, column_index, s);
  CodeGenerator::getInstance();

  filter_process_func gen_func = getFilterProcessFunc(op, s);

  const unsigned b_tuple_count = 7;
  const unsigned c_tuple_count = 5;
  int b_cur = 0;
  int c_cur = 0;
  void* b_start;
  void* c_start;

  b_start = malloc(s->getTupleMaxSize() * b_tuple_count);
  memset(b_start, 255, s->getTupleMaxSize() * b_tuple_count);

  /* populate the block beginning at c_start with tuples such that only
   * the first four tuples can pass the filter
   */
  c_start = malloc(s->getTupleMaxSize() * c_tuple_count);
  char* c = (char*)c_start;
  for (int i = 0; i < c_tuple_count; i++) {
    *(unsigned long*)s->getColumnAddess(0, c) = 2011012161127060000 + i * 5;
    *(unsigned long*)s->getColumnAddess(1, c) = 2011012161127060000 + 20;
    c += s->getTupleMaxSize();
  }

  gen_func(b_start, &b_cur, b_tuple_count, c_start, &c_cur, c_tuple_count);

  EXPECT_TRUE(c_cur == c_tuple_count && b_cur == 1);
}

TEST_F(CodeGenerationTest, FilterLogicReal) {
  std::vector<column_type> columns;
  columns.push_back(data_type(t_u_long));
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_int));
  columns.push_back(data_type(t_u_long));
  columns.push_back(column_type(data_type(t_string), 1000));
  columns.push_back(column_type(data_type(t_string), 1000));
  columns.push_back(data_type(t_datetime));
  Schema* s = new SchemaFix(columns);
  map<string, int> column_index;
  column_index["row_id"] = 0;
  column_index["EventId"] = 1;
  column_index["c"] = 2;
  column_index["RTMid"] = 3;
  column_index["e"] = 4;
  column_index["f"] = 5;
  column_index["g"] = 5;

  //  QColcumns* a = new QColcumns("T", "a", t_u_long, "a");
  QExpr* a = new QExpr("2011012161127060000", t_u_long, "2011012161127060000");
  QColcumns* b = new QColcumns("retweet", "RTMid", t_u_long, "retweet.RTMid");

  QExpr_binary* op =
      new QExpr_binary(b, a, t_u_long, oper_equal, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op, t_boolean, column_index, s);
  CodeGenerator::getInstance();

  filter_process_func gen_func = getFilterProcessFunc(op, s);

  const unsigned b_tuple_count = 7;
  const unsigned c_tuple_count = 5;
  int b_cur = 0;
  int c_cur = 0;
  void* b_start;
  void* c_start;

  b_start = malloc(s->getTupleMaxSize() * b_tuple_count);
  memset(b_start, 255, s->getTupleMaxSize() * b_tuple_count);

  /* populate the block beginning at c_start with tuples such that only
   * the first four tuples can pass the filter
   */
  c_start = malloc(s->getTupleMaxSize() * c_tuple_count);
  char* c = (char*)c_start;
  for (int i = 0; i < c_tuple_count; i++) {
    *(unsigned long*)s->getColumnAddess(0, c) = i;
    *(unsigned long*)s->getColumnAddess(3, c) = 2011012161127060000;
    c += s->getTupleMaxSize();
  }

  gen_func(b_start, &b_cur, b_tuple_count, c_start, &c_cur, c_tuple_count);

  EXPECT_TRUE(c_cur == c_tuple_count && b_cur == 5 &&
              *(unsigned long*)s->getColumnAddess(3, b_start) ==
                  2011012161127060000);
}

TEST_F(CodeGenerationTest, Memcpy) {
  void* a = malloc(sizeof(long));
  long v = 100;
  LLVMMemcpy f = getMemcpy(sizeof(long));
  f(a, &v);
  EXPECT_TRUE(*(long*)a == 100);
}
TEST_F(CodeGenerationTest, Memcat) {
  void* a = malloc(sizeof(long) * 2);
  long v1 = 100;
  long v2 = 200;
  LLVMMemcat f = getMemcat(sizeof(long), sizeof(long));
  f(a, &v1, &v2);
  EXPECT_TRUE(*(long*)a == 100 && *((long*)a + 1) == 200);
}

TEST_F(CodeGenerationTest, LessCompare1) {
  /* #      #1    | #2
   * Tuple: decimal  | decimal
   *        3     | 4
   * Express: #1 < #2 = true
   */
  NValue* l_class = new NValue;
  l_class->createDecimalFromString("23");
  NValue* r_class = new NValue;
  r_class->createDecimalFromString("34");

  // this sentence is necessary to make sure compiler know and compile this
  // method
  std::cout << "expected result is :" << NValueLess(l_class, r_class) << endl;

  llvm::IRBuilder<>* builder = CodeGenerator::getInstance()->getBuilder();
  llvm::Function* ff = CreateNValueCompareFunc(oper_less);
  //	verifyFunction(*ff);
  //	ff->dump();

  llvm::Constant* l_const_int = llvm::ConstantInt::get(
      llvm::Type::getInt64Ty(llvm::getGlobalContext()), (uint64_t)l_class);
  lv* l_const_ptr = builder->CreateIntToPtr(
      l_const_int,
      llvm::PointerType::getUnqual(llvm::Type::getInt64Ty(
          llvm::
              getGlobalContext())));  // Type::getInt64PtrTy(builder->getContext())//);
  llvm::Constant* r_const_int = llvm::ConstantInt::get(
      llvm::Type::getInt64Ty(llvm::getGlobalContext()), (uint64_t)r_class);
  lv* r_const_ptr = builder->CreateIntToPtr(
      r_const_int, llvm::PointerType::getUnqual(
                       llvm::Type::getInt64Ty(llvm::getGlobalContext())));

  std::vector<llvm::GenericValue> args(2);
  //	args[0].PointerVal = l_const_ptr;	//error
  //	args[1].PointerVal = r_const_ptr;	// but
  //	args[0].PointerVal = l_const_int;	// i don't
  //	args[1].PointerVal = r_const_int;	// know why

  // one way to call llvm function
  args[0].PointerVal = l_class;
  args[1].PointerVal = r_class;
  llvm::GenericValue ret_value =
      CodeGenerator::getInstance()->getExecutionEngine()->runFunction(ff, args);
  bool ret = ret_value.IntVal.getBoolValue();

  // another way to call llvm function
  //	int (*create_less)(int*, int*) =
  //CodeGenerator::getInstance()->getExecutionEngine()->getPointerToFunction(ff);
  //	bool ret = create_less((int*)l_class, (int*)r_class);

  delete l_class;
  delete r_class;
  EXPECT_TRUE(ret);
}

TEST_F(CodeGenerationTest, LessCompare2) {
  /* #      #1    | #2
   * Tuple: decimal  | decimal
   *        3     | 4
   * Express: #1 < #2 = true
   */
  NValue* l_class = new NValue;
  l_class->createDecimalFromString("23");
  NValue* r_class = new NValue;
  r_class->createDecimalFromString("34");

  // this sentence is necessary to make sure compiler know and compile this
  // method
  std::cout << "expected result is :" << NValueLess(l_class, r_class) << endl;

  llvm::Function* ff = CreateNValueCompareFunc(oper_less);
  //	verifyFunction(*ff);
  //	ff->dump();

  typedef int (*tmp_fuc)(int*, int*);
  // another way to call llvm Function
  tmp_fuc create_less = (tmp_fuc)CodeGenerator::getInstance()
                            ->getExecutionEngine()
                            ->getPointerToFunction(ff);

  bool ret = create_less((int*)l_class, (int*)r_class);

  delete l_class;
  delete r_class;
  EXPECT_TRUE(ret);
}

TEST_F(CodeGenerationTest, LessCompare3) {
  /* #      #1    | #2
   * Tuple: decimal  | decimal
   *        3     | 4
   * Express: #1 < #2 = true
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_decimal));
  columns.push_back(data_type(t_decimal));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_decimal, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_decimal, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_decimal, oper_less, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  ((NValue*)s->getColumnAddess(0, tuple))->createDecimalFromString("3");
  ((NValue*)s->getColumnAddess(1, tuple))->createDecimalFromString("4");
  //	*((NValue*)s->getColumnAddess(1,tuple))=4;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_TRUE(ret);
}
TEST_F(CodeGenerationTest, LessCompare4) {
  /* #      #1    | #2
   * Tuple: decimal  | decimal
   *        3     | 4
   * Express: #1 < #2 = true
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_decimal));
  columns.push_back(data_type(t_decimal));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_decimal, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_decimal, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_decimal, oper_less, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  ((NValue*)s->getColumnAddess(0, tuple))->createDecimalFromString("4");
  ((NValue*)s->getColumnAddess(1, tuple))->createDecimalFromString("4");
  //	*((NValue*)s->getColumnAddess(1,tuple))=4;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_FALSE(ret);
}
TEST_F(CodeGenerationTest, LessCompare5) {
  /* #      #1    | #2
   * Tuple: decimal  | decimal
   *        3     | 4
   * Express: #1 < #2 = true
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_decimal));
  columns.push_back(data_type(t_decimal));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_decimal, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_decimal, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_decimal, oper_less, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  ((NValue*)s->getColumnAddess(0, tuple))
      ->createDecimalFromString("4000000000000000");
  ((NValue*)s->getColumnAddess(1, tuple))
      ->createDecimalFromString("4999999999999999");
  //	*((NValue*)s->getColumnAddess(1,tuple))=4;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_TRUE(ret);
}

TEST_F(CodeGenerationTest, LessCompare6) {
  /* #      #1    | #2
   * Tuple: decimal  | decimal
   *        3     | 4
   * Express: #1 < #2 = true
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_decimal));
  columns.push_back(data_type(t_decimal));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_decimal, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_decimal, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_decimal, oper_less, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  ((NValue*)s->getColumnAddess(0, tuple))
      ->createDecimalFromString("4000000000000000");
  ((NValue*)s->getColumnAddess(1, tuple))
      ->createDecimalFromString("3999999999999999");
  //	*((NValue*)s->getColumnAddess(1,tuple))=4;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_FALSE(ret);
}

TEST_F(CodeGenerationTest, GreatCompareDecimal) {
  /* #      #1    | #2
   * Tuple: decimal  | decimal
   *        3     | 4
   * Express: #1 < #2 = true
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_decimal));
  columns.push_back(data_type(t_decimal));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_decimal, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_decimal, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_decimal, oper_great, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  ((NValue*)s->getColumnAddess(0, tuple))
      ->createDecimalFromString("400000000000000000");
  ((NValue*)s->getColumnAddess(1, tuple))
      ->createDecimalFromString("399999999999999999");
  //	*((NValue*)s->getColumnAddess(1,tuple))=4;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_TRUE(ret);
}

TEST_F(CodeGenerationTest, EqualCompare) {
  /* #      #1    | #2
   * Tuple: decimal  | decimal
   *        3     | 4
   * Express: #1 < #2 = true
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_decimal));
  columns.push_back(data_type(t_decimal));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_decimal, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_decimal, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_decimal, oper_equal, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  ((NValue*)s->getColumnAddess(0, tuple))
      ->createDecimalFromString("3992345766342542999");
  ((NValue*)s->getColumnAddess(1, tuple))
      ->createDecimalFromString("3992345766342542999");
  //	*((NValue*)s->getColumnAddess(1,tuple))=4;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_TRUE(ret);
}

TEST_F(CodeGenerationTest, LessCompareDate) {
  /* #      #1             | #2
   * Tuple: date           | date
   *        2011-11-11     | 2011-11-12
   * Express: #1 < #2 = true
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_date));
  columns.push_back(data_type(t_date));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_date, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_date, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_date, oper_less, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *((date*)s->getColumnAddess(0, tuple)) = from_string("2010-11-11");
  *((date*)s->getColumnAddess(1, tuple)) = from_string("2010-11-12");
  //	*((NValue*)s->getColumnAddess(1,tuple))=4;
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_TRUE(ret);
}

TEST_F(CodeGenerationTest, GreatCompareDate) {
  /* #      #1             | #2
   * Tuple: date           | date
   *        2011-11-11     | 2011-11-12
   * Express: #1 > #2 = false
   */
  std::vector<column_type> columns;
  columns.push_back(data_type(t_date));
  columns.push_back(data_type(t_date));
  Schema* s = new SchemaFix(columns);
  map<std::string, int> column_index;
  column_index["#1"] = 0;
  column_index["#2"] = 1;
  QColcumns* a = new QColcumns("T", "#1", t_date, "#1");
  QColcumns* b = new QColcumns("T", "#2", t_date, "#2");

  QExpr_binary* op1 =
      new QExpr_binary(a, b, t_date, oper_great, t_qexpr_cmp, "result");

  InitExprAtLogicalPlan(op1, t_boolean, column_index, s);

  expr_func f = getExprFunc(op1, s);

  void* tuple = malloc(s->getTupleMaxSize());
  *((date*)s->getColumnAddess(0, tuple)) = from_string("2010-11-11");
  *((date*)s->getColumnAddess(1, tuple)) = from_string("2010-11-12");
  bool ret;
  f(tuple, &ret);
  delete tuple;
  delete s;
  delete op1;
  EXPECT_FALSE(ret);
}

#endif /* CODEGEN_TEST_H_ */
