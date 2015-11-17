/*
 * block_stream_iterator_test.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */
#include "../BlockStreamIterator/BlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/BlockStreamFilter.h"

#include "../common/Schema/SchemaFix.h"
#include "../utility/rdtsc.h"
#include "../common/AttributeComparator.h"

int mainasdfaf234(int argc,const char** argv){

	std::vector<column_type> column_list,column_list_;
	column_list.push_back(column_type(t_int));

	Schema* input=new SchemaFix(column_list);

	BlockStreamSingleColumnScan::State bsscs_state("/home/claims/temp/Uniform_0_99.column",input);
	PhysicalOperatorBase* bsscs1=new BlockStreamSingleColumnScan(bsscs_state);
	PhysicalOperatorBase* bsscs2=new BlockStreamSingleColumnScan(bsscs_state);

	int f=atoi(argv[2]);



	AttributeComparator fA(column_type(t_int),Comparator::L,0,&f);
	std::vector<AttributeComparator> ComparatorList;
	ComparatorList.push_back(fA);
	BlockStreamFilter::State bsf_state(input,bsscs1,ComparatorList,4096);
	PhysicalOperatorBase* bsf=new BlockStreamFilter(bsf_state);

//
	BlockStreamBase *block=new BlockStreamFix(4096,4);
	int choice=0;

	while(choice==0){


//		bsf->open();
		bsf->Open();
		unsigned long long int start=curtick();

		while(bsf->Next(block)){
			block->setEmpty();
		}
		printf("Time=%f Throughput=%f.\n",getSecond(start),1024/getSecond(start));
		bsf->Close();
		printf("Continue(0) or Not(1) ?\n");

		scanf("%d",&choice);

	}

}


