#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include "../BlockStreamIterator/BlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/BlockStreamFilter.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamCombinedIterator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamJoinIterator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExchangeLowerBroadcast.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExchangeBroadcast.h"
#include "../Serialization/RegisterDerivedClass.h"
#include "../common/Schema/SchemaFix.h"

#include "../common/Message.h"
#include "../utility/rdtsc.h"
#include "../Environment.h"
#include "../PerformanceMonitor/BlockStreamPerformanceMonitorTop.h"

int main_broadcast(int argc, const char** argv){
//	int main(int argc, const char** argv){
	int master;
	printf("Master(0) or Slave(others)?\n");
	scanf("%d",&master);
	if(master==0){






		Environment::getInstance(true);

		const unsigned block_size=4096;
		const unsigned thread_count=4;
//		int f=atoi(argv[3]);
		const unsigned expander_buffer=4;
		const unsigned lowers=4;
		std::vector<std::string> upper_ip_list;
		upper_ip_list.push_back("10.11.1.208");

		std::vector<std::string> lower_ip_list;
//		lower_ip_list.push_back("10.11.1.201");
//		lower_ip_list.push_back("10.11.1.202");
//		lower_ip_list.push_back("10.11.1.203");
//		lower_ip_list.push_back("10.11.1.204");
		lower_ip_list.push_back("10.11.1.205");
		lower_ip_list.push_back("10.11.1.206");
		lower_ip_list.push_back("10.11.1.207");
		lower_ip_list.push_back("10.11.1.204");
//		lower_ip_list.push_back("10.11.1.209");
//		lower_ip_list.push_back("10.11.1.210");
//		lower_ip_list.push_back("10.11.1.211");
//		lower_ip_list.push_back("10.11.1.214");

		std::vector<std::string> used_lowers;
		for(unsigned i=0;i<lowers;i++){
			used_lowers.push_back(lower_ip_list[i]);
		}

		std::vector<column_type> column_list,column_list_;
		column_list.push_back(column_type(t_int));

		Schema* schema=new SchemaFix(column_list);

		ExpandableBlockStreamSingleColumnScan::State ebssc_state1("/home/imdb/temp/zhanglei/Uniform_0_99.column_1G",schema,block_size);
		BlockStreamIteratorBase* ebssc1=new ExpandableBlockStreamSingleColumnScan(ebssc_state1);

		ExpandableBlockStreamSingleColumnScan::State ebssc_state2("/home/imdb/temp/zhanglei/Uniform_0_99.column_1G_copy",schema,block_size);
		BlockStreamIteratorBase* ebssc2=new ExpandableBlockStreamSingleColumnScan(ebssc_state2);

//		FilterIterator::AttributeComparator filter1(column_type(t_int),Comparator::L,0,&f);
//		std::vector<FilterIterator::AttributeComparator> ComparatorList;
//		ComparatorList.push_back(filter1);
//		ExpandableBlockStreamFilter::State ebsf_state(schema,ebssc,ComparatorList,block_size);
//		BlockStreamIteratorBase* bbsf=new ExpandableBlockStreamFilter(ebsf_state);

		std::vector<Schema *> inputs;
		inputs.push_back(schema);
		inputs.push_back(schema);

		column_list_.push_back(column_type(t_int));
		column_list_.push_back(column_type(t_int));
		Schema* output=new SchemaFix(column_list_);

		std::vector<BlockStreamIteratorBase *> children_;
		children_.push_back(ebssc1);
		children_.push_back(ebssc2);

		////////////////////////////////////////combined\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


		BlockStreamCombinedIterator::State bsci_state1(inputs,output,children_);
		BlockStreamCombinedIterator *bsc1=new BlockStreamCombinedIterator(bsci_state1);

		BlockStreamExpander::State bse_state(schema,bsc1,thread_count,block_size,expander_buffer);
		BlockStreamIteratorBase* bse=new BlockStreamExpander(bse_state);



		const int exchange_id=0;
		BlockStreamExchangeBroadcast::State ebse_state(schema,bse,block_size,used_lowers,upper_ip_list,exchange_id);
		BlockStreamIteratorBase* ebse=new BlockStreamExchangeBroadcast(ebse_state);

		BlockStreamPerformanceMonitorTop::State bspfm_state(schema,ebse,block_size,1000);
		BlockStreamIteratorBase* bspfm=new BlockStreamPerformanceMonitorTop(bspfm_state);

		BlockStreamBase *block=new BlockStreamFix(block_size,8);


		volatile int choice;

		printf("Continue(1) or Not(0) ?\n");
		scanf("%d",&choice);
		unsigned tuple_count=0;
		while(choice==1){


	//		bsf->open();

			bspfm->open();
			unsigned long long int start=curtick();
			tuple_count=0;
			while(bspfm->next(block)){
				BlockStreamBase::BlockStreamTraverseIterator *it=block->createIterator();
				void* tuple;
				while(tuple=it->nextTuple()){
//					printf("tuple:%d \n",*(int*)tuple);
					tuple_count++;
				}
				block->setEmpty();
			}
			printf("Total tupls:%d\n",tuple_count);
			printf("Time=%f Throughput=%f.\n",getSecond(start),1024/getSecond(start));
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
