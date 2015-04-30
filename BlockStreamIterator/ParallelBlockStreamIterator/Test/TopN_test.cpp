/*
 * test.cpp
 *
 *  Created on: Nov 18, 2013
 *      Author: wangli
 */
#ifndef __TEST_TopN__
#define __TEST_TopN__
#include <iostream>
#include "../ExpandableBlockStreamProjectionScan.h"
#include "../BlockStreamExpander.h"
#include "../ExpandableBlockStreamBuffer.h"

#include "../../BlockStreamPrint.h"
#include "../../BlockStreamPerformanceMonitorTop.h"
#include "../../BlockStreamLimit.h"

#include "../../../Environment.h"
#include "../../../Catalog/ProjectionBinding.h"

using namespace std;
static int test_TopN(){
	Environment::getInstance(true);

	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();

	TableDescriptor* table_1=new TableDescriptor("Left",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	table_1->addAttribute("A1",data_type(t_u_long),3);
	table_1->addAttribute("A2",data_type(t_int));
	table_1->addAttribute("A3",data_type(t_double));
	table_1->addAttribute("A4",data_type(t_int));
	table_1->addAttribute("A5",data_type(t_int));
	table_1->addAttribute("A6",data_type(t_int));
	vector<ColumnOffset> index_1;
	index_1.push_back(0);
	index_1.push_back(1);
	index_1.push_back(2);
	index_1.push_back(3);
	index_1.push_back(4);
	index_1.push_back(5);
	const int partition_key_index_1=0;
	table_1->createHashPartitionedProjection(index_1,partition_key_index_1,4);
	catalog->add_table(table_1);

	////////////////////////////////////Create table right//////////////////////////
//	TableDescriptor* table_2=new TableDescriptor("right",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
//	table_2->addAttribute("Name",data_type(t_string),10);
//	table_2->addAttribute("Age",data_type(t_int));
//	table_2->addAttribute("Gender",data_type(t_int));
//	table_2->addAttribute("Score",data_type(t_int));
//
//	vector<ColumnOffset> index_2;
//	index_2.push_back(0);
//	index_2.push_back(1);
//	index_2.push_back(3);
//	const int partition_key_index_2=3;
//	table_2->createHashPartitionedProjection(index_2,partition_key_index_2,3);
//	catalog->add_table(table_2);
	///////////////////////////////////////////////////////////








	///////////////////////////////////////


	////////////////////////////////////////
	/* the following codes should be triggered by Load module*/

	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
	}


//	for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){
//
//		catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,"Partition_"+i,4);
//	}

	////////////////////////////////////////



	ProjectionBinding *pb=new ProjectionBinding();
	pb->BindingEntireProjection(catalog->getTable(0)->getProjectoin(0)->getPartitioner(),MEMORY);
//	pb->BindingEntireProjection(catalog->getTable(1)->getProjectoin(0)->getPartitioner());



	/* build the iterator*/

	/////////////////////Scan///////////////////////////
	ExpandableBlockStreamProjectionScan::State scan_state;
	scan_state.block_size_=64*1024;
	scan_state.projection_id_=catalog->getTable(0)->getProjectoin(0)->getProjectionID();
	vector<column_type> column_list;
	column_list.push_back(column_type(t_u_long));
	column_list.push_back(column_type(t_int));
	column_list.push_back(column_type(t_u_long));
	column_list.push_back(column_type(t_int));
	column_list.push_back(column_type(t_int));
	column_list.push_back(column_type(t_int));

	scan_state.schema_=new SchemaFix(column_list);

	BlockStreamIteratorBase* scan=new ExpandableBlockStreamProjectionScan(scan_state);

	////////Buffer////////
	ExpandableBlockStreamBuffer::State b_state(scan_state.schema_,scan,scan_state.block_size_);
	BlockStreamIteratorBase* buffer=new ExpandableBlockStreamBuffer(b_state);

	//Expander//
	BlockStreamExpander::State exp_state(scan_state.schema_,scan,4,scan_state.block_size_);
	BlockStreamIteratorBase* expander=new BlockStreamExpander(exp_state);

	BlockStreamLimit::State top_state(scan_state.schema_,scan,-1,scan_state.block_size_,100000);
	BlockStreamIteratorBase* top=new BlockStreamLimit(top_state);
//
//	///Performance Monitor//
//	BlockStreamPerformanceMonitorTop::State p_state(scan_state.schema_,top,scan_state.block_size_);
//	BlockStreamIteratorBase* perf_top=new BlockStreamPerformanceMonitorTop(p_state);

//	/////////////////Print/////////////////////////////
	BlockStreamPrint::State print_state;
	print_state.block_size_=64*1024;
	print_state.child_=top;
	print_state.schema_=scan_state.schema_;
	print_state.spliter_="-|-";

	BlockStreamIteratorBase* print=new BlockStreamPrint(print_state);
	int input=1;
	while(input>0){
		IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(print,0);
		printf("continue or not?\n");
		scanf("%d",&input);
	}


	////////Run////////////
//	sleep(1);
//	BlockStreamBase* block=BlockStreamBase::createBlock(scan_state.schema_,64*1024-sizeof(unsigned));
//	block->setEmpty();
//	int c=1;
//	while(c==1){
//		unsigned tuple_count=0;
//
//		if(!print->open(0)){
//			printf("open failed!!!\n");
//		}
//		while(print->next(block)){
////			BlockStreamBase::BlockStreamTraverseIterator* it=block->createIterator();
////			void* tuple;
////			while(tuple=it->nextTuple()){
////				tuple_count++;
////				scan_state.schema_->displayTuple(tuple," | ");
////			}
////			block->setEmpty();
//		}
//		print->close();
//		cout<<"Total "<<tuple_count<<"tuples"<<endl;
//		cout<<"Continue(1) or not (otherwise)?"<<endl;
//		cin>>c;
//	}
//////////////////////////////////////////////////////

	cout<<"Wait~~~"<<endl;
	while(true){
		sleep(1);
	}
}
#endif
