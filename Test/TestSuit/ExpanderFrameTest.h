#include "../../utility/test_tool.h"
#include "../../common/Block/ResultSet.h"
#include "../set_up_environment.h"
#include "../../common/AttributeComparator.h"
#include "../../Parsetree/ExecuteLogicalQueryPlan.h"
#include "../../Executor/IteratorExecutorSlave.h"
#include "../../physical_operator/result_collector.h"
/*
 * ExpanderFrameTest.h
 *
 *  Created on: Mar 29, 2014
 *      Author: wangli
 */
using namespace std;

static int test_scan(){

	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
//	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::EQ,std::string("20101008"));
	const int sec_code=600036;
//	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::GEQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,filter_1,LogicalQueryPlanRoot::kPerformance);

	PerformanceMonitor* executable_query_plan=(PerformanceMonitor*)root->GetPhysicalPlan(1024*64);
//	executable_query_plan->print();
	executable_query_plan->Open();
	while(executable_query_plan->Next(0));
	executable_query_plan->Close();

//	executable_query_plan
//	ResultSet *result_set=executable_query_plan->getResultSet();

	const unsigned long int number_of_tuples=executable_query_plan->GetNumberOfTuples();
	if(!print_test_name_result(number_of_tuples==3966020,"Scan")){
		printf("\tExpected:3966020 actual: %d\n",number_of_tuples);
	}
//	result_set->~ResultSet();
	delete executable_query_plan;
	delete root;

	return 1;


}
static int test_scan_filter_high_selectivity(){




	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::GEQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,filter_1,LogicalQueryPlanRoot::kPerformance);

	PerformanceMonitor* executable_query_plan=(PerformanceMonitor*)root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();

	IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan(executable_query_plan));
//	executable_query_plan->open();
//	while(executable_query_plan->next(0));
//	executable_query_plan->close();

//	executable_query_plan
//	ResultSet *result_set=executable_query_plan->getResultSet();

	const unsigned long int number_of_tuples=executable_query_plan->GetNumberOfTuples();
	if(!print_test_name_result(number_of_tuples==3651348,"High selectivity filter")){
		printf("\tExpected:3651348 actual: %d\n",number_of_tuples);
	}
//	result_set->~ResultSet();
	delete executable_query_plan;
	delete root;

	return 1;


}
static int test_scan_filter_low_selectivity(){

	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));


//	QColcumns *qcol=new QColcumns("cj","row_id",t_u_long,"cj.row_id");//table_name,column_name,actual_type,alias
//	QExpr *qexpr=new QExpr("1",t_u_long,"1");//data(string),type,alias
//	QExpr_binary *qcalnode=new QExpr_binary(qcol,qexpr,t_u_long,oper_equal,t_qexpr_cmp,"cj.row_id=1");
//	vector<QNode *>qual;
//	qual.push_back(qcalnode);
	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::EQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,filter_1,LogicalQueryPlanRoot::kPerformance);

	PerformanceMonitor* executable_query_plan=(PerformanceMonitor*)root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan(executable_query_plan));

//	executable_query_plan
//	ResultSet *result_set=executable_query_plan->getResultSet();

	const unsigned long int number_of_tuples=executable_query_plan->GetNumberOfTuples();
	if(!print_test_name_result(number_of_tuples==26695,"Low selectivity filter")){
		printf("\tExpected:26695 actual: %d\n",number_of_tuples);
	}
//	result_set->~ResultSet();
	delete executable_query_plan;
	delete root;

	return 1;


}
static int test_scan_Aggregation_small_Groups(){

	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));



	std::vector<Attribute> group_by_attributes;
	group_by_attributes.push_back(table_1->getAttribute("sec_code"));

	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));


	std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;

	aggregation_function.push_back(PhysicalAggregation::State::kCount);
	LogicalOperator* aggregation=new LogicalAggregation(group_by_attributes,aggregation_attributes,aggregation_function,cj_join_key_scan);




	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,aggregation,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan(executable_query_plan));

