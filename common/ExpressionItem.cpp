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

bool ExpressionItem::setVariable(const char * table,const char * column){
	type=variable_type;
//	string s(variable);
//	int position=s.find(".");
////	string tablename(s.substr(0,position).c_str());
////	string columnname(s.substr(position+1,s.length()).c_str());
////	content.var.table_name=s.substr(0,position);
////	content.var.column_name=s.substr(position+1,s.length());
//	char *t_c=new char(s.substr(0,position).length()+1);
//	strcpy(t_c,s.substr(0,position).c_str());
//	char *c_c=new char(s.substr(position+1,s.length()).length()+1);
//	strcpy(c_c,s.substr(position+1,s.length()).c_str());
	content.var.table_name=table;//).assign(tablename);
	string table_(table);
	string table_1(content.var.table_name);
	cout<<"table_: "<<table_<<endl;
	cout<<"table_1: "<<table_1<<endl;
	content.var.column_name=column;
	string column_(column);
	string column_1(content.var.column_name);
	cout<<"column_: "<<column_<<endl;
	cout<<"column_1: "<<column_1<<endl;
	return true;
}
