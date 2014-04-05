#include "tpc_h_test.cpp"
/*
 * in_segment_scalability_test.cpp
 *
 *  Created on: Apr 5, 2014
 *      Author: wangli
 */

#include "tpc_h_test.cpp"
#include "../../Config.h"

typedef double QueryTime;
typedef QueryTime (*query_function)();
static double lineitem_scan_self_join(){
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("LINEITEM");

	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));

	Filter::Condition filter_condition_1;
	filter_condition_1.add(table->getAttribute("row_id"),FilterIterator::AttributeComparator::EQ,std::string("0"));
	LogicalOperator* filter=new Filter(filter_condition_1,scan);


	std::vector<EqualJoin::JoinPair> s_ps_join_condition;
	s_ps_join_condition.push_back(EqualJoin::JoinPair(table->getAttribute("L_ORDERKEY"),table->getAttribute("L_ORDERKEY")));
//	s_ps_join_condition.push_back(EqualJoin::JoinPair(table->getAttribute("L_PARTKEY"),table->getAttribute("L_SUPPKEY")));
//	s_ps_join_condition.push_back(EqualJoin::JoinPair(table->getAttribute("L_PARTKEY"),table->getAttribute("L_SUPPKEY")));
	LogicalOperator* s_ps_join=new EqualJoin(s_ps_join_condition,filter,scan);

	LogicalOperator* root=new LogicalQueryPlanRoot(0,s_ps_join,LogicalQueryPlanRoot::RESULTCOLLECTOR);

	BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024-sizeof(unsigned));
//	root->print();
//	physical_iterator_tree->print();
	physical_iterator_tree->open();
	while(physical_iterator_tree->next(0));
	physical_iterator_tree->close();

	ResultSet* result_set=physical_iterator_tree->getResultSet();
//	printf("tuples %d\n",result_set->getNumberOftuples());
	double ret=result_set->query_time_;

	physical_iterator_tree->~BlockStreamIteratorBase();
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
	std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;

	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
	aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
	LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,scan);




	LogicalOperator* root=new LogicalQueryPlanRoot(0,aggregation,LogicalQueryPlanRoot::RESULTCOLLECTOR);

	BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024-sizeof(unsigned));
//	root->print();
//	physical_iterator_tree->print();
	physical_iterator_tree->open();
	while(physical_iterator_tree->next(0));
	physical_iterator_tree->close();

	ResultSet* result_set=physical_iterator_tree->getResultSet();
//	printf("tuples %d\n",result_set->getNumberOftuples());
	double ret=result_set->query_time_;

	physical_iterator_tree->~BlockStreamIteratorBase();
	root->~LogicalOperator();
	return ret;
}

static double lineitem_scan_filter(){
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("LINEITEM");

	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));


	Filter::Condition filter_condition_1;
	filter_condition_1.add(table->getAttribute("row_id"),FilterIterator::AttributeComparator::EQ,std::string("0"));
	LogicalOperator* filter=new Filter(filter_condition_1,scan);



	LogicalOperator* root=new LogicalQueryPlanRoot(0,filter,LogicalQueryPlanRoot::RESULTCOLLECTOR);

	BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024-sizeof(unsigned));
//	root->print();
//	physical_iterator_tree->print();
	physical_iterator_tree->open();
	while(physical_iterator_tree->next(0));
	physical_iterator_tree->close();

	ResultSet* result_set=physical_iterator_tree->getResultSet();
	double ret=result_set->query_time_;

	physical_iterator_tree->~BlockStreamIteratorBase();
	root->~LogicalOperator();
	return ret;
}

static void scalability_test(query_function qf,const char* test_name,int max_test_degree_of_parallelism=4){
	unsigned repeated_times=10;
	double standard_throughput=0;
	/* warm up the memory*/
	qf();

	printf("_______Test Scalability for %s ___________\n",test_name);

	for(unsigned i=1;i<=max_test_degree_of_parallelism;i++){
		Config::initial_degree_of_parallelism=i;
		double total_time=0;
		for(unsigned j=0;j<repeated_times;j++){
			total_time+=qf();
		}


		if(i==1){
			standard_throughput=1/total_time;
			printf("D=%d\ts=%4.4f scale:1\n",i,total_time,1);
		}
		else{
			printf("D=%d\ts=%4.4f scale:%f\n",i,total_time,1/total_time/standard_throughput);
		}


	}


}

static int in_segment_scalability_test(int repeated_times=10){
	init_single_node_tpc_h_envoriment();
	double total_time=0;

//	scalability_test(lineitem_scan_filter,"Scan-->filter",8);
//	scalability_test(lineitem_scan_aggregation,"Scan-->aggregation",8);
	scalability_test(lineitem_scan_self_join,"Scan-->join",8);

	Environment::getInstance()->~Environment();

}