//	executable_query_plan
	ResultSet *result_set=executable_query_plan->GetResultSet();

	const unsigned long int number_of_tuples=result_set->getNumberOftuples();
	if(!print_test_name_result(number_of_tuples==1022,"Scan Aggregation small groups")){
		printf("\tExpected:870 actual: %d\n",number_of_tuples);
	}
	delete executable_query_plan;
	delete root;
	delete result_set;

	return 1;


}
static int test_scan_Aggregation_large_Groups(){

	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));



	std::vector<Attribute> group_by_attributes;
	group_by_attributes.push_back(table_1->getAttribute("row_id"));

	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));


	std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;

	aggregation_function.push_back(PhysicalAggregation::State::kCount);
	LogicalOperator* aggregation=new LogicalAggregation(group_by_attributes,aggregation_attributes,aggregation_function,cj_join_key_scan);




	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,aggregation,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan(executable_query_plan));

	ResultSet *result_set=executable_query_plan->GetResultSet();

	const unsigned long int number_of_tuples=result_set->getNumberOftuples();
	if(!print_test_name_result(number_of_tuples==3966020,"Scan Aggregation large groups")){
		printf("\tExpected:870 actual: %d\n",number_of_tuples);
	}
	delete executable_query_plan;
	delete root;
	delete result_set;

	return 1;


}
static int test_scan_filter_Aggregation(){

	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::GEQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	std::vector<Attribute> group_by_attributes;
	group_by_attributes.push_back(table_1->getAttribute("sec_code"));

	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));


	std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;

	aggregation_function.push_back(PhysicalAggregation::State::kCount);
	LogicalOperator* aggregation=new LogicalAggregation(group_by_attributes,aggregation_attributes,aggregation_function,filter_1);




	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,aggregation,LogicalQueryPlanRoot::kPerformance);

	PerformanceMonitor* executable_query_plan=(PerformanceMonitor*)root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan(executable_query_plan));

//	executable_query_plan
//	ResultSet *result_set=executable_query_plan->getResultSet();

	const unsigned long int number_of_tuples=executable_query_plan->GetNumberOfTuples();
	if(!print_test_name_result(number_of_tuples==870,"Filtered Aggregation")){
		printf("\tExpected:870 actual: %d\n",number_of_tuples);
	}
	delete executable_query_plan;
	delete root;

	return 1;


}
static int test_scan_filter_Scalar_Aggregation(){

	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::GEQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	std::vector<Attribute> group_by_attributes;
//	group_by_attributes.push_back(table_1->getAttribute("sec_code"));

	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));


	std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;

	aggregation_function.push_back(PhysicalAggregation::State::kCount);
	LogicalOperator* aggregation=new LogicalAggregation(group_by_attributes,aggregation_attributes,aggregation_function,filter_1);




	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,aggregation,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan(executable_query_plan));

//	executable_query_plan
	ResultSet *result_set=executable_query_plan->GetResultSet();
	ResultSet::Iterator it=result_set->createIterator();
	BlockStreamBase::BlockStreamTraverseIterator *b_it=it.nextBlock()->createIterator();

	const unsigned long int number_of_tuples=*(unsigned long*)b_it->currentTuple();
	delete b_it;

	if(!print_test_name_result(number_of_tuples==3651348,"Filtered Scalar Aggregation")){
		printf("\tExpected:3651348 actual: %d\n",number_of_tuples);
	}
	printf("%4.4f seconds\n",result_set->query_time_);
	delete executable_query_plan;
	delete root;

	return 1;


}

