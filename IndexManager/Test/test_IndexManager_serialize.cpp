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
#include "../IndexScanIterator.h"
#include "../LogicalIndexScan.h"
#include "../../utility/test_tool.h"

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

static void test_deserialize()
{
	IndexManager::getInstance()->deserialize("/home/scdong/code/index.dat");
}

static unsigned long int test_logical_index_scan(int value)
{
	vector<IndexScanIterator::query_range> q_range;
	q_range.clear();
	int value_low = 10110;
	int value_high = 10112;

//	IndexScanIterator::query_range q1;
//	q1.value_low = malloc(sizeof(int));
//	q1.value_low = (void*)(&value_low);
//	q1.comp_low = FilterIterator::AttributeComparator::GEQ;
//	q1.value_high = malloc(sizeof(int));
//	q1.value_high = (void*)(&value_high);
//	q1.comp_high = FilterIterator::AttributeComparator::LEQ;
//	q1.c_type.type = t_int;
//	q1.c_type.operate = new OperateInt();
//	q1.valuebytes_low.clear();
//	q1.valuebytes_high.clear();
//	q_range.push_back(q1);

	IndexScanIterator::query_range q2;
	q2.value_low = malloc(sizeof(int));
	q2.value_low = (void*)(&value);
	q2.comp_low = FilterIterator::AttributeComparator::EQ;
	q2.value_high = malloc(sizeof(int));
	q2.value_high = (void*)(&value);
	q2.comp_high = FilterIterator::AttributeComparator::EQ;
	q2.c_type.type = t_int;
	q2.c_type.operate = new OperateInt();
	q_range.push_back(q2);

	TableDescriptor* table = Catalog::getInstance()->getTable("cj");
	LogicalOperator* index_scan = new LogicalIndexScan(table->getProjectoin(0)->getProjectionID(), table->getAttribute("sec_code"), q_range);
	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,index_scan,LogicalQueryPlanRoot::RESULTCOLLECTOR);
	root->print();
	BlockStreamIteratorBase* executable_query_plan=root->getIteratorTree(1024*64);
	executable_query_plan->open();
	while (executable_query_plan->next(0));
	executable_query_plan->close();

	ResultSet* result_set = executable_query_plan->getResultSet();

//	if(!print_test_name_result(number_of_tuples==3966020,"Scan")){
//		printf("\tExpected:3966020 actual: %d\n",number_of_tuples);
//	}
//	cout << "Sec_Code = " << value << "\tTotal tuples: " << result_set->getNumberOftuples() << endl;

	const unsigned long int number_of_tuples = result_set->getNumberOftuples();
	executable_query_plan->~BlockStreamIteratorBase();
	root->~LogicalOperator();
	return number_of_tuples;
}

static int test_index_manager()
{
	init_poc_environment();

//	test_logical_index_building();
//	test_serialize();
//
	test_deserialize();

	int count = 1022;
	ifstream infile("/home/scdong/code/sec_code", ios::in);
	ofstream outfile("/home/scdong/code/fail_log.dat", ios::out);
	unsigned long int value = 0;
	unsigned long int ret_num;
	unsigned long int ecpect_num;
	while (count > 0)
	{
		infile >> value;
		ret_num = test_logical_index_scan(value);

		infile >> ecpect_num;
		cout << "Sec_code: " << value << "\t Result: " << ret_num << endl;
		if(!print_test_name_result(ret_num == ecpect_num,"Index Scan")){
			printf("\tIndex Scan sec_code = %d, Expected:%d actual: %d\n", value, ecpect_num, ret_num);
			outfile << "Index Scan sec_code = " << value << "\tFAIL!\n";
			outfile << "\tExcepted: " << ecpect_num << "\tActual: " << ret_num << endl;
		}
		count--;
//		sleep(1);
	}

	Environment::getInstance()->~Environment();
	return 0;
}
