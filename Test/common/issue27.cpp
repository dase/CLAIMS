/*
 * tpc_h_test.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: wangli
 */
//This file will not be used in current version,because it is not contained by Makefile.am
#ifndef ISSUE27_CPP_
#define ISSUE27_CPP_
#include "../../Environment.h"

#include "../../Catalog/table.h"

#include "../../Loader/Hdfsloader.h"

#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"

#include "../../logical_operator/logical_aggregation.h"
#include "../../logical_operator/logical_scan.h"
#include "../../logical_operator/logical_filter.h"
#include "../../logical_operator/logical_equal_join.h"
#include "../../common/ExpressionItem.h"
#include "../../common/ExpressionCalculator.h"
#include "../../common/types/NValue.hpp"
#include "../../logical_operator/logical_project.h"
#include "../../utility/rdtsc.h"

static void query_select_aggregation(){
	/*
	 * select sum(a+1)+count(a),b
	 * from T
	 * group by b
	 *
	 * notation: p a p s
	 * */
	unsigned long long int start=curtick();
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("LINEITEM");
	//===========================scan===========================
	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));

	//==========================project=========================
	vector< vector<ExpressionItem> >expr_list1;

	vector<ExpressionItem> expr1;
	vector<ExpressionItem> expr2;
	vector<ExpressionItem> expr3;
	vector<ExpressionItem> expr4;
	vector<ExpressionItem> expr5;
	vector<ExpressionItem> expr6;
	vector<ExpressionItem> expr7;
	vector<ExpressionItem> expr8;
	vector<ExpressionItem> expr9;
	vector<ExpressionItem> expr10;
	vector<ExpressionItem> expr11;
	vector<ExpressionItem> expr12;
	vector<ExpressionItem> expr13;
	vector<ExpressionItem> expr14;
	vector<ExpressionItem> expr15;
	vector<ExpressionItem> expr16;
	vector<ExpressionItem> expr17;

	ExpressionItem ei1;
	ExpressionItem ei1_1;
	ExpressionItem ei1_2;
	ExpressionItem ei1_3;
	ExpressionItem ei1_4;
	ExpressionItem ei1_5;
	ExpressionItem ei1_6;
	ExpressionItem ei1_7;
	ExpressionItem ei1_8;
	ExpressionItem ei1_9;
	ExpressionItem ei2;
	ExpressionItem ei3;
	ExpressionItem ei4;
	ExpressionItem ei5;
	ExpressionItem ei6;
	ExpressionItem ei7;
	ExpressionItem ei8;
	ExpressionItem ei9;
	ExpressionItem ei10;
	ExpressionItem ei11;
	ExpressionItem ei12;
	ExpressionItem ei13;
	ExpressionItem ei14;
	ExpressionItem ei15;
	ExpressionItem ei16;
	ExpressionItem ei17;


	ei1_1.setVariable("LINEITEM","L_EXTENDEDPRICE");
	ei1_2.setIntValue("1");
	ei1_3.setVariable("LINEITEM","L_DISCOUNT");
	ei1_4.setOperator("-");
	ei1_5.setOperator("*");
	ei1_6.setIntValue("1");
	ei1_7.setVariable("LINEITEM","L_TEX");
	ei1_8.setOperator("+");
	ei1_9.setOperator("*");


	ei1.setVariable("LINEITEM","row_id");
	ei2.setVariable("LINEITEM","L_ORDERKEY");
	ei3.setVariable("LINEITEM","L_PARTKEY");
	ei4.setVariable("LINEITEM","L_SUPPKEY");
	ei5.setVariable("LINEITEM","L_LINENUMBER");
	ei6.setVariable("LINEITEM","L_QUANTITY");
	ei7.setVariable("LINEITEM","L_EXTENDEDPRICE");
	ei8.setVariable("LINEITEM","L_DISCOUNT");
	ei9.setVariable("LINEITEM","L_TEX");
	ei10.setVariable("LINEITEM","L_RETURNFLAG");
//	ei10.size=1;
	ei11.setVariable("LINEITEM","L_LINESTATUS");
