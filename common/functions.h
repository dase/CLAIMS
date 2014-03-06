/*
 * functions.h
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "ExpressionCalculator.h"
#include "TypePromotionMap.h"
#include "TypeCast.h"
//extern data_type arith_type_promotion_map[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];

bool check_data_type_for_add(data_type dt){
	switch(dt){
	case t_int:{
		return true;
	}
	case t_float:{
		return true;
	}
	case t_double:{
		return true;
	}
	case t_u_long:{
		return true;
	}
	case t_smallInt:{
		return true;
	}
	default:{
		return false;
	}
	}
}

static void add_in_same_type(const ExpressionItem& left,const ExpressionItem& right,ExpressionItem& target){
	assert(left.return_type==right.return_type);
	switch(left.return_type){
		case t_int:{
			target.return_type=t_int;
			target.content.data.value._int=left.content.data.value._int+right.content.data.value._int;
			break;
		}
		case t_float:{
			target.return_type=t_float;
			target.content.data.value._float=left.content.data.value._float+right.content.data.value._float;
			break;
		}
		default:{
			printf("add type not supproted!\n");
		}
	}
}

static void mins_in_same_type(const ExpressionItem& left,const ExpressionItem& right,ExpressionItem& target){
	assert(left.return_type==right.return_type);
	switch(left.return_type){
		case t_int:{
			target.return_type=t_int;
			target.content.data.value._int=left.content.data.value._int-right.content.data.value._int;
			break;
		}
		case t_float:{
			target.return_type=t_float;
			target.content.data.value._float=left.content.data.value._float-right.content.data.value._float;
			break;
		}
		default:{
			printf("add type not supproted!\n");
		}
	}
}

static void add(ExpressionItemStack& stack, ExpressionItem& target){
	//assuming all types are int.
	assert(stack.size()>=2);
	ExpressionItem right=stack.top();
	stack.pop();
	if(!check_data_type_for_add(right.return_type)){
		printf("%s is not supported for +!\n",getReturnTypeName(right.return_type).c_str());
	}
	ExpressionItem left=stack.top();
	stack.pop();
	if(!check_data_type_for_add(left.return_type)){
		printf("%s is not supported for +!\n",getReturnTypeName(left.return_type).c_str());
	}
//	target.return_type=right.return_type>left.return_type?right.return_type:left.return_type;
	target.return_type=TypePromotion::arith_type_promotion_map[left.return_type][right.return_type];

	if(target.return_type!=left.return_type){
		//left should be converted into target.return_type if two types are different
		TypeCast::type_cast_functions[left.return_type][target.return_type](left);
	}
	if(target.return_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][target.return_type](right);
	}

	add_in_same_type(left,right,target);

}

static void mins(ExpressionItemStack& stack, ExpressionItem& target){
	//assuming all types are int.
	assert(stack.size()>=2);
	ExpressionItem right=stack.top();
	stack.pop();
	if(!check_data_type_for_add(right.return_type)){
		printf("%s is not supported for +!\n",getReturnTypeName(right.return_type).c_str());
	}
	ExpressionItem left=stack.top();
	stack.pop();
	if(!check_data_type_for_add(left.return_type)){
		printf("%s is not supported for +!\n",getReturnTypeName(left.return_type).c_str());
	}
//	target.return_type=right.return_type>left.return_type?right.return_type:left.return_type;
	target.return_type=TypePromotion::arith_type_promotion_map[left.return_type][right.return_type];

	if(target.return_type!=left.return_type){
		//left should be converted into target.return_type if two types are different
		TypeCast::type_cast_functions[left.return_type][target.return_type](left);
	}
	if(target.return_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][target.return_type](right);
	}

	mins_in_same_type(left,right,target);
}

static void adds(SchemaTypeStack& stack, schema_type& target){
	assert(stack.size()>=2);
	schema_type right=stack.top();
	stack.pop();
	if(!check_data_type_for_add(right.type_union.datatype_)){
		printf("%s is not supported for +!\n",getReturnTypeName(right.type_union.datatype_).c_str());
	}
	schema_type left=stack.top();
	stack.pop();
	if(!check_data_type_for_add(left.type_union.datatype_)){
		printf("%s is not supported for +!\n",getReturnTypeName(left.type_union.datatype_).c_str());
	}
//	target.return_type=right.return_type>left.return_type?right.return_type:left.return_type;
	target.type_union.datatype_=TypePromotion::arith_type_promotion_map[left.type_union.datatype_][right.type_union.datatype_];
}

static void mins(SchemaTypeStack& stack, schema_type& target){
	assert(stack.size()>=2);
	schema_type right=stack.top();
	stack.pop();
	if(!check_data_type_for_add(right.type_union.datatype_)){
		printf("%s is not supported for +!\n",getReturnTypeName(right.type_union.datatype_).c_str());
	}
	schema_type left=stack.top();
	stack.pop();
	if(!check_data_type_for_add(left.type_union.datatype_)){
		printf("%s is not supported for +!\n",getReturnTypeName(left.type_union.datatype_).c_str());
	}
//	target.return_type=right.return_type>left.return_type?right.return_type:left.return_type;
	target.type_union.datatype_=TypePromotion::arith_type_promotion_map[left.type_union.datatype_][right.type_union.datatype_];
}

static void compare_less_in_same_type(const ExpressionItem& left,const ExpressionItem& right,ExpressionItem& target){
	assert(left.return_type==right.return_type);
	target.return_type=t_boolean;
	switch(left.return_type){
	case t_int:{
		target.content.data.value._bool=left.content.data.value._int<right.content.data.value._int;
		break;
	}
	case t_float:{

		target.content.data.value._bool=(left.content.data.value._float<right.content.data.value._float);
		break;
	}
	defalut:{
		printf("add type not supproted!\n");
	}
	}
}

static void compare_less(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=2);
	ExpressionItem left,right;

	right=stack.top();
	stack.pop();

	left=stack.top();
	stack.pop();

	/**In the current implementation, arithmetic type promotion map is used.
	 * TODO: Use specific mapping for compare function if needed.
	 */
	data_type promoted_type=TypePromotion::arith_type_promotion_map[left.return_type][right.return_type];

	if(promoted_type!=left.return_type){
		//left should be converted into tpromoted_type if two types are different
		TypeCast::type_cast_functions[left.return_type][promoted_type](left);
	}
	if(promoted_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][promoted_type](right);
	}

	target.type=ExpressionItem::const_type;
	compare_less_in_same_type(left,right,target);


}

