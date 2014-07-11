/*
 * functions.h
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <algorithm>
#include <string>
#include <vector>
#include <functional>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "ExpressionCalculator.h"
#include "TypePromotionMap.h"
#include "TypeCast.h"
#include "types/NValue.hpp"
using namespace std;

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
		case t_decimal:{
			return true;
		}
		case t_u_long:{
			return true;
		}
		case t_smallInt:{
			return true;
		}
		case t_date:{
			return true;
		}
		case t_boolean:
		{
			return true;
		}
		default:{
			return false;
		}
	}
}
bool check_data_type_for_boolean(data_type dt){
	switch(dt){
		case t_int:{
			return true;
		}
		case t_boolean:
		{
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
		case t_u_long:{
			target.return_type=t_u_long;
			target.content.data.value._ulong=left.content.data.value._ulong+right.content.data.value._ulong;
			break;
		}
		case t_decimal:{
			target.return_type=t_decimal;
			NValue l=left._decimal;
			NValue r=right._decimal;
			target._decimal=l.op_add(r);
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
		case t_u_long:{
			target.return_type=t_u_long;
			target.content.data.value._ulong=left.content.data.value._ulong-right.content.data.value._ulong;
			break;
		}
		case t_decimal:{
			target.return_type=t_decimal;
			NValue l=left._decimal;
			NValue r=right._decimal;
			target._decimal=l.op_subtract(r);
			break;
		}
		default:{
			printf("add type not supproted!\n");
		}
	}
}

static void muls_in_same_type(const ExpressionItem& left,const ExpressionItem& right,ExpressionItem& target){
	assert(left.return_type==right.return_type);
	switch(left.return_type){
		case t_int:{
			target.return_type=t_int;
			target.content.data.value._int=left.content.data.value._int*right.content.data.value._int;
			break;
		}
		case t_float:{
			target.return_type=t_float;
			target.content.data.value._float=left.content.data.value._float*right.content.data.value._float;
			break;
		}
		case t_u_long:{
			target.return_type=t_u_long;
			target.content.data.value._ulong=left.content.data.value._ulong*right.content.data.value._ulong;
			break;
		}
		case t_decimal:{
			target.return_type=t_decimal;
			NValue l=left._decimal;
			NValue r=right._decimal;
			target._decimal=l.op_multiply(r);
			break;
		}
		default:{
			printf("add type not supproted!\n");
		}
	}
}

static void adds(ExpressionItemStack& stack, ExpressionItem& target){
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
	target.return_type=TypePromotion::arith_type_promotion_map[left.return_type][right.return_type];
}

static void add(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=2);
	ExpressionItem right=stack.top();
	stack.pop();
	ExpressionItem left=stack.top();
	stack.pop();

	if(!check_data_type_for_add(right.return_type)){
		printf("%s is not supported for +!\n",getReturnTypeName(right.return_type).c_str());
	}
	if(!check_data_type_for_add(left.return_type)){
		printf("%s is not supported for +!\n",getReturnTypeName(left.return_type).c_str());
	}

	target.return_type=TypePromotion::arith_type_promotion_map[left.return_type][right.return_type];
	if(target.return_type!=left.return_type){
		TypeCast::type_cast_functions[left.return_type][target.return_type](left);
	}
	if(target.return_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][target.return_type](right);
	}

	add_in_same_type(left,right,target);
}

static void minss(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=2);
	ExpressionItem right=stack.top();
	stack.pop();
	if(!check_data_type_for_add(right.return_type)){
		printf("%s is not supported for -s r!\n",getReturnTypeName(right.return_type).c_str());
	}
	ExpressionItem left=stack.top();
	stack.pop();
	if(!check_data_type_for_add(left.return_type)){
		printf("%s is not supported for -s l!\n",getReturnTypeName(left.return_type).c_str());
	}
	target.return_type=TypePromotion::arith_type_promotion_map[left.return_type][right.return_type];
}

static void mins(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=2);

	ExpressionItem right=stack.top();
	stack.pop();
	if(!check_data_type_for_add(right.return_type)){
		printf("%s is not supported for - r!\n",getReturnTypeName(right.return_type).c_str());
	}

	ExpressionItem left=stack.top();
	stack.pop();
	if(!check_data_type_for_add(left.return_type)){
		printf("%s is not supported for - l!\n",getReturnTypeName(left.return_type).c_str());
	}

	target.return_type=TypePromotion::arith_type_promotion_map[left.return_type][right.return_type];
	if(target.return_type!=left.return_type){
		TypeCast::type_cast_functions[left.return_type][target.return_type](left);
	}
	if(target.return_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][target.return_type](right);
	}
	mins_in_same_type(left,right,target);
}

static void muls(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=2);
	ExpressionItem right=stack.top();
	stack.pop();
	if(!check_data_type_for_add(right.return_type)){
		printf("%s is not supported for *!\n",getReturnTypeName(right.return_type).c_str());
	}
	ExpressionItem left=stack.top();
	stack.pop();
	if(!check_data_type_for_add(left.return_type)){
		printf("%s is not supported for *!\n",getReturnTypeName(left.return_type).c_str());
	}
	target.return_type=TypePromotion::arith_type_promotion_map[left.return_type][right.return_type];
}

static void mul(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=2);
	ExpressionItem right=stack.top();
	stack.pop();
	if(!check_data_type_for_add(right.return_type)){
		printf("%s is not supported for *!\n",getReturnTypeName(right.return_type).c_str());
	}
	ExpressionItem left=stack.top();
	stack.pop();
	if(!check_data_type_for_add(left.return_type)){
		printf("%s is not supported for *!\n",getReturnTypeName(left.return_type).c_str());
	}

	target.return_type=TypePromotion::arith_type_promotion_map[left.return_type][right.return_type];

	if(target.return_type!=left.return_type){
		TypeCast::type_cast_functions[left.return_type][target.return_type](left);
	}
	if(target.return_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][target.return_type](right);
	}

	muls_in_same_type(left,right,target);
}

static void compare_less_in_same_type(const ExpressionItem& left,const ExpressionItem& right,ExpressionItem& target){
	assert(left.return_type==right.return_type);
	target.return_type=t_int;
	switch(left.return_type){
		case t_int:{
			target.content.data.value._int=(left.content.data.value._int<right.content.data.value._int);
			break;
		}
		case t_float:{
			target.content.data.value._int=(left.content.data.value._float<right.content.data.value._float);
			break;
		}
		case t_double:{
			target.content.data.value._int=(left.content.data.value._double<right.content.data.value._double);
			break;
		}
		case t_smallInt:{
			target.content.data.value._int=(left.content.data.value._sint<right.content.data.value._sint);
			break;
		}
		case t_u_long:{
			target.content.data.value._int=(left.content.data.value._ulong<right.content.data.value._ulong);
			break;
		}
		case t_decimal:{
			target.content.data.value._int=left._decimal.op_less(right._decimal);
			break;
		}
		default:{
			printf("add type not supproted!\n");
		}
	}
}


static void compare_great_in_same_type(const ExpressionItem& left,const ExpressionItem& right,ExpressionItem& target){
	assert(left.return_type==right.return_type);
	target.return_type=t_int;
	switch(left.return_type){
		case t_int:{
			target.content.data.value._int=(left.content.data.value._int>right.content.data.value._int);
			break;
		}
		case t_float:{
			target.content.data.value._int=(left.content.data.value._float>right.content.data.value._float);
			break;
		}
		case t_double:{
			target.content.data.value._int=(left.content.data.value._double>right.content.data.value._double);
			break;
		}
		case t_smallInt:{
			target.content.data.value._int=(left.content.data.value._sint>right.content.data.value._sint);
			break;
		}
		case t_u_long:{
			target.content.data.value._int=(left.content.data.value._ulong>right.content.data.value._ulong);
			break;
		}
		case t_decimal:{
			target.content.data.value._int=left._decimal.op_great(right._decimal);
			break;
		}
		default:{
			printf("add type not supproted!\n");
		}
	}
}

static void compare_equal_in_same_type(const ExpressionItem& left,const ExpressionItem& right,ExpressionItem& target){
	assert(left.return_type==right.return_type);
	target.return_type=t_int;
	switch(left.return_type){
		case t_int:{
			target.content.data.value._int=(left.content.data.value._int==right.content.data.value._int);
			break;
		}
		case t_float:{
			target.content.data.value._int=(left.content.data.value._float==right.content.data.value._float);
			break;
		}
		case t_double:{
			target.content.data.value._int=(left.content.data.value._double==right.content.data.value._double);
			break;
		}
		case t_smallInt:{
			target.content.data.value._int=(left.content.data.value._sint==right.content.data.value._sint);
			break;
		}
		case t_u_long:{
			target.content.data.value._int=(left.content.data.value._ulong==right.content.data.value._ulong);
			break;
		}
		case t_decimal:{
			target.content.data.value._int=left._decimal.op_equals(right._decimal);
			break;
		}
		default:{
			printf("add type not supproted!\n");
		}
	}
}

static void compare_not_equal_in_same_type(const ExpressionItem& left,const ExpressionItem& right,ExpressionItem& target){
	assert(left.return_type==right.return_type);
	target.return_type=t_int;
	switch(left.return_type){
		case t_int:{
			target.content.data.value._int=(left.content.data.value._int!=right.content.data.value._int);
			break;
		}
		case t_float:{
			target.content.data.value._int=(left.content.data.value._float!=right.content.data.value._float);
			break;
		}
		case t_double:{
			target.content.data.value._int=(left.content.data.value._double!=right.content.data.value._double);
			break;
		}
		case t_smallInt:{
			target.content.data.value._int=(left.content.data.value._sint!=right.content.data.value._sint);
			break;
		}
		case t_u_long:{
			target.content.data.value._int=(left.content.data.value._ulong!=right.content.data.value._ulong);
			break;
		}
		case t_decimal:{
			target.content.data.value._int=left._decimal.op_not_equals(right._decimal);
			break;
		}
		default:{
			printf("add type not supproted!\n");
		}
	}
}
static void compare_great_equal_in_same_type(const ExpressionItem& left,const ExpressionItem& right,ExpressionItem& target){
	assert(left.return_type==right.return_type);
	target.return_type=t_int;
	switch(left.return_type){
		case t_int:{
			target.content.data.value._int=(left.content.data.value._int>=right.content.data.value._int);
			break;
		}
		case t_float:{
			target.content.data.value._int=(left.content.data.value._float>=right.content.data.value._float);
			break;
		}
		case t_double:{
			target.content.data.value._int=(left.content.data.value._double>=right.content.data.value._double);
			break;
		}
		case t_smallInt:{
			target.content.data.value._int=(left.content.data.value._sint>=right.content.data.value._sint);
			break;
		}
		case t_u_long:{
			target.content.data.value._int=(left.content.data.value._ulong>=right.content.data.value._ulong);
			break;
		}
		case t_decimal:{
			target.content.data.value._int=left._decimal.op_great_equals(right._decimal);///////////////
			break;
		}
		default:{
			printf("add type not supproted!\n");
		}
	}
}
static void compare_less_equal_in_same_type(const ExpressionItem& left,const ExpressionItem& right,ExpressionItem& target){
	assert(left.return_type==right.return_type);
	target.return_type=t_int;
	switch(left.return_type){
		case t_int:{
			target.content.data.value._int=(left.content.data.value._int<=right.content.data.value._int);
			break;
		}
		case t_float:{
			target.content.data.value._int=(left.content.data.value._float<=right.content.data.value._float);
			break;
		}
		case t_double:{
			target.content.data.value._int=(left.content.data.value._double<=right.content.data.value._double);
			break;
		}
		case t_smallInt:{
			target.content.data.value._int=(left.content.data.value._sint<=right.content.data.value._sint);
			break;
		}
		case t_u_long:{
			target.content.data.value._int=(left.content.data.value._ulong<=right.content.data.value._ulong);
			break;
		}
		case t_decimal:{
			target.content.data.value._int=left._decimal.op_less_equals(right._decimal);//////////////
			break;
		}
		default:{
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
		TypeCast::type_cast_functions[left.return_type][promoted_type](left);
	}
	if(promoted_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][promoted_type](right);
	}

	target.type=ExpressionItem::const_type;
	compare_less_in_same_type(left,right,target);
}

static void compare_great(ExpressionItemStack& stack, ExpressionItem& target){
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
		TypeCast::type_cast_functions[left.return_type][promoted_type](left);
	}
	if(promoted_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][promoted_type](right);
	}

	target.type=ExpressionItem::const_type;
	compare_great_in_same_type(left,right,target);
}

static void compare_equal(ExpressionItemStack& stack, ExpressionItem& target){
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
		TypeCast::type_cast_functions[left.return_type][promoted_type](left);
	}
	if(promoted_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][promoted_type](right);
	}

	target.type=ExpressionItem::const_type;
	compare_equal_in_same_type(left,right,target);
}

static void compare_not_equal(ExpressionItemStack& stack, ExpressionItem& target){
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
		TypeCast::type_cast_functions[left.return_type][promoted_type](left);
	}
	if(promoted_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][promoted_type](right);
	}

	target.type=ExpressionItem::const_type;
	compare_not_equal_in_same_type(left,right,target);
}

static void compare_less_equal(ExpressionItemStack& stack, ExpressionItem& target){
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
		TypeCast::type_cast_functions[left.return_type][promoted_type](left);
	}
	if(promoted_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][promoted_type](right);
	}

	target.type=ExpressionItem::const_type;
	compare_less_equal_in_same_type(left,right,target);
}

static void compare_great_equal(ExpressionItemStack& stack, ExpressionItem& target){
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
		TypeCast::type_cast_functions[left.return_type][promoted_type](left);
	}
	if(promoted_type!=right.return_type){
		TypeCast::type_cast_functions[right.return_type][promoted_type](right);
	}

	target.type=ExpressionItem::const_type;
	compare_great_equal_in_same_type(left,right,target);
}


static void get_compare_return_type(ExpressionItemStack& stack, ExpressionItem& target,bool isnot)
{
	if(isnot)
	{
		assert(stack.size()>=1);
		stack.pop();
	}
	else
	{
		assert(stack.size()>=2);
		ExpressionItem left,right;

		right=stack.top();
		stack.pop();

		left=stack.top();
		stack.pop();
	}
	/**In the current implementation, arithmetic type promotion map is used.
	 * TODO: Use specific mapping for compare function if needed.
	 */