static int test_no_repartition_filtered_join(){
	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	TableDescriptor* table_2=Catalog::getInstance()->getTable("sb");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));
	LogicalOperator* sb_join_key_scan=new LogicalScan(table_2->getProjectoin(0));


	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::EQ,std::string("600036"));


	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	LogicalFilter::Condition filter_condition_2;
	const int order_type_=1;
	filter_condition_2.add(table_2->getAttribute(4),AttributeComparator::EQ,std::string("1"));
	const int entry_date=20101008;
	filter_condition_2.add(table_2->getAttribute(2),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code_=600036;
	filter_condition_2.add(table_2->getAttribute(3),AttributeComparator::EQ,std::string("600036"));
	LogicalOperator* filter_2=new LogicalFilter(filter_condition_2,sb_join_key_scan);


	std::vector<LogicalEqualJoin::JoinPair> sb_cj_join_pair_list;
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("row_id"),table_2->getAttribute("row_id")));
//	sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("trade_date"),table_2->getAttribute("entry_date")));
//	sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("trade_dir"),table_2->getAttribute("entry_dir")));
	LogicalOperator* sb_cj_join=new LogicalEqualJoin(sb_cj_join_pair_list,filter_1,filter_2);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,sb_cj_join,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan(executable_query_plan));

	ResultSet *result_set=executable_query_plan->GetResultSet();

	const unsigned long int number_of_tuples=result_set->getNumberOftuples();

	if(!print_test_name_result(number_of_tuples==15,"Filtered no_repartition Join")){
		printf("\tExpected:15 actual: %d\n",number_of_tuples);
	}
	printf("%4.4f seconds\n",result_set->query_time_);
	delete executable_query_plan;
	delete root;

	return 1;

}
static int test_complete_repartition_filtered_join(){
	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	TableDescriptor* table_2=Catalog::getInstance()->getTable("sb");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));
	LogicalOperator* sb_join_key_scan=new LogicalScan(table_2->getProjectoin(0));


	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::EQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	LogicalFilter::Condition filter_condition_2;
	const int order_type_=1;
	filter_condition_2.add(table_2->getAttribute(4),AttributeComparator::EQ,std::string("1"));
	const int entry_date=20101008;
	filter_condition_2.add(table_2->getAttribute(2),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code_=600036;
	filter_condition_2.add(table_2->getAttribute(3),AttributeComparator::EQ,std::string("600036"));
	LogicalOperator* filter_2=new LogicalFilter(filter_condition_2,sb_join_key_scan);


	std::vector<LogicalEqualJoin::JoinPair> sb_cj_join_pair_list;
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("order_no"),table_2->getAttribute("order_no")));
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("trade_date"),table_2->getAttribute("entry_date")));
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("trade_dir"),table_2->getAttribute("entry_dir")));
	LogicalOperator* sb_cj_join=new LogicalEqualJoin(sb_cj_join_pair_list,filter_1,filter_2);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,sb_cj_join,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan(executable_query_plan));

	ResultSet *result_set=executable_query_plan->GetResultSet();

	const unsigned long int number_of_tuples=result_set->getNumberOftuples();

	if(!print_test_name_result(number_of_tuples==27012,"Filtered Re-partitioned Join")){
		printf("\tExpected:27012 actual: %d\n",number_of_tuples);
	}
	printf("%4.4f seconds\n",result_set->query_time_);
	delete executable_query_plan;
	delete root;

	return 1;

}
static int test_complete_repartition_scan_join(){
	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	TableDescriptor* table_2=Catalog::getInstance()->getTable("sb");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));
	LogicalOperator* sb_join_key_scan=new LogicalScan(table_2->getProjectoin(0));

	std::vector<LogicalEqualJoin::JoinPair> sb_cj_join_pair_list;
