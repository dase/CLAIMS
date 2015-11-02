/*
 * test_IndexManager_serialize.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: scdong
 */

#include "../../Environment.h"

#include "../../Catalog/Catalog.h"
#include "../../Catalog/table.h"

#include "../../common/ids.h"
#include "../../common/AttributeComparator.h"

#include "../LogicalCSBIndexBuilding.h"
#include "../IndexScanIterator.h"
#include "../LogicalIndexScan.h"
#include "../CSBPlusTree.h"

#include "../../utility/test_tool.h"
#include "../../utility/rdtsc.h"

#include "../../logical_operator/logical_scan.h"
#include "../../logical_operator/Filter.h"
#include "../../logical_operator/LogicalQueryPlanRoot.h"
#include "../../logical_operator/logical_operator.h"

#include "../../BlockStreamIterator/BlockStreamPerformanceMonitorTop.h"

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
<<<<<<< HEAD
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,csb_building,LogicalQueryPlanRoot::RESULTCOLLECTOR);
	root->Print();
=======
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,csb_building,LogicalQueryPlanRoot::kResultCollector);
	root->Print();
>>>>>>> master-yk-150927
	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64);
	executable_query_plan->Open();
	while (executable_query_plan->Next(0));
	executable_query_plan->Close();

//	ResultSet* result_set = executable_query_plan->getResultSet();

	executable_query_plan->~PhysicalOperatorBase();
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

static void test_logical_index_scan()
{
	vector<IndexScanIterator::query_range> q_range;
	TableDescriptor* table = Catalog::getInstance()->getTable("cj");
	IndexScanIterator::query_range q;
	int value = 0;
	while (true)
	{
		q_range.clear();
		cout << "Input the search sec_code: ";
		cin >> value;

		q.value_low = malloc(sizeof(int));		//newmalloc
		q.value_low = (void*)(&value);
		q.comp_low = EQ;
		q.value_high = malloc(sizeof(int));		//newmalloc
		q.value_high = (void*) (&value);
		q.comp_high = EQ;
		q.c_type.type = t_int;
		q.c_type.operate = new OperateInt();
		q_range.push_back(q);

		LogicalOperator* index_scan = new LogicalIndexScan(table->getProjectoin(0)->getProjectionID(), table->getAttribute("sec_code"), q_range);
		const NodeID collector_node_id = 0;
<<<<<<< HEAD
		LogicalOperator* root = new LogicalQueryPlanRoot(collector_node_id, index_scan, LogicalQueryPlanRoot::PRINT);
=======
		LogicalOperator* root = new LogicalQueryPlanRoot(collector_node_id, index_scan, LogicalQueryPlanRoot::kPrint);
>>>>>>> master-yk-150927
		PhysicalOperatorBase* executable_query_plan = root->GetPhysicalPlan(1024 * 64);
		executable_query_plan->Open();
		while (executable_query_plan->Next(0));
		executable_query_plan->Close();
		executable_query_plan->~PhysicalOperatorBase();
		root->~LogicalOperator();
	}
}

static void bulk_test_logical_index_scan()
{
	vector<IndexScanIterator::query_range> q_range;
	int count = 1022;
	ifstream infile("/home/scdong/code/sec_code", ios::in);
	ofstream outfile("/home/scdong/code/fail_log.dat", ios::out);
	unsigned long int value = 0;
	unsigned long int expect_num;
	TableDescriptor* table = Catalog::getInstance()->getTable("cj");
	IndexScanIterator::query_range q2;
	while (count > 0)
	{
		q_range.clear();
		infile >> value >> expect_num;

		q2.value_low = malloc(sizeof(int));		//newmalloc
		q2.value_low = (void*)(&value);
		q2.comp_low = EQ;
		q2.value_high = malloc(sizeof(int));		//newmalloc
		q2.value_high = (void*) (&value);
		q2.comp_high = EQ;
		q2.c_type.type = t_int;
		q2.c_type.operate = new OperateInt();
		q_range.push_back(q2);

		LogicalOperator* index_scan = new LogicalIndexScan(table->getProjectoin(0)->getProjectionID(), table->getAttribute("sec_code"), q_range);
		const NodeID collector_node_id = 0;
<<<<<<< HEAD
		LogicalOperator* root = new LogicalQueryPlanRoot(collector_node_id, index_scan, LogicalQueryPlanRoot::RESULTCOLLECTOR);
=======
		LogicalOperator* root = new LogicalQueryPlanRoot(collector_node_id, index_scan, LogicalQueryPlanRoot::kResultCollector);
>>>>>>> master-yk-150927
		PhysicalOperatorBase* executable_query_plan = root->GetPhysicalPlan(1024 * 64);
		executable_query_plan->Open();
		while (executable_query_plan->Next(0));
		executable_query_plan->Close();

		ResultSet* result_set = executable_query_plan->getResultSet();

		const unsigned long int number_of_tuples = result_set->getNumberOftuples();
		executable_query_plan->~PhysicalOperatorBase();
		root->~LogicalOperator();
		cout << 1022-count << ": Sec_code: " << value << "\t Result: " << number_of_tuples << endl;
		if(!print_test_name_result(number_of_tuples == expect_num,"Index Scan")){
			printf("\tIndex Scan sec_code = %d, Expected:%d actual: %d\n", value, expect_num, number_of_tuples);
			outfile << "Index Scan sec_code = " << value << "\tFAIL!\n";
			outfile << "\tExcepted: " << expect_num << "\tActual: " << number_of_tuples << endl;
		}
		count--;
	}
}

