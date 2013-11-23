/*
 * testGenerateIteratorTree.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: wangli
 */
#ifndef __TESTGENERATEITERATORTREE__
#define __TESTGENERATEITERATORTREE__
#include <vector>
#include <iostream>
#include "../../Environment.h"
#include "../Scan.h"
#include "../LogicalQueryPlanRoot.h"
#include "../EqualJoin.h"
#include "../../Catalog/ProjectionBinding.h"
#include "../Filter.h"
using namespace std;
static int testGenerateIteratorTree(){
	int master;
	cout<<"Master(0) or Slave(others)"<<endl;
	cin>>master;
	if(master!=0){
		Environment::getInstance(false);
	}
	else{

		Environment::getInstance(true);

		ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
		Catalog* catalog=Environment::getInstance()->getCatalog();

		TableDescriptor* table_1=new TableDescriptor("Left",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_1->addAttribute("row_id",data_type(t_u_long));  				//0
		table_1->addAttribute("trade_date",data_type(t_int));
		table_1->addAttribute("order_no",data_type(t_u_long));
		table_1->addAttribute("sec_code",data_type(t_int));
		table_1->addAttribute("trade_dir",data_type(t_int));
		table_1->addAttribute("order_type",data_type(t_int));				//5
		table_1->addAttribute("trade_no",data_type(t_int));
		table_1->addAttribute("trade_time",data_type(t_int));
		table_1->addAttribute("trade_time_dec",data_type(t_u_long));
		table_1->addAttribute("order_time",data_type(t_int));
		table_1->addAttribute("order_time_dec",data_type(t_u_long));		//10
		table_1->addAttribute("trade_price",data_type(t_double));
		table_1->addAttribute("trade_amt",data_type(t_double));
		table_1->addAttribute("trade_vol",data_type(t_double));
		table_1->addAttribute("pbu_id",data_type(t_int));
		table_1->addAttribute("acct_id",data_type(t_int));					//15
		table_1->addAttribute("order_prtfil_code",data_type(t_int));
		table_1->addAttribute("tran_type",data_type(t_int));
		table_1->addAttribute("trade_type",data_type(t_int));
		table_1->addAttribute("proc_type",data_type(t_int));

		vector<ColumnOffset> cj_proj0_index;
		cj_proj0_index.push_back(0);
		cj_proj0_index.push_back(1);
		cj_proj0_index.push_back(2);
		cj_proj0_index.push_back(3);
		cj_proj0_index.push_back(4);
		cj_proj0_index.push_back(5);
		const int partition_key_index_1=2;
		table_1->createHashPartitionedProjection(cj_proj0_index,partition_key_index_1,1);
		catalog->add_table(table_1);
		vector<ColumnOffset> cj_proj1_index;
		cj_proj1_index.push_back(0);
		cj_proj1_index.push_back(6);
		cj_proj1_index.push_back(7);
		cj_proj1_index.push_back(8);
		cj_proj1_index.push_back(9);
		cj_proj1_index.push_back(10);
		cj_proj1_index.push_back(11);
		cj_proj1_index.push_back(12);
		cj_proj1_index.push_back(13);
		cj_proj1_index.push_back(14);
		cj_proj1_index.push_back(15);
		cj_proj1_index.push_back(16);
		cj_proj1_index.push_back(17);
		cj_proj1_index.push_back(18);
		cj_proj1_index.push_back(18);
		const int proj1_partition_index=0;
		table_1->createHashPartitionedProjection(cj_proj1_index,proj1_partition_index,1);
		catalog->add_table(table_1);

		////////////////////////////////////Create table right//////////////////////////
		TableDescriptor* table_2=new TableDescriptor("right",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_2->addAttribute("row_id",data_type(t_u_long));
		table_2->addAttribute("order_no",data_type(t_u_long));
		table_2->addAttribute("entry_date",data_type(t_int));
		table_2->addAttribute("sec_code",data_type(t_int));
		table_2->addAttribute("order_type",data_type(t_int));
		table_2->addAttribute("entry_dir",data_type(t_int));

		vector<ColumnOffset> index_2;
		index_2.push_back(0);
		index_2.push_back(1);
		index_2.push_back(2);
		index_2.push_back(3);
		index_2.push_back(4);
		index_2.push_back(5);
		const int partition_key_index_2=1;
		table_2->createHashPartitionedProjection(index_2,partition_key_index_2,4);
		catalog->add_table(table_2);
		///////////////////////////////////////////////////////////








		///////////////////////////////////////


		////////////////////////////////////////
		/* the following codes should be triggered by Load module*/

		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}


		for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		////////////////////////////////////////



	//	ProjectionBinding *pb=new ProjectionBinding();
	//	pb->BindingEntireProjection(catalog->getTable(0)->getProjectoin(0)->getPartitioner(),MEMORY);
	//	pb->BindingEntireProjection(catalog->getTable(1)->getProjectoin(0)->getPartitioner());

	//	sleep(1);
		cout<<"ready(?)"<<endl;
		int input;
		cin>>input;


		const TableID table_id_1=0;
		LogicalOperator* scan_1=new LogicalScan(table_1->getProjectoin(0));


		const TableID table_id_2=1;
		LogicalOperator* scan_2=new LogicalScan(table_id_2);

		LogicalOperator* cj_payload_scan=new LogicalScan(table_1->getProjectoin(1));

		Filter::Condition filter_condition_1;
		const int trade_date=20101008;
		filter_condition_1.add(table_1->getAttribute(1),FilterIterator::AttributeComparator::EQ,&trade_date);
		const int sec_code=600036;
		filter_condition_1.add(table_1->getAttribute(3),FilterIterator::AttributeComparator::EQ,&sec_code);
		const int order_type=1;
		filter_condition_1.add(table_1->getAttribute(5),FilterIterator::AttributeComparator::EQ,&order_type);
		LogicalOperator* filter_1=new Filter(filter_condition_1,scan_1);

		Filter::Condition filter_condition_2;
		const int entry_date=20101008;
		filter_condition_2.add(table_2->getAttribute(2),FilterIterator::AttributeComparator::EQ,&entry_date);
		const int sec_code_=600036;
		filter_condition_2.add(table_2->getAttribute(3),FilterIterator::AttributeComparator::EQ,&sec_code_);
		const int order_type_=1;
		filter_condition_2.add(table_2->getAttribute(4),FilterIterator::AttributeComparator::EQ,&order_type_);
		LogicalOperator* filter_2=new Filter(filter_condition_2,scan_2);

		std::vector<EqualJoin::JoinPair> sb_cj_join_pair_list;
		sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute(2),table_2->getAttribute(1)));
		sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute(1),table_2->getAttribute(2)));
		sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute(4),table_2->getAttribute(5)));
		LogicalOperator* sb_cj_join=new EqualJoin(sb_cj_join_pair_list,filter_1,filter_2);

		std::vector<EqualJoin::JoinPair> cj_payload_join_pari_list;
		cj_payload_join_pari_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("row_id"),table_1->getAttribute("row_id")));
		LogicalOperator* cj_payload_join=new EqualJoin(cj_payload_join_pari_list,sb_cj_join,cj_payload_scan);



		const NodeID collector_node_id=0;
		LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,cj_payload_join,LogicalQueryPlanRoot::PERFORMANCE);
		root->getDataflow();
		BlockStreamIteratorBase* executable_query_plan=root->getIteratorTree(64*1024-sizeof(unsigned));

		int c=1;
		while(c!=0){
			IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(executable_query_plan,"10.11.1.208");
			cout<<"Terminal(0) or continue(others)?"<<endl;
			cin>>c;
		}
	}
	cout<<"Waiting~~~!~"<<endl;
	while(true){
		sleep(1);
	}

}

#endif