//	sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("order_no"),table_2->getAttribute("order_no")));
//	sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("trade_date"),table_2->getAttribute("entry_date")));
//	sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("trade_dir"),table_2->getAttribute("entry_dir")));
////	sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("row_id"),table_2->getAttribute("row_id")));
//	LogicalOperator* sb_cj_join=new EqualJoin(sb_cj_join_pair_list,cj_join_key_scan,sb_join_key_scan);


	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_2->getAttribute("order_no"),table_1->getAttribute("order_no")));
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_2->getAttribute("entry_date"),table_1->getAttribute("trade_date")));
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_2->getAttribute("entry_dir"),table_1->getAttribute("trade_dir")));
	LogicalOperator* sb_cj_join=new LogicalEqualJoin(sb_cj_join_pair_list,sb_join_key_scan,cj_join_key_scan);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,sb_cj_join,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
	IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan(executable_query_plan));

	ResultSet *result_set=executable_query_plan->GetResultSet();

	const unsigned long int number_of_tuples=result_set->getNumberOftuples();

	if(!print_test_name_result(number_of_tuples==4059609,"Re-partitioned Join")){
		printf("\tExpected:4059609 actual: %d\n",number_of_tuples);
		sleep(2);
//		assert(false);
	}
	printf("%4.4f seconds\n",result_set->query_time_);
	delete executable_query_plan;
	delete root;

	return 1;

}
static int test_no_repartition_scan_join(){
	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	TableDescriptor* table_2=Catalog::getInstance()->getTable("sb");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));
	LogicalOperator* sb_join_key_scan=new LogicalScan(table_2->getProjectoin(0));



	std::vector<LogicalEqualJoin::JoinPair> sb_cj_join_pair_list;
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("row_id"),table_2->getAttribute("row_id")));
	LogicalOperator* sb_cj_join=new LogicalEqualJoin(sb_cj_join_pair_list,cj_join_key_scan,sb_join_key_scan);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,sb_cj_join,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
	IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan(executable_query_plan));

	ResultSet *result_set=executable_query_plan->GetResultSet();

	const unsigned long int number_of_tuples=result_set->getNumberOftuples();

	if(!print_test_name_result(number_of_tuples==944925,"no_repartition Join")){
		printf("\tExpected:944925 actual: %d\n",number_of_tuples);
	}
//	result_set->print();
	printf("%4.4f seconds\n",result_set->query_time_);
