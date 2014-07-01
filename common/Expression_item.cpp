/*
 * Expression_item.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */
#include <vector>
#include "ExpressionItem.h"
#include "ExpressionCalculator.h"
#include "TypeCast.h"
#include "TypePromotionMap.h"
#include "../utility/test_tool.h"
static void test_add_(){
	std::vector<ExpressionItem> express_item_list;

	ExpressionItem ei1;
	ExpressionItem ei2;
	ExpressionItem ei3;
	ExpressionItem ei4;
	ExpressionItem ei5;
	ExpressionItem ei6;
	ExpressionItem ei7;
	ExpressionItem ei8;
	ExpressionItem ei9;

	ei1.setIntValue("2");
	ei2.setIntValue("3");
	ei3.setIntValue("4");
	ei4.setOperator("+");
	ei5.setOperator("*");
	ei6.setIntValue("6");
	ei7.setIntValue("5");
	ei8.setOperator("-");
	ei9.setOperator("*");

	express_item_list.push_back(ei1);
	express_item_list.push_back(ei2);
	express_item_list.push_back(ei3);
	express_item_list.push_back(ei4);
	express_item_list.push_back(ei5);
	express_item_list.push_back(ei6);
	express_item_list.push_back(ei7);
	express_item_list.push_back(ei8);
	express_item_list.push_back(ei9);


	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);
	cout<<result.content.data.value._int<<endl;
	print_test_name_result(is_equal(result.content.data.value._int,14)&&result.return_type==t_int,"(+)");
}

static void test_add(){
	std::vector<ExpressionItem> express_item_list;

	ExpressionItem ei1;
	ei1.setIntValue("3");
	express_item_list.push_back(ei1);

	ExpressionItem ei2;
	ei2.setDecimalValue("8.8");
	express_item_list.push_back(ei2);

	ExpressionItem ei3;
	ei3.setOperator("+");
	express_item_list.push_back(ei3);

	ExpressionItem ei4;
	ei4.setIntValue("-10");
	express_item_list.push_back(ei4);

	ExpressionItem ei5;
	ei5.setOperator("+");
	express_item_list.push_back(ei5);

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);
	string s=result._decimal.createStringFromDecimal();
	print_test_name_result(is_equal(NValue::getDecimalValueFromString(s),NValue::getDecimalValueFromString("1.8"))&&result.return_type==t_decimal,"(+)");
}

static void test_multiple(){
	std::vector<ExpressionItem> express_item_list;

	ExpressionItem e1;
	e1.setIntValue("3");
	express_item_list.push_back(e1);

	ExpressionItem e2;
	e2.setDecimalValue("1.8");
	express_item_list.push_back(e2);

	ExpressionItem e3;
	e3.setOperator("*");
	express_item_list.push_back(e3);

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);
	string s=result._decimal.createStringFromDecimal();
	print_test_name_result(is_equal(NValue::getDecimalValueFromString(s),NValue::getDecimalValueFromString("5.40"))&&result.return_type==t_decimal,"(*)");
}

static void test_multiple_(){
	std::vector<ExpressionItem> express_item_list;

	ExpressionItem e1;
	e1.setIntValue("3");
	express_item_list.push_back(e1);

	ExpressionItem e2;
	e2.setFloatValue("1.8");
	express_item_list.push_back(e2);

	ExpressionItem e3;
	e3.setOperator("*");
	express_item_list.push_back(e3);

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);
	print_test_name_result(is_equal(result.content.data.value._float,5.4)&&result.return_type==t_float,"(*)");
}

static void test_com_less(){
	//(3.3+4.4)<10
	std::vector<ExpressionItem> express_item_list;

	ExpressionItem ei1;
	ei1.setFloatValue("3.3");
	express_item_list.push_back(ei1);

	ExpressionItem ei2;
	ei2.setFloatValue("4.4");
	express_item_list.push_back(ei2);

	ExpressionItem ei3;
	ei3.setOperator("-");
	express_item_list.push_back(ei3);

	ExpressionItem ei4;
	ei4.setIntValue("10");
	express_item_list.push_back(ei4);

	ExpressionItem ei5;
	ei5.setOperator("<");
	express_item_list.push_back(ei5);

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);
	print_test_name_result(result.content.data.value._bool==true&&result.return_type==t_boolean,"compare(<)");
}

