/*
 * ExpressionItem.h
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */

#ifndef EXPRESSIONITEM_H_
#define EXPRESSIONITEM_H_
#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/binary_object.hpp>

#include "data_type.h"
#include "types/NValue.hpp"
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../common/data_type.h"
#include "boost/date_time/gregorian/formatters.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "boost/date_time/posix_time/time_formatters.hpp"
enum op_type{op_add,op_mins,op_multiple,op_cast_int,op_com_L,op_case,op_case_when,op_case_then,op_case_else,op_upper,op_substring,op_trim,op_cast,op_and,op_or,op_not,op_com_G,op_com_EQ,op_com_NEQ,op_com_GEQ,op_com_LEQ};
using namespace boost::gregorian;
using namespace boost::posix_time;
static std::string getReturnTypeName(data_type return_type){
	switch(return_type){
		case t_int:{
			return std::string("int");
		}
		case t_float:{
			return std::string("t_float");
		}
		case t_double:{
			return std::string("t_double");
		}
		case t_string:{
			return std::string("t_string");
		}
		case t_u_long:{
			return std::string("t_u_long");
		}
		case t_date:{
			return std::string("t_date");
		}
		case t_datetime:{
			return std::string("t_datetime");
		}
		case t_decimal:{
			return std::string("t_decimal");
		}
		case t_smallInt:{
			return std::string("t_smallInt");
		}
		case t_boolean:{
			return std::string("t_boolean");
		}
		default:{
			assert(false);
			return std::string("Not given");
		}
	}
	return std::string();
}

struct express_operator{
	void set(op_type tp,unsigned number_of_parameter=0){
		op_=tp;
		switch(tp){
			case op_add:number_of_parameter=2;break;
			case op_multiple:number_of_parameter=2;break;
			case op_cast_int:number_of_parameter=1;break;

			case op_and:number_of_parameter=2;break;
			case op_or:number_of_parameter=2;break;
			case op_not:number_of_parameter=1;break;

		}
	}
	op_type op_;
	unsigned num_of_parameter;
};

struct data__{
	union{
	int _int;
	float _float;
	double _double;
	unsigned long _ulong;
	bool _bool;
	short _sint;
	}value;
};

struct variable{
	const char* table_column;
	const char* table_name;
	const char* column_name;
};

class ExpressionItem {
public:
	enum ItemType{const_type,variable_type,operator_type};

	void print(){
		printf("Item:%s  \n",getItemTypeName().c_str());
		switch(type){
		case const_type:{
			printf("Return type: %s",getReturnTypeName(return_type).c_str());
			printf("value:%s ",data_value_to_string().c_str());
			printf("\n");
			break;
		}
		case variable_type:{
			break;
		}
		case operator_type:{
			printf("Return type: %s",getReturnTypeName(return_type).c_str());
			printf("operator: %s ",getOperatorName().c_str());
			printf("\n");
			break;
		}
		}
	}
	void print_value(){
		switch(type){
		case const_type:{

			printf(" %s \n",data_value_to_string().c_str());

			break;
		}
		case variable_type:{
			printf(" variable %s,%s\n",this->content.var.table_name,this->content.var.column_name);
			break;
		}
		case operator_type:{

			printf(" %s \n",getOperatorName().c_str());

			break;
		}
		}
	}
	ExpressionItem();
	virtual ~ExpressionItem();
	bool setValue(void*,data_type);
	bool setData(data__&);
	bool setData(data__&,data_type);
	bool setIntValue(const char *);
	bool setIntValue(int);
	bool setFloatValue(const char*);
	bool setFloatValue(float&);
	bool setDoubleValue(const char*);
	//currently,decimal only const char * supported!
	bool setDecimalValue(const char*);
	bool setDoubleValue(double&);
	bool setULongValue(const char*);
	bool setULongValue(unsigned long&);
	bool setOperator(const char*);
	bool setStringValue(std::string);
	bool setVariable(const char *,const char *);
	bool setVariable(const char *);

	//currently,date only const char * supported!
	bool setDateValue(const char *);

	bool setBooleanValue(bool );////////////
	bool setSmallIntValue(short &);
public:
	union {
		variable var;
		data__ 	data;
		express_operator op;
	}content;
	ItemType type;
	date _date;
	time_duration _time;
	ptime _datatime;
	NValue _decimal;
	std::string _string;
	std::string item_name;
	data_type return_type;
	unsigned size;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		char* start_of_union=(char*)(&content.data.value._int);
		ar & return_type & _string & type;
		for(unsigned i=0;i<sizeof(content);i++){
			ar& *start_of_union;
			start_of_union++;
		}
	}

private:
	std::string getItemTypeName()const{
		switch(type){
			case const_type:{
				return std::string("const type");
			}
			case variable_type:{
				return std::string("variable type");
			}
			case operator_type:{
				return std::string("operator type");
			}
			default:{
				assert(false);
			}
		}
		return std::string();
	}

	std::string data_value_to_string()const{
		assert(type==const_type);
		std::stringstream ss;
		switch(return_type){
			case t_int:{
				ss<<content.data.value._int;
				break;
			}
			case t_float:{
				ss<<content.data.value._float;
				break;
			}
			case t_double:{
				ss<<content.data.value._double;
				break;
			}
			case t_string:{
				ss<<_string;
				break;
			}
			case t_u_long:{
				ss<<content.data.value._ulong;
				break;
			}
			case t_date:{
				ss<<to_simple_string(_date);
				break;
			}
			case t_datetime:{
				return std::string("t_datetime");
			}
			case t_decimal:{
				ss<<_decimal.createStringFromDecimal();
				break;
			}
			case t_smallInt:{
				ss<<content.data.value._sint;
				break;
			}
			case t_boolean:{
				ss<<content.data.value._bool;
				break;
			}
			default:{
				assert(false);
			}
		}
		return ss.str();
	}

public:
	std::string getOperatorName()const{
		switch(content.op.op_){
		case op_add:{
			return std::string("+");
		}
		case op_mins:{
			return std::string("-");
		}
		case op_cast_int:{
			return std::string("(int)");
		}
		case op_multiple:{
			return std::string("*");
		}
		case op_case:{
			return std::string("case");
		}
		case op_case_when:{
			return std::string("when");
		}
		case op_case_then:{
			return std::string("then");
		}
		case op_case_else:{
			return std::string("else");
		}
		case op_upper:{
			return std::string("upper");
		}
		case op_substring:{
			return std::string("substring");
		}
		case op_trim:{
			return std::string("trim");
		}
		case op_cast:{
			return std::string("cast");
		}
		case op_and:
		{
			return std::string("and");
		}
		case op_or:
		{
			return std::string("or");
		}
		case op_not:
		{
			return std::string("not");
		}
		case op_com_L:{
			return std::string("<");
		}
		case op_com_G:{
			return std::string(">");
		}
		case op_com_EQ:{
			return std::string("=");
		}
		case op_com_NEQ:{
			return std::string("!=");
		}
		case op_com_GEQ:{
			return std::string(">=");
		}
		case op_com_LEQ:{
			return std::string("<=");
		}
		default:{
			assert(false);
		}
		return std::string();
		}
	}

	bool getType(const char* type_str){
		type=variable_type;
		std::string tmp(type_str);
		if(tmp=="int"){
			return_type=t_int;
		}
		else if(tmp=="string"){
			return_type=t_string;
		}
		else{
			printf("[%s] fails to match to any existing data type\n",type_str);
			return false;
		}
		return true;
	}
};

#endif /* EXPRESSIONITEM_H_ */
