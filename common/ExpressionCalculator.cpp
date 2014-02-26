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
	}
	}
	stack.push(result);
}
