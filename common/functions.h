/*
 * functions.h
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "ExpressionCalculator.h"

static void add(ExpressionItemStack& stack, ExpressionItem& target){
	//assuming all types are int.
	assert(stack.size()>=2);
	ExpressionItem right=stack.top();
	stack.pop();
	if(!check_data_type_for_add(right.content.data.return_type)){
		printf("%s is not supported for +!\n",getReturnTypeName(right.content.data.return_type).c_str());
	}
	ExpressionItem left=stack.top();
	stack.pop();
	if(!check_data_type_for_add(left.content.data.return_type)){
		printf("%s is not supported for +!\n",getReturnTypeName(left.content.data.return_type).c_str());
	}
	target.content.data.return_type=right.content.data.return_type>left.content.data.return_type?right.content.data.return_type:left.content.data.return_type;
	target.content.data.value._int=left.content.data.value._int+right.content.data.value._int;
	switch(left.content.data.return_type){
	case t_int: {
		switch (right.content.data.return_type) {
		case t_int: {
			target.content.data.return_type = t_int;
			target.content.data.value._int = left.content.data.value._int
					+ right.content.data.value._int;
			break;
		}
		case t_float: {
			target.content.data.return_type = t_float;
			target.content.data.value._int = left.content.data.value._int
					+ right.content.data.value._float;
			break;
		}
		case t_double: {
			target.content.data.return_type = t_double;
			target.content.data.value._int = left.content.data.value._int
					+ right.content.data.value._double;
			break;
		}
		case t_u_long: {
			target.content.data.return_type = t_u_long;
			target.content.data.value._int = left.content.data.value._int
					+ right.content.data.value._ulong;
			break;
		}
		}
		break;
	}
	case t_float:{
		switch (right.content.data.return_type) {
		case t_int: {
			target.content.data.return_type = t_float;
			target.content.data.value._float = left.content.data.value._float
					+ right.content.data.value._int;
			break;
		}
		case t_float: {
			target.content.data.return_type = t_float;
			target.content.data.value._float = left.content.data.value._float
					+ right.content.data.value._float;
			break;
		}
		case t_double: {
			target.content.data.return_type = t_double;
			target.content.data.value._double = left.content.data.value._float
					+ right.content.data.value._double;
			break;
		}
		case t_u_long: {
			target.content.data.return_type = t_u_long;
			target.content.data.value._ulong = left.content.data.value._float
					+ right.content.data.value._ulong;
			break;
		}
		}
		break;
	}
	case t_double:{
		switch (right.content.data.return_type) {
		case t_int: {
			target.content.data.return_type = t_double;
			target.content.data.value._double = left.content.data.value._double
					+ right.content.data.value._int;
			break;
		}
		case t_float: {
			target.content.data.return_type = t_double;
			target.content.data.value._double = left.content.data.value._double
					+ right.content.data.value._float;
			break;
		}
		case t_double: {
			target.content.data.return_type = t_double;
			target.content.data.value._double = left.content.data.value._double
					+ right.content.data.value._double;
			break;
		}
		case t_u_long: {
			target.content.data.return_type = t_double;
			target.content.data.value._double = left.content.data.value._double
					+ right.content.data.value._ulong;
			break;
		}
		}
		break;
	}
	case t_u_long:{
		switch (right.content.data.return_type) {
		case t_int: {
			target.content.data.return_type = t_double;
			target.content.data.value._double = left.content.data.value._double
					+ right.content.data.value._int;
			break;
		}
		case t_float: {
			target.content.data.return_type = t_double;
			target.content.data.value._double = left.content.data.value._double
					+ right.content.data.value._float;
			break;
		}
		case t_double: {
			target.content.data.return_type = t_double;
			target.content.data.value._double = left.content.data.value._double
					+ right.content.data.value._double;
			break;
		}
		case t_u_long: {
			target.content.data.return_type = t_double;
			target.content.data.value._double = left.content.data.value._double
					+ right.content.data.value._ulong;
			break;
		}
		}
		break;
	}
	default:{

	}
	}
}
//enum data_type{t_int,t_float,t_string,t_double,t_u_long, t_date, t_time, t_datetime, t_decimal, t_smallInt};
bool check_data_type_for_add(data_type dt){
	switch(data_type){
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
#endif /* FUNCTIONS_H_ */
