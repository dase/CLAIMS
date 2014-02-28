/*
 * ExpressionItem.h
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */

#ifndef EXPRESSIONITEM_H_
#define EXPRESSIONITEM_H_
#include <sstream>
#include "../data_type.h"
enum op_type{op_add,op_multiple,op_cast_int,op_com_L,op_case,op_case_when,op_case_then,op_case_else};
using namespace boost::gregorian;
using namespace boost::posix_time;
static std::string getReturnTypeName(data_type return_type){
	//t_int,t_float,t_string,t_double,t_u_long, t_date, t_time, t_datetime, t_decimal, t_smallInt
	switch(return_type){
		case t_int:{
			return std::string("int");
		}
		case t_float:{
			return std::string("t_float");
		}
		case t_string:{
			return std::string("t_string");
		}
		case t_double:{
			return std::string("t_double");
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
//			assert(false);
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
		}
	}
	op_type op_;
	unsigned num_of_parameter;
//	data_type return_type;
};
struct data__{
	union{
	int _int;
	float _float;
	double _double;
	unsigned long _ulong;
	bool _bool;
	char _date[4];//date
	char _time[8];//time
	char _datatime[8];//datetime
	char _decimal[16];//decimal
	}value;



};
struct variable{
//	data_type return_type;
	std::string* table_name;
	std::string* column_name;
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
			printf(" variable \n");
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
	bool setIntValue(const char*);
	bool setIntValue(int);
	bool setFloatValue(const char*);
	bool setOperator(const char*);
	bool setStringValue(std::string);
public:
	union {
		variable var;
		data__ 	data;
		express_operator op;
	}content;
	ItemType type;
	std::string _string;// string cannot be in unoin.
	data_type return_type;
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
			case t_string:{
				ss<<_string;
				break;
			}
			case t_double:{
				ss<<content.data.value._double;
				break;
			}
			case t_u_long:{
				ss<<content.data.value._ulong;
				break;
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
				ss<<content.data.value._bool;
				break;
			}
			default:{
				assert(false);
			}
		}
		return ss.str();
	}
	std::string getOperatorName()const{
		switch(content.op.op_){
		case op_add:{
			return std::string("+");
		}
		case op_cast_int:{
			return std::string("(int)");
		}
		case op_multiple:{
			return std::string("*");
		}
		case op_com_L:{
			return std::string("<");
		}
		case op_case:{
			return std::string("CASE");
		}
		case op_case_when:{
			return std::string("CASE_WHEN");
		}
		case op_case_then:{
			return std::string("CASE_THEN");
		}
		case op_case_else:{
			return std::string("CASE_ELSE");
		}
		default:{
			assert(false);
		}
		return std::string();
		}
	}
};

#endif /* EXPRESSIONITEM_H_ */