//	result_set->~ResultSet();
	delete executable_query_plan;
	delete root;

	return 1;

}
static int test_expanderFramework_single_node(int repeated_times=20){


	startup_single_node_environment_of_poc();

//	LogicalOperator* root=convert_sql_to_logical_operator_tree("selects * from cj;");
//	if(root!=0){
//		printf("OK!\n");
//	}

	printf("This test requires one partition of POC sb and cj\n");

	printf("This test requires one partition of POC sb and cj\n");

//	for(unsigned i=0;i<repeated_times;i++){
//		test_scan();
//	}
//	for(unsigned i=0;i<repeated_times;i++){
//		test_scan_filter_high_selectivity();
//	}
//	for(unsigned i=0;i<repeated_times;i++){
//		test_scan_filter_low_selectivity();
//	}
//	while(true){
		int input;
		scanf("%d",&input);
	for(unsigned i=0;i<input;i++){
		test_scan_Aggregation_small_Groups();
	}
//	for(unsigned i=0;i<input;i++){
//		test_scan_Aggregation_large_Groups();
//	}
//	}
//	for(unsigned i=0;i<repeated_times;i++){
//		test_scan_Aggregation_large_Groups();
//	}
//	for(unsigned i=0;i<repeated_times;i++){
//		test_scan_filter_low_selectivity();
//	}
//	for(unsigned i=0;i<repeated_times;i++){
//		test_scan_filter_Aggregation();
//	}
//	for(unsigned i=0;i<repeated_times;i++){
//		test_scan_filter_Scalar_Aggregation();
//	}
//	for(unsigned i=0 ; i < repeated_times ; i++){
//		test_no_repartition_filtered_join();
//	}
//	for(unsigned i=0 ; i < repeated_times ; i++){
//		test_complete_repartition_filtered_join();
//	}
//	for(unsigned i=0 ; i < repeated_times ; i++){
//		test_complete_repartition_scan_join();
//	}
//	for(unsigned i=0 ; i < repeated_times ; i++){
//		test_no_repartition_scan_join();
//	}
	printf("__________________FINISHED__________________\n");
//	sleep(1);
	delete Environment::getInstance();
//	sleep(100000000);
//
}
static void startup_multiple_node_environment(){
//	int master;
	int master=0;
	printf("Master(0) or Slave(others)??\n");
	scanf("%d",&master);
	if(master!=0){
		Environment::getInstance(false);
	}
	else{

		Environment::getInstance(true);

		ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
		Catalog* catalog=Environment::getInstance()->getCatalog();

		TableDescriptor* table_1=new TableDescriptor("cj",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
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
		table_1->createHashPartitionedProjection(cj_proj0_index,"sec_code",4);	//G0
//		catalog->add_table(table_1);
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

		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",4);	//G1


		catalog->add_table(table_1);

		////////////////////////////////////Create table right//////////////////////////
		TableDescriptor* table_2=new TableDescriptor("sb",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_2->addAttribute("row_id",data_type(t_u_long));
		table_2->addAttribute("order_no",data_type(t_u_long));
		table_2->addAttribute("entry_date",data_type(t_int));
		table_2->addAttribute("sec_code",data_type(t_int));
		table_2->addAttribute("order_type",data_type(t_int));
		table_2->addAttribute("entry_dir",data_type(t_int));			//5
		table_2->addAttribute("tran_maint_code",data_type(t_int));
		table_2->addAttribute("Last_upd_date",data_type(t_int));
		table_2->addAttribute("Last_upd_time",data_type(t_int));
		table_2->addAttribute("Last_upd_time_dec",data_type(t_u_long));
		table_2->addAttribute("entry_time",data_type(t_int));			//10
		table_2->addAttribute("entry_time_dec",data_type(t_double));
		table_2->addAttribute("order_price",data_type(t_double));
		table_2->addAttribute("order_exec_vol",data_type(t_double));
		table_2->addAttribute("order_vol",data_type(t_double));
		table_2->addAttribute("pbu_id",data_type(t_int));				//15
		table_2->addAttribute("acct_id",data_type(t_int));
		table_2->addAttribute("acct_attr",data_type(t_int));
		table_2->addAttribute("branch_id",data_type(t_int));
		table_2->addAttribute("pbu_inter_order_no",data_type(t_int));
		table_2->addAttribute("pub_inter_txt",data_type(t_int));		//20
		table_2->addAttribute("aud_type",data_type(t_int));
		table_2->addAttribute("trade_restr_type",data_type(t_int));
		table_2->addAttribute("order_star",data_type(t_int));
		table_2->addAttribute("order_restr_type",data_type(t_int));
		table_2->addAttribute("short_sell_flag",data_type(t_int));		//25















		vector<ColumnOffset> sb_proj0_index;
		sb_proj0_index.push_back(0);
		sb_proj0_index.push_back(1);
		sb_proj0_index.push_back(2);
		sb_proj0_index.push_back(3);
		sb_proj0_index.push_back(4);
		sb_proj0_index.push_back(5);

		table_2->createHashPartitionedProjection(sb_proj0_index,"sec_code",4);	//G0



		vector<ColumnOffset> sb_proj1_index;
		sb_proj1_index.push_back(0);
		sb_proj1_index.push_back(6);
		sb_proj1_index.push_back(7);
		sb_proj1_index.push_back(8);
		sb_proj1_index.push_back(9);
		sb_proj1_index.push_back(10);
		sb_proj1_index.push_back(11);
		sb_proj1_index.push_back(12);
		sb_proj1_index.push_back(13);
		sb_proj1_index.push_back(14);
		sb_proj1_index.push_back(15);
		sb_proj1_index.push_back(16);
		sb_proj1_index.push_back(17);
		sb_proj1_index.push_back(18);
		sb_proj1_index.push_back(19);
		sb_proj1_index.push_back(20);
		sb_proj1_index.push_back(21);
		sb_proj1_index.push_back(22);
		sb_proj1_index.push_back(23);
		sb_proj1_index.push_back(24);
		sb_proj1_index.push_back(25);




		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",4);	//G1


		catalog->add_table(table_2);
		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_1->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}


		//sb_table
		for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_2->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}
	}
}
static int test_multiple_scan(){

	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
//	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
//	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::GEQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,filter_1,LogicalQueryPlanRoot::kPerformance);

	PerformanceMonitor* executable_query_plan=(PerformanceMonitor*)root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	executable_query_plan->Open();
	while(executable_query_plan->Next(0));
	executable_query_plan->Close();

//	executable_query_plan
//	ResultSet *result_set=executable_query_plan->getResultSet();

	const unsigned long int number_of_tuples=executable_query_plan->GetNumberOfTuples();
	if(!print_test_name_result(number_of_tuples==15882988,"Scan")){
		printf("\tExpected:15882988 actual: %d\n",number_of_tuples);
	}
//	result_set->~ResultSet();
	delete executable_query_plan;
	delete root;

	return 1;


}
static int test_multiple_scan_filter_high_selectivity(){

	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::GEQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,filter_1,LogicalQueryPlanRoot::kPerformance);

	PerformanceMonitor* executable_query_plan=(PerformanceMonitor*)root->GetPhysicalPlan(1024*64 );
	executable_query_plan->Print();
	executable_query_plan->Open();
	while(executable_query_plan->Next(0));
	executable_query_plan->Close();

