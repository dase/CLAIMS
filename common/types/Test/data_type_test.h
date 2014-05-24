/*
 * data_type_test.cpp
 *
 *  Created on: Feb 24, 2014
 *      Author: wangli
 */
#ifndef __DATA_TYPE_TEST__
#define __DATA_TYPE_TEST__
#include <stdio.h>
#include <fstream>
#include "../NValue.hpp"
#include "../../Schema/SchemaFix.h"

//#include "../../Comparator.h"

#include "../../../utility/test_tool.h"
#include <gtest/gtest.h>
//#include "../../Comparator.cpp"


std::string test_decimal_to_string(char* decimal,int number_of_fractions){
	NValue v1 = NValue::getDecimalValueFromString(decimal);
	column_type* op=new column_type(data_type(t_decimal),number_of_fractions);
	std::string ret=op->operate->toString(&v1);
	op->~column_type();
	return ret;
}

std::string test_decimal_add(char* left,char* right,int number_of_fractions){
	NValue v1 = NValue::getDecimalValueFromString(left);
	NValue v2 = NValue::getDecimalValueFromString(right);
	column_type* op=new column_type(data_type(t_decimal),number_of_fractions);
	op->operate->add(&v1,&v2);
	std::string ret=op->operate->toString(&v1);
	op->~column_type();
	return ret;
}
 void test_decimal_to_value1(){
	NValue v1 = NValue::getDecimalValueFromString("1.033333");
	column_type* op=new column_type(data_type(t_decimal),4);
	if(!print_test_name_result(op->operate->toString(&v1)==string("1.0333"),"Decimal toString()")){
		printf("Expected 1.0333, actual: %s\n",op->operate->toString(&v1).c_str());
	}
}
 void test_decimal_to_value2(){
	NValue v1 = NValue::getDecimalValueFromString("1.2220");
	column_type* op=new column_type(data_type(t_decimal),4);
	if(!print_test_name_result(op->operate->toString(&v1)==string("1.2220"),"Decimal toString()")){
		printf("Expected 1.2220, actual: %s\n",op->operate->toString(&v1).c_str());
	}
}
 void test_decimal_to_value3(){
	NValue v1 = NValue::getDecimalValueFromString("1333");
	column_type* op=new column_type(data_type(t_decimal),2);
	if(!print_test_name_result(op->operate->toString(&v1)==string("1333.00"),"Decimal toString()")){
		printf("Expected 1333.00, actual: %s\n",op->operate->toString(&v1).c_str());
	}
}
 void test_decimal_to_value4(){
	NValue v1 = NValue::getDecimalValueFromString("0.00001");
	column_type* op=new column_type(data_type(t_decimal),2);
	if(!print_test_name_result(op->operate->toString(&v1)==string("0.00"),"Decimal toString()")){
		printf("Expected 0.00, actual: %s\n",op->operate->toString(&v1).c_str());
	}
}
 void test_decimal_to_value5(){


	NValue v1 = NValue::getDecimalValueFromString("-0.02");
	column_type* op=new column_type(data_type(t_decimal),2);
	if(!print_test_name_result(op->operate->toString(&v1)==string("-0.02"),"Decimal toString()")){
		printf("Expected -0.02, actual: %s\n",op->operate->toString(&v1).c_str());
	}
}
 void test_decimal_add1(){
	NValue v1 = NValue::getDecimalValueFromString("4.4");
	NValue v2 = NValue::getDecimalValueFromString("-18");
	column_type* op=new column_type(data_type(t_decimal),2);
	op->operate->add(&v1,&v2);
	if(!print_test_name_result(op->operate->toString(&v1)==string("-13.60"),"Decimal add")){
		printf("Expected -13.60, actual: %s\n",op->operate->toString(&v1).c_str());
	}
}
 int test_decimal(){
	test_decimal_to_value1();
	test_decimal_to_value2();
	test_decimal_to_value3();
	test_decimal_to_value4();
	test_decimal_to_value5();
	test_decimal_add1();
}

TEST(DATA_TYPE,DECIMAL_TO_STRING){
	 EXPECT_STREQ("1.0333",test_decimal_to_string("1.033333",4).c_str());
	 EXPECT_STREQ("1.2220",test_decimal_to_string("1.2220",4).c_str());
	 EXPECT_STREQ("1333.00",test_decimal_to_string("1333",2).c_str());
	 EXPECT_STREQ("0.00",test_decimal_to_string("0.00001",2).c_str());
	 EXPECT_STREQ("-0.02",test_decimal_to_string("-0.02",2).c_str());
}

TEST(DATA_TYPE,DECIMAL_ADD){
	EXPECT_STREQ("0.2",test_decimal_add("0.1","0.1",1).c_str());
	EXPECT_STREQ("-13.60",test_decimal_add("4.4","-18",2).c_str());
}


#endif
