/*
 * DeepIteratorNoExchange.cpp
 *
 *  Created on: Aug 22, 2013
 *      Author: wangli
 */


#include <stdlib.h>
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include "../Executor/IteratorExecutorMaster.h"
#include "../Executor/IteratorExecutorSlave.h"
#include "../iterator.hpp"
#include "../iterator/FilterIterator.h"
#include "../iterator/PrintIterator.h"
#include "../iterator/SingleColumnScanIterator.h"
#include "../iterator/SingleColumnScanIteratorFix.h"
#include "../iterator/ExchangeIteratorEager.h"
#include "../Environment.h"
#include "../PerformanceMonitor/PerformanceIteratorTop.h"
#include "../rdtsc.h"
#include "../Schema/SchemaFix.h"

int mainasfasdfasf(int argc, const char** argv){
	std::vector<column_type> column_list;
	column_list.push_back(column_type(t_int));

	SchemaFix* input=new SchemaFix(column_list);
	SchemaFix* output=new SchemaFix(column_list);


	SingleColumnScanIteratorFix::State SCstate1("/home/imdb/temp/Uniform_0_99.column",input,output);
	Iterator* scsfix=new SingleColumnScanIteratorFix(SCstate1);

	Schema* in=new SchemaFix(column_list);

	SingleColumnScanIterator::State SCstate("/home/imdb/temp/Uniform_0_99.column",in,in);
	Iterator* scs=new SingleColumnScanIterator(SCstate);


	int f=100;
	FilterIterator::AttributeComparator filter1(column_type(t_int),Comparator::L,0,&f);
	std::vector<FilterIterator::AttributeComparator> ComparatorList;
	ComparatorList.push_back(filter1);
	FilterIterator::State FIstate3(output,output,scs,ComparatorList);
	Iterator* fi2=new FilterIterator(FIstate3);

//	PerformanceIteratorTop::State PITstate(output,scs1,1000);
//	Iterator *pi=new PerformanceIteratorTop(PITstate);
	int choice=0;

	void* tuple=memalign(cacheline_size,input->getTupleMaxSize());
	while(choice==0){




		scs->open();
		unsigned long long int start=curtick();
		while(scs->next(tuple));
		scs->close();
		printf("Time=%f Throughput=%f.\n",getSecond(start),1024/getSecond(start));

		printf("Continue(0) or Not(1) ?\n");

		scanf("%d",&choice);
	}



}
