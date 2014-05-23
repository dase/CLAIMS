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
#include "../../../iterator/SingleColumnScanIterator.h"
#include "../../../iterator/PrintIterator.h"
#include "../../../iterator/FilterIterator.h"
#include "../../../iterator/AggregationIterator.h"

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


 int test_data_type(){
//	//	const int cn=8;
//	//	ofstream outFile("/home/scdong/data/decimal.out", ios::out);
//	//	string str[cn] = {"123.001", "4582.55", "12", "0.569797", "1356456352", "0.569797","123.001","1356456352"};
//	//	char* result = new char [1024];
//	//	int count = 0;
//	//	memset(result, 0, 1024);
//	//
//	//	for (int i = 0; i < cn; i++)
//	//	{
//	//		NValue* nv = new NValue();
//	//		*nv = NValue::getDecimalValueFromString(str[i]);
//	//		memcpy((void*)(result+count), (void*)nv, sizeof(*nv));
//	//		count += sizeof(*nv);
//	//	}
//	//	for (int i = 0; i < count; i++)
//	//		outFile << result[i];
//	//	cout << count << endl;
//	//	return 0;
//
//		std::vector<column_type> column_list;
//		column_list.push_back(column_type(t_decimal));
//		column_list.push_back(column_type(t_decimal));
//		Schema* input=new SchemaFix(column_list);
//		Schema* output=new SchemaFix(column_list);
//		SingleColumnScanIterator::State SCstate1("/home/scdong/data/decimal.out",input, output);
//		Iterator* scs1=new SingleColumnScanIterator(SCstate1);
//
//	//	///////////////////////////////////////////Filter////////////////////////////////////////
//	//	FilterIterator::State FIstate;
//	//	FIstate.child=scs1;
//	//	FIstate.input=output;
//	//	FIstate.output=output;
//	//
//	//	NValue f1 = NValue::getDecimalValueFromString("123.001");
//	//	FilterIterator::AttributeComparator filter1(column_type(t_decimal),Comparator::GEQ,0,&f1);
//	//	FIstate.ComparatorList.push_back(filter1);
//	//
//	//	NValue f2 = NValue::getDecimalValueFromString("1356456352");
//	//	FilterIterator::AttributeComparator filter2(column_type(t_decimal),Comparator::L,0,&f2);
//	//	FIstate.ComparatorList.push_back(filter2);
//	//	Iterator* fi=new FilterIterator(FIstate);
//
//		///////////////////////////////////////Aggregation////////////////////////////////////|
//		std::vector<unsigned> GroIndex;
//		GroIndex.push_back(0);
//	//	GroIndex.push_back(1);
//		std::vector<unsigned> AggIndex;
//		AggIndex.push_back(1);
//	//	AggIndex.push_back(2);
//		std::vector<AggregationIterator::State::aggregation> aggtype;
//		aggtype.push_back(AggregationIterator::State::min);
//	//	aggtype.push_back(AggregationIterator::State::count);
//		AggregationIterator::State AIstate(input,output,scs1,GroIndex,AggIndex,aggtype,1024,48);
//		Iterator* ai=new AggregationIterator(AIstate);
//
//		PrintIterator::State PIstate(output, ai);
//		Iterator *pi=new PrintIterator(PIstate);
//
//		int count = 0;
//		pi->open();
//		void* tuple=malloc(100);
//		cout << count++ << ": ";
//		while(pi->next(tuple)){cout << count++ << ": ";}
//		pi->close();
//		cout << endl;
//		return 0;
//
//		NValue nv = NValue::getDecimalValueFromString("12345");
//	//	cout << sizeof(nv) << endl;
//		NValue nv1 = NValue::getDecimalValueFromString("5678");
//
//		OperateDecimal od;
//		cout << od.toString((void*)&nv);
//		return 0;
//
//		ADD<NValue*>((void*)(&nv),(void*)(&nv1));
//		char buf[1024] = {'\0'};
//		ExportSerializeOutput out(buf, 1024);
//
//		cout << "decimal outputting: \n";
//		nv.serializeToExport(out);
//		cout << "size of decimal: " << *(int*)buf << endl;
//		cout << "value is: " << buf+4 << endl << endl;
//
//		boost::gregorian::date d;
//		printf("size=%d\n",sizeof(d));
	test_decimal();
	return 0;


	//	const int cn=8;
//	//	ofstream outFile("/home/scdong/data/decimal.out", ios::out);
//	//	string str[cn] = {"123.001", "4582.55", "12", "0.569797", "1356456352", "0.569797","123.001","1356456352"};
//	//	char* result = new char [1024];
//	//	int count = 0;
//	//	memset(result, 0, 1024);
//	//
//	//	for (int i = 0; i < cn; i++)
//	//	{
//	//		NValue* nv = new NValue();
//	//		*nv = NValue::getDecimalValueFromString(str[i]);
//	//		memcpy((void*)(result+count), (void*)nv, sizeof(*nv));
//	//		count += sizeof(*nv);
//	//	}
//	//	for (int i = 0; i < count; i++)
//	//		outFile << result[i];
//	//	cout << count << endl;
//	//	return 0;
//
//		std::vector<column_type> column_list;
//		column_list.push_back(column_type(t_decimal));
//		column_list.push_back(column_type(t_decimal));
//		Schema* input=new SchemaFix(column_list);
//		Schema* output=new SchemaFix(column_list);
//		SingleColumnScanIterator::State SCstate1("/home/scdong/data/decimal.out",input, output);
//		Iterator* scs1=new SingleColumnScanIterator(SCstate1);
//
//	//	///////////////////////////////////////////Filter////////////////////////////////////////
//	//	FilterIterator::State FIstate;
//	//	FIstate.child=scs1;
//	//	FIstate.input=output;
//	//	FIstate.output=output;
//	//
//	//	NValue f1 = NValue::getDecimalValueFromString("123.001");
//	//	FilterIterator::AttributeComparator filter1(column_type(t_decimal),Comparator::GEQ,0,&f1);
//	//	FIstate.ComparatorList.push_back(filter1);
//	//
//	//	NValue f2 = NValue::getDecimalValueFromString("1356456352");
//	//	FilterIterator::AttributeComparator filter2(column_type(t_decimal),Comparator::L,0,&f2);
//	//	FIstate.ComparatorList.push_back(filter2);
//	//	Iterator* fi=new FilterIterator(FIstate);
//
//		///////////////////////////////////////Aggregation////////////////////////////////////|
//		std::vector<unsigned> GroIndex;
//		GroIndex.push_back(0);
//	//	GroIndex.push_back(1);
//		std::vector<unsigned> AggIndex;
//		AggIndex.push_back(1);
//	//	AggIndex.push_back(2);
//		std::vector<AggregationIterator::State::aggregation> aggtype;
//		aggtype.push_back(AggregationIterator::State::min);
//	//	aggtype.push_back(AggregationIterator::State::count);
//		AggregationIterator::State AIstate(input,output,scs1,GroIndex,AggIndex,aggtype,1024,48);
//		Iterator* ai=new AggregationIterator(AIstate);
//
//		PrintIterator::State PIstate(output, ai);
//		Iterator *pi=new PrintIterator(PIstate);
//
//		int count = 0;
//		pi->open();
//		void* tuple=malloc(100);
//		cout << count++ << ": ";
//		while(pi->next(tuple)){cout << count++ << ": ";}
//		pi->close();
//		cout << endl;
//		return 0;

		NValue nv = NValue::getDecimalValueFromString("1.03");
	//	cout << sizeof(nv) << endl;
		NValue nv1 = NValue::getDecimalValueFromString("1.2");

		const void *v1,*v22;
		v1=&nv;
		v22=&nv1;

		column_type* op=new column_type(data_type(t_decimal),4);
		if(op->operate->less(v1,v22)){
			printf("%s is smaller than %s\n!",op->operate->toString(&nv).c_str(),op->operate->toString(&nv1).c_str());
		}

		if(op->operate->less(v22,v1)){
			printf("less2!");
		}
		if(op->operate->equal(v22,v1)){
			printf("equal!\n");
		}

		OperateDecimal od;
		cout << od.toString((void*)&nv);
		return 0;

		ADD<NValue*>((void*)(&nv),(void*)(&nv1));
		char buf[1024] = {'\0'};
		ExportSerializeOutput out(buf, 1024);

		cout << "decimal outputting: \n";
		nv.serializeToExport(out);
		cout << "size of decimal: " << *(int*)buf << endl;
		cout << "value is: " << buf+4 << endl << endl;

		boost::gregorian::date d;
		printf("size=%d\n",sizeof(d));
		return 0;
}
#endif
