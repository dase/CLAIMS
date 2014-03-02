/*
 * ExpressionItem.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */

#include "ExpressionItem.h"

ExpressionItem::ExpressionItem() {
	// TODO Auto-generated constructor stub

}

ExpressionItem::~ExpressionItem() {
	// TODO Auto-generated destructor stub
}

bool ExpressionItem::setIntValue(const char* int_str){
	this->type=const_type;
	this->return_type=t_int;
	this->content.data.value._int=atoi(int_str);
	return true;
}

bool ExpressionItem::setIntValue(int value){
	type=const_type;
	return_type=t_int;
	content.data.value._int=value;
	return true;
}

bool ExpressionItem::setFloatValue(const char* float_str){
	type=const_type;
	return_type=t_float;
	content.data.value._float=atof(float_str);
	return true;
}

bool ExpressionItem::setOperator(const char* op_str){
//	enum op_type{op_add,op_multiple,op_cast_int,op_com_L,op_case,op_case_when,op_case_then,op_case_else};
	type=operator_type;
	std::string tmp(op_str);
	if(tmp=="+"){
		content.op.op_=op_add;
	}
	else if(tmp=="<"){
		content.op.op_=op_com_L;
	}
	else if(tmp=="case"){
		content.op.op_=op_case;
	}
	else if(tmp=="when"){
		content.op.op_=op_case_when;
	}
	else if(tmp=="then"){
		content.op.op_=op_case_then;
	}
	else if(tmp=="else"){
		content.op.op_=op_case_else;
	}
	else{
		printf("[%s] fails to match to any existing operator\n",op_str);
		return false;
	}
	return true;
}

bool ExpressionItem::setStringValue(std::string str){
	type=const_type;
	return_type=t_string;
	_string=str;
	return true;
}

bool ExpressionItem::setVariable(const char * variable){
	type=variable_type;
//	content.var.variable_name=variable;
	return true;
}
