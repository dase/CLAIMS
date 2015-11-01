/*
 * tpc_h_test.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: wangli
 */
//This file will not be used in current version,because it is not contained by Makefile.am
#ifndef ISSUE27_SORT_CPP_
#define ISSUE27_SORT_CPP_
#include "../../Environment.h"

#include "../../Catalog/table.h"

#include "../../Loader/Hdfsloader.h"

#include "../../logical_operator/Scan.h"
#include "../../logical_operator/Filter.h"
#include "../../logical_operator/Sort.h"
#include "../../logical_operator/EqualJoin.h"
#include "../../logical_operator/LogicalQueryPlanRoot.h"
#include "../../logical_operator/Aggregation.h"

#include "../../common/types/NValue.hpp"

#include "../../utility/rdtsc.h"

#include "../../common/ExpressionItem.h"
#include "../../common/ExpressionCalculator.h"
#include "../../logical_operator/logical_project.h"
#include "../../physical_operator/physical_aggregation.h"

static void query_select_sort() {
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
	ExpressionItem ei1_1;
	ExpressionItem ei1_2;
	ExpressionItem ei1_3;
	ei1_1.setVariable("LINEITEM.L_ORDERKEY");
	ei1_2.setIntValue("1");
	ei1_3.setOperator("+");

	expr1.push_back(ei1_1);
	expr1.push_back(ei1_2);
	expr1.push_back(ei1_3);

	expr_list1.push_back(expr1);

	LogicalOperator* project1=new LogicalProject(scan,expr_list1);

	//==========================project=========================
	vector< vector<ExpressionItem> >expr_list2;

	ExpressionItem ei21_1;
	ei21_1.setVariable("LINEITEM.L_ORDERKEY+1");
	vector<ExpressionItem> expr21;
	expr21.push_back(ei21_1);
	expr_list2.push_back(expr21);
	LogicalOperator* project2=new LogicalProject(project1,expr_list2);

	//============================sort==========================
	vector<LogicalSort::OrderByAttr*> vo;

	LogicalSort::OrderByAttr tmp=LogicalSort::OrderByAttr("LINEITEM.L_ORDERKEY+1",0);
	vo.push_back(&tmp);
	LogicalOperator* sort=new LogicalSort(project1,vo);

	//===========================root===========================
	LogicalOperator* root=new LogicalQueryPlanRoot(0,sort,LogicalQueryPlanRoot::PRINT);

	cout<<"performance is ok!"<<endl;
	PhysicalOperatorBase* physical_iterator_tree=root->GetPhysicalPlan(64*1024);
//	physical_iterator_tree->print();
	physical_iterator_tree->Open();
	while(physical_iterator_tree->Next(0));
	physical_iterator_tree->Close();
	printf("Q1: execution time: %4.4f second.\n",getSecond(start));
}
static void get_orderby_att(vector<LogicalSort::OrderByAttr * >  & vo)
{
	string orderstr1="LINEITEM.L_LINESTATUS";
	string orderstr2="LINEITEM.L_RETURNFLAG";

//	LogicalSort::OrderByAttr * ob1=new LogicalSort::OrderByAttr(orderstr1.c_str());
	LogicalSort::OrderByAttr tmp1(string(orderstr1).c_str(),0);
	LogicalSort::OrderByAttr tmp2(string(orderstr2).c_str(),0);
	vo.push_back(&tmp1);
	vo.push_back(&tmp2);
//	vo.push_back(ob1);
//	LogicalSort::OrderByAttr * ob2=new LogicalSort::OrderByAttr(orderstr2.c_str());
//	printf("ob1=%x ob2=%x\n",ob1,ob2);
//	printf("st1=%x st2=%x\n",orderstr1.c_str(),orderstr2.c_str());
//	vo.push_back(ob2);
}

