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

bool ExpressionItem::setValue(void* value_str,data_type data){
	switch(data){
		case t_int:{
//			cout<<"the value in the expressionitem is: "<<*(int *)value_str<<endl;
			setIntValue(*(int *)value_str);
			break;
		}
		case t_float:{
			setFloatValue(*(float *)value_str);
			break;
		}
		case t_double:{
			setDoubleValue(*(double *)value_str);
			break;
		}
		case t_u_long:{
			setULongValue(*(unsigned long*)value_str);
			break;
		}
		case t_string:{
			setStringValue(*(string *)value_str);
		}
		default:{
			cout<<"no matching operator exists!!!"<<endl;
			/*
			 * In the debug phase, it's better to show the assert failure in case of unexpected input.
			 * The bug can be found much more easily in such a way.
			 */
			assert(false);
			break;
		}
	}
	return true;
}

bool ExpressionItem::setData(data__& data){
	this->type=const_type;
	/*return type not need!!!*/
	this->content.data=data;
	return true;
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

bool ExpressionItem::setFloatValue(float &float_){
	type=const_type;
	return_type=t_float;
	content.data.value._float=float_;
	return true;
}

bool ExpressionItem::setDoubleValue(const char* double_str){
	type=const_type;
	return_type=t_double;
	content.data.value._double=atof(double_str);
	return true;
}

bool ExpressionItem::setDoubleValue(double &double_){
	type=const_type;
	return_type=t_double;
	content.data.value._double=double_;
	return true;
}
bool ExpressionItem::setULongValue(const char* u_long_str){
	type=const_type;
	return_type=t_u_long;
	content.data.value._ulong=atol(u_long_str);
	return true;
}

bool ExpressionItem::setULongValue(unsigned long &u_long){
	type=const_type;
	return_type=t_u_long;
//	content.data.value._ulong=u_long;
	content.data.value._ulong=u_long;
	return true;
}

bool ExpressionItem::setStringValue(std::string str){
	type=const_type;
	return_type=t_string;
	_string=str;
	return true;
}

bool ExpressionItem::setOperator(const char* op_str){
//	enum op_type{op_add,op_multiple,op_cast_int,op_com_L,op_case,op_case_when,op_case_then,op_case_else};
	type=operator_type;
	std::string tmp(op_str);
	if(tmp=="+"){
		content.op.op_=op_add;
	}
	else if(tmp=="-"){
		content.op.op_=op_mins;
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
	else if(tmp=="upper"){
		content.op.op_=op_upper;
	}
	else if(tmp=="substring"){
		content.op.op_=op_substring;
	}
	else if(tmp=="trim"){
		content.op.op_=op_trim;
	}
	else if(tmp=="cast"){
		content.op.op_=op_cast;
	}
	else{
		printf("[%s] fails to match to any existing operator\n",op_str);
		return false;
	}
	return true;
}

bool ExpressionItem::setVariable(const char * table,const char * column){
	type=variable_type;
	content.var.table_name=table;
	string table_(table);
	string table_1(content.var.table_name);
	content.var.column_name=column;
	string column_(column);
	string column_1(content.var.column_name);
	return true;
}