static void test_scan_filter_performance(int value)
{
	unsigned long long int start=curtick();
	TableDescriptor* table=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_scan=new LogicalScan(table->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	filter_condition_1.add(table->getAttribute(3),AttributeComparator::GEQ,std::string("10107"));
	filter_condition_1.add(table->getAttribute(3),AttributeComparator::L,(void*)&value);
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_scan);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,filter_1,LogicalQueryPlanRoot::PERFORMANCE);

	PerformanceMonitor* executable_query_plan=(PerformanceMonitor*)root->GetPhysicalPlan(1024*64);
//	executable_query_plan->print();
	executable_query_plan->Open();
	while(executable_query_plan->Next(0));
	executable_query_plan->Close();

//	ResultSet *result_set=executable_query_plan->getResultSet();

	const unsigned long int number_of_tuples=executable_query_plan->GetNumberOfTuples();
	printf("execution time: %4.4f seconds.\n",getSecond(start));
	if(!print_test_name_result(number_of_tuples==26820,"Low selectivity filter")){
		printf("\tExpected:26695 actual: %d\n",number_of_tuples);
	}
//	result_set->~ResultSet();
	delete executable_query_plan;
	root->~LogicalOperator();
}

static void test_index_filter_performance(int value_high)
{
	unsigned long long int start=curtick();
	vector<IndexScanIterator::query_range> q_range;
	q_range.clear();
	int value_low = 10107;
//	int value_high = 600257;
	TableDescriptor* table = Catalog::getInstance()->getTable("cj");

	IndexScanIterator::query_range q;
	q.value_low = malloc(sizeof(int));		//newmalloc
	q.value_low = (void*)(&value_low);
	q.comp_low = GEQ;
	q.value_high = malloc(sizeof(int));		//newmalloc
	q.value_high = (void*) (&value_high);
	q.comp_high = L;
	q.c_type.type = t_int;
	q.c_type.operate = new OperateInt();
	q_range.push_back(q);

	LogicalOperator* index_scan = new LogicalIndexScan(table->getProjectoin(0)->getProjectionID(), table->getAttribute("sec_code"), q_range);
	const NodeID collector_node_id = 0;
	LogicalOperator* root = new LogicalQueryPlanRoot(collector_node_id, index_scan, LogicalQueryPlanRoot::PERFORMANCE);
//	root->print();
	PerformanceMonitor* executable_query_plan = (PerformanceMonitor*)root->GetPhysicalPlan(1024 * 64);
	executable_query_plan->Open();
	while (executable_query_plan->Next(0));
	executable_query_plan->Close();

//	ResultSet* result_set = executable_query_plan->getResultSet();

	const unsigned long int number_of_tuples = executable_query_plan->GetNumberOfTuples();
	delete executable_query_plan;
	root->~LogicalOperator();
//	cout << "Sec_code: " << value_low << "\t Result: " << number_of_tuples << endl;
	printf("execution time: %4.4f seconds.\n",getSecond(start));
	if(!print_test_name_result(number_of_tuples == 26820,"Index Scan")){
		printf("\tIndex Scan sec_code = %d, Expected:%d actual: %d\n", value_low, 26820, number_of_tuples);
	}
}

// For testing
static void init_tt_environment()
{
	Environment::getInstance(true);
	sleep(1);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();

	// num int not null, s int, ss int primary key, c char, vc varchar(10)
	TableDescriptor* table_1=new TableDescriptor("tt",2);
	table_1->addAttribute("row_id",data_type(t_u_long));  				//0
	table_1->addAttribute("num",data_type(t_int));
	table_1->addAttribute("s",data_type(t_int));
	table_1->addAttribute("ss",data_type(t_int));
	table_1->addAttribute("c",data_type(t_string), 4);
	table_1->addAttribute("vc",data_type(t_string), 10);				//5

	vector<ColumnOffset> proj_0;
	proj_0.push_back(0);
	proj_0.push_back(1);
	proj_0.push_back(2);
	proj_0.push_back(3);
	proj_0.push_back(4);
	proj_0.push_back(5);
	const int partition_key_index_1=1;
	table_1->createHashPartitionedProjection(proj_0,"num",1);	//G0

	catalog->add_table(table_1);

	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(2)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
}

//static void test_print_tt()
//{
//	TableDescriptor* table = Catalog::getInstance()->getTable("tt");
//	LogicalOperator* scan = new LogicalScan(table->getProjectoin(0));
//
//	const NodeID collector_node_id = 0;
//	LogicalOperator* root = new LogicalQueryPlanRoot(collector_node_id, scan, LogicalQueryPlanRoot::PRINT);
//	BlockStreamPerformanceMonitorTop* executable_query_plan = (BlockStreamPerformanceMonitorTop*)root->getIteratorTree(1024 * 64);
//	executable_query_plan->open();
//	while (executable_query_plan->next(0));
//	executable_query_plan->close();
//}


static int test_index_manager()
{
	init_poc_environment();

	test_logical_index_building();
	test_serialize();
	test_deserialize();
	test_logical_index_scan();
	bulk_test_logical_index_scan();
	int tmp = 0;
	while (true)
	{
	cout << "Input sec_code: ";
	cin >> tmp;
	test_scan_filter_performance(tmp);
	test_index_filter_performance(tmp);
	}
	Environment::getInstance()->~Environment();



//// For testing
//	init_tt_environment();
//	test_print_tt();

	return 0;
}
