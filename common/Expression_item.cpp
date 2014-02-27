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
static int test_expression_item(){
	initialize_arithmetic_type_promotion_matrix();
	initialize_type_cast_functions();

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

	for(unsigned i=0;i<express_item_list.size();i++){
		express_item_list[i].print();
	}
//
//	Expression exp;
//	for(unsigned i=0;i<express_item_list.size();i++){
//		exp.push_back(express_item_list[i]);
//	}


	ExpressionCalculator::calcuate(express_item_list);

	express_item_list[1].content.data.value._float+=100;

	ExpressionCalculator::calcuate(express_item_list);


}

