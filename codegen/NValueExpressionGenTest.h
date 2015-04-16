/*
 * NValueExpressionGenTest.h
 *
 *  Created on: 2015-4-5
 *      Author: imdb
 */

#ifndef NVALUEEXPRESSIONGENTEST_H_
#define NVALUEEXPRESSIONGENTEST_H_


#include "gtest/gtest.h"
#include "CodeGenerator.h"
#include "NValueExpressionGenerator.h"
#include "ExpressionGenerator.h"

#include "../common/TypePromotionMap.h"
#include "../common/TypeCast.h"
#include "../common/Expression/queryfunc.h"
#include "../common/Schema/SchemaFix.h"
#include "../common/Expression/qnode.h"
#include "../common/Expression/initquery.h"

#include "llvm/IR/Verifier.h"
#include "llvm/ExecutionEngine/GenericValue.h"

using namespace llvm;
class NValueExpressionGenTest: public ::testing::Test {
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


TEST_F(NValueExpressionGenTest, LessCompare1) {
	/* #      #1    | #2
	 * Tuple: decimal  | decimal
	 *        3     | 4
	 * Express: #1 < #2 = true
	 */
	NValue* l_class = new NValue;
	l_class->createDecimalFromString("23");
	NValue* r_class = new NValue;
	r_class->createDecimalFromString("34");

	// this sentence is necessary to make sure compiler know and compile this method
	std::cout<<"expected result is :"<<NValueLess(l_class, r_class)<<endl;

	llvm::IRBuilder<>* builder=CodeGenerator::getInstance()->getBuilder();
	llvm::Function* ff = CreateNValueCompareFunc(LESS);
//	verifyFunction(*ff);
//	ff->dump();

	Constant* l_const_int = ConstantInt::get(Type::getInt64Ty(llvm::getGlobalContext()), (uint64_t)l_class);
	lv* l_const_ptr = builder->CreateIntToPtr(l_const_int, PointerType::getUnqual(Type::getInt64Ty(llvm::getGlobalContext())));//Type::getInt64PtrTy(builder->getContext())//);
	Constant* r_const_int = ConstantInt::get(Type::getInt64Ty(getGlobalContext()), (uint64_t)r_class);
	lv* r_const_ptr = builder->CreateIntToPtr(r_const_int, PointerType::getUnqual(Type::getInt64Ty(llvm::getGlobalContext())));

	std::vector<GenericValue> args(2);
//	args[0].PointerVal = l_const_ptr;	//error
//	args[1].PointerVal = r_const_ptr;	// but
//	args[0].PointerVal = l_const_int;	// i don't
//	args[1].PointerVal = r_const_int;	// know why

	//one way to call llvm function
	args[0].PointerVal = l_class;
	args[1].PointerVal = r_class;
	GenericValue ret_value = CodeGenerator::getInstance()->getExecutionEngine()->runFunction(ff, args);
	bool ret = ret_value.IntVal.getBoolValue();

	// another way to call llvm function
//	int (*create_less)(int*, int*) = CodeGenerator::getInstance()->getExecutionEngine()->getPointerToFunction(ff);
//	bool ret = create_less((int*)l_class, (int*)r_class);

	delete l_class;
	delete r_class;
	EXPECT_TRUE(ret);
}

TEST_F(NValueExpressionGenTest, LessCompare2) {
	/* #      #1    | #2
	 * Tuple: decimal  | decimal
	 *        3     | 4
	 * Express: #1 < #2 = true
	 */
	NValue* l_class = new NValue;
	l_class->createDecimalFromString("23");
	NValue* r_class = new NValue;
	r_class->createDecimalFromString("34");

	// this sentence is necessary to make sure compiler know and compile this method
	std::cout<<"expected result is :"<<NValueLess(l_class, r_class)<<endl;


	llvm::Function* ff = CreateNValueCompareFunc(LESS);
//	verifyFunction(*ff);
//	ff->dump();

	// another way to call llvm Function
	int (*create_less)(int*, int*) = CodeGenerator::getInstance()->getExecutionEngine()->getPointerToFunction(ff);
	bool ret = create_less((int*)l_class, (int*)r_class);

	delete l_class;
	delete r_class;
	EXPECT_TRUE(ret);
}

TEST_F(NValueExpressionGenTest, LessCompare3) {
	/* #      #1    | #2
	 * Tuple: decimal  | decimal
	 *        3     | 4
	 * Express: #1 < #2 = true
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_decimal));
	columns.push_back(data_type(t_decimal));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	QColcumns* a=new QColcumns("T","#1",t_decimal,"#1");
	QColcumns* b=new QColcumns("T","#2",t_decimal,"#2");

	QExpr_binary* op1=new QExpr_binary(a,b,t_decimal,oper_less,t_qexpr_cmp,"result");

	InitExprAtLogicalPlan(op1,t_boolean,column_index,s);

	expr_func f=getExprFunc(op1,s);

	void* tuple=malloc(s->getTupleMaxSize());
	((NValue*)s->getColumnAddess(0,tuple))->createDecimalFromString("3");
	((NValue*)s->getColumnAddess(1,tuple))->createDecimalFromString("4");
//	*((NValue*)s->getColumnAddess(1,tuple))=4;
	bool ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op1;
	EXPECT_TRUE(ret);
}
TEST_F(NValueExpressionGenTest, LessCompare4) {
	/* #      #1    | #2
	 * Tuple: decimal  | decimal
	 *        3     | 4
	 * Express: #1 < #2 = true
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_decimal));
	columns.push_back(data_type(t_decimal));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	QColcumns* a=new QColcumns("T","#1",t_decimal,"#1");
	QColcumns* b=new QColcumns("T","#2",t_decimal,"#2");

	QExpr_binary* op1=new QExpr_binary(a,b,t_decimal,oper_less,t_qexpr_cmp,"result");

	InitExprAtLogicalPlan(op1,t_boolean,column_index,s);

	expr_func f=getExprFunc(op1,s);

	void* tuple=malloc(s->getTupleMaxSize());
	((NValue*)s->getColumnAddess(0,tuple))->createDecimalFromString("4");
	((NValue*)s->getColumnAddess(1,tuple))->createDecimalFromString("4");
//	*((NValue*)s->getColumnAddess(1,tuple))=4;
	bool ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op1;
	EXPECT_FALSE(ret);
}
TEST_F(NValueExpressionGenTest, LessCompare5) {
	/* #      #1    | #2
	 * Tuple: decimal  | decimal
	 *        3     | 4
	 * Express: #1 < #2 = true
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_decimal));
	columns.push_back(data_type(t_decimal));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	QColcumns* a=new QColcumns("T","#1",t_decimal,"#1");
	QColcumns* b=new QColcumns("T","#2",t_decimal,"#2");

	QExpr_binary* op1=new QExpr_binary(a,b,t_decimal,oper_less,t_qexpr_cmp,"result");

	InitExprAtLogicalPlan(op1,t_boolean,column_index,s);

	expr_func f=getExprFunc(op1,s);

	void* tuple=malloc(s->getTupleMaxSize());
	((NValue*)s->getColumnAddess(0,tuple))->createDecimalFromString("4000000000000000");
	((NValue*)s->getColumnAddess(1,tuple))->createDecimalFromString("4999999999999999");
//	*((NValue*)s->getColumnAddess(1,tuple))=4;
	bool ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op1;
	EXPECT_TRUE(ret);
}

TEST_F(NValueExpressionGenTest, LessCompare6) {
	/* #      #1    | #2
	 * Tuple: decimal  | decimal
	 *        3     | 4
	 * Express: #1 < #2 = true
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_decimal));
	columns.push_back(data_type(t_decimal));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	QColcumns* a=new QColcumns("T","#1",t_decimal,"#1");
	QColcumns* b=new QColcumns("T","#2",t_decimal,"#2");

	QExpr_binary* op1=new QExpr_binary(a,b,t_decimal,oper_less,t_qexpr_cmp,"result");

	InitExprAtLogicalPlan(op1,t_boolean,column_index,s);

	expr_func f=getExprFunc(op1,s);

	void* tuple=malloc(s->getTupleMaxSize());
	((NValue*)s->getColumnAddess(0,tuple))->createDecimalFromString("4000000000000000");
	((NValue*)s->getColumnAddess(1,tuple))->createDecimalFromString("3999999999999999");
//	*((NValue*)s->getColumnAddess(1,tuple))=4;
	bool ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op1;
	EXPECT_FALSE(ret);
}

TEST_F(NValueExpressionGenTest, GreatCompare) {
	/* #      #1    | #2
	 * Tuple: decimal  | decimal
	 *        3     | 4
	 * Express: #1 < #2 = true
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_decimal));
	columns.push_back(data_type(t_decimal));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	QColcumns* a=new QColcumns("T","#1",t_decimal,"#1");
	QColcumns* b=new QColcumns("T","#2",t_decimal,"#2");

	QExpr_binary* op1=new QExpr_binary(a,b,t_decimal,oper_great,t_qexpr_cmp,"result");

	InitExprAtLogicalPlan(op1,t_boolean,column_index,s);

	expr_func f=getExprFunc(op1,s);

	void* tuple=malloc(s->getTupleMaxSize());
	((NValue*)s->getColumnAddess(0,tuple))->createDecimalFromString("400000000000000000");
	((NValue*)s->getColumnAddess(1,tuple))->createDecimalFromString("399999999999999999");
//	*((NValue*)s->getColumnAddess(1,tuple))=4;
	bool ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op1;
	EXPECT_TRUE(ret);
}

TEST_F(NValueExpressionGenTest, EqualCompare) {
	/* #      #1    | #2
	 * Tuple: decimal  | decimal
	 *        3     | 4
	 * Express: #1 < #2 = true
	 */
	std::vector<column_type> columns;
	columns.push_back(data_type(t_decimal));
	columns.push_back(data_type(t_decimal));
	Schema* s=new SchemaFix(columns);
	map<std::string,int> column_index;
	column_index["#1"]=0;
	column_index["#2"]=1;
	QColcumns* a=new QColcumns("T","#1",t_decimal,"#1");
	QColcumns* b=new QColcumns("T","#2",t_decimal,"#2");

	QExpr_binary* op1=new QExpr_binary(a,b,t_decimal,oper_equal,t_qexpr_cmp,"result");

	InitExprAtLogicalPlan(op1,t_boolean,column_index,s);

	expr_func f=getExprFunc(op1,s);

	void* tuple=malloc(s->getTupleMaxSize());
	((NValue*)s->getColumnAddess(0,tuple))->createDecimalFromString("3992345766342542999");
	((NValue*)s->getColumnAddess(1,tuple))->createDecimalFromString("3992345766342542999");
//	*((NValue*)s->getColumnAddess(1,tuple))=4;
	bool ret;
	f(tuple,&ret);
	delete tuple;
	delete s;
	delete op1;
	EXPECT_TRUE(ret);
}



#endif /* NVALUEEXPRESSIONGENTEST_H_ */