inline bool search_for_and_construct_else_exp(ExpressionItemStack& stack,Expression& case_exp){
	bool finished=false;
	while((!stack.empty())){
		switch(stack.top().type){
		case ExpressionItem::const_type:{
			case_exp.push_back(stack.top());
			stack.pop();
			break;
		}
		case ExpressionItem::operator_type:{
			op_type tp=stack.top().content.op.op_;
			if(tp==op_case_else){
				case_exp.push_back(stack.top());
				stack.pop();
				finished=true;
				break;
			}
			else if(tp==op_case_when||tp==op_case){
				finished=true;
			}
			else{
				case_exp.push_back(stack.top());
				stack.pop();
			}

			break;
		}
		case ExpressionItem::variable_type:{

		}
		defalut:{
			assert(false);
		}
		}
		if(finished)
			break;
	}

	/**check if the top of stack_when is when. If it is not, it means that we fails
	 * to find a when cause, so we recovery the original stack as it was.
	 */
	if(case_exp.back().type!=ExpressionItem::operator_type||case_exp.back().content.op.op_!=op_case_else){
		for(unsigned i=case_exp.size()-1;i>=0;i--){
			stack.push(case_exp[i]);
		}
		return false;
	}
	return true;
}

inline bool search_for_and_construct_then_exp(ExpressionItemStack& stack,Expression& then_exp){
	bool finished=false;
	while((!stack.empty())){
		switch(stack.top().type){
		case ExpressionItem::const_type:{
			then_exp.push_back(stack.top());
			stack.pop();
			break;
		}
		case ExpressionItem::operator_type:{
			op_type tp=stack.top().content.op.op_;
			if(tp==op_case_then){
				then_exp.push_back(stack.top());
				stack.pop();
				finished=true;
				break;
			}
			else if(tp==op_case_when||tp==op_case){
				finished=true;
			}
			else{
				then_exp.push_back(stack.top());
				stack.pop();
			}

			break;
		}
		case ExpressionItem::variable_type:{

		}
		defalut:{
			assert(false);
		}
		}
		if(finished)
			break;
	}

	/**check if the top of stack_when is when. If it is not, it means that we fails
	 * to find a when cause, so we recovery the original stack as it was.
	 */
	if(then_exp.back().type!=ExpressionItem::operator_type||then_exp.back().content.op.op_!=op_case_then){
		for(int i=then_exp.size()-1;i>=0;i--){
			stack.push(then_exp[i]);
		}
		return false;
	}
	return true;
}