static void query_select_sort_string() {
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

//	vector<ExpressionItem> expr1;
	vector<ExpressionItem> expr2;
	vector<ExpressionItem> expr3;
	ExpressionItem ei1_1;
	ExpressionItem ei1_2;
	ExpressionItem ei1_3;
	ExpressionItem ei2;
	ExpressionItem ei3;
//	ei1_1.setVariable("LINEITEM.L_ORDERKEY");
//	ei1_2.setIntValue("1");
//	ei1_3.setOperator("+");
	ei2.setVariable("LINEITEM.L_RETURNFLAG");
	ei3.setVariable("LINEITEM.L_LINESTATUS");

//	expr1.push_back(ei1_1);
//	expr1.push_back(ei1_2);
//	expr1.push_back(ei1_3);
	expr2.push_back(ei2);
	expr3.push_back(ei3);

//	expr_list1.push_back(expr1);
	expr_list1.push_back(expr2);
	expr_list1.push_back(expr3);

	LogicalOperator* project1=new LogicalProject(scan,expr_list1);

	//============================groupby==========================
//	std::vector<Attribute> group_by_attributes;
//	group_by_attributes.push_back(project1->getDataflow().getAttribute("LINEITEM.L_RETURNFLAG"));
//	group_by_attributes.push_back(project1->getDataflow().getAttribute("LINEITEM.L_LINESTATUS"));
//	std::vector<Attribute> aggregation_attributes;
//	aggregation_attributes.push_back(table->getAttribute("L_QUANTITY"));
//	aggregation_attributes.push_back(table->getAttribute("L_EXTENDEDPRICE"));
//	aggregation_attributes.push_back(table->getAttribute("L_DISCOUNT"));
//	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
//	std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;

//	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
//	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
//	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
//	aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
//	LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,project1);
	//==========================project=========================
//	vector< vector<ExpressionItem> >expr_list2;
//
////	ExpressionItem ei21_1;
//	ExpressionItem ei22;
//	ExpressionItem ei23;
////	ei21_1.setVariable("LINEITEM.L_ORDERKEY+1");
//	ei22.setVariable("LINEITEM.L_RETURNFLAG");
//	ei23.setVariable("LINEITEM.L_LINESTATUS");
////	vector<ExpressionItem> expr21;
//	vector<ExpressionItem> expr22;
//	vector<ExpressionItem> expr23;
////	expr21.push_back(ei21_1);
//	expr22.push_back(ei22);
//	expr23.push_back(ei23);
////	expr_list2.push_back(expr21);
//	expr_list2.push_back(expr22);
//	expr_list2.push_back(expr23);
//	LogicalOperator* project2=new LogicalProject(aggregation,expr_list2);



	//============================sort==========================
	vector<LogicalSort::OrderByAttr*>vo;
//	vo.push_back(&LogicalSort::OrderByAttr("LINEITEM.L_ORDERKEY+1"));
	//get_orderby_att(vo);
//	char * orderstr1="LINEITEM.L_LINESTATUS";
//	char * orderstr2="LINEITEM.L_RETURNFLAG";
	LogicalSort::OrderByAttr tmp1("LINEITEM.L_LINESTATUS",1);
	LogicalSort::OrderByAttr tmp2("LINEITEM.L_RETURNFLAG",0);
	vo.push_back(&tmp1);
	vo.push_back(&tmp2);
	for(int i=0;i<vo.size();i++)
	{
		printf("vo[%d]= %s len=%d  aa=%x  sa=%x\n",i,(vo[i])->ta_,strlen(vo[i]->ta_) ,vo[i],vo[i]->ta_);
	}
	LogicalOperator* sort1=new LogicalSort(project1,vo);
	sort1->Print();
	cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;

	//===========================root===========================
	LogicalOperator* root=new LogicalQueryPlanRoot(0,sort1,LogicalQueryPlanRoot::PRINT);
	root->Print();
	cout<<"performance is ok!"<<endl;
	getchar();
	getchar();
//	BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024);
////	physical_iterator_tree->print();
//	physical_iterator_tree->open();
//	while(physical_iterator_tree->next(0));
//	physical_iterator_tree->close();
	printf("Q1: execution time: %4.4f second.\n",getSecond(start));
}

static void init_single_node_tpc_h_envoriment_sort(bool master=true){
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

static int issue27_sort_single_node(){

	unsigned repeated_times=1;

	init_single_node_tpc_h_envoriment_sort();
	for(unsigned i=0;i<repeated_times;i++){
//		query_select_aggregation_ing();
//		query_select_fzh();
//		query_select_sort();
		query_select_sort_string();
	}

	Environment::getInstance()->~Environment();
	return 0;
}

#endif /* TPC_H_TEST_CPP_ */