//	data_type promoted_type=TypePromotion::arith_type_promotion_map[left.return_type][right.return_type];
//
//	if(promoted_type!=left.return_type){
//		TypeCast::type_cast_functions[left.return_type][promoted_type](left);
//	}
//	if(promoted_type!=right.return_type){
//		TypeCast::type_cast_functions[right.return_type][promoted_type](right);
//	}
	target.return_type=t_int;
	target.type=ExpressionItem::const_type;
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
				case_exp.push_back(stack.top());
				stack.pop();
				break;
			}

			default:{
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
			then_exp.push_back(stack.top());
			stack.pop();
			break;
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
			when_exp.push_back(stack.top());
			stack.pop();
			break;
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

static void upper(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=1);

	ExpressionItem uppered_string;

	uppered_string=stack.top();
	stack.pop();

	target.return_type=t_string;
	target.type=ExpressionItem::const_type;

	string str=uppered_string._string;
	transform(str.begin(),str.end(),str.begin(),::toupper);

	target._string=str;

}

static void substring(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=3);

	ExpressionItem Str;
	ExpressionItem Begin;
	ExpressionItem Length;

	Length=stack.top();
	stack.pop();
	Begin=stack.top();
	stack.pop();
	Str=stack.top();
	stack.pop();

	unsigned begin=Begin.content.data.value._int;
	unsigned length=Length.content.data.value._int;

	string subed_str=Str._string.substr(begin,length);

	target.return_type=t_string;
	target.type=ExpressionItem::const_type;
	target._string=subed_str;
}

