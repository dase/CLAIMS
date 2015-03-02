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

 std::string test_decimal_minus(char* left, char* right, int number_of_fractions)
 {
	vector<column_type> column_list;
	column_list.push_back(column_type(t_decimal, number_of_fractions));
	Schema* schema = new SchemaFix(column_list);

	void* left_ = malloc(16);		//newmalloc
	void* right_ = malloc(16);		//newmalloc
	schema->getcolumn(0).operate->toValue(left_, left);
	schema->getcolumn(0).operate->toValue(right_, right);
	schema->getcolumn(0).operate->add(left_, right_);
	return schema->getcolumn(0).operate->toString(left_);
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
	EXPECT_STREQ("-13.60",test_decimal_minus("4.4","-18",2).c_str());
}

string test_to_date(std::string ds) {
//	column_type* op=new column_type(data_type(t_date));
//	date d;
//	op->operate->toValue(d, ds);


	bool all_digit = true;
	if (ds.length() == 8)
	{
		for (int i = 0; i < 8; i++)
		{
			if (isdigit(ds[i]) == 0)
			{
				all_digit = false;
				break;
			}
		}

		if (all_digit == true)
			return to_iso_string(from_undelimited_string(ds));
	}
	return to_iso_string(from_string(ds));
}
TEST(DATA_TYPE, DATE_FROM_STRING) {
	EXPECT_STREQ("20100101", test_to_date("2010/1/1").c_str());
	EXPECT_STREQ("20100101", test_to_date("2010-1-1").c_str());
	EXPECT_STREQ("20100101", test_to_date("2010-Jan-01").c_str());
	EXPECT_STREQ("20100101", test_to_date("20100101").c_str());
	EXPECT_STREQ("20101111", test_to_date("2010/11/11").c_str());
	EXPECT_STREQ("20101111", test_to_date("2010-11-11").c_str());
	EXPECT_STREQ("20101111", test_to_date("20101111").c_str());
	EXPECT_STREQ("20101111", test_to_date("2010-Nov-11").c_str());
}

std::string test_date_computation(std::string dt, std::string d, data_type d_t)
{
	std::string ret = "";
	switch (d_t)
	{
	case t_date:
	{
		void* value = malloc(sizeof(date));		//newmalloc
		Operate* op = new OperateDate();
		op->toValue(value, dt.c_str());
		date_duration dd(atoi(d.c_str()));
		op->add(value, (void*)(&dd));
		ret = op->toString(value);
		op->~Operate();
		free(value);
		return ret;
	}
	case t_time:
	{
		void* value = malloc(sizeof(time_duration));		//newmalloc
		void* increament = malloc(sizeof(time_duration));		//newmalloc
		Operate* op = new OperateTime();
		op->toValue(value, dt.c_str());
		op->toValue(increament, d.c_str());
		op->add(value, increament);
		ret = op->toString(value);
		op->~Operate();
		free(value);
		free(increament);
		return ret;
	}
	case t_datetime:
	{
		void* value = malloc(sizeof(ptime));		//newmalloc
		Operate* op = new OperateDatetime();
		op->toValue(value, dt.c_str());
		time_duration td(duration_from_string(d));
		op->add(value, (void*)(&td));
		ret = op->toString(value);
		op->~Operate();
		free(value);
		return ret;
	}
	}
}
TEST(DATE_TYPE, DATE_COMPUTATION)
{
	EXPECT_STREQ("2010-10-20", test_date_computation("20101010", "10", t_date).c_str());
	EXPECT_STREQ("25:34:24", test_date_computation("12:47:12", "12:47:12", t_time).c_str());
	EXPECT_STREQ("2010-10-11T01:34:24.123456", test_date_computation("2010-10-10 12:47:12.12345678", "12:47:12", t_datetime).c_str());
}

std::string null_value_test(unsigned int size, data_type t)
{
	column_type* ct = new column_type(t, 12, true);
	void* value = malloc(size);		//newmalloc
	ct->operate->toValue(value, "");
	std::string ret =  ct->operate->toString(value);
	ct->~column_type();
	free(value);
	return ret;
}
TEST(DATA_TYPE, NULL_VALUE_TEST)
{
	EXPECT_STREQ("NULL", null_value_test(2, t_smallInt).c_str());
	EXPECT_STREQ("NULL", null_value_test(4, t_int).c_str());
	EXPECT_STREQ("NULL", null_value_test(8, t_u_long).c_str());
	EXPECT_STREQ("NULL", null_value_test(4, t_float).c_str());
	EXPECT_STREQ("NULL", null_value_test(8, t_double).c_str());
	EXPECT_STREQ("NULL", null_value_test(4, t_string).c_str());
	EXPECT_STREQ("NULL", null_value_test(4, t_date).c_str());
	EXPECT_STREQ("NULL", null_value_test(8, t_time).c_str());
	EXPECT_STREQ("NULL", null_value_test(8, t_datetime).c_str());
	EXPECT_STREQ("NULL", null_value_test(16, t_decimal).c_str());
	EXPECT_STREQ("NULL", null_value_test(2, t_u_smallInt).c_str());
}

std::string set_null_test(unsigned int size, data_type t)
{
	column_type* ct = new column_type(t, 12, true);
	void* value = malloc(size);		//newmalloc
	ct->operate->setNull(value);
	std::string ret =  ct->operate->toString(value);
	ct->~column_type();
	free(value);
	return ret;
}
TEST(DATA_TYPE, SET_NULL_TEST)
{
	EXPECT_STREQ("NULL", null_value_test(2, t_smallInt).c_str());
	EXPECT_STREQ("NULL", null_value_test(4, t_int).c_str());
	EXPECT_STREQ("NULL", null_value_test(8, t_u_long).c_str());
	EXPECT_STREQ("NULL", null_value_test(4, t_float).c_str());
	EXPECT_STREQ("NULL", null_value_test(8, t_double).c_str());
	EXPECT_STREQ("NULL", null_value_test(4, t_string).c_str());
	EXPECT_STREQ("NULL", null_value_test(4, t_date).c_str());
	EXPECT_STREQ("NULL", null_value_test(8, t_time).c_str());
	EXPECT_STREQ("NULL", null_value_test(8, t_datetime).c_str());
	EXPECT_STREQ("NULL", null_value_test(16, t_decimal).c_str());
	EXPECT_STREQ("NULL", null_value_test(2, t_u_smallInt).c_str());
}

#endif