//	executable_query_plan
//	ResultSet *result_set=executable_query_plan->getResultSet();

	const unsigned long int number_of_tuples=executable_query_plan->GetNumberOfTuples();
	if(!print_test_name_result(number_of_tuples==14623495,"High selectivity filter")){
		printf("\tExpected:14623495 actual: %d\n",number_of_tuples);
	}
//	result_set->~ResultSet();
	delete executable_query_plan;
	delete root;

	return 1;


}
static int test_multiple_scan_filter_low_selectivity(){

	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::EQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,filter_1,LogicalQueryPlanRoot::kPerformance);

	PerformanceMonitor* executable_query_plan=(PerformanceMonitor*)root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	executable_query_plan->Open();
	while(executable_query_plan->Next(0));
	executable_query_plan->Close();

//	executable_query_plan
//	ResultSet *result_set=executable_query_plan->getResultSet();

	const unsigned long int number_of_tuples=executable_query_plan->GetNumberOfTuples();
	if(!print_test_name_result(number_of_tuples==104010,"Low selectivity filter")){
		printf("\tExpected:104010 actual: %d\n",number_of_tuples);
	}
//	result_set->~ResultSet();
	delete executable_query_plan;
	delete root;

	return 1;


}

static int test_multiple_scan_filter_Aggregation(){

	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::GEQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	std::vector<Attribute> group_by_attributes;
	group_by_attributes.push_back(table_1->getAttribute("sec_code"));

	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));


	std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;

	aggregation_function.push_back(PhysicalAggregation::State::kCount);
	LogicalOperator* aggregation=new LogicalAggregation(group_by_attributes,aggregation_attributes,aggregation_function,filter_1);




	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,aggregation,LogicalQueryPlanRoot::kPerformance);

	PerformanceMonitor* executable_query_plan=(PerformanceMonitor*)root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	executable_query_plan->Open();
	while(executable_query_plan->Next(0));
	executable_query_plan->Close();

//	executable_query_plan
//	ResultSet *result_set=executable_query_plan->getResultSet();

	const unsigned long int number_of_tuples=executable_query_plan->GetNumberOfTuples();
	if(!print_test_name_result(number_of_tuples==3480,"Filtered Aggregation")){
		printf("\tExpected:3480 actual: %d\n",number_of_tuples);
	}
	delete executable_query_plan;
	delete root;

	return 1;


}
static int test_multiple_scan_filter_Scalar_Aggregation(){

	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));

	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::GEQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	std::vector<Attribute> group_by_attributes;
//	group_by_attributes.push_back(table_1->getAttribute("sec_code"));

	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));


	std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;

	aggregation_function.push_back(PhysicalAggregation::State::kCount);
	LogicalOperator* aggregation=new LogicalAggregation(group_by_attributes,aggregation_attributes,aggregation_function,filter_1);




	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,aggregation,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	executable_query_plan->Open();
	while(executable_query_plan->Next(0));
	executable_query_plan->Close();

//	executable_query_plan
	ResultSet *result_set=executable_query_plan->GetResultSet();
	ResultSet::Iterator it=result_set->createIterator();
	BlockStreamBase::BlockStreamTraverseIterator *b_it=it.nextBlock()->createIterator();

	const unsigned long int number_of_tuples=*(unsigned long*)b_it->currentTuple();
	delete b_it;

	if(!print_test_name_result(number_of_tuples==14623495,"Filtered Scalar Aggregation")){
		printf("\tExpected:14623495 actual: %d\n",number_of_tuples);
	}
	printf("%4.4f seconds\n",result_set->query_time_);
	delete executable_query_plan;
	delete root;

	return 1;


}

