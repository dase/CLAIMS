#include "tpc_h_test.cpp"
/*
 * in_segment_scalability_test.cpp
 *
 *  Created on: Apr 5, 2014
 *      Author: wangli
 */

#include "tpc_h_test.cpp"
#include "../../Config.h"
#include "../set_up_environment.h"
#include "../../common/AttributeComparator.h"
#include "../common/TestNew.cpp"

typedef double QueryTime;
typedef QueryTime (*query_function)();


static double lineitem_scan_self_join(){
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("LINEITEM");
	TableDescriptor* table_right=Environment::getInstance()->getCatalog()->getTable("LINEITEM");

	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));

	LogicalOperator* scan_right=new LogicalScan(table_right->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	filter_condition_1.add(table->getAttribute("row_id"),AttributeComparator::EQ,std::string("0"));
	LogicalOperator* filter=new LogicalFilter(filter_condition_1,scan);


	std::vector<LogicalEqualJoin::JoinPair> s_ps_join_condition;
//	s_ps_join_condition.push_back(EqualJoin::JoinPair(table->getAttribute("L_PARTKEY"),table_right->getAttribute("N_NATIONKEY")));
	s_ps_join_condition.push_back(LogicalEqualJoin::JoinPair(table->getAttribute("L_ORDERKEY"),table_right->getAttribute("L_ORDERKEY")));
//	s_ps_join_condition.push_back(EqualJoin::JoinPair(table->getAttribute("L_PARTKEY"),table->getAttribute("L_SUPPKEY")));
//	s_ps_join_condition.push_back(EqualJoin::JoinPair(table->getAttribute("L_PARTKEY"),table->getAttribute("L_SUPPKEY")));
	LogicalOperator* s_ps_join=new LogicalEqualJoin(s_ps_join_condition,scan,scan_right);

	LogicalOperator* root=new LogicalQueryPlanRoot(0,s_ps_join,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* physical_iterator_tree=root->GetPhysicalPlan(64*1024);
//	root->print();
//	physical_iterator_tree->print();
	physical_iterator_tree->Open();
	while(physical_iterator_tree->Next(0));
	physical_iterator_tree->Close();

	ResultSet* result_set=physical_iterator_tree->GetResultSet();
	double ret=result_set->query_time_;


	physical_iterator_tree->~PhysicalOperatorBase();
	root->~LogicalOperator();
	return ret;
}

static double sb_scan_self_join(){
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("cj");
	TableDescriptor* table_right=Environment::getInstance()->getCatalog()->getTable("sb");

	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));

	LogicalOperator* scan_right=new LogicalScan(table_right->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	filter_condition_1.add(table->getAttribute("row_id"),AttributeComparator::EQ,std::string("0"));
	LogicalOperator* filter=new LogicalFilter(filter_condition_1,scan);


	std::vector<LogicalEqualJoin::JoinPair> s_ps_join_condition;
	s_ps_join_condition.push_back(LogicalEqualJoin::JoinPair(table->getAttribute("order_no"),table_right->getAttribute("order_no")));
	s_ps_join_condition.push_back(LogicalEqualJoin::JoinPair(table->getAttribute("trade_dir"),table_right->getAttribute("entry_dir")));
	s_ps_join_condition.push_back(LogicalEqualJoin::JoinPair(table->getAttribute("trade_date"),table_right->getAttribute("entry_date")));
	LogicalOperator* s_ps_join=new LogicalEqualJoin(s_ps_join_condition,scan,scan_right);

	LogicalOperator* root=new LogicalQueryPlanRoot(0,s_ps_join,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* physical_iterator_tree=root->GetPhysicalPlan(64*1024);
//	root->print();
//	physical_iterator_tree->print();
	physical_iterator_tree->Open();
	while(physical_iterator_tree->Next(0));
	physical_iterator_tree->Close();

	ResultSet* result_set=physical_iterator_tree->GetResultSet();
	double ret=result_set->query_time_;


	physical_iterator_tree->~PhysicalOperatorBase();
	root->~LogicalOperator();
	return ret;
}

static double lineitem_scan_aggregation(){
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("LINEITEM");

	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));

	std::vector<Attribute> group_by_attributes;
