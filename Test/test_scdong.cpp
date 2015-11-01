/*
 * test_scdong.cpp
 *
 *  Created on: 2013年9月9日
 *      Author: SCDONG
 */

#include <typeinfo>
#include <malloc.h>
#include <stdlib.h>

#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <fstream>

#include "../Executor/IteratorExecutorMaster.h"
#include "../Executor/IteratorExecutorSlave.h"

#include "../utility/rdtsc.h"

#include "../Environment.h"

#include "../common/Comparator.h"
#include "../common/Message.h"

#include "../common/Schema/SchemaFix.h"

#include "../physical_operator/physical_operator_base.h"
#include "../BlockStreamIterator/BlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/BlockStreamPerformanceTest.h"

#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScanDisk.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStredomDiskAccess.h"
#include "../physical_operator/BlockStreamExpander.h"

using namespace#include "../physical_operator/ExpandableBlockStreamRandomDiskAccess.h"
e***************************#include "../physical_operator/ExpandableBlockStreamRandomMemAccess.h"
****************/


/*******#include "../physical_operator/ExpandableBlockStreamSingleColumnScan.h"
*****Local Disk Access******#include "../physical_operator/ExpandableBlockStreamSingleColumnScanDisk.h"
******/
int main_ld (int argc, char** argv)
{
	const unsigned block_size = atoi(argv[1]);
	const unsigned thread_count = atoi(argv[2]);
	const unsigned expander_buffer = atoi(argv[3]);
	const int columns_num = atoi(argv[4]);
	unsigned long random_size = atoi(argv[5]);	//number of columns which are randomly accessed
	random_size = random_size*sizeof(int);
	unsigned long scan_size = (unsigned long)4*1024*1024*1024-random_size*columns_num;

	cout << "columns_num: " << columns_num << "\t random_size: " << random_size/sizeof(int) << endl;

	std::vector<column_type> column_list,column_list_;
	column_list.push_back(column_type(t_int));
	for (int i = 0; i < columns_num; i++)
		column_list_.push_back(column_type(t_int));

	Schema* i_schema = new SchemaFix(column_list);
	Schema* d_schema = new SchemaFix(column_list_);

//	ExpandableBlockStreamSingleColumnScan::State ebssc_state1("/home/claims/temp/6_0.column", d_schema, block_size);
//	BlockStreamIteratorBase* ebssc1 = new ExpandableBlockStreamSingleColumnScan(ebssc_state1);
//	ebssc1->open();
//	BlockStreamBase* block_=BlockStreamBase::createBlock(i_schema,block_size);
//	while (ebssc1->next(block_)) {};
//	ebssc1->close();
//	block_->~BlockStreamBase();

	ExpandableBlockStreamSingleColumnScanDisk::State ebsscsd_state("/home/claims/temp/5_0.column", d_schema, block_size, scan_size);
	PhysicalOperatorBase* ebsscsd = new ExpandableBlockStreamSingleColumnScanDisk(ebsscsd_state);

	ExpandableBlockStreamSingleColumnScan::State ebssc_state("/home/claims/temp/6_0.column", i_schema, block_size, random_size);
	PhysicalOperatorBase* ebssc = new ExpandableBlockStreamSingleColumnScan(ebssc_state);

//	BlockStreamExpander::State bse_state(i_schema,ebssc,thread_count,block_size,expander_buffer);
//	BlockStreamIteratorBase* bse=new BlockStreamExpander(bse_state);

	ExpandableBlockStreamRandomDiskAccess::State ebsrda_state("/home/claims/temp/Uniform_0_9999.column", ebssc, d_schema, i_schema, block_size);
	PhysicalOperatorBase* ebsrda = new ExpandableBlockStreamRandomDiskAccess(ebsrda_state);

	Expander::State bse_state1(d_schema,ebsscsd,thread_count,block_size,expander_buffer);
	PhysicalOperatorBase* bse1=new Expander(bse_state1);
	Expander::State bse_state2(d_schema,ebsrda,thread_count,block_size,expander_buffer);
	PhysicalOperatorBase* bse2=new Expander(bse_state2);

	BlockStreamPerformanceTest::State bspt_state(d_schema, bse1, bse2, block_size, 1000);
	PhysicalOperatorBase* bspt = new BlockStreamPerformanceTest(bspt_state);

	bspt->Open();
	while (bspt->Next(0));
	bspt->Close();

	return 0;
}


