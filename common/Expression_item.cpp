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
		ei1.type=ExpressionItem::const_type;
		ei1.return_type=t_int;
		ei1.content.data.value._int=3;
		express_item_list.push_back(ei1);

		ExpressionItem ei2;
		ei2.type=ExpressionItem::const_type;
		ei2.return_type=t_float;
		ei2.content.data.value._float=8.8;
		express_item_list.push_back(ei2);

		ExpressionItem ei3;
		ei3.type=ExpressionItem::operator_type;
	//	ei3.return_type=t_int;
	//	ei3.content.op.num_of_parameter=2;
		ei3.content.op.op_=op_add;
		express_item_list.push_back(ei3);

		ExpressionItem ei4;
		ei4.type=ExpressionItem::const_type;
		ei4.return_type=t_int;
		ei4.content.data.value._int=-10;
		express_item_list.push_back(ei4);

		ExpressionItem ei5;
		ei5.type=ExpressionItem::operator_type;
	//	ei5.return_type=t_int;
	//	ei5.content.op.num_of_parameter=2;
		ei5.content.op.op_=op_add;
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
		ei1.type=ExpressionItem::const_type;
		ei1.return_type=t_float;
		ei1.content.data.value._float=3.3;
		express_item_list.push_back(ei1);

		ExpressionItem ei2;
		ei2.type=ExpressionItem::const_type;
		ei2.return_type=t_float;
		ei2.content.data.value._float=4.4;
		express_item_list.push_back(ei2);

		ExpressionItem ei3;
		ei3.type=ExpressionItem::operator_type;
	//	ei3.return_type=t_int;
	//	ei3.content.op.num_of_parameter=2;
		ei3.content.op.op_=op_add;
		express_item_list.push_back(ei3);

		ExpressionItem ei4;
		ei4.type=ExpressionItem::const_type;
		ei4.return_type=t_int;
		ei4.content.data.value._int=10;
		express_item_list.push_back(ei4);

		ExpressionItem ei5;
		ei5.type=ExpressionItem::operator_type;
	//	ei5.return_type=t_int;
	//	ei5.content.op.num_of_parameter=2;
		ei5.content.op.op_=op_com_L;
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
	 * 			<
	 * 			60
	 * 			WHEN
	 * 			"FAIL"
	 * 			THEN
	 * 			X
	 * 			<
	 * 			80
	 * 			WHEN
	 * 			"GOOD"
	 * 			THEN
	 * 			"PERFECT"
	 * 			ELSE
	 * 			CASE
	 */
	const int score=40; //x value


	std::vector<ExpressionItem> express_item_list;

	ExpressionItem ei1;								// case
	ei1.type=ExpressionItem::operator_type;
	ei1.content.op.op_=op_case;
	express_item_list.push_back(ei1);


	ExpressionItem ei2;								// x
	ei2.type=ExpressionItem::const_type;
	ei2.content.data.value._int=score;
	ei2.return_type=t_int;
	express_item_list.push_back(ei2);

	ExpressionItem ei3;							 	// 60
	ei3.type=ExpressionItem::const_type;
	ei3.content.data.value._int=60;
	ei3.return_type=t_int;
	express_item_list.push_back(ei3);

	ExpressionItem ei4;								// <
	ei4.type=ExpressionItem::operator_type;
	ei4.content.op.op_=op_com_L;
	express_item_list.push_back(ei4);

	ExpressionItem ei5;								// when
	ei5.type=ExpressionItem::operator_type;
	ei5.content.op.op_=op_case_when;
	express_item_list.push_back(ei5);

	ExpressionItem ei6;							 	// value
	ei6.type=ExpressionItem::const_type;
	ei6._string="FAILED";
	ei6.return_type=t_string;
	express_item_list.push_back(ei6);

	ExpressionItem ei7;								// then
	ei7.type=ExpressionItem::operator_type;
	ei7.content.op.op_=op_case_then;
	express_item_list.push_back(ei7);

	ExpressionItem ei8;								// x
	ei2.type=ExpressionItem::const_type;
	ei2.content.data.value._int=score;
	ei2.return_type=t_int;
	express_item_list.push_back(ei2);

	ExpressionItem ei9;							 	// 60
	ei3.type=ExpressionItem::const_type;
	ei3.content.data.value._int=80;
	ei3.return_type=t_int;
	express_item_list.push_back(ei3);

	ExpressionItem ei10;								// <
	ei4.type=ExpressionItem::operator_type;
	ei4.content.op.op_=op_com_L;
	express_item_list.push_back(ei4);

	ExpressionItem ei11;								// when
	ei5.type=ExpressionItem::operator_type;
	ei5.content.op.op_=op_case_when;
	express_item_list.push_back(ei5);

	ExpressionItem ei12;							 	// value
	ei6.type=ExpressionItem::const_type;
	ei6._string="Good";
	ei6.return_type=t_string;
	express_item_list.push_back(ei6);

	ExpressionItem ei13;								// then
	ei7.type=ExpressionItem::operator_type;
	ei7.content.op.op_=op_case_then;
	express_item_list.push_back(ei7);

	ExpressionItem ei14;							 	// value
	ei14.type=ExpressionItem::const_type;
	ei14._string="PERFECT";
	ei14.return_type=t_string;
	express_item_list.push_back(ei14);

	ExpressionItem ei15;								// else
	ei15.type=ExpressionItem::operator_type;
	ei15.content.op.op_=op_case_else;
	express_item_list.push_back(ei15);

	ExpressionItem ei16;								// case
	ei16.type=ExpressionItem::operator_type;
	ei16.content.op.op_=op_case;
	express_item_list.push_back(ei16);

	ExpressionItem result;
	ExpressionCalculator::calcuate(express_item_list,result);
//	result.print();

	print_test_name_result(result._string=="FAILED"&&result.return_type==t_string,"case");


}
static int test_expression_item(){
	initialize_arithmetic_type_promotion_matrix();
	initialize_type_cast_functions();

	test_add();
	test_com_less();
	test_case_exp();
}