//	ei11.size=1;
	ei12.setVariable("LINEITEM","L_SHIPDATE");
	ei13.setVariable("LINEITEM","L_COMMITDATE");
	ei14.setVariable("LINEITEM","L_RECEIPTDATE");
	ei15.setVariable("LINEITEM","L_SHIPINSTRUCT");
//	ei15.size=25;
	ei16.setVariable("LINEITEM","L_SHIPMODE");
//	ei16.size=10;
	ei17.setVariable("LINEITEM","L_COMMENT");
//	ei17.size=44;



	expr1.push_back(ei1_1);
	expr1.push_back(ei1_2);
	expr1.push_back(ei1_3);
	expr1.push_back(ei1_4);
	expr1.push_back(ei1_5);

//	expr1.push_back(ei1_6);
//	expr1.push_back(ei1_7);
//	expr1.push_back(ei1_8);
//	expr1.push_back(ei1_9);
//	expr1.push_back(ei1);

	expr2.push_back(ei1_1);
	expr2.push_back(ei1_2);
	expr2.push_back(ei1_3);
	expr2.push_back(ei1_4);
	expr2.push_back(ei1_5);
	expr2.push_back(ei1_6);
	expr2.push_back(ei1_7);
	expr2.push_back(ei1_8);
	expr2.push_back(ei1_9);

	expr3.push_back(ei1_2);
	expr3.push_back(ei1_3);
	expr3.push_back(ei1_4);

//	expr3.push_back(ei3);
	expr4.push_back(ei4);
	expr5.push_back(ei5);
	expr6.push_back(ei6);
	expr7.push_back(ei7);
	expr8.push_back(ei8);
	expr9.push_back(ei9);
	expr10.push_back(ei10);
	expr11.push_back(ei11);
	expr12.push_back(ei12);
	expr13.push_back(ei13);
	expr14.push_back(ei14);
	expr15.push_back(ei15);
	expr16.push_back(ei16);
	expr17.push_back(ei17);

	expr_list1.push_back(expr10);
	expr_list1.push_back(expr11);
	expr_list1.push_back(expr6);
	expr_list1.push_back(expr7);
	expr_list1.push_back(expr1);
	expr_list1.push_back(expr2);
	expr_list1.push_back(expr8);
	expr_list1.push_back(expr3);
	expr_list1.push_back(expr10);
	expr_list1.push_back(expr11);




//	expr_list1.push_back(expr3);
//	expr_list1.push_back(expr4);
//	expr_list1.push_back(expr5);
//	expr_list1.push_back(expr8);
//	expr_list1.push_back(expr9);
//	expr_list1.push_back(expr12);
//	expr_list1.push_back(expr13);
//	expr_list1.push_back(expr14);
//	expr_list1.push_back(expr15);
//	expr_list1.push_back(expr16);
//	expr_list1.push_back(expr17);

	LogicalOperator* project1=new LogicalProject(scan,expr_list1);

	//========================aggregation=======================
	std::vector<Attribute> group_by_attributes;
	group_by_attributes.push_back(table->getAttribute("L_RETURNFLAG"));
	group_by_attributes.push_back(table->getAttribute("L_LINESTATUS"));
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
	LogicalOperator* aggregation=new LogicalAggregation(group_by_attributes,aggregation_attributes,aggregation_function,project1);

	//==========================project=========================
	vector< vector<ExpressionItem> >expr_list2;
	LogicalOperator* project2=new LogicalProject(aggregation,expr_list2);
	//===========================root===========================
	LogicalOperator* root=new LogicalQueryPlanRoot(0,project1,LogicalQueryPlanRoot::PERFORMANCE);

	cout<<"performance is ok!"<<endl;
	PhysicalOperatorBase* physical_iterator_tree=root->GetPhysicalPlan(64*1024);
//	physical_iterator_tree->print();
	physical_iterator_tree->Open();
	while(physical_iterator_tree->Next(0));
	physical_iterator_tree->Close();
	printf("Q1: execution time: %4.4f second.\n",getSecond(start));

}

