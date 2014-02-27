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
	defalut:{
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
static void compare_less_in_same_type(const ExpressionItem& left,const ExpressionItem& right,ExpressionItem& target){
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



}

#endif /* FUNCTIONS_H_ */
