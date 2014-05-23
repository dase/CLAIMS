/*
 * expandable_block_stream_test.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: wangli
 */
#include <stdlib.h>
#include <string>
#include "../common/Schema/SchemaFix.h"
#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScanDisk.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamFilter.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchange.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeEpoll.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchangeMaterialized.h"
#include "../common/Message.h"
#include "../utility/rdtsc.h"
#include "../Environment.h"
#include "../PerformanceMonitor/BlockStreamPerformanceMonitorTop.h"
int main___S_S_(int argc, const char** argv){

	printf("Yes!\n");
	int master;
	printf("Master(0) or Slave(others)?\n");
	scanf("%d",&master);
	if(master==0){






		Environment::getInstance(true);

		const unsigned block_size=atoi(argv[1]);
		const unsigned thread_count=atoi(argv[2]);
		int f=atoi(argv[3]);
		const unsigned expander_buffer=atoi(argv[4]);
		const unsigned lowers_1=atoi(argv[5]);
		const unsigned lowers_2=atoi(argv[6]);
//		const unsigned column_count=atoi(argv[7]);
		std::vector<std::string> upper_ip_list;
		upper_ip_list.push_back("10.11.1.208");

		std::vector<std::string> lower_ip_list;

		lower_ip_list.push_back("10.11.1.201");
		lower_ip_list.push_back("10.11.1.202");
		lower_ip_list.push_back("10.11.1.203");
		lower_ip_list.push_back("10.11.1.204");


		lower_ip_list.push_back("10.11.1.211");
//		lower_ip_list.push_back("10.11.1.210");
//		lower_ip_list.push_back("10.11.1.201");
		lower_ip_list.push_back("10.11.1.214");

		lower_ip_list.push_back("10.11.1.209");
		lower_ip_list.push_back("10.11.1.207");
		lower_ip_list.push_back("10.11.1.206");
		lower_ip_list.push_back("10.11.1.205");




		std::vector<std::string> used_lowers_1;
		for(unsigned i=0;i<lowers_1;i++){
			used_lowers_1.push_back(lower_ip_list[i]);
		}

//
		std::vector<std::string> used_lowers_2;

		for(unsigned i=lower_ip_list.size()-1;i>=lower_ip_list.size()-lowers_2;i--){
			used_lowers_2.push_back(lower_ip_list[i]);
		}



		std::vector<column_type> column_list,column_list_;
		column_list.push_back(column_type(t_int));
		column_list.push_back(column_type(t_int));
//		for(unsigned i=0;i<column_count;i++){
//			column_list.push_back(column_type(t_int));
//		}

		Schema* schema=new SchemaFix(column_list);

		/*ColumnScan*/
		ExpandableBlockStreamSingleColumnScan::State ebssc_state("/home/imdb/temp/Uniform_0_99.column",schema,block_size);
		BlockStreamIteratorBase* ebssc=new ExpandableBlockStreamSingleColumnScan(ebssc_state);
//
//		FilterIterator::AttributeComparator filter0(column_type(t_int),Comparator::L,1,&f);
//		std::vector<FilterIterator::AttributeComparator> ComparatorList0;
//		ComparatorList0.push_back(filter0);
//		ExpandableBlockStreamFilter::State ebsf_state0(schema,ebssc,ComparatorList0,block_size);
//		BlockStreamIteratorBase* bbsf0=new ExpandableBlockStreamFilter(ebsf_state0);

		/*Expander*/
		BlockStreamExpander::State bse_state_1(schema,ebssc,thread_count,block_size,expander_buffer);
		BlockStreamIteratorBase* bse_1=new BlockStreamExpander(bse_state_1);

		/* Exchange */
		const int exchange_id_1=1;
		ExpandableBlockStreamExchangeEpoll::State ebse_state_1(schema,bse_1,block_size,used_lowers_2,used_lowers_1,exchange_id_1);
		BlockStreamIteratorBase* ebse_1=new ExpandableBlockStreamExchangeEpoll(ebse_state_1);

		/*Filter*/
		FilterIterator::AttributeComparator filter1(column_type(t_int),Comparator::L,1,&f);
		std::vector<FilterIterator::AttributeComparator> ComparatorList;
		ComparatorList.push_back(filter1);
		ExpandableBlockStreamFilter::State ebsf_state(schema,ebse_1,ComparatorList,block_size);
		BlockStreamIteratorBase* bbsf=new ExpandableBlockStreamFilter(ebsf_state);

		/*Expander*/
		BlockStreamExpander::State bse_state(schema,bbsf,thread_count,block_size,expander_buffer);
		BlockStreamIteratorBase* bse=new BlockStreamExpander(bse_state);


		/* Exchange */
		const int exchange_id=0;
		ExpandableBlockStreamExchangeEpoll::State ebse_state(schema,bse,block_size,used_lowers_1,upper_ip_list,exchange_id);
		BlockStreamIteratorBase* ebse=new ExpandableBlockStreamExchangeEpoll(ebse_state);

		/* Performance Monitor*/
		BlockStreamPerformanceMonitorTop::State bspfm_state(schema,ebse,block_size,1000);
		BlockStreamIteratorBase* bspfm=new BlockStreamPerformanceMonitorTop(bspfm_state);

//		BlockStreamBase *block=new BlockStreamFix(block_size,schema->getTupleMaxSize());


		int choice;

		std::cout<<"Continue(1) or Not(0) ?\n";
		std::cin>>choice;
		unsigned tuple_count=0;
		while(choice==1){


	//		bsf->open();

			bspfm->open();
//			unsigned long long int start=curtick();
//			tuple_count=0;
//			while(bspfm->next(0)){
			while(bspfm->next(0)){
//				BlockStreamBase::BlockStreamTraverseIterator *it=block->createIterator();
//				void* tuple;
//				while(tuple=it->nextTuple()){
//					printf("tuple:%d,%d \n",*(int*)tuple,*((int*)tuple+1));
//					tuple_count++;
//				}
//				bspfm->setEmpty();
			}
//			printf("Total tupls:%d\n",tuple_count);
//			printf("Time=%f Throughput=%f.\n",getSecond(start),1024/getSecond(start));
			bspfm->close();

			std::cout<<("Continue(0) or Not(1) ?\n");
//			getchar();
			std::cin>>choice;
//			scanf("%d",&choice);
//			printf("you input %d\n",choice);

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


