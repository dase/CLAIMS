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
#include "../Executor/Test/Executor.cpp"
#include "../LogicalQueryPlan/Test/getOptimalQueryPlan.cpp"
#include "../Parsetree/Test/parser_test.cpp"
//#include "hashtable_test.cpp"
int main(){
//	return testGenerateIteratorTree();
//	return iterator_test();
//	return testIteratorExecutor();
//	return hashtable_test();
//	return getOptimalQueryPlan();
//	return ResultCollect_test();
	return parser_test();
}