/************Remote Memory Access************/
//int main2342342342 (int argc, char** argv)
//{
//	int master;
//	printf("Master(0) or Slave(others)?\n");
//	scanf("%d",&master);
//
//	if(master==0){
//		Environment::getInstance(true);
//
//		const unsigned block_size = atoi(argv[1]);
//		const unsigned thread_count = atoi(argv[2]);
//		const unsigned expander_buffer = atoi(argv[3]);
//		const int columns_num = atoi(argv[4]);
//		const unsigned lowers=atoi(argv[5]);
//		unsigned long random_size = atoi(argv[6]);	//number of columns which are randomly accessed
//		random_size = random_size*sizeof(int);
//		unsigned long scan_size = ((unsigned long)4*1024*1024*1024-random_size*columns_num)/lowers;
//
//		std::vector<std::string> upper_ip_list;
//		upper_ip_list.push_back("10.11.1.208");
//
//		std::vector<std::string> lower_ip_list;
//		lower_ip_list.push_back("10.11.1.201");
//		lower_ip_list.push_back("10.11.1.202");
//		lower_ip_list.push_back("10.11.1.203");
//		lower_ip_list.push_back("10.11.1.204");
//		lower_ip_list.push_back("10.11.1.205");
//		lower_ip_list.push_back("10.11.1.206");
//		lower_ip_list.push_back("10.11.1.207");
//		lower_ip_list.push_back("10.11.1.209");
//		lower_ip_list.push_back("10.11.1.211");
//		lower_ip_list.push_back("10.11.1.214");
//
//
//		std::vector<std::string> used_lowers;
//		for(unsigned i=0;i<lowers;i++){
////			used_lowers.push_back(lower_ip_list[atoi(argv[6+i])-1]);
//			used_lowers.push_back(lower_ip_list[i]);
//		}
//
//		std::vector<column_type> column_list,column_list_;
//		column_list.push_back(column_type(t_int));
//		for (int i = 0; i < columns_num; i++)
//			column_list_.push_back(column_type(t_int));
//
//		Schema* i_schema = new SchemaFix(column_list);
//		Schema* d_schema = new SchemaFix(column_list_);
//
//		//Scanning Data
//		ExpandableBlockStreamSingleColumnScan::State ebssc_state("/home/claims/temp/3_0.column",d_schema,block_size, scan_size);
//		BlockStreamIteratorBase* ebssc=new ExpandableBlockStreamSingleColumnScan(ebssc_state);
//
//		BlockStreamExpander::State bse_state(d_schema,ebssc,thread_count,block_size,expander_buffer);
//		BlockStreamIteratorBase* bse=new BlockStreamExpander(bse_state);
//
//		const unsigned long long int exchange_id=0;
//		ExpandableBlockStreamExchangeEpoll::State ebse_state(d_schema,bse,block_size,used_lowers,upper_ip_list,exchange_id);
//		BlockStreamIteratorBase* ebse=new ExpandableBlockStreamExchangeEpoll(ebse_state);
//
//
//
//		//Random Accessing Data
//		ExpandableBlockStreamSingleColumnScan::State ebssc_state1("/home/claims/temp/6_0.column",i_schema,block_size, random_size);
//		BlockStreamIteratorBase* ebssc1=new ExpandableBlockStreamSingleColumnScan(ebssc_state1);
//
////		BlockStreamExpander::State bse_state1(i_schema,ebssc1,thread_count,block_size,expander_buffer);
////		BlockStreamIteratorBase* bse1=new BlockStreamExpander(bse_state1);
//
//		const unsigned long long int exchange_id1=1;
//		ExpandableBlockStreamExchangeEpoll::State ebse_state1(i_schema,ebssc1,block_size,upper_ip_list,used_lowers,exchange_id1);
//		BlockStreamIteratorBase* ebse1=new ExpandableBlockStreamExchangeEpoll(ebse_state1);
//
//		ExpandableBlockStreamRandomMemAccess::State ebsrma_state("/home/claims/temp/5_0.column", ebse1, d_schema, i_schema, block_size);
//		BlockStreamIteratorBase* ebsrma = new ExpandableBlockStreamRandomMemAccess(ebsrma_state);
//
//		BlockStreamExpander::State bse_state2(d_schema,ebsrma,thread_count,block_size,expander_buffer);
//		BlockStreamIteratorBase* bse2=new BlockStreamExpander(bse_state2);
//
//		const unsigned long long int exchange_id2=2;
//		ExpandableBlockStreamExchangeEpoll::State ebse_state2(d_schema,bse2,block_size,used_lowers,upper_ip_list,exchange_id2);
//		BlockStreamIteratorBase* ebse2=new ExpandableBlockStreamExchangeEpoll(ebse_state2);
//
//
//
//		//Performance Test Outputting
//		BlockStreamPerformanceTest::State bspt_state(d_schema, ebse, ebse2, block_size, 1000);
//		BlockStreamIteratorBase* bspt = new BlockStreamPerformanceTest(bspt_state);
//
//		volatile int choice;
//
//		printf("Continue(1) or Not(0) ?\n");
//		scanf("%d",&choice);
//		while(choice==1){
//			bspt->open();
//			while(bspt->next(0)){
//			}
//			bspt->close();
//			printf("Continue(0) or Not(1) ?\n");
//			scanf("%d",&choice);
//			printf("you input %d\n",choice);
//		}
//	}
//	else{
//		Environment::getInstance(false);
//	}
//
//	printf("Waiting~~~~~....\n");
//	while(true){
//		sleep(1);
//	}
//	return 0;
//}





