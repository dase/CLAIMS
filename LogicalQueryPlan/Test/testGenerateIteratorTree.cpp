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
using namespace std;
static int testGenerateIteratorTree(){
	Environment::getInstance(true);

	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();

	TableDescriptor* table_1=new TableDescriptor("Left",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	table_1->addAttribute("A1",data_type(t_u_long),3);
	table_1->addAttribute("A2",data_type(t_int));
	table_1->addAttribute("A3",data_type(t_u_long));
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
	table_1->createHashPartitionedProjection(index_1,partition_key_index_1,2);
	catalog->add_table(table_1);

	////////////////////////////////////Create table right//////////////////////////
	TableDescriptor* table_2=new TableDescriptor("right",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	table_2->addAttribute("A1",data_type(t_u_long),3);
	table_2->addAttribute("A2",data_type(t_int));
	table_2->addAttribute("A3",data_type(t_u_long));
	table_2->addAttribute("A4",data_type(t_int));
	table_2->addAttribute("A5",data_type(t_int));
	table_2->addAttribute("A6",data_type(t_int));

	vector<ColumnOffset> index_2;
	index_2.push_back(0);
	index_2.push_back(1);
	index_2.push_back(2);
	index_2.push_back(3);
	index_2.push_back(4);
	index_2.push_back(5);
	const int partition_key_index_2=0;
	table_2->createHashPartitionedProjection(index_2,partition_key_index_2,2);
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



	ProjectionBinding *pb=new ProjectionBinding();
	pb->BindingEntireProjection(catalog->getTable(0)->getProjectoin(0)->getPartitioner(),MEMORY);
//	pb->BindingEntireProjection(catalog->getTable(1)->getProjectoin(0)->getPartitioner());

//	sleep(1);
	const TableID table_id_1=0;
	LogicalOperator* scan_1=new LogicalScan(table_id_1);

	const TableID table_id_2=1;
	LogicalOperator* scan_2=new LogicalScan(table_id_2);

	std::vector<EqualJoin::JoinPair> join_pair_list;
	join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute(0),table_2->getAttribute(0)));
	LogicalOperator* join=new EqualJoin(join_pair_list,scan_1,scan_2);



	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,join,LogicalQueryPlanRoot::PERFORMANCE);
	root->getDataflow();
	BlockStreamIteratorBase* executable_query_plan=root->getIteratorTree(64*1024-sizeof(unsigned));

	int c=1;
	while(c!=0){
		IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(executable_query_plan,"127.0.0.1");
		cout<<"Terminal(0) or continue(others)?"<<endl;
		cin>>c;
	}
	cout<<"Waiting~~~!~"<<endl;
	while(true){
		sleep(1);
	}

}

#endif
