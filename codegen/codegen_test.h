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

using std::map;

class CodeGenerationTest : public ::testing::Test{
protected:

	static void SetUpTestCase() {
		CodeGenerator::getInstance();
		initialize_arithmetic_type_promotion_matrix();
		initialize_type_cast_functions();
		initialize_operator_function();
	}

	static void TearDownTestCase() {
		delete CodeGenerator::getInstance();
	}
};

TEST_F(CodeGenerationTest,AddInt){

	/* #      1# |2#
	 * Tuple: int|int
	 *         1   2
	 * Express: #1 + #2
	 */

	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_int));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["a"]=0;
	column_index["b"]=1;

	QColcumns* a=new QColcumns("T","a",t_int,"a");
	QColcumns* b=new QColcumns("T","b",t_int,"b");

	QExpr_binary* op=new QExpr_binary(a,b,t_int,oper_add,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op,t_int,column_index,s);
	CodeGenerator::getInstance();

	expr_func f=getExprFunc(op,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(int*)tuple=1;
	*((int*)tuple+1)=2;
	int ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op;
	EXPECT_EQ(ret,3);
}


TEST_F(CodeGenerationTest,AddInt2){
	/* #      #1 |#2 |#3
	 * Tuple: int|int|int
	 *         -1   2   3
	 * Express: #1 + #2 + #3
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_int));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	column_index["#3"]=2;
	QColcumns* a=new QColcumns("T","#1",t_int,"#1");
	QColcumns* b=new QColcumns("T","#2",t_int,"#2");
	QColcumns* c=new QColcumns("T","#3",t_int,"#3");

	QExpr_binary* op1=new QExpr_binary(a,b,t_int,oper_add,t_qexpr_cal,"result");
	QExpr_binary* op2=new QExpr_binary(op1,c,t_int,oper_add,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op2,t_int,column_index,s);
	CodeGenerator::getInstance();

	expr_func f=getExprFunc(op2,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(int*)tuple=-1;
	*((int*)tuple+1)=2;
	*((int*)tuple+2)=3;
	int ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op2;
	EXPECT_EQ(ret,4);
}
TEST_F(CodeGenerationTest,AddFloat){
	/* #      #1    |#2    |#3
	 * Tuple: float |float |float
	 *         0     -1.2    3.8
	 * Express: #1 + #2 + #3
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_float));
	columns.push_back(data_type(t_float));
	columns.push_back(data_type(t_float));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	column_index["#3"]=2;
	QColcumns* a=new QColcumns("T","#1",t_float,"#1");
	QColcumns* b=new QColcumns("T","#2",t_float,"#2");
	QColcumns* c=new QColcumns("T","#3",t_float,"#3");

	QExpr_binary* op1=new QExpr_binary(a,b,t_float,oper_add,t_qexpr_cal,"result");
	QExpr_binary* op2=new QExpr_binary(op1,c,t_float,oper_add,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op2,t_float,column_index,s);

	expr_func f=getExprFunc(op2,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(float*)tuple=0;
	*((float*)tuple+1)=-1.2;
	*((float*)tuple+2)=3.8;
	float ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op2;
	EXPECT_LE(abs(ret-2.6),0.00001);
}
TEST_F(CodeGenerationTest,AddFloatPromote){
	/* #      #1    |#2    |#3
	 * Tuple: int |float |float
	 *         1     -1.2    3.8
	 * Express: #1 + #2 + #3
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_float));
	columns.push_back(data_type(t_float));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	column_index["#3"]=2;
	QColcumns* a=new QColcumns("T","#1",t_int,"#1");
	QColcumns* b=new QColcumns("T","#2",t_float,"#2");
	QColcumns* c=new QColcumns("T","#3",t_float,"#3");

	QExpr_binary* op1=new QExpr_binary(a,b,t_float,oper_add,t_qexpr_cal,"result");
	QExpr_binary* op2=new QExpr_binary(op1,c,t_float,oper_add,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op2,t_float,column_index,s);

	expr_func f=getExprFunc(op2,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(int*)tuple=1;
	*((float*)tuple+1)=-1.2;
	*((float*)tuple+2)=3.8;
	float ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op2;
	EXPECT_LE(abs(ret-3.6),0.00001);
}
TEST_F(CodeGenerationTest,AddFloatPromote2){
	/* #      #1    |#2    |#3
	 * Tuple: int |float |float
	 *         1     -1.2    3.8
	 * Express: #1 + #2 + #3
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_float));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	column_index["#3"]=2;
	QColcumns* a=new QColcumns("T","#1",t_int,"#1");
	QColcumns* b=new QColcumns("T","#2",t_int,"#2");
	QColcumns* c=new QColcumns("T","#3",t_float,"#3");

	QExpr_binary* op1=new QExpr_binary(a,b,t_int,oper_add,t_qexpr_cal,"result");
	QExpr_binary* op2=new QExpr_binary(op1,c,t_float,oper_add,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op2,t_float,column_index,s);

	expr_func f=getExprFunc(op2,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(int*)tuple=1;
	*((int*)tuple+1)=-2;
	*((float*)tuple+2)=3.8;
	float ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op2;
	EXPECT_LE(abs(ret-2.8),0.00001);
}
TEST_F(CodeGenerationTest,AddLongPromote){
	/* #      #1    |#2    |#3
	 * Tuple: int   |int   |long
	 *         1     -1     200
	 * Express: #1 + #2 + #3
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_u_long));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	column_index["#3"]=2;
	QColcumns* a=new QColcumns("T","#1",t_int,"#1");
	QColcumns* b=new QColcumns("T","#2",t_int,"#2");
	QColcumns* c=new QColcumns("T","#3",t_u_long,"#3");

	QExpr_binary* op1=new QExpr_binary(a,b,t_int,oper_add,t_qexpr_cal,"result");
	QExpr_binary* op2=new QExpr_binary(op1,c,t_u_long,oper_add,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op2,t_u_long,column_index,s);

	expr_func f=getExprFunc(op2,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(int*)tuple=1;
	*((int*)s->getColumnAddess(1,tuple))=-1;
	*((long*)s->getColumnAddess(2,tuple))=200;
	long ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op2;
	EXPECT_LE(ret,200);
}
TEST_F(CodeGenerationTest,SUB){
	/* #      #1    |#2    |#3
	 * Tuple: int   |int   |long
	 *         1     -1     200
	 * Express: #1 + #2 + #3
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_u_long));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	column_index["#3"]=2;
	QColcumns* a=new QColcumns("T","#1",t_int,"#1");
	QColcumns* b=new QColcumns("T","#2",t_int,"#2");
	QColcumns* c=new QColcumns("T","#3",t_u_long,"#3");

	QExpr_binary* op1=new QExpr_binary(a,b,t_int,oper_minus,t_qexpr_cal,"result");
	QExpr_binary* op2=new QExpr_binary(op1,c,t_u_long,oper_minus,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op2,t_u_long,column_index,s);

	expr_func f=getExprFunc(op2,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(int*)tuple=1;
	*((int*)s->getColumnAddess(1,tuple))=-1;
	*((long*)s->getColumnAddess(2,tuple))=200;
	long ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op2;
	EXPECT_LE(ret,-198);
}
TEST_F(CodeGenerationTest,Multiply){
	/* #      #1    |#2    |#3
	 * Tuple: int   |int   |long
	 *         1     -1     200
	 * Express: #1 + #2 + #3
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_u_long));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	column_index["#3"]=2;
	QColcumns* a=new QColcumns("T","#1",t_int,"#1");
	QColcumns* b=new QColcumns("T","#2",t_int,"#2");
	QColcumns* c=new QColcumns("T","#3",t_u_long,"#3");

	QExpr_binary* op1=new QExpr_binary(a,b,t_int,oper_multiply,t_qexpr_cal,"result");
	QExpr_binary* op2=new QExpr_binary(op1,c,t_u_long,oper_multiply,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op2,t_u_long,column_index,s);

	expr_func f=getExprFunc(op2,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(int*)tuple=1;
	*((int*)s->getColumnAddess(1,tuple))=-1;
	*((long*)s->getColumnAddess(2,tuple))=200;
	long ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op2;
	EXPECT_LE(ret,-200);
}
TEST_F(CodeGenerationTest,Multiply1){
	/* #      #1    |#2    |#3
	 * Tuple: int   |float |long
	 *         2     0.5    3
	 * Express: #1 x #2 x #3 = 3
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_float));
	columns.push_back(data_type(t_u_long));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	column_index["#3"]=2;
	QColcumns* a=new QColcumns("T","#1",t_int,"#1");
	QColcumns* b=new QColcumns("T","#2",t_float,"#2");
	QColcumns* c=new QColcumns("T","#3",t_u_long,"#3");

	QExpr_binary* op1=new QExpr_binary(a,b,t_float,oper_multiply,t_qexpr_cal,"result");
	QExpr_binary* op2=new QExpr_binary(op1,c,t_float,oper_multiply,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op2,t_float,column_index,s);

	expr_func f=getExprFunc(op2,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(int*)tuple=2;
	*((float*)s->getColumnAddess(1,tuple))=0.5;
	*((long*)s->getColumnAddess(2,tuple))=3;
	float ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op2;
	EXPECT_LE(ret,3);
}
TEST_F(CodeGenerationTest,Divide){
	/* #      #1    |#2    |#3
	 * Tuple: int   |int   |float
	 *         4     3     0.5
	 * Express: #1 / #2 / #3 = 2
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_float));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	column_index["#3"]=2;
	QColcumns* a=new QColcumns("T","#1",t_int,"#1");
	QColcumns* b=new QColcumns("T","#2",t_int,"#2");
	QColcumns* c=new QColcumns("T","#3",t_float,"#3");

	QExpr_binary* op1=new QExpr_binary(a,b,t_int,oper_divide,t_qexpr_cal,"result");
	QExpr_binary* op2=new QExpr_binary(op1,c,t_float,oper_divide,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op2,t_float,column_index,s);

	expr_func f=getExprFunc(op2,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(int*)tuple=4;
	*((int*)s->getColumnAddess(1,tuple))=3;
	*((float*)s->getColumnAddess(2,tuple))=0.5;
	float ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op2;
	EXPECT_LE(abs(ret-2),0.0001);
}
TEST_F(CodeGenerationTest,Divide2){
	/* #      #1    |#2    |#3
	 * Tuple: int   |float |double
	 *         4     0.5     -0.5
	 * Express: #1 / #2 / #3 = -16
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_float));
	columns.push_back(data_type(t_double));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	column_index["#3"]=2;
	QColcumns* a=new QColcumns("T","#1",t_int,"#1");
	QColcumns* b=new QColcumns("T","#2",t_float,"#2");
	QColcumns* c=new QColcumns("T","#3",t_double,"#3");

	QExpr_binary* op1=new QExpr_binary(a,b,t_float,oper_divide,t_qexpr_cal,"result");
	QExpr_binary* op2=new QExpr_binary(op1,c,t_double,oper_divide,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op2,t_double,column_index,s);

	expr_func f=getExprFunc(op2,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*(int*)tuple=4;
	*((float*)s->getColumnAddess(1,tuple))=0.5;
	*((double*)s->getColumnAddess(2,tuple))=-0.5;
	double ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op2;
	EXPECT_LE(abs(ret+16),0.0001);
}
TEST_F(CodeGenerationTest,Const){
	/* #      #1
	 * Tuple: double
	 *        -0.5
	 * Express: (int)4*(float)0.5*#1 = -1.0
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_double));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	QExpr* a=new QExpr("4",t_int,"#1");
	QExpr* b=new QExpr("0.5",t_float,"#2");
	QColcumns* c=new QColcumns("T","#3",t_double,"#3");

	QExpr_binary* op1=new QExpr_binary(a,c,t_double,oper_multiply,t_qexpr_cal,"result");
	QExpr_binary* op2=new QExpr_binary(op1,b,t_double,oper_multiply,t_qexpr_cal,"result");

	InitExprAtLogicalPlan(op2,t_double,column_index,s);

	expr_func f=getExprFunc(op2,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*((double*)s->getColumnAddess(0,tuple))=-0.5;
	double ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op2;
	EXPECT_LE(abs(ret+1),0.0001);
}

TEST_F(CodeGenerationTest,Compare){
	/* #      #1    | #2
	 * Tuple: long  | long
	 *        3     | 4
	 * Express: #1 < #2 = true
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_u_long));
	columns.push_back(data_type(t_u_long));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	QColcumns* a=new QColcumns("T","#1",t_u_long,"#1");
	QColcumns* b=new QColcumns("T","#2",t_u_long,"#2");

	QExpr_binary* op1=new QExpr_binary(a,b,t_u_long,oper_less,t_qexpr_cmp,"result");

	InitExprAtLogicalPlan(op1,t_boolean,column_index,s);

	expr_func f=getExprFunc(op1,s);

	void* tuple=malloc(s->getTupleMaxSize());
	*((long*)s->getColumnAddess(0,tuple))=3;
	*((long*)s->getColumnAddess(1,tuple))=4;
	bool ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op1;
	EXPECT_TRUE(ret);
}


static  void process_func(char* b_start, int * b_cur_addr, int b_tuple_count, char* c_start,int * c_cur_addr, int c_tuple_count,const int length,expr_func ff){
	 int b_cur=*b_cur_addr;
	 int c_cur=*c_cur_addr;
	 while(c_cur<c_tuple_count){
		 bool ret;
		 char* c_tuple_addr= c_start+length*c_cur;
		 ff(c_tuple_addr,&ret);
		 bool pass=ret;
		 if(pass){
			 if(b_cur<b_tuple_count){
				 char* b_tuple_addr=b_start+length*b_cur;
				 b_cur=b_cur+1;
				 memcpy(b_tuple_addr,c_tuple_addr,length);
			 }
			 else{
				 break;
			 }
		 }
		 c_cur=c_cur+1;
	 }
	 *b_cur_addr=b_cur;
	 *c_cur_addr=c_cur;
 }



TEST_F(CodeGenerationTest,FilterLogic){
	std::vector<column_type> columns;
	columns.push_back(data_type(t_int));
	columns.push_back(data_type(t_int));
	Schema* s=new SchemaFix(columns);
	map<string,int> column_index;
	column_index["a"]=0;
	column_index["b"]=1;

	QColcumns* a=new QColcumns("T","a",t_int,"a");
	QColcumns* b=new QColcumns("T","b",t_int,"b");

	QExpr_binary* op=new QExpr_binary(a,b,t_int,oper_less,t_qexpr_cmp,"result");

	InitExprAtLogicalPlan(op,t_boolean,column_index,s);
	CodeGenerator::getInstance();

	filter_process_func gen_func=getFilterProcessFunc(op,s);

	const unsigned b_tuple_count=100;
	int b_cur=0;
	void* b_start= malloc(s->getTupleMaxSize()*b_tuple_count);

	const unsigned c_tuple_count=100;
	int c_cur=0;
	void* c_start=malloc(s->getTupleMaxSize()*c_tuple_count);
	memset(c_start,0,s->getTupleMaxSize()*b_tuple_count);
	*(int*)s->getColumnAddess(1,c_start)=10;

//	gen_func(b_start,&b_cur,b_tuple_count,c_start,&c_cur,c_tuple_count);

	expr_func f=getExprFunc(op,s);
	process_func((char*)b_start,&b_cur,b_tuple_count,(char*)c_start,&c_cur,c_tuple_count,8,f);

	EXPECT_TRUE(c_cur==c_tuple_count&&b_cur==1);
	printf("b_cur = %d c_cur = %d\n",b_cur,c_cur);


}
#endif /* CODEGEN_TEST_H_ */
