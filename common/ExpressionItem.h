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
enum op_type{op_add,op_multiple,op_cast_int};
using namespace boost::gregorian;
using namespace boost::posix_time;
std::string getReturnTypeName(data_type return_type)const{
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
		default:{
			assert(false);
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
	data_type return_type;
};
struct data__{
	union{
	int _int;
	float _float;
	double _double;
	unsigned long _ulong;
	char _date[4];//date
	char _time[8];//time
	char _datatime[8];//datetime
	char _decimal[16];//decimal
	}value;
	data_type return_type;
};
struct variable{
	data_type return_type;
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
			printf("Return type: %s",getReturnTypeName(content.data.return_type).c_str());
			printf("value:%s ",data_value_to_string().c_str());
			printf("\n");
			break;
		}
		case variable_type:{
			break;
		}
		case operator_type:{
			printf("Return type: %s",getReturnTypeName(content.op.return_type).c_str());
			printf("operator: %s ",getOperatorName().c_str());
			printf("\n");
			break;
		}
		}
	}
	ExpressionItem();
	virtual ~ExpressionItem();

	union {
		variable var;
		data__ 	data;
		express_operator op;
	}content;
	std::string str;
	ItemType type;
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

		switch(content.data.return_type){
			case t_int:{
				ss<<content.data.value._int;
				break;
			}
			case t_float:{
				ss<<content.data.value._float;
				break;
			}
			case t_string:{
				ss<<str;
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
		default:{
			assert(false);
		}
		return std::string();
		}
	}
};

#endif /* EXPRESSIONITEM_H_ */
