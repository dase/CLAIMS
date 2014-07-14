/*
 * ExpressionItem.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */

#include "ExpressionItem.h"

ExpressionItem::ExpressionItem() {

}

ExpressionItem::~ExpressionItem() {

}

bool ExpressionItem::setValue(void* value_str,const data_type type){
	switch(type){
		case t_int:{
			setIntValue(*(int *)value_str);
			break;
		}
		case t_float:{
			setFloatValue(*(float *)value_str);
			break;
		}
		case t_smallInt:
		{
			setSmallIntValue(*(short *)value_str);
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
			setStringValue(string((char *)value_str));
			break;
		}
		case t_decimal:{
			setDecimalValue((const char *)value_str);
			break;
		}
		case t_date:{
			setDateValue((const char *)value_str);
			break;
		}
		case t_boolean:{
			setBooleanValue(*(bool *)value_str);
			break;
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

bool ExpressionItem::setData(data__& data,data_type rt){
	this->type=const_type;
	this->return_type=rt;
	this->content.data=data;
	return true;
}

bool ExpressionItem::setData(data__& data){
	this->type=const_type;
	this->content.data=data;
	return true;
}

bool ExpressionItem::setIntValue(const char* int_str){
	this->type=const_type;
	this->return_type=t_int;
	this->content.data.value._int=atoi(int_str);
	string itemname(int_str);
	this->item_name=itemname;
	return true;
}

bool ExpressionItem::setIntValue(int value){
	this->type=const_type;
	this->return_type=t_int;
	this->content.data.value._int=value;
	return true;
}

bool ExpressionItem::setFloatValue(const char* float_str){
	this->type=const_type;
	this->return_type=t_float;
	this->content.data.value._float=atof(float_str);
	string itemname(float_str);
	this->item_name=itemname;
	return true;
}

bool ExpressionItem::setFloatValue(float &float_){
	this->type=const_type;
	this->return_type=t_float;
	this->content.data.value._float=float_;
	return true;
}
bool ExpressionItem::setSmallIntValue(short &sint_){
	this->type=const_type;
	this->return_type=t_smallInt;
	this->content.data.value._sint=sint_;
	return true;
}

bool ExpressionItem::setDoubleValue(const char* double_str){
	this->type=const_type;
	this->return_type=t_double;
	this->content.data.value._double=atof(double_str);
	string itemname(double_str);
	this->item_name=itemname;
	return true;
}

bool ExpressionItem::setDoubleValue(double &double_){
	this->type=const_type;
	this->return_type=t_double;
	this->content.data.value._double=double_;
	return true;
}
bool ExpressionItem::setULongValue(const char* u_long_str){
	this->type=const_type;
	this->return_type=t_u_long;
	this->content.data.value._ulong=atol(u_long_str);
	string itemname(u_long_str);
	this->item_name=itemname;
	return true;
}

bool ExpressionItem::setULongValue(unsigned long u_long){
	this->type=const_type;
	this->return_type=t_u_long;
	this->content.data.value._ulong=u_long;
	return true;
}

bool ExpressionItem::setDecimalValue(const char * decimal_str){
	this->type=const_type;
	this->return_type=t_decimal;
	NValue nvalue=*(NValue*)decimal_str;
	this->_decimal=nvalue;
	string itemname(decimal_str);
	this->item_name=itemname;
	return true;
}

bool ExpressionItem::setStringValue(std::string str){
	this->type=const_type;
	this->return_type=t_string;
	this->_string=str;
	return true;
}

bool ExpressionItem::setStringValue(const char * str){
	this->type=const_type;
	this->return_type=t_string;
	this->_string=string(str);
	return true;
}

bool ExpressionItem::setDateValue(const char * date_str){
	this->type=const_type;
	this->return_type=t_date;
	this->_date=*(date*)date_str;
	string itemname(date_str);
	this->item_name=itemname;
//	strcpy(content.data.value._date,date_str);
	return true;
}

bool ExpressionItem::setBooleanValue(bool value){
	this->type=const_type;
	this->return_type=t_boolean;
	this->content.data.value._bool=value;
	return true;
}


bool ExpressionItem::setOperator(const char* op_str){
	type=operator_type;
	std::string tmp(op_str);
	if(tmp=="+"){
		content.op.op_=op_add;
	}
	else if(tmp=="-"){
		content.op.op_=op_mins;
	}
	else if(tmp=="*"){
		content.op.op_=op_multiple;
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
	else if(tmp=="and"){
		content.op.op_=op_and;
	}
	else if(tmp=="or"){
		content.op.op_=op_or;
	}
	else if(tmp=="not"){
		content.op.op_=op_not;
	}
	else if(tmp=="<"){
		content.op.op_=op_com_L;
	}
	else if(tmp==">"){
		content.op.op_=op_com_G;
	}
	else if(tmp=="="){
		content.op.op_=op_com_EQ;
	}
	else if(tmp=="!="){
		content.op.op_=op_com_NEQ;
	}
	else if(tmp==">="){
		content.op.op_=op_com_GEQ;
	}
	else if(tmp=="<="){
		content.op.op_=op_com_LEQ;
	}
	else{
		printf("[%s] fails to match to any existing operator\n",op_str);
		return false;
	}
	item_name=tmp;
	return true;
}

bool ExpressionItem::setVariable(const char * table,const char * column){
	this->type=variable_type;
	this->content.var.table_name=table;
	this->content.var.column_name=column;
	stringstream item;
	item<<string(table).c_str()<<"."<<string(column).c_str();
	this->item_name=item.str();
	return true;
}

bool ExpressionItem::setVariable(const char *table_column) {
	this->type=variable_type;
	this->content.var.table_column=table_column;
	stringstream item;
	item<<string(table_column).c_str();
	this->item_name=item.str();
	return true;
}