//	group_by_attributes.push_back(table->getAttribute("L_RETURNFLAG"));
//	group_by_attributes.push_back(table->getAttribute("L_LINESTATUS"));
	group_by_attributes.push_back(table->getAttribute("L_COMMITDATE"));
	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(table->getAttribute("L_QUANTITY"));
	aggregation_attributes.push_back(table->getAttribute("L_EXTENDEDPRICE"));
	aggregation_attributes.push_back(table->getAttribute("L_DISCOUNT"));
	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
	std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;

	aggregation_function.push_back(PhysicalAggregation::State::kSum);
	aggregation_function.push_back(PhysicalAggregation::State::kSum);
	aggregation_function.push_back(PhysicalAggregation::State::kSum);
	aggregation_function.push_back(PhysicalAggregation::State::kCount);
	LogicalOperator* aggregation=new LogicalAggregation(group_by_attributes,aggregation_attributes,aggregation_function,scan);




	LogicalOperator* root=new LogicalQueryPlanRoot(0,aggregation,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* physical_iterator_tree=root->GetPhysicalPlan(64*1024);
//	root->print();
//	physical_iterator_tree->print();
	physical_iterator_tree->Open();
	while(physical_iterator_tree->Next(0));
	physical_iterator_tree->Close();

	ResultSet* result_set=physical_iterator_tree->GetResultSet();
	printf("tuples %d\n",result_set->getNumberOftuples());
	double ret=result_set->query_time_;

	physical_iterator_tree->~PhysicalOperatorBase();
	root->~LogicalOperator();
	return ret;
}

static double lineitem_scan_filter(){
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("LINEITEM");
//	printf("Tuple size:%d\n",table->getProjectoin(0)->getSchema()->getTupleMaxSize());
	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));


	LogicalFilter::Condition filter_condition_1;
	filter_condition_1.add(table->getAttribute("row_id"),AttributeComparator::EQ,std::string("0"));
	LogicalOperator* filter=new LogicalFilter(filter_condition_1,scan);



	LogicalOperator* root=new LogicalQueryPlanRoot(0,filter,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* physical_iterator_tree=root->GetPhysicalPlan(64*1024);
//	root->print();
//	physical_iterator_tree->print();
	physical_iterator_tree->Open();
	while(physical_iterator_tree->Next(0));
	physical_iterator_tree->Close();

	ResultSet* result_set=physical_iterator_tree->GetResultSet();
	double ret=result_set->query_time_;

	physical_iterator_tree->~PhysicalOperatorBase();
	root->~LogicalOperator();
	return ret;
}

static double sb_scan_filter(){
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("sb");

	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));
//	printf("Tuple size:%d\n",table->getProjectoin(0)->getSchema()->getTupleMaxSize());

	LogicalFilter::Condition filter_condition_1;
	filter_condition_1.add(table->getAttribute("row_id"),AttributeComparator::EQ,std::string("0"));
//	filter_condition_1.add(table->getAttribute("entry_dir"),AttributeComparator::EQ,std::string("1"));
//	const int trade_date=20101008;
//	filter_condition_1.add(table->getAttribute("entry_date"),AttributeComparator::GEQ,std::string("20101008"));
//	const int sec_code=600036;
//	filter_condition_1.add(table->getAttribute("sec_code"),AttributeComparator::EQ,std::string("600036"));
	LogicalOperator* filter=new LogicalFilter(filter_condition_1,scan);



	LogicalOperator* root=new LogicalQueryPlanRoot(0,filter,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* physical_iterator_tree=root->GetPhysicalPlan(64*1024);
//	root->print();
//	physical_iterator_tree->print();
	physical_iterator_tree->Open();
	while(physical_iterator_tree->Next(0));
	physical_iterator_tree->Close();

	ResultSet* result_set=physical_iterator_tree->GetResultSet();
	double ret=result_set->query_time_;

	physical_iterator_tree->~PhysicalOperatorBase();
	root->~LogicalOperator();
	return ret;
}
static double sb_scan_aggregation(){
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("sb");

	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));

	std::vector<Attribute> group_by_attributes;
