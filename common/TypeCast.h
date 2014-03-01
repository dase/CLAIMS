/*
 * TypeCast.h
 *
 *  Created on: Feb 26, 2014
 *      Author: wangli
 */

#ifndef TYPECAST_H_
#define TYPECAST_H_
#include "ExpressionItem.h"
#include "../data_type.h"
typedef bool (*TypeCastFunction) (ExpressionItem& in);
class TypeCast{
public:
	static TypeCastFunction type_cast_functions[DATA_TYPE_NUMBER][DATA_TYPE_NUMBER];
};


inline bool int_to_int(ExpressionItem& in){
	assert(in.return_type==t_int);
	return true;
}
inline bool int_to_float(ExpressionItem& in){
	assert(in.return_type==t_int);
	float new_value=(float)in.content.data.value._int;
	in.return_type=t_float;
	in.content.data.value._float=new_value;
	return true;
}
/**
 * Note: This function must be called before computing any Expression
 */
inline void initialize_type_cast_functions(){
	TypeCast::type_cast_functions[t_int][t_int]=int_to_int;
	TypeCast::type_cast_functions[t_int][t_float]=int_to_float;

}



#endif /* TYPECAST_H_ */
