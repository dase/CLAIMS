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
		case op_mins:{
			mins(stack,result);
			break;
		}
		case op_com_L:{
			compare_less(stack,result);
			break;
		}
		case op_case:{
			reverse_stack(stack);
			do_case(stack,result);
			break;
		}
		default:{
			printf("The operator[%d] is not supported currently!\n",operator_item.content.op.op_);
		}
	}
	stack.push(result);
}

void ExpressionCalculator::computes(ExpressionItem operator_item, ExpressionItemStack& stack){
	ExpressionItem result;
	result.type=ExpressionItem::const_type;

	switch(operator_item.content.op.op_){
		case op_add:{
			//Currently, only the adds can be sopported!!!
			//todo: find a solution to solve everything!!!
			adds(stack,result);
			break;
		}
		case op_mins:{
			minss(stack,result);
			break;
		}
		case op_com_L:{
			compare_lesss(stack,result);
			break;
		}
		case op_case:{
			reverse_stack(stack);
			do_case(stack,result);
			break;
		}
		default:{
			printf("The operator[%d] is not supported currently!\n",operator_item.content.op.op_);
		}
	}

	stack.push(result);
}

data_type ExpressionCalculator::getOutputType(std::vector<ExpressionItem> &exp){
	ExpressionItemStack stack;
	for(unsigned i=0;i<exp.size();i++){
		if(exp[i].type!=ExpressionItem::operator_type){
			stack.push(exp[i]);
		}
		else{
			if(isComposeOperator(exp[i].content.op.op_)){
				op_type compose_op=exp[i].content.op.op_;
				stack.push(exp[i++]);
				unsigned j=i;
				bool processed_compose_op=false;
				for(;j<exp.size();j++){
					if(exp[j].type==ExpressionItem::operator_type&&exp[j].content.op.op_==compose_op)
					{
						unsigned before = stack.size();
						assert(stack.size()==before);
						computes(exp[j],stack);
						i=j+1;
						processed_compose_op=true;
						break;
					}
					else{
						stack.push(exp[j]);
					}
				}
				if(!processed_compose_op){
					printf("No end operator for operator[%d] is found!\n",compose_op);
					assert(false);
				}
			}
			else{
				computes(exp[i],stack);
			}
		}
	}
	assert(stack.size()==1);
	return stack.top().return_type;
}
