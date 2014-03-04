/*
 * ExpressionCalculator.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */

#include "ExpressionCalculator.h"
#include "functions.h"
ExpressionCalculator::ExpressionCalculator() {
	// TODO Auto-generated constructor stub
}

ExpressionCalculator::~ExpressionCalculator() {
	// TODO Auto-generated destructor stub
}

void ExpressionCalculator::compute(ExpressionItem operator_item,ExpressionItemStack& stack){
//	assert(operator_item.content.op.num_of_parameter<=stack.size());
	ExpressionItem result;
	result.type=ExpressionItem::const_type;

	switch(operator_item.content.op.op_){
	case op_add:{
		add(stack,result);
		break;
	}
	case op_com_L:{
		compare_less(stack,result);
		break;
	}
	case op_case:{
		reverse_stack(stack);
//		print_expression(stack);
		do_case(stack,result);
		break;
	}
	default:{
		printf("The operator[%d] is not supported currently!\n",operator_item.content.op.op_);
	}
	}
	stack.push(result);
}