//	group_by_attributes.push_back(table->getAttribute("L_RETURNFLAG"));
//	group_by_attributes.push_back(table->getAttribute("L_LINESTATUS"));
	group_by_attributes.push_back(table->getAttribute("sec_code"));
	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(table->getAttribute("row_id"));
	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
	std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;

	aggregation_function.push_back(PhysicalAggregation::State::kSum);
	aggregation_function.push_back(PhysicalAggregation::State::kCount);
	LogicalOperator* aggregation=new LogicalAggregation(group_by_attributes,aggregation_attributes,aggregation_function,scan);




	LogicalOperator* root=new LogicalQueryPlanRoot(0,aggregation,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* physical_iterator_tree=root->GetPhysicalPlan(64*1024);
//	root->print();
//	physical_iterator_tree->print();
	physical_iterator_tree->Open();
	while(physical_iterator_tree->Next(0));
	physical_iterator_tree->Close();

	ResultSet* result_set=physical_iterator_tree->GetResultSet();
	printf("tuples %d\n",result_set->getNumberOftuples());
	double ret=result_set->query_time_;

	physical_iterator_tree->~PhysicalOperatorBase();
	root->~LogicalOperator();
	return ret;
}


static void scalability_test(query_function qf,const char* test_name,int max_test_degree_of_parallelism=4){
	/*disable expander adaptivity*/
	Config::enable_expander_adaptivity=false;
	unsigned repeated_times=3;
	double standard_throughput=0;
	/* warm up the memory*/
	qf();

	printf("_______Test Scalability for %s ___________\n",test_name);

	for(unsigned i=1;i<=max_test_degree_of_parallelism;i++){
		Config::initial_degree_of_parallelism=i;
		double total_time=0;
		for(unsigned j=0;j<repeated_times;j++){
//			printf("--------------%d---------------\n",j);
			total_time+=qf();
//			sleep(3);
		}


		if(i==1){
			standard_throughput=1/(total_time/repeated_times);
			printf("D=%d\ts=%4.4f scale:1\n",i,total_time/repeated_times,1);
		}
		else{
			printf("D=%d\ts=%4.4f scale:%f\n",i,total_time/repeated_times,1/(total_time/repeated_times)/standard_throughput);
		}


	}


}

static int in_segment_scalability_test_on_tpch(int repeated_times=10){
	startup_single_node_one_partition_environment_of_tpch();
	double total_time=0;

//	scalability_test(lineitem_scan_filter,"Scan-->filter",Config::max_degree_of_parallelism);
//	scalability_test(lineitem_scan_aggregation,"Scan-->aggregation",Config::max_degree_of_parallelism);
	scalability_test(lineitem_scan_self_join,"Scan-->join",Config::max_degree_of_parallelism);

	sleep(1);
	Environment::getInstance()->~Environment();
//	printf("hash table =%d\n",BasicHashTable::getNumberOfInstances());
	sleep(100);
}

static int in_segment_scalability_test_on_poc(int repeated_times=10){
	printf("-----poc-----\n");
	startup_single_node_environment_of_poc();
	double total_time=0;

	scalability_test(sb_scan_filter,"Scan-->filter",Config::max_degree_of_parallelism);
	scalability_test(sb_scan_aggregation,"Scan-->aggregation",Config::max_degree_of_parallelism);
	scalability_test(sb_scan_self_join,"Scan-->join",Config::max_degree_of_parallelism);

	sleep(1);
	Environment::getInstance()->~Environment();
//	printf("hash table =%d\n",BasicHashTable::getNumberOfInstances());
	sleep(100);
}

static void test_block_construct(){
	startup_single_node_environment_of_poc();
	sleep(1);
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("sb");

	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));
	scan->GetPlanContext();
	PhysicalOperatorBase* s=scan->GetPhysicalPlan(64*1024);
	s->Print();

	std::vector<BlockStreamBase*> vect;
	BlockStreamBase* block=BlockStreamBase::createBlock(table->getProjectoin(0)->getSchema(),64*1024);
	s->Open();
	while(s->Next(block));
	s->Close();
	s->Open();
	unsigned long long int start=curtick();
	while(s->Next(block));
	printf("time: %lf \n",getSecond(start));
	s->Close();






}

static int in_segment_scalability_test(int repeated_times=10){
//	in_segment_scalability_test_on_tpch(repeated_times);
	in_segment_scalability_test_on_poc(repeated_times);
//	test_block_construct();
	return 0;
}
