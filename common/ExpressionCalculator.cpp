/*
 * ExpressionCalculator.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */

#include "ExpressionCalculator.h"
#include "functions.h"
ExpressionCalculator::ExpressionCalculator() {

}

ExpressionCalculator::~ExpressionCalculator() {

}

void ExpressionCalculator::compute(ExpressionItem operator_item,ExpressionItemStack& stack){
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
		case op_multiple:{
			mul(stack,result);
			break;
		}

		case op_case:{
			reverse_stack(stack);
			do_case(stack,result);
			break;
		}
		case op_upper:{
			upper(stack,result);
			break;
		}
		case op_substring:{
			substring(stack,result);
			break;
		}
		case op_trim:{
			trim(stack,result);
			break;
		}
		case op_cast:{
			cast(stack,result);
			break;
		}
		case op_and:{
			andop(stack,result);
			break;
		}
		case op_or:{
			orop(stack,result);
			break;
		}
		case op_not:{
			notop(stack,result);
			break;
		}
		case op_com_L:{
			compare_less(stack,result);
			break;
		}
		case op_com_G:{
			compare_great(stack,result);
			break;
		}
		case op_com_EQ:{
			compare_equal(stack,result);
			break;
		}
		case op_com_NEQ:{
			compare_not_equal(stack,result);
			break;
		}
		case op_com_GEQ:{
			compare_great_equal(stack,result);
			break;
		}
		case op_com_LEQ:{
			compare_less_equal(stack,result);
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
		case op_multiple:{
			muls(stack,result);
			break;
		}
		case op_mins:{
			minss(stack,result);
			break;
		}
		case op_case:{
			reverse_stack(stack);
			do_case(stack,result);
			break;
		}
		case op_upper:{

			break;
		}
		case op_substring:{

			break;
		}
		case op_trim:{

			break;
		}
		case op_cast:{

			break;
		}
		case op_and:
		case op_or:
		case op_com_G:
		case op_com_EQ:
		case op_com_NEQ:
		case op_com_GEQ:
		case op_com_LEQ:
		case op_com_L:
		{
			get_compare_return_type(stack,result,0);
			break;
		}
		case op_not:
		{
			get_compare_return_type(stack,result,1);
		}break;
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

column_type ExpressionCalculator::getOutputType_(std::vector<ExpressionItem> &exp){
	ExpressionItemStack stack;
	column_type *ct=0;
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
	data_type dt=stack.top().return_type;
	if(dt==t_string){
		ct=new column_type(dt,exp[0].size);
	}
	else{
		ct=new column_type(dt);
	}
	return *ct;
}