static string& lTrim(string &ss){
    string::iterator p=find_if(ss.begin(),ss.end(),not1(ptr_fun(::isspace)));
    ss.erase(ss.begin(),p);
    return ss;
}

static string& rTrim(string &ss){
    string::reverse_iterator p=find_if(ss.rbegin(),ss.rend(),not1(ptr_fun(::isspace)));
    ss.erase(p.base(),ss.end());
    return ss;
}

static string& atrim(string &st){
    lTrim(rTrim(st));
    return st;
}

static void trim(ExpressionItemStack& stack, ExpressionItem& target){
	/*
	 * now the trim function only support deleting space on head and tail.
	 * */
	assert(stack.size()>=3);
	ExpressionItem Str;
	ExpressionItem mode;
	ExpressionItem subStr;
	Str=stack.top();
	stack.pop();
	mode=stack.top();
	stack.pop();
	subStr=stack.top();
	stack.pop();

	if(mode.content.data.value._int==0){
		string str=Str._string;
		string rt=atrim(str);
		target._string=rt;
		target.return_type=t_string;
		target.type=ExpressionItem::const_type;
	}
}

static void cast(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=2);

	ExpressionItem type;
	ExpressionItem variable;

	type=stack.top();
	stack.pop();
	variable=stack.top();
	stack.pop();

	type.getType(type._string.c_str());
	TypeCast::type_cast_functions[variable.return_type][type.return_type](variable);

	target=variable;
}


