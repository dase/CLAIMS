///*
// * tpc_h_test.cpp
// *
// *  Created on: Apr 1, 2014
// *      Author: wangli
// */
//
//#ifndef TEST_NEW
//#define TEST_NEW
//#include "../../Environment.h"
//#include "../../Catalog/table.h"
//#include "../../Loader/Hdfsloader.h"
//#include "../../logical_operator/LogicalQueryPlanRoot.h"
//#include "../../logical_operator/logical_aggregation.h"
//#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"
//#include "../../logical_operator/logical_scan.h"
//#include "../../logical_operator/Filter.h"
//#include "../../logical_operator/Project.h"
//#include "../../logical_operator/logical_equal_join.h"
//#include "../../common/types/NValue.hpp"
//#include "../../utility/rdtsc.h"
//#include "../../common/ExpressionItem.h"
//#include "../../common/ExpressionCalculator.h"
//<<<<<<< HEAD
////
////static void query_select_aggregation(){
////	/*
////	 * select sum(a+1)+count(a),b
////	 * from T
////	 * group by b
////	 *
////	 * notation: p a p s
////	 * */
////	unsigned long long int start=curtick();
////	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("PART");
////	//===========================scan===========================
////	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));
////
////	//==========================project=========================
////	vector< vector<ExpressionItem> >expr_list;
////
////	Expression expr1;
////	Expression expr2;
////	Expression expr3;
////	Expression expr9;
////
////	ExpressionItem expr_item1;
////	ExpressionItem expr_item2;
////	ExpressionItem expr_item3;
////	ExpressionItem expr_item9;
////
////	expr_item1.setVariable("PART","row_id");
////	expr_item2.setVariable("PART","P_PARTKEY");
////	expr_item3.setVariable("PART","P_NAME");
////	expr_item3.size=54;
////	expr_item9.setVariable("PART","P_RETAILPRICE");
////
////	expr1.push_back(expr_item1);
////	expr2.push_back(expr_item2);
////	expr3.push_back(expr_item3);
////	expr9.push_back(expr_item9);
////
////	expr_list.push_back(expr1);
////	expr_list.push_back(expr2);
////	expr_list.push_back(expr3);
////	expr_list.push_back(expr9);
////
////	LogicalOperator* project1=new LogicalProject(scan,expr_list);
////
////	//========================aggregation=======================
////		std::vector<Attribute> group_by_attributes;
////		group_by_attributes.push_back(table->getAttribute("L_RETURNFLAG"));
////		group_by_attributes.push_back(table->getAttribute("L_LINESTATUS"));
////		std::vector<Attribute> aggregation_attributes;
////		aggregation_attributes.push_back(table->getAttribute("L_QUANTITY"));
////		aggregation_attributes.push_back(table->getAttribute("L_EXTENDEDPRICE"));
////		aggregation_attributes.push_back(table->getAttribute("L_DISCOUNT"));
////		aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
////		std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;
////
////		aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
////		aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
////		aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
////		aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
////		LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,filter);
////
////	//==========================project=========================
////	vector< vector<ExpressionItem> >expr_list;
////
////	Expression expr1;
////	Expression expr2;
////	Expression expr3;
////	Expression expr9;
////
////	ExpressionItem expr_item1;
////	ExpressionItem expr_item2;
////	ExpressionItem expr_item3;
////	ExpressionItem expr_item9;
////
////	expr_item1.setVariable("PART","row_id");
////	expr_item2.setVariable("PART","P_PARTKEY");
////	expr_item3.setVariable("PART","P_NAME");
////	expr_item3.size=54;
////	expr_item9.setVariable("PART","P_RETAILPRICE");
////
////	expr1.push_back(expr_item1);
////	expr2.push_back(expr_item2);
////	expr3.push_back(expr_item3);
////	expr9.push_back(expr_item9);
////
////	expr_list.push_back(expr1);
////	expr_list.push_back(expr2);
////	expr_list.push_back(expr3);
////	expr_list.push_back(expr9);
////
////	LogicalOperator* project2=new LogicalProject(scan,expr_list);
////	//===========================root===========================
////	LogicalOperator* root=new LogicalQueryPlanRoot(0,project2,LogicalQueryPlanRoot::PERFORMANCE);
////
////	cout<<"performance is ok!"<<endl;
////	BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024);
//////	physical_iterator_tree->print();
////	physical_iterator_tree->open();
////	while(physical_iterator_tree->next(0));
////	physical_iterator_tree->close();
////	printf("Q1: execution time: %4.4f second.\n",getSecond(start));
////
////}
//=======
//#include "../set_up_environment.h"
//>>>>>>> refs/remotes/origin/master
//
//static void query_select_a_b_c_d(){
//	unsigned long long int start=curtick();
//	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("PART");
//	//===========================scan===========================
//	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));
//
//	//==========================project=========================
//	vector< vector<ExpressionItem> >expr_list;
//
//	Expression expr1;
//	Expression expr2;
//	Expression expr3;
//	Expression expr9;
//
//	ExpressionItem expr_item1;
//	ExpressionItem expr_item2;
//	ExpressionItem expr_item3;
//	ExpressionItem expr_item9;
//
//	expr_item1.setVariable("PART","row_id");
//	expr_item2.setVariable("PART","P_PARTKEY");
//	expr_item3.setVariable("PART","P_NAME");
//	expr_item3.size=54;
//	expr_item9.setVariable("PART","P_RETAILPRICE");
//
//	expr1.push_back(expr_item1);
//	expr2.push_back(expr_item2);
//	expr3.push_back(expr_item3);
//	expr9.push_back(expr_item9);
//
//	expr_list.push_back(expr1);
//	expr_list.push_back(expr2);
//	expr_list.push_back(expr3);
//	expr_list.push_back(expr9);
//
//	LogicalOperator* project=new LogicalProject(scan,expr_list);
//
//	//===========================root===========================
//	LogicalOperator* root=new LogicalQueryPlanRoot(0,project,LogicalQueryPlanRoot::PERFORMANCE);
//
//	cout<<"performance is ok!"<<endl;
//	BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024);
////	physical_iterator_tree->print();
//	physical_iterator_tree->open();
//	while(physical_iterator_tree->next(0));
//	physical_iterator_tree->close();
//	printf("Q1: execution time: %4.4f second.\n",getSecond(start));
//
//}
//
//static void query_select_star(){
//	unsigned long long int start=curtick();
//	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("PART");
//
////	Filter::Condition filter_condition_1;
////	filter_condition_1.add(table->getAttribute("L_SHIPDATE"),FilterIterator::AttributeComparator::L,std::string("1998-12-01"));
////	LogicalOperator* filter=new Filter(filter_condition_1,scan);
//////	LogicalOperator* filter=new Filter();
////
////	std::vector<Attribute> group_by_attributes;
////	group_by_attributes.push_back(table->getAttribute("L_RETURNFLAG"));
////	group_by_attributes.push_back(table->getAttribute("L_LINESTATUS"));
////	std::vector<Attribute> aggregation_attributes;
////	aggregation_attributes.push_back(table->getAttribute("L_QUANTITY"));
////	aggregation_attributes.push_back(table->getAttribute("L_EXTENDEDPRICE"));
////	aggregation_attributes.push_back(table->getAttribute("L_DISCOUNT"));
////	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
////	std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;
////
////	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
////	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
////	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
////	aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
////	LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,filter);
//
//	//===========================scan===========================
//	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));
//
//	//==========================project=========================
//	vector< vector<ExpressionItem> >expr_list;
//
//	Expression expr1;
//	Expression expr2;
//	Expression expr3;
//	Expression expr4;
//	Expression expr5;
//	Expression expr6;
//	Expression expr7;
//	Expression expr8;
//	Expression expr9;
//	Expression expr10;
//
//	ExpressionItem expr_item1;
//	ExpressionItem expr_item2;
//	ExpressionItem expr_item3;
//	ExpressionItem expr_item4;
//	ExpressionItem expr_item5;
//	ExpressionItem expr_item6;
//	ExpressionItem expr_item7;
//	ExpressionItem expr_item8;
//	ExpressionItem expr_item9;
//	ExpressionItem expr_item10;
//
//	expr_item1.setVariable("PART","row_id");
//	expr_item2.setVariable("PART","P_PARTKEY");
//	expr_item3.setVariable("PART","P_NAME");
//	expr_item3.size=55;
//	expr_item4.setVariable("PART","P_MFGR");
//	expr_item4.size=25;
//	expr_item5.setVariable("PART","P_BRAND");
//	expr_item5.size=10;
//	expr_item6.setVariable("PART","P_TYPE");
//	expr_item6.size=25;
//	expr_item7.setVariable("PART","P_SIZE");
//	expr_item8.setVariable("PART","P_CONTAINER");
//	expr_item8.size=10;
//	expr_item9.setVariable("PART","P_RETAILPRICE");
//	expr_item10.setVariable("PART","P_COMMENT");
//	expr_item10.size=23;
//
//	expr1.push_back(expr_item1);
//	expr2.push_back(expr_item2);
//	expr3.push_back(expr_item3);
//	expr4.push_back(expr_item4);
//	expr5.push_back(expr_item5);
//	expr6.push_back(expr_item6);
//	expr7.push_back(expr_item7);
//	expr8.push_back(expr_item8);
//	expr9.push_back(expr_item9);
//	expr10.push_back(expr_item10);
//
//	expr_list.push_back(expr1);
//	expr_list.push_back(expr2);
//	expr_list.push_back(expr3);
//	expr_list.push_back(expr4);
//	expr_list.push_back(expr5);
//	expr_list.push_back(expr6);
//	expr_list.push_back(expr7);
//	expr_list.push_back(expr8);
//	expr_list.push_back(expr9);
//	expr_list.push_back(expr10);
//
//	LogicalOperator* project=new LogicalProject(scan,expr_list);
//
//	//===========================root===========================
//	LogicalOperator* root=new LogicalQueryPlanRoot(0,project,LogicalQueryPlanRoot::PRINT);
//
//	cout<<"performance is ok!"<<endl;
//	BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024);
////	physical_iterator_tree->print();
//	physical_iterator_tree->open();
//	while(physical_iterator_tree->next(0));
//	physical_iterator_tree->close();
//	printf("Q1: execution time: %4.4f second.\n",getSecond(start));
//
//}
//
//<<<<<<< HEAD
//static void init_single_node_tpc_h_envoriment(bool master=true){
//	Environment::getInstance(master);
//	printf("Press any key to continue!\n");
//	int input;
//	scanf("%d",&input);
//	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
//	Catalog* catalog=Environment::getInstance()->getCatalog();
//
//	/////////////////////////////// PART TABLE //////////////////////////////////
//	TableDescriptor* table_1=new TableDescriptor("PART",0);
//	table_1->addAttribute("row_id", data_type(t_u_long),0,true);
//	table_1->addAttribute("P_PARTKEY",data_type(t_u_long),0,true);  				//0
//	table_1->addAttribute("P_NAME",data_type(t_string),55);
//	table_1->addAttribute("P_MFGR",data_type(t_string),25);
//	table_1->addAttribute("P_BRAND",data_type(t_string),10);
//	table_1->addAttribute("P_TYPE",data_type(t_string),25);
//	table_1->addAttribute("P_SIZE",data_type(t_int));
//	table_1->addAttribute("P_CONTAINER",data_type(t_string),10);
//	table_1->addAttribute("P_RETAILPRICE",data_type(t_decimal),4);
//	table_1->addAttribute("P_COMMENT",data_type(t_string),23);
//
//	table_1->createHashPartitionedProjectionOnAllAttribute("P_PARTKEY",1);//should be 4
//
//	catalog->add_table(table_1);
//
//	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){
//
//		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
//	}
//}
//
//static void init_multi_node_tpc_h_envoriment(bool master=true){
//	Environment::getInstance(master);
//	printf("Press any key to continue!\n");
//	int input;
//	scanf("%d",&input);
//	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
//	Catalog* catalog=Environment::getInstance()->getCatalog();
//
//	/////////////////////////////// PART TABLE //////////////////////////////////
//	TableDescriptor* table_1=new TableDescriptor("PART",0);
//	table_1->addAttribute("row_id", data_type(t_u_long),0,true);
//	table_1->addAttribute("P_PARTKEY",data_type(t_u_long),0,true);  				//0
//	table_1->addAttribute("P_NAME",data_type(t_string),55);
//	table_1->addAttribute("P_MFGR",data_type(t_string),25);
//	table_1->addAttribute("P_BRAND",data_type(t_string),10);
//	table_1->addAttribute("P_TYPE",data_type(t_string),25);
//	table_1->addAttribute("P_SIZE",data_type(t_int));
//	table_1->addAttribute("P_CONTAINER",data_type(t_string),10);
//	table_1->addAttribute("P_RETAILPRICE",data_type(t_decimal),4);
//	table_1->addAttribute("P_COMMENT",data_type(t_string),23);
//
//	table_1->createHashPartitionedProjectionOnAllAttribute("P_PARTKEY",4);//should be 4
//
//	catalog->add_table(table_1);
//
//	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){
//		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
//	}
//}
//
//static int common_project_tcp_h_test_single_node(){
//
//	unsigned repeated_times=3;
//
//	init_single_node_tpc_h_envoriment();
//	for(unsigned i=0;i<repeated_times;i++){
//		query_select_a_b_c_d();
////		query_select_star();
//	}
//
//	Environment::getInstance()->~Environment();
//	return 0;
//}
//
//=======
//>>>>>>> refs/remotes/origin/master
//static int common_project_tcp_h_test_multi_nodes(){
//	unsigned repeated_times=2;
//	printf("Master or slave?\n");
//	int input;
//	scanf("%d",&input);
//	if(input==0){
//		startup_multiple_node_environment_of_tpch(true);
//		for(unsigned i=0;i<repeated_times;i++){
////			query_select_star();
//			query_select_a_b_c_d();
//		}
//	}
//	else{
//		startup_multiple_node_environment_of_tpch(false);
//	}
//
//	Environment::getInstance()->~Environment();
//	return 0;
//}
//
//#endif /* TPC_H_TEST_CPP_ */
