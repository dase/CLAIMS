/*
 * TestMain.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: wangli
 */

#include "../LogicalQueryPlan/Test/testGenerateIteratorTree.cpp"
#include "../LogicalQueryPlan/Test/ResultCollect_test.cpp"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/Test/projectionScan.cpp"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/Test/iterator_test.cpp"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/Test/Project_test.cpp"
#include "../Executor/Test/Executor.cpp"
#include "../LogicalQueryPlan/Test/getOptimalQueryPlan.cpp"
#include "../LogicalQueryPlan/Test/testProject.cpp"
#include "../LogicalQueryPlan/Test/testSort.cpp"
#include "../LogicalQueryPlan/Test/testProject_wl.cpp"
#include "../Parsetree/Test/parser_test.cpp"
#include "../common/Schema/Test/SubTuple_test.cpp"
#include "../common/Schema/Test/VariableSchema_test.cpp"
#include "../common/Schema/Test/generateVariableData_test.cpp"
#include "../LogicalQueryPlan/Test/Aggregation_test.cpp"
#include "../Catalog/Test/statistic_manager_test.cpp"
#include "../LogicalQueryPlan/Test/query_optmization_based_on_statisitic_test.cpp"

#include "../BlockStreamIterator/ParallelBlockStreamIterator/Test/Sort_test.cpp"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/Test/Expanded_iterators_test.cpp"
#include "../Daemon/Test/daemon_test.h"
//#include "hashtable_test.cpp"

#include "../Client/Test/TestSeverClient.cpp"
#include "../common/types/Test/data_type_test.h"
#include "../common/Expression_item.cpp"
#include "common/TestNew.cpp"
#include "../Loader/Test/test_load.cpp"

#include "../BlockStreamIterator/ParallelBlockStreamIterator/Test/in_iterator_test.cpp"

#include "../IndexManager/Test/test_csb_classes.cpp"
#include "../IndexManager/Test/test_csb_indexing.cpp"
#include "../IndexManager/Test/test_CSB_index_building.cpp"
#include "../IndexManager/Test/test_logical_csb_index_building.cpp"
#include "../IndexManager/Test/test_index_scan_iterator.cpp"
#include "../IndexManager/Test/test_IndexManager_serialize.cpp"

#include "../LogicalQueryPlan/Test/testIn.cpp"
#include "TestSuit/ExpanderFrameTest.h"
#include "TestSuit/tpc_h_test.cpp"
#include "TestSuit/in_segment_scalability_test.cpp"
#include "TestSuit/hash_table_test.h"
#include "gtest.h"
#include "../common/ExpandedThreadTracker_test.cpp"
#include "common/insert_optimized_hash_table_test.h"
#include "memory_leak_test.h"
int main(int argc , char ** argv){

//	return testGenerateIteratorTree();
//	return iterator_test();
//	return testIteratorExecutor();
//	return hashtable_test();
//	return getOptimalQueryPlan();
//	return ResultCollect_test();
//	return parser_test();
//	return sub_tuple_test();
//	return aggregation_test();
//	return statistic_manager_test();
//	return query_optimization_based_on_statistics();
//	return sort_iterator_test();
//	return variable_schema_test();
//	return generateVariableData_test();
//	return query_optimization_based_on_statistics_aggregation();
//	return query_optimization_outputdata();

//	return query_optimization_based_on_statistics();
//	return daemon_test();
//	return testServerClient();
//	return test_data_type();
//	return test_expression_item();
//	return testProject();
//	return testSort();


//	return testProject_wl();
//	return Project_test();
//	return expanded_iterators_test();

//	return test_load();
//	return in_iterator_test();

//	return test_csb_classes();
//	return test_csb_indexing();
//	return test_CSBIndexBuilding();		//for physical iterator testing and IndexManager testing
//	return test_logical_csb_index_building();
//	return test_index_scan_iterator();
//	return test_index_manager();

//	in_test();
//	test_expanderFramework_single_node(5);
//	test_multiple_expanderFramework_single_node(3);
//	tcp_h_test_single_node();
//	tcp_h_test_multi_nodes();
//	in_segment_scalability_test(10);
//	hash_table_test();
//	g_test_main(argc,argv);

//	test_ExpandedThreadTracker();

//	sleep(1000);
//	insert_optimziaed_hash_table_test();
	memory_leak_test();
}
