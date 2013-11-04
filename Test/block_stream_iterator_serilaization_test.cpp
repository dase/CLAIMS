/*
 * block_stream_iterator_test.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */
#include "../BlockStreamIterator/BlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/BlockStreamFilter.h"

#include "../Schema/SchemaFix.h"
#include "../rdtsc.h"

int mainasdfaf234(int argc,const char** argv){

	std::vector<column_type> column_list,column_list_;
	column_list.push_back(column_type(t_int));

	Schema* input=new SchemaFix(column_list);

	BlockStreamSingleColumnScan::State bsscs_state("/home/imdb/temp/Uniform_0_99.column",input);
	BlockStreamIteratorBase* bsscs1=new BlockStreamSingleColumnScan(bsscs_state);
	BlockStreamIteratorBase* bsscs2=new BlockStreamSingleColumnScan(bsscs_state);

	int f=atoi(argv[2]);



	FilterIterator::AttributeComparator fA(column_type(t_int),Comparator::L,0,&f);
	std::vector<FilterIterator::AttributeComparator> ComparatorList;
	ComparatorList.push_back(fA);
	BlockStreamFilter::State bsf_state(input,bsscs1,ComparatorList,4096);
	BlockStreamIteratorBase* bsf=new BlockStreamFilter(bsf_state);

//
	BlockStreamBase *block=new BlockStreamFix(4096,4);
	int choice=0;

	while(choice==0){


//		bsf->open();
		bsf->open();
		unsigned long long int start=curtick();

		while(bsf->next(block)){
			block->setEmpty();
		}
		printf("Time=%f Throughput=%f.\n",getSecond(start),1024/getSecond(start));
		bsf->close();
		printf("Continue(0) or Not(1) ?\n");

		scanf("%d",&choice);

	}

}