static void get_boolean_expr(ExpressionItem& expr)
{
	bool temp=false;

	switch(expr.return_type)
	{
		case t_int:
		{
			temp=expr.content.data.value._int!=0?true:false;
		}break;
		case t_smallInt:
		{
			temp=expr.content.data.value._sint!=0?true:false;
		}break;
		case t_double:
		{
			temp=expr.content.data.value._double!=0.0?true:false;
		}break;
		case t_float:
		{
			temp=expr.content.data.value._float!=0.0?true:false;
		}break;
		case t_decimal:
		{
			temp=expr._decimal.op_not_equals(NValue::getDecimalValueFromString("0"));
		}break;
		case t_u_long:
		{
			temp=expr.content.data.value._ulong!=0?true:false;
		}break;
		default:
		{
			printf("not op not support this type/n");
		}
	}
	expr.return_type=t_int;
	expr.type=ExpressionItem::const_type;
	expr.content.data.value._int=temp;
}
static void andop(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=2);
	ExpressionItem right=stack.top();
	stack.pop();
	get_boolean_expr(right);

	ExpressionItem left=stack.top();
	stack.pop();
	get_boolean_expr(left);

	target.return_type=t_int;
	target.type=ExpressionItem::const_type;
	target.content.data.value._int=(left.content.data.value._int&&right.content.data.value._int);
}
static void orop(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=2);
	ExpressionItem right=stack.top();
	stack.pop();
	get_boolean_expr(right);
	ExpressionItem left=stack.top();
	stack.pop();
	get_boolean_expr(left);
	target.return_type=t_int;
	target.type=ExpressionItem::const_type;
	target.content.data.value._int=(left.content.data.value._int||right.content.data.value._int);
}
static void notop(ExpressionItemStack& stack, ExpressionItem& target){
	assert(stack.size()>=1);
	ExpressionItem notexpr;
	notexpr=stack.top();
	stack.pop();
	get_boolean_expr(notexpr);
	target.return_type=t_int;
	target.type=ExpressionItem::const_type;
	target.content.data.value._int=!(notexpr.content.data.value._int);

}
#endif /* FUNCTIONS_H_ */
