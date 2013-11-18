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
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamBroadcastExchange.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamExchange.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExchangeBroadcast.h"
#include "../Serialization/RegisterDerivedClass.h"
#include "../Schema/SchemaFix.h"

#include "../Message.h"
#include "../rdtsc.h"
#include "../Environment.h"
#include "../PerformanceMonitor/BlockStreamPerformanceMonitorTop.h"

//int main_join_success(int argc, const char** argv){
	int main_join_success22(int argc, const char** argv){
	int master;
	printf("Master(0) or Slave(others)?\n");
	scanf("%d",&master);
	if(master==0){






		Environment::getInstance(true);

//		const unsigned block_size=atoi(argv[1]);
//		const unsigned thread_count=atoi(argv[2]);
//		const unsigned expander_buffer=atoi(argv[3]);
//		std::vector<std::string> upper_ip_list;
//		const unsigned lowers_1=atoi(argv[4]);
//		const unsigned lowers_2=atoi(argv[5]);

		const unsigned block_size=4096;
		const unsigned thread_count=3;
		const unsigned expander_buffer=3;
		std::vector<std::string> upper_ip_list;
		const unsigned lowers_1=3;
		const unsigned lowers_2=3;

		upper_ip_list.push_back("10.11.1.208");

		std::vector<std::string> lower_ip_list;
//		lower_ip_list.push_back("10.11.1.201");
		lower_ip_list.push_back("10.11.1.202");
		lower_ip_list.push_back("10.11.1.203");
		lower_ip_list.push_back("10.11.1.204");

		lower_ip_list.push_back("10.11.1.205");
		lower_ip_list.push_back("10.11.1.206");
		lower_ip_list.push_back("10.11.1.207");
//		lower_ip_list.push_back("10.11.1.208");
//		lower_ip_list.push_back("10.11.1.209");
//		lower_ip_list.push_back("10.11.1.210");
//		lower_ip_list.push_back("10.11.1.211");
//		lower_ip_list.push_back("10.11.1.214");

//		std::vector<std::string> used_lowers;
//		for(unsigned i=0;i<lowers;i++){
//			used_lowers.push_back(lower_ip_list[i]);
//		}

		std::vector<std::string> used_lowers_1;
		for(unsigned i=0;i<lowers_1;i++){
			used_lowers_1.push_back(lower_ip_list[i]);
		}

		std::vector<std::string> used_lowers_2;

		for(unsigned i=lower_ip_list.size()-1;i>=lower_ip_list.size()-lowers_2;i--){
			used_lowers_2.push_back(lower_ip_list[i]);
		}

		std::vector<column_type> column_list,column_list_;
		column_list.push_back(column_type(t_int));

		Schema* schema=new SchemaFix(column_list);
//		ExpandableBlockStreamSingleColumnScan::State ebssc_state("/home/imdb/temp/Uniform_0_99.column",schema,block_size);
//		BlockStreamIteratorBase* ebssc=new ExpandableBlockStreamSingleColumnScan(ebssc_state);

		ExpandableBlockStreamSingleColumnScan::State ebssc_state1("/home/imdb/temp/zhanglei/join_2",schema,block_size);
		BlockStreamIteratorBase* ebssc1=new ExpandableBlockStreamSingleColumnScan(ebssc_state1);

//		ExpandableBlockStreamSingleColumnScan::State ebssc_state2("/home/imdb/temp/zhanglei/Uniform_0_99.column_10000",schema,block_size);
//		BlockStreamIteratorBase* ebssc2=new ExpandableBlockStreamSingleColumnScan(ebssc_state2);

		ExpandableBlockStreamSingleColumnScan::State ebssc_state3("/home/imdb/temp/zhanglei/join_2_cp",schema,block_size);
		BlockStreamIteratorBase* ebssc3=new ExpandableBlockStreamSingleColumnScan(ebssc_state3);

//		ExpandableBlockStreamSingleColumnScan::State ebssc_state4("/home/imdb/temp/zhanglei/Uniform_0_99.column_10000_copy",schema,block_size);
//		BlockStreamIteratorBase* ebssc4=new ExpandableBlockStreamSingleColumnScan(ebssc_state4);

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
		children_.push_back(ebssc3);

		////////////////////////////////////////combined\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


		BlockStreamCombinedIterator::State bsci_state1(inputs,output,children_);
		BlockStreamCombinedIterator *bsc1=new BlockStreamCombinedIterator(bsci_state1);


		BlockStreamExpander::State bse_state(output,bsc1,thread_count,block_size,expander_buffer);
		BlockStreamIteratorBase* bse=new BlockStreamExpander(bse_state);


		const int exchange_id=1;
		ExpandableBlockStreamBroadcastExchange::State ebse_state(output,bse,block_size,used_lowers_2,used_lowers_1,exchange_id);
		BlockStreamIteratorBase* ebse=new ExpandableBlockStreamBroadcastExchange(ebse_state);

		BlockStreamCombinedIterator::State bsci_state2(inputs,output,children_);
		BlockStreamCombinedIterator *bsc2=new BlockStreamCombinedIterator(bsci_state2);

		std::vector<column_type> column_list_join;
		column_list_join.push_back(column_type(t_int));
		column_list_join.push_back(column_type(t_int));
		column_list_join.push_back(column_type(t_int));
		Schema* output_join=new SchemaFix(column_list_join);

		std::vector<unsigned> joinIndex_left;
		joinIndex_left.push_back(0);
		std::vector<unsigned> joinIndex_right;
		joinIndex_right.push_back(1);
		std::vector<unsigned> payload_left;
		payload_left.push_back(1);
		std::vector<unsigned> payload_right;
		payload_right.push_back(0);

		BlockStreamJoinIterator::State bsji_state;//(ebse,bsc2,output,output,output_join,joinIndex_left,joinIndex_right,payload_left,payload_right,100,1024,4096);
		BlockStreamJoinIterator* bsji=new BlockStreamJoinIterator(bsji_state);

		BlockStreamExpander::State bse_state_(output_join,bsji,thread_count,block_size,expander_buffer);
		BlockStreamIteratorBase* bse_=new BlockStreamExpander(bse_state_);

		const int exchange_id_1=0;
		ExpandableBlockStreamBroadcastExchange::State ebse_state_(output,bse_,block_size,used_lowers_1,upper_ip_list,exchange_id_1);
		BlockStreamIteratorBase* ebse_=new ExpandableBlockStreamBroadcastExchange(ebse_state_);

		BlockStreamExpander::State bse_state_1(output_join,ebse_,thread_count,block_size,expander_buffer);
		BlockStreamIteratorBase* bse_1=new BlockStreamExpander(bse_state_1);


		BlockStreamPerformanceMonitorTop::State bspfm_state(output_join,bse_1,block_size,1000);
		BlockStreamIteratorBase* bspfm=new BlockStreamPerformanceMonitorTop(bspfm_state);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			BlockStreamBase *block=new BlockStreamFix(block_size,12);


			volatile int choice;

			printf("Continue(1) or Not(0) ?\n");
			scanf("%d",&choice);
			unsigned tuple_count=0;
			while(choice==1){


		//		bsf->open();

				ebse_->open();
				unsigned long long int start=curtick();
//				tuple_count=0;
				while(ebse_->next(block)){
					BlockStreamBase::BlockStreamTraverseIterator *it=block->createIterator();
					void* tuple;
					while(tuple=it->nextTuple()){
						printf("tuple:%d \n",*(int*)tuple);
						tuple_count++;
					}
					block->setEmpty();
				}

				printf("Total tupls:%d\n",tuple_count);
				printf("Time=%f Throughput=%f.\n",getSecond(start),1024/getSecond(start));
				ebse_->close();

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
