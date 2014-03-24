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
static void test_add(){
	//3+8.8+(-10)=1.8
	std::vector<ExpressionItem> express_item_list;

	ExpressionItem ei1;
	ei1.setIntValue("3");
	express_item_list.push_back(ei1);

	ExpressionItem ei2;
	ei2.setFloatValue("8.8");
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

//		for(unsigned i=0;i<express_item_list.size();i++){
//			express_item_list[i].print();
//		}

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);
	print_test_name_result(is_equal(result.content.data.value._float,1.8)&&result.return_type==t_float,"(+)");
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

//		for(unsigned i=0;i<express_item_list.size();i++){
//			express_item_list[i].print();
//		}

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
//	result.print();

	print_test_name_result(result._string=="FAILED"&&result.return_type==t_string,"case");


}

inline void test_upper(){
	Expression express_item_list;

	ExpressionItem ei0;								// x
	ei0.setStringValue("abc");
	express_item_list.push_back(ei0);

	ExpressionItem ei1;							 	// 60
	ei1.setOperator("upper");
	express_item_list.push_back(ei1);

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);

}

static int test_expression_item(){
	initialize_arithmetic_type_promotion_matrix();
	initialize_type_cast_functions();

	test_add();
	test_com_less();
	test_case_exp();
	test_upper();
	return 0;
}

