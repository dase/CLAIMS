/*
 * expandable_block_stream_test.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: wangli
 */
#include <stdlib.h>
#include <string>
#include "../Schema/SchemaFix.h"
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScanDisk.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamFilter.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchange.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeMaterialized.h"
#include "../Message.h"
#include "../rdtsc.h"
#include "../Environment.h"
#include "../PerformanceMonitor/BlockStreamPerformanceMonitorTop.h"
int main234234(int argc, const char** argv){

	int master;
	printf("Master(0) or Slave(others)?\n");
	scanf("%d",&master);
	if(master==0){






		Environment::getInstance(true);

		const unsigned block_size=atoi(argv[1]);
		const unsigned thread_count=atoi(argv[2]);
		int f=atoi(argv[3]);
		const unsigned expander_buffer=atoi(argv[4]);
		const unsigned lowers=atoi(argv[5]);
		std::vector<std::string> upper_ip_list;
		upper_ip_list.push_back("10.11.1.208");

		std::vector<std::string> lower_ip_list;
//		lower_ip_list.push_back("10.11.1.201");
		lower_ip_list.push_back("10.11.1.214");
//		lower_ip_list.push_back("10.11.1.210");
		lower_ip_list.push_back("10.11.1.202");
		lower_ip_list.push_back("10.11.1.203");
		lower_ip_list.push_back("10.11.1.204");
		lower_ip_list.push_back("10.11.1.205");
		lower_ip_list.push_back("10.11.1.206");
		lower_ip_list.push_back("10.11.1.207");
		lower_ip_list.push_back("10.11.1.211");
		lower_ip_list.push_back("10.11.1.209");

		lower_ip_list.push_back("10.11.1.201");


		std::vector<std::string> used_lowers;
		for(unsigned i=0;i<lowers;i++){
			used_lowers.push_back(lower_ip_list[i]);
		}

		std::vector<column_type> column_list,column_list_;
		column_list.push_back(column_type(t_int));

		Schema* schema=new SchemaFix(column_list);
		ExpandableBlockStreamSingleColumnScan::State ebssc_state("/home/imdb/temp/Uniform_0_99.column",schema,block_size);
		BlockStreamIteratorBase* ebssc=new ExpandableBlockStreamSingleColumnScan(ebssc_state);


		FilterIterator::AttributeComparator filter1(column_type(t_int),Comparator::L,0,&f);
		std::vector<FilterIterator::AttributeComparator> ComparatorList;
		ComparatorList.push_back(filter1);
		ExpandableBlockStreamFilter::State ebsf_state(schema,ebssc,ComparatorList,block_size);
		BlockStreamIteratorBase* bbsf=new ExpandableBlockStreamFilter(ebsf_state);


		BlockStreamExpander::State bse_state(schema,bbsf,thread_count,block_size,expander_buffer);
		BlockStreamIteratorBase* bse=new BlockStreamExpander(bse_state);



		const int exchange_id=0;
		ExpandableBlockStreamExchangeEpoll::State ebse_state(schema,bse,block_size,used_lowers,upper_ip_list,exchange_id);
		BlockStreamIteratorBase* ebse=new ExpandableBlockStreamExchangeEpoll(ebse_state);

		BlockStreamPerformanceMonitorTop::State bspfm_state(schema,ebse,block_size,1000);
		BlockStreamIteratorBase* bspfm=new BlockStreamPerformanceMonitorTop(bspfm_state);

//		BlockStreamBase *block=new BlockStreamFix(block_size,4);


		volatile int choice;

		printf("Continue(1) or Not(0) ?\n");
		scanf("%d",&choice);
		unsigned tuple_count=0;
		while(choice==1){


	//		bsf->open();

			bspfm->open();
//			unsigned long long int start=curtick();
//			tuple_count=0;
			while(bspfm->next(0)){
//				BlockStreamBase::BlockStreamTraverseIterator *it=block->createIterator();
//				void* tuple;
//				while(tuple=it->nextTuple()){
////					printf("tuple:%d \n",*(int*)tuple);
//					tuple_count++;
//				}
//				block->setEmpty();
			}
//			printf("Total tupls:%d\n",tuple_count);
//			printf("Time=%f Throughput=%f.\n",getSecond(start),1024/getSecond(start));
			bspfm->close();

			printf("Continue(0) or Not(1) ?\n");
//			getchar();
			scanf("%d",&choice);
			printf("you input %d\n",choice);

		}
	}
	else{
		Environment::getInstance(false);
	}

	printf("Waiting~~~~~....\n");
	while(true){
		sleep(1);
	}
}


