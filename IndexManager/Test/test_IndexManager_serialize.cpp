/*
 * test_IndexManager_serialize.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: scdong
 */

#include "../../Environment.h"
#include "../../Catalog/Catalog.h"
#include "../../Catalog/table.h"
#include "../../ids.h"
#include "../../LogicalQueryPlan/LogicalOperator.h"
#include "../LogicalCSBIndexBuilding.h"
#include "../../LogicalQueryPlan/LogicalQueryPlanRoot.h"

static void init_poc_environment()
{
	Environment::getInstance(true);
	sleep(1);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();

	TableDescriptor* table_1=new TableDescriptor("cj",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	table_1->addAttribute("row_id",data_type(t_u_long));  				//0
	table_1->addAttribute("trade_date",data_type(t_int));
	table_1->addAttribute("order_no",data_type(t_u_long));
	table_1->addAttribute("sec_code",data_type(t_int));
	table_1->addAttribute("trade_dir",data_type(t_int));
	table_1->addAttribute("order_type",data_type(t_int));				//5

	vector<ColumnOffset> cj_proj0_index;
	cj_proj0_index.push_back(0);
	cj_proj0_index.push_back(1);
	cj_proj0_index.push_back(2);
	cj_proj0_index.push_back(3);
	cj_proj0_index.push_back(4);
	cj_proj0_index.push_back(5);
	const int partition_key_index_1=2;
	table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",1);	//G0

	catalog->add_table(table_1);

	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
	}
}

static void test_logical_index_building()
{
	TableDescriptor* table = Catalog::getInstance()->getTable("cj");
	LogicalOperator* csb_building = new LogicalCSBIndexBuilding(table->getProjectoin(0)->getProjectionID(), table->getAttribute(3), "sec_code_index");
	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,csb_building,LogicalQueryPlanRoot::RESULTCOLLECTOR);
	root->print();
	BlockStreamIteratorBase* executable_query_plan=root->getIteratorTree(1024*64);
	executable_query_plan->open();
	while (executable_query_plan->next(0));
	executable_query_plan->close();

//	ResultSet* result_set = executable_query_plan->getResultSet();

	executable_query_plan->~BlockStreamIteratorBase();
	root->~LogicalOperator();
	cout << "index building finished!\n";
}

static void test_serialize()
{
	IndexManager::getInstance()->serialize("/home/scdong/code/index.dat");
}

static int test_index_manager()
{
	init_poc_environment();

	test_logical_index_building();
	test_serialize();

	Environment::getInstance()->~Environment();
	return 0;
}