static int test_multiple_no_repartition_filtered_join(){
	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	TableDescriptor* table_2=Catalog::getInstance()->getTable("sb");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));
	LogicalOperator* sb_join_key_scan=new LogicalScan(table_2->getProjectoin(0));


	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::EQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	LogicalFilter::Condition filter_condition_2;
	const int order_type_=1;
	filter_condition_2.add(table_2->getAttribute(4),AttributeComparator::EQ,std::string("1"));
	const int entry_date=20101008;
	filter_condition_2.add(table_2->getAttribute(2),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code_=600036;
	filter_condition_2.add(table_2->getAttribute(3),AttributeComparator::EQ,std::string("600036"));
	LogicalOperator* filter_2=new LogicalFilter(filter_condition_2,sb_join_key_scan);


	std::vector<LogicalEqualJoin::JoinPair> sb_cj_join_pair_list;
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("row_id"),table_2->getAttribute("row_id")));
//	sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("trade_date"),table_2->getAttribute("entry_date")));
//	sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("trade_dir"),table_2->getAttribute("entry_dir")));
	LogicalOperator* sb_cj_join=new LogicalEqualJoin(sb_cj_join_pair_list,filter_1,filter_2);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,sb_cj_join,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	executable_query_plan->Open();
	while(executable_query_plan->Next(0));
	executable_query_plan->Close();

	ResultSet *result_set=executable_query_plan->GetResultSet();

	const unsigned long int number_of_tuples=result_set->getNumberOftuples();

	if(!print_test_name_result(number_of_tuples==278,"Filtered no_repartition Join")){
		printf("\tExpected:278 actual: %d\n",number_of_tuples);
	}
	printf("%4.4f seconds\n",result_set->query_time_);
	delete executable_query_plan;
	delete root;

	return 1;

}
static int test_multiple_complete_repartition_filtered_join(){
	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	TableDescriptor* table_2=Catalog::getInstance()->getTable("sb");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));
	LogicalOperator* sb_join_key_scan=new LogicalScan(table_2->getProjectoin(0));


	LogicalFilter::Condition filter_condition_1;
	const int order_type=1;
	filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
	const int trade_date=20101008;
	filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code=600036;
	filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::EQ,std::string("600036"));
	LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,cj_join_key_scan);

	LogicalFilter::Condition filter_condition_2;
	const int order_type_=1;
	filter_condition_2.add(table_2->getAttribute(4),AttributeComparator::EQ,std::string("1"));
	const int entry_date=20101008;
	filter_condition_2.add(table_2->getAttribute(2),AttributeComparator::GEQ,std::string("20101008"));
	const int sec_code_=600036;
	filter_condition_2.add(table_2->getAttribute(3),AttributeComparator::EQ,std::string("600036"));
	LogicalOperator* filter_2=new LogicalFilter(filter_condition_2,sb_join_key_scan);


	std::vector<LogicalEqualJoin::JoinPair> sb_cj_join_pair_list;
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("order_no"),table_2->getAttribute("order_no")));
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("trade_date"),table_2->getAttribute("entry_date")));
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("trade_dir"),table_2->getAttribute("entry_dir")));
	LogicalOperator* sb_cj_join=new LogicalEqualJoin(sb_cj_join_pair_list,filter_1,filter_2);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,sb_cj_join,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	executable_query_plan->Open();
	while(executable_query_plan->Next(0));
	executable_query_plan->Close();

	ResultSet *result_set=executable_query_plan->GetResultSet();

	const unsigned long int number_of_tuples=result_set->getNumberOftuples();

	if(!print_test_name_result(number_of_tuples==105480,"Filtered Re-partitioned Join")){
		printf("\tExpected:105480 actual: %d\n",number_of_tuples);
	}
	printf("%4.4f seconds\n",result_set->query_time_);
	delete executable_query_plan;
	delete root;

	return 1;

}
static int test_multiple_complete_repartition_scan_join(){
	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	TableDescriptor* table_2=Catalog::getInstance()->getTable("sb");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));
	LogicalOperator* sb_join_key_scan=new LogicalScan(table_2->getProjectoin(0));



	std::vector<LogicalEqualJoin::JoinPair> sb_cj_join_pair_list;
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("order_no"),table_2->getAttribute("order_no")));
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("trade_date"),table_2->getAttribute("entry_date")));
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("trade_dir"),table_2->getAttribute("entry_dir")));
	LogicalOperator* sb_cj_join=new LogicalEqualJoin(sb_cj_join_pair_list,cj_join_key_scan,sb_join_key_scan);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,sb_cj_join,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	executable_query_plan->Open();
	while(executable_query_plan->Next(0));
	executable_query_plan->Close();

	ResultSet *result_set=executable_query_plan->GetResultSet();

	const unsigned long int number_of_tuples=result_set->getNumberOftuples();

	if(!print_test_name_result(number_of_tuples==16257984,"Re-partitioned Join")){
		printf("\tExpected:16257984 actual: %d\n",number_of_tuples);
		sleep(2);
//		assert(false);
	}
	printf("%4.4f seconds\n",result_set->query_time_);
	delete executable_query_plan;
	delete root;

	return 1;

}
static int test_multiple_no_repartition_scan_join(){
	TableDescriptor* table_1=Catalog::getInstance()->getTable("cj");
	TableDescriptor* table_2=Catalog::getInstance()->getTable("sb");
	LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));
	LogicalOperator* sb_join_key_scan=new LogicalScan(table_2->getProjectoin(0));



	std::vector<LogicalEqualJoin::JoinPair> sb_cj_join_pair_list;
	sb_cj_join_pair_list.push_back(LogicalEqualJoin::JoinPair(table_1->getAttribute("row_id"),table_2->getAttribute("row_id")));
	LogicalOperator* sb_cj_join=new LogicalEqualJoin(sb_cj_join_pair_list,cj_join_key_scan,sb_join_key_scan);

	const NodeID collector_node_id=0;
	LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,sb_cj_join,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64 );
