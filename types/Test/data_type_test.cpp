/*
 * data_type_test.cpp
 *
 *  Created on: Feb 24, 2014
 *      Author: wangli
 */
#include <stdio.h>
#include <fstream>
#include "../NValue.hpp"
#include "../../Schema/SchemaFix.h"
#include "../../iterator/SingleColumnScanIterator.h"
#include "../../iterator/PrintIterator.h"
#include "../../iterator/FilterIterator.h"
#include "../../iterator/AggregationIterator.h"
using namespace std;
static int test_data_type(){
	//	const int cn=8;
	//	ofstream outFile("/home/scdong/data/decimal.out", ios::out);
	//	string str[cn] = {"123.001", "4582.55", "12", "0.569797", "1356456352", "0.569797","123.001","1356456352"};
	//	char* result = new char [1024];
	//	int count = 0;
	//	memset(result, 0, 1024);
	//
	//	for (int i = 0; i < cn; i++)
	//	{
	//		NValue* nv = new NValue();
	//		*nv = NValue::getDecimalValueFromString(str[i]);
	//		memcpy((void*)(result+count), (void*)nv, sizeof(*nv));
	//		count += sizeof(*nv);
	//	}
	//	for (int i = 0; i < count; i++)
	//		outFile << result[i];
	//	cout << count << endl;
	//	return 0;

		std::vector<column_type> column_list;
		column_list.push_back(column_type(t_decimal));
		column_list.push_back(column_type(t_decimal));
		Schema* input=new SchemaFix(column_list);
		Schema* output=new SchemaFix(column_list);
		SingleColumnScanIterator::State SCstate1("/home/scdong/data/decimal.out",input, output);
		Iterator* scs1=new SingleColumnScanIterator(SCstate1);

	//	///////////////////////////////////////////Filter////////////////////////////////////////
	//	FilterIterator::State FIstate;
	//	FIstate.child=scs1;
	//	FIstate.input=output;
	//	FIstate.output=output;
	//
	//	NValue f1 = NValue::getDecimalValueFromString("123.001");
	//	FilterIterator::AttributeComparator filter1(column_type(t_decimal),Comparator::GEQ,0,&f1);
	//	FIstate.ComparatorList.push_back(filter1);
	//
	//	NValue f2 = NValue::getDecimalValueFromString("1356456352");
	//	FilterIterator::AttributeComparator filter2(column_type(t_decimal),Comparator::L,0,&f2);
	//	FIstate.ComparatorList.push_back(filter2);
	//	Iterator* fi=new FilterIterator(FIstate);

		///////////////////////////////////////Aggregation////////////////////////////////////|
		std::vector<unsigned> GroIndex;
		GroIndex.push_back(0);
	//	GroIndex.push_back(1);
		std::vector<unsigned> AggIndex;
		AggIndex.push_back(1);
	//	AggIndex.push_back(2);
		std::vector<AggregationIterator::State::aggregation> aggtype;
		aggtype.push_back(AggregationIterator::State::min);
	//	aggtype.push_back(AggregationIterator::State::count);
		AggregationIterator::State AIstate(input,output,scs1,GroIndex,AggIndex,aggtype,1024,48);
		Iterator* ai=new AggregationIterator(AIstate);

		PrintIterator::State PIstate(output, ai);
		Iterator *pi=new PrintIterator(PIstate);

		int count = 0;
		pi->open();
		void* tuple=malloc(100);
		cout << count++ << ": ";
		while(pi->next(tuple)){cout << count++ << ": ";}
		pi->close();
		cout << endl;
		return 0;

		NValue nv = NValue::getDecimalValueFromString("12345");
	//	cout << sizeof(nv) << endl;
		NValue nv1 = NValue::getDecimalValueFromString("5678");

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