inline bool search_for_and_construct_when_exp(ExpressionItemStack& stack, Expression& when_exp){
	when_exp.clear();
	bool finished=false;
	while((!stack.empty())){
		switch(stack.top().type){
		case ExpressionItem::const_type:{
			when_exp.push_back(stack.top());
			stack.pop();
			break;
		}
		case ExpressionItem::operator_type:{
			op_type tp=stack.top().content.op.op_;
			if(tp==op_case_when){
				when_exp.push_back(stack.top());
				stack.pop();
				finished=true;
				break;
			}
			else if(tp==op_case_else||tp==op_case){
				finished=true;
			}
			else{
				when_exp.push_back(stack.top());
				stack.pop();
			}

			break;
		}
		case ExpressionItem::variable_type:{

		}
		defalut:{
			assert(false);
		}
		}
		if(finished)
			break;
	}

	/**check if the top of stack_when is when. If it is not, it means that we fails
	 * to find a when cause, so we recovery the original stack as it was.
	 */
	if(when_exp.back().type!=ExpressionItem::operator_type||when_exp.back().content.op.op_!=op_case_when){
		for(int i=when_exp.size()-1;i>=0;i--){
			stack.push(when_exp[i]);
		}
		return false;
	}
	return true;


}

static void do_case(ExpressionItemStack& stack, ExpressionItem& target){
	//The bottom of the stack is the begin symbol of case operator,



	/**     the stack for
	 * 						case
	 * 							when x<60 then "FAIL"
	 * 							when x<80 then "GOOD"
	 * 							else "MISSING"
	 * 						end
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


	stack.pop();// drop the top element ----> CASE

	Expression when_exp;
	ExpressionItem when_result;
	while(search_for_and_construct_when_exp(stack,when_exp)){
		assert(when_exp.back().type==ExpressionItem::operator_type&&when_exp.back().content.op.op_==op_case_when);
		when_exp.pop_back();//drop when, so the remaining is the boolean expression

		/* calculate the boolean expression */
		ExpressionCalculator::calcuate(when_exp,when_result);
		assert(when_result.type==ExpressionItem::const_type&&when_result.return_type==t_boolean);


		Expression then_exp;
		bool found=search_for_and_construct_then_exp(stack,then_exp);
		assert(found);

		if(when_result.content.data.value._bool==true){
			// when cause is true, the value for the entire case cause is decided by the subsequent then cause
			ExpressionItem then_result;


			//calculate the value.
			assert(then_exp.back().type==ExpressionItem::operator_type&&then_exp.back().content.op.op_==op_case_then);
			then_exp.pop_back();//drop then, so the remaining is the value expression for the result of the entire case cause.
			ExpressionCalculator::calcuate(then_exp,then_result);
			assert(then_result.type==ExpressionItem::const_type);

			//set stack empty
			while(!stack.empty()){
				stack.pop();
			}
			target=then_result;
			return;



		}
		else{
//			when_exp.clear();
			continue;
		}

	}

	/* no when cause is true */
	Expression else_exp;
	bool found=search_for_and_construct_else_exp(stack,else_exp);

	assert(found);
	assert(else_exp.back().type==ExpressionItem::operator_type&&else_exp.back().content.op.op_==op_case_else);

	//calculate the value
	ExpressionItem else_result;
	else_exp.pop_back();// drop else, so the remaining is the value for the result of the entire case cause;
	ExpressionCalculator::calcuate(else_exp,else_result);
	assert(else_result.type==ExpressionItem::const_type);
	assert(stack.size()==0);
	target=else_result;
	return;




}
#endif /* FUNCTIONS_H_ */