//	executable_query_plan->print();
	executable_query_plan->Open();
	while(executable_query_plan->Next(0));
	executable_query_plan->Close();

	ResultSet *result_set=executable_query_plan->GetResultSet();

	const unsigned long int number_of_tuples=result_set->getNumberOftuples();

	if(!print_test_name_result(number_of_tuples==944925,"no_repartition Join")){
		printf("\tExpected:944925 actual: %d\n",number_of_tuples);
	}
//	result_set->print();
	printf("%4.4f seconds\n",result_set->query_time_);
//	result_set->~ResultSet();
	delete executable_query_plan;
	delete root;

	return 1;

}
static int test_multiple_expanderFramework_single_node(int repeated_times=20){

	startup_multiple_node_environment();

	if(Environment::getInstance()->getNodeID()==0){
		printf("Press any key to continue;\n");
		int i;
		scanf("%d",&i);
	printf("============Scan->Filter->Expander->Exchange->root============\n");
//	for(unsigned i=0;i<repeated_times;i++){
//		test_multiple_scan();
//	}

//	for(unsigned i=0;i<repeated_times;i++){
//		test_multiple_scan_filter_high_selectivity();
//		printf("----------------------------------\n");
//	}
	for(unsigned i=0;i<repeated_times;i++){
		test_multiple_scan_filter_low_selectivity();
	}
//	for(unsigned i=0;i<repeated_times;i++){
//		test_multiple_scan_filter_Aggregation();
//	}
//	for(unsigned i=0;i<repeated_times;i++){
//		test_multiple_scan_filter_Scalar_Aggregation();
//	}
//	for(unsigned i=0 ; i < repeated_times ; i++){
//		test_multiple_complete_repartition_filtered_join();
//	}
//	for(unsigned i=0 ; i < repeated_times ; i++){
//		test_multiple_no_repartition_filtered_join();
//	}
//	for(unsigned i=0 ; i < repeated_times ; i++){
//		test_multiple_complete_repartition_scan_join();
//	}
//	for(unsigned i=0 ; i < repeated_times ; i++){
//		test_no_repartition_scan_join();
//	}

	}
	printf("Press any key to continue;\n");
	int i;
	scanf("%d",&i);
	delete Environment::getInstance();
//
}
