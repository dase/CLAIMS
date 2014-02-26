/*
 * Expression_item.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */
#include <vector>
#include "ExpressionItem.h"
#include "ExpressionCalculator.h"
static int test_expression_item(){
	std::vector<ExpressionItem> express_item_list;

	ExpressionItem ei1;
	ei1.type=ExpressionItem::const_type;
	ei1.content.data.return_type=t_int;
	ei1.content.data.value._int=3;
	express_item_list.push_back(ei1);

	ExpressionItem ei2;
	ei2.type=ExpressionItem::const_type;
	ei2.content.data.return_type=t_int;
	ei2.content.data.value._int=8;
	express_item_list.push_back(ei2);

	ExpressionItem ei3;
	ei3.type=ExpressionItem::operator_type;
	ei3.content.op.return_type=t_int;
//	ei3.content.op.num_of_parameter=2;
	ei3.content.op.op_=op_add;
	express_item_list.push_back(ei3);

	ExpressionItem ei4;
	ei4.type=ExpressionItem::const_type;
	ei4.content.data.return_type=t_int;
	ei4.content.data.value._int=-10;
	express_item_list.push_back(ei4);

	ExpressionItem ei5;
	ei5.type=ExpressionItem::operator_type;
	ei5.content.op.return_type=t_int;
//	ei5.content.op.num_of_parameter=2;
	ei5.content.op.op_=op_add;
	express_item_list.push_back(ei5);

	for(unsigned i=0;i<express_item_list.size();i++){
		express_item_list[i].print();
	}

	Expression exp;
	for(unsigned i=0;i<express_item_list.size();i++){
		exp.push_back(express_item_list[i]);
	}


	ExpressionCalculator::calcuate(express_item_list);



}