inline void test_case_exp(){
	/**     Expression:
	 * 						case
	 * 							when x<60 then "FAIL"
	 * 							when x<80 then "GOOD"
	 * 							else "PERFECT"
	 * 						end
	 *
	 *
	 *      is as following:
	 *
	 * 			CASE-end
	 * 			X
	 * 			60
	 * 			<
	 * 			WHEN
	 * 			"FAIL"
	 * 			THEN
	 * 			X
	 * 			80
	 * 			<
	 * 			WHEN
	 * 			"GOOD"
	 * 			THEN
	 * 			"PERFECT"
	 * 			ELSE
	 * 			CASE
	 */
	const int score=40; //x value


	Expression express_item_list;

	ExpressionItem ei1;								// case
	ei1.setOperator("case");
	express_item_list.push_back(ei1);


	ExpressionItem ei2;								// x
	ei2.setIntValue(score);
	express_item_list.push_back(ei2);

	ExpressionItem ei3;							 	// 60
	ei3.setIntValue("60");
	express_item_list.push_back(ei3);

	ExpressionItem ei4;								// <
	ei4.setOperator("<");
	express_item_list.push_back(ei4);

	ExpressionItem ei5;								// when
	ei5.setOperator("when");
	express_item_list.push_back(ei5);

	ExpressionItem ei6;							 	// value
	ei6.setStringValue("FAILED");
	express_item_list.push_back(ei6);

	ExpressionItem ei7;								// then
	ei7.setOperator("then");
	express_item_list.push_back(ei7);

	ExpressionItem ei8;								// x
	ei8.setIntValue(score);
	express_item_list.push_back(ei2);

	ExpressionItem ei9;							 	// 60
	ei9.setIntValue(80);
	express_item_list.push_back(ei3);

	ExpressionItem ei10;								// <
	ei10.setOperator("<");
	express_item_list.push_back(ei4);

	ExpressionItem ei11;								// when
	ei11.setOperator("when");
	express_item_list.push_back(ei5);

	ExpressionItem ei12;							 	// value
	ei12.setStringValue("Good");
	express_item_list.push_back(ei6);

	ExpressionItem ei13;								// then
	ei13.setOperator("then");
	express_item_list.push_back(ei7);

	ExpressionItem ei14;							 	// value
	ei14.setStringValue("PERFECT");
	express_item_list.push_back(ei14);

	ExpressionItem ei15;								// else
	ei15.setOperator("else");
	express_item_list.push_back(ei15);

	ExpressionItem ei16;								// case
	ei16.setOperator("case");
	express_item_list.push_back(ei16);

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);

	print_test_name_result(result._string=="FAILED"&&result.return_type==t_string,"case");
}

inline void test_upper(){
	Expression express_item_list;

	ExpressionItem ei0;								// x
	ei0.setStringValue("asbcd");
	express_item_list.push_back(ei0);

	ExpressionItem ei1;							 	// 60
	ei1.setOperator("upper");
	express_item_list.push_back(ei1);

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);

	print_test_name_result(result._string=="ASBCD"&&result.return_type==t_string,"upper");
}

inline void test_substring(){
	Expression express_item_list;

	ExpressionItem ei0;								// x
	ei0.setStringValue("abdcsga");
	express_item_list.push_back(ei0);

	ExpressionItem ei1;							 	// 60
	ei1.setIntValue("1");
	express_item_list.push_back(ei1);

	ExpressionItem ei2;							 	// 60
	ei2.setIntValue("5");
	express_item_list.push_back(ei2);

	ExpressionItem ei3;							 	// 60
	ei3.setOperator("substring");
	express_item_list.push_back(ei3);

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);

	print_test_name_result(result._string=="bdcsg"&&result.return_type==t_string,"substring");
}

inline void test_trim(){
	Expression express_item_list;

	ExpressionItem ei1;							 	// 60
	ei1.setStringValue(" ");
	express_item_list.push_back(ei1);

	ExpressionItem ei0;							 	// 60
	ei0.setIntValue("0");
	express_item_list.push_back(ei0);

	ExpressionItem ei2;								// x
	ei2.setStringValue("  abdsc       ");
	express_item_list.push_back(ei2);

	ExpressionItem ei3;								// x
	ei3.setOperator("trim");
	express_item_list.push_back(ei3);

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);

	print_test_name_result(result._string=="abdsc"&&result.return_type==t_string,"trim");
}

inline void test_cast(){
	Expression express_item_list;

	ExpressionItem ei0;							 	// 60
	ei0.setIntValue("000012");
	express_item_list.push_back(ei0);

	ExpressionItem ei1;								// x
	ei1.setStringValue("int");
	express_item_list.push_back(ei1);

	ExpressionItem ei2;								// x
	ei2.setOperator("cast");
	express_item_list.push_back(ei2);

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);

	print_test_name_result(result.content.data.value._int==12&&result.return_type==t_int,"cast");
}

inline void test_decimal_add2(){
	NValue v1 = NValue::getDecimalValueFromString("4.4");
	NValue v2 = NValue::getDecimalValueFromString("-8");
	column_type* op=new column_type(data_type(t_decimal),2);
	op->operate->multiple(&v1,&v2);
	if(!print_test_name_result(op->operate->toString(&v1)==string("-35.20"),"Decimal multiple")){
		printf("Expected -35.20, actual: %s\n",op->operate->toString(&v1).c_str());
	}
}

static int test_expression_item(){
	initialize_arithmetic_type_promotion_matrix();
	initialize_type_cast_functions();

	test_add();
//	test_add_();
//	test_com_less();
//	test_case_exp();
//	test_upper();
//	test_substring();
//	test_trim();
//	test_cast();
//	test_multiple();
//	test_multiple_();

//	test_decimal_add2();

	return 0;
}