static void init_single_node_tpc_h_envoriment_(bool master=true){
	Environment::getInstance(master);
	printf("Press any key to continue!\n");
	int input;
	scanf("%d",&input);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();

	/////////////////////////////// LINEITEM TABLE //////////////////////////////////
	TableDescriptor* table_1=new TableDescriptor("LINEITEM",0);
	table_1->addAttribute("row_id", data_type(t_u_long));
	table_1->addAttribute("L_ORDERKEY",data_type(t_u_long));  				//0
	table_1->addAttribute("L_PARTKEY",data_type(t_u_long));
	table_1->addAttribute("L_SUPPKEY",data_type(t_u_long));
	table_1->addAttribute("L_LINENUMBER",data_type(t_u_long));
	table_1->addAttribute("L_QUANTITY",data_type(t_decimal));
	table_1->addAttribute("L_EXTENDEDPRICE",data_type(t_decimal));
	table_1->addAttribute("L_DISCOUNT",data_type(t_decimal));
	table_1->addAttribute("L_TEX",data_type(t_decimal));
	table_1->addAttribute("L_RETURNFLAG",data_type(t_string),1);
	table_1->addAttribute("L_LINESTATUS",data_type(t_string),1);
	table_1->addAttribute("L_SHIPDATE",data_type(t_date));
	table_1->addAttribute("L_COMMITDATE",data_type(t_date));
	table_1->addAttribute("L_RECEIPTDATE",data_type(t_date));
	table_1->addAttribute("L_SHIPINSTRUCT",data_type(t_string),25);
	table_1->addAttribute("L_SHIPMODE",data_type(t_string),10);
	table_1->addAttribute("L_COMMENT",data_type(t_string),44);

	table_1->createHashPartitionedProjectionOnAllAttribute("L_ORDERKEY",1);

	catalog->add_table(table_1);

	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,5);
	}
}

static void init_multi_node_tpc_h_envoriment_(bool master=true){
	Environment::getInstance(master);
	printf("Press any key to continue!\n");
	int input;
	scanf("%d",&input);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();

	/////////////////////////////// LINEITEM TABLE //////////////////////////////////
	TableDescriptor* table_1=new TableDescriptor("LINEITEM",5);
	table_1->addAttribute("row_id", data_type(t_u_long));
	table_1->addAttribute("L_ORDERKEY",data_type(t_u_long));  				//0
	table_1->addAttribute("L_PARTKEY",data_type(t_u_long));
	table_1->addAttribute("L_SUPPKEY",data_type(t_u_long));
	table_1->addAttribute("L_LINENUMBER",data_type(t_u_long));
	table_1->addAttribute("L_QUANTITY",data_type(t_decimal));
	table_1->addAttribute("L_EXTENDEDPRICE",data_type(t_decimal));
	table_1->addAttribute("L_DISCOUNT",data_type(t_decimal));
	table_1->addAttribute("L_TEX",data_type(t_decimal));
	table_1->addAttribute("L_RETURNFLAG",data_type(t_string),1);
	table_1->addAttribute("L_LINESTATUS",data_type(t_string),1);
	table_1->addAttribute("L_SHIPDATE",data_type(t_date));
	table_1->addAttribute("L_COMMITDATE",data_type(t_date));
	table_1->addAttribute("L_RECEIPTDATE",data_type(t_date));
	table_1->addAttribute("L_SHIPINSTRUCT",data_type(t_string),25);
	table_1->addAttribute("L_SHIPMODE",data_type(t_string),10);
	table_1->addAttribute("L_COMMENT",data_type(t_string),44);

	table_1->createHashPartitionedProjectionOnAllAttribute("L_ORDERKEY",4);

	catalog->add_table(table_1);

	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){
		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
	}
}

static int issue27_single_node(){

	unsigned repeated_times=3;

	init_single_node_tpc_h_envoriment_();
	for(unsigned i=0;i<repeated_times;i++){
		query_select_aggregation();
	}

	Environment::getInstance()->~Environment();
	return 0;
}

static int issue27_multi_node(){
	unsigned repeated_times=2;
	printf("Master or slave?\n");
	int input;
	scanf("%d",&input);
	if(input==0){
		init_multi_node_tpc_h_envoriment_(true);
		for(unsigned i=0;i<repeated_times;i++){
			query_select_aggregation();
		}
	}
	else{
		init_multi_node_tpc_h_envoriment_(false);
	}

	Environment::getInstance()->~Environment();
	return 0;
}

#endif /* TPC_H_TEST_CPP_ */
