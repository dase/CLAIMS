/*
 * tpc_h_test.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: wangli
 */

#ifndef TEST_NEW
#define TEST_NEW
#include "../../Environment.h"
#include "../../Catalog/table.h"
#include "../../Loader/Hdfsloader.h"
#include "../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../LogicalQueryPlan/Aggregation.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"
#include "../../LogicalQueryPlan/Scan.h"
#include "../../LogicalQueryPlan/Filter.h"
#include "../../LogicalQueryPlan/Project.h"
#include "../../LogicalQueryPlan/EqualJoin.h"
#include "../../common/types/NValue.hpp"
#include "../../utility/rdtsc.h"
#include "../../common/ExpressionItem.h"
#include "../../common/ExpressionCalculator.h"
#include "../set_up_environment.h"

static void query_project(){
	unsigned long long int start=curtick();
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("PART");

//	Filter::Condition filter_condition_1;
//	filter_condition_1.add(table->getAttribute("L_SHIPDATE"),FilterIterator::AttributeComparator::L,std::string("1998-12-01"));
//	LogicalOperator* filter=new Filter(filter_condition_1,scan);
////	LogicalOperator* filter=new Filter();
//
//	std::vector<Attribute> group_by_attributes;
//	group_by_attributes.push_back(table->getAttribute("L_RETURNFLAG"));
//	group_by_attributes.push_back(table->getAttribute("L_LINESTATUS"));
//	std::vector<Attribute> aggregation_attributes;
//	aggregation_attributes.push_back(table->getAttribute("L_QUANTITY"));
//	aggregation_attributes.push_back(table->getAttribute("L_EXTENDEDPRICE"));
//	aggregation_attributes.push_back(table->getAttribute("L_DISCOUNT"));
//	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
//	std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;
//
//	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
//	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
//	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
//	aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
//	LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,filter);

	//===========================scan===========================
	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));

	//==========================project=========================
	vector< vector<ExpressionItem> >expr_list;

	Expression expr1;
	Expression expr2;
	Expression expr3;
	Expression expr4;
	Expression expr5;
	Expression expr6;
	Expression expr7;
	Expression expr8;
	Expression expr9;
	Expression expr10;

	ExpressionItem expr_item1;
	ExpressionItem expr_item2;
	ExpressionItem expr_item3;
	ExpressionItem expr_item4;
	ExpressionItem expr_item5;
	ExpressionItem expr_item6;
	ExpressionItem expr_item7;
	ExpressionItem expr_item8;
	ExpressionItem expr_item9;
	ExpressionItem expr_item10;

	expr_item1.setVariable("PART","row_id");
	expr_item2.setVariable("PART","P_PARTKEY");
	expr_item3.setVariable("PART","P_NAME");
	expr_item4.setVariable("PART","P_MFGR");
	expr_item5.setVariable("PART","P_BRAND");
	expr_item6.setVariable("PART","P_TYPE");
	expr_item7.setVariable("PART","P_SIZE");
	expr_item8.setVariable("PART","P_CONTAINER");
	expr_item9.setVariable("PART","P_RETAILPRICE");
	expr_item10.setVariable("PART","P_COMMENT");

	expr1.push_back(expr_item1);
	expr2.push_back(expr_item2);
	expr3.push_back(expr_item3);
	expr4.push_back(expr_item4);
	expr5.push_back(expr_item5);
	expr6.push_back(expr_item6);
	expr7.push_back(expr_item7);
	expr8.push_back(expr_item8);
	expr9.push_back(expr_item9);
	expr10.push_back(expr_item10);

	expr_list.push_back(expr1);
	expr_list.push_back(expr2);
	expr_list.push_back(expr3);
	expr_list.push_back(expr4);
	expr_list.push_back(expr5);
	expr_list.push_back(expr6);
	expr_list.push_back(expr7);
	expr_list.push_back(expr8);
	expr_list.push_back(expr9);
	expr_list.push_back(expr10);

	LogicalOperator* project=new LogicalProject(scan,expr_list);

	//===========================root===========================
	LogicalOperator* root=new LogicalQueryPlanRoot(0,project,LogicalQueryPlanRoot::PRINT);

	cout<<"performance is ok!"<<endl;
	BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024);
//	physical_iterator_tree->print();
	physical_iterator_tree->open();
	while(physical_iterator_tree->next(0));
	physical_iterator_tree->close();
	printf("Q1: execution time: %4.4f second.\n",getSecond(start));

}

static int common_project_tcp_h_test_multi_nodes(){
	unsigned repeated_times=2;
	printf("Master or slave?\n");
	int input;
	scanf("%d",&input);
	if(input==0){
		startup_multiple_node_environment_of_tpch(true);
		for(unsigned i=0;i<repeated_times;i++){
			query_project();
		}
	}
	else{
		startup_multiple_node_environment_of_tpch(false);
	}

	Environment::getInstance()->~Environment();
	return 0;
}

#endif /* TPC_H_TEST_CPP_ */
