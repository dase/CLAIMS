/*
 * tpc_h_test.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: wangli
 */

#ifndef TPC_H_TEST_CPP_
#define TPC_H_TEST_CPP_
#include "../../Environment.h"
#include "../../Catalog/table.h"
#include "../../Loader/Hdfsloader.h"
#include "../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../LogicalQueryPlan/Aggregation.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"
#include "../../LogicalQueryPlan/Scan.h"
#include "../../LogicalQueryPlan/Filter.h"
#include "../../LogicalQueryPlan/EqualJoin.h"
#include "../../types/NValue.hpp"
#include "../../utility/rdtsc.h"

static void query_1(){
	unsigned long long int start=curtick();
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("LINEITEM");

	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));


	Filter::Condition filter_condition_1;
	filter_condition_1.add(table->getAttribute("L_SHIPDATE"),FilterIterator::AttributeComparator::L,std::string("1998-12-01"));
	LogicalOperator* filter=new Filter(filter_condition_1,scan);
//	LogicalOperator* filter=new Filter();

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
	LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,filter);


	LogicalOperator* root=new LogicalQueryPlanRoot(0,aggregation,LogicalQueryPlanRoot::PERFORMANCE);

	BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024-sizeof(unsigned));
//	physical_iterator_tree->print();
	physical_iterator_tree->open();
	while(physical_iterator_tree->next(0));
	physical_iterator_tree->close();
	printf("Q1: execution time: %4.4f second.\n",getSecond(start));
}

static void query_2(){
	unsigned long long int start=curtick();

	TableDescriptor* part=Environment::getInstance()->getCatalog()->getTable("PART"); //400
	TableDescriptor* partsupp=Environment::getInstance()->getCatalog()->getTable("PARTSUPP"); //2000
	TableDescriptor* supplier=Environment::getInstance()->getCatalog()->getTable("SUPPLIER"); //28
	TableDescriptor* nation=Environment::getInstance()->getCatalog()->getTable("NATION");	  //2
	TableDescriptor* region=Environment::getInstance()->getCatalog()->getTable("REGION");	  //1

	LogicalOperator* p_scan=new LogicalScan(part->getProjectoin(0));

	LogicalOperator* ps_scan=new LogicalScan(partsupp->getProjectoin(0));

	LogicalOperator* r_scan=new LogicalScan(region->getProjectoin(0));

	LogicalOperator* n_scan=new LogicalScan(nation->getProjectoin(0));

	LogicalOperator* s_scan=new LogicalScan(supplier->getProjectoin(0));


	Filter::Condition filter_condition_1;
	filter_condition_1.add(region->getAttribute("R_NAME"),FilterIterator::AttributeComparator::EQ,std::string("EUROPE"));
	LogicalOperator* r_filter=new Filter(filter_condition_1,r_scan);

	std::vector<EqualJoin::JoinPair> r_n_join_condition;
	r_n_join_condition.push_back(EqualJoin::JoinPair(region->getAttribute("R_REGIONKEY"),nation->getAttribute("N_REGIONKEY")));
	LogicalOperator* r_n_join=new EqualJoin(r_n_join_condition,r_scan,n_scan);

	std::vector<EqualJoin::JoinPair> n_s_join_condition;
	n_s_join_condition.push_back(EqualJoin::JoinPair(nation->getAttribute("N_NATIONKEY"),supplier->getAttribute("S_NATIONKEY")));
	LogicalOperator* n_s_join=new EqualJoin(n_s_join_condition,r_n_join,s_scan);

	std::vector<EqualJoin::JoinPair> s_ps_join_condition;
	s_ps_join_condition.push_back(EqualJoin::JoinPair(supplier->getAttribute("S_SUPPKEY"),partsupp->getAttribute("PS_SUPPKEY")));
	LogicalOperator* s_ps_join=new EqualJoin(s_ps_join_condition,n_s_join,ps_scan);

	std::vector<EqualJoin::JoinPair> p_ps_join_condition;
	p_ps_join_condition.push_back(EqualJoin::JoinPair(partsupp->getAttribute("PS_PARTKEY"),part->getAttribute("P_PARTKEY")));
	LogicalOperator* s_ps_n_join=new EqualJoin(p_ps_join_condition,s_ps_join,p_scan);



	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(s_ps_join->getDataflow().getAttribute("PS_SUPPLYCOST"));
	std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;
	aggregation_function.push_back(BlockStreamAggregationIterator::State::min);
	LogicalOperator* aggregation=new Aggregation(std::vector<Attribute>(),aggregation_attributes,aggregation_function,s_ps_n_join);

	LogicalOperator* root=new LogicalQueryPlanRoot(0,s_ps_n_join,LogicalQueryPlanRoot::RESULTCOLLECTOR);

	BlockStreamIteratorBase* sub_physical_iterator_tree=root->getIteratorTree(64*1024-sizeof(unsigned));

	sub_physical_iterator_tree->open();
	while(sub_physical_iterator_tree->next(0));
	sub_physical_iterator_tree->close();

	ResultSet *result_set=sub_physical_iterator_tree->getResultSet();

	BlockStreamBase::BlockStreamTraverseIterator* it=result_set->createIterator().atomicNextBlock()->createIterator();
	NValue sub_query_result=*(NValue*)it->nextTuple();
	it->~BlockStreamTraverseIterator();
	sub_physical_iterator_tree->~BlockStreamIteratorBase();

	Filter::Condition p_filter_condition_1;
	p_filter_condition_1.add(part->getAttribute("P_SIZE"),FilterIterator::AttributeComparator::EQ,std::string("15"));//randomly 0~50
	//TODO like predicates
	LogicalOperator* p_filter=new Filter(p_filter_condition_1,p_scan);

	Filter::Condition ps_filter_condition_1;
	ps_filter_condition_1.add(partsupp->getAttribute("PS_SUPPLYCOST"),FilterIterator::AttributeComparator::EQ,OperateDecimal::toString(sub_query_result));
//	ps_filter_condition_1.add(partsupp->getAttribute("PS_SUPPLYCOST"),FilterIterator::AttributeComparator::EQ,std::string("1.00"));
	LogicalOperator* ps_filter=new Filter(ps_filter_condition_1,ps_scan);

	std::vector<EqualJoin::JoinPair> p_ps_farther_join_condition;
	p_ps_farther_join_condition.push_back(EqualJoin::JoinPair(part->getAttribute("P_PARTKEY"),partsupp->getAttribute("PS_PARTKEY")));
	LogicalOperator* p_ps_farther_join=new EqualJoin(p_ps_farther_join_condition,p_scan,ps_filter);



	///////////////////////////

	Filter::Condition r_filter_father_condition;
	r_filter_father_condition.add(region->getAttribute("R_NAME"),FilterIterator::AttributeComparator::EQ,std::string("AFRICA"));
	LogicalOperator* r_filter_father=new Filter(r_filter_father_condition,r_scan);

	std::vector<EqualJoin::JoinPair> r_n_farther_join_condition;
	r_n_farther_join_condition.push_back(EqualJoin::JoinPair(region->getAttribute("R_REGIONKEY"),nation->getAttribute("N_REGIONKEY")));
	LogicalOperator* r_n_farther_join=new EqualJoin(r_n_farther_join_condition,r_scan,n_scan);

	std::vector<EqualJoin::JoinPair> r_n_s_farther_join_condition;
	r_n_s_farther_join_condition.push_back(EqualJoin::JoinPair(nation->getAttribute("N_NATIONKEY"),supplier->getAttribute("S_NATIONKEY")));
	LogicalOperator* r_n_s_farther_join=new EqualJoin(r_n_s_farther_join_condition,r_n_farther_join,s_scan);

	std::vector<EqualJoin::JoinPair> r_n_s_p_ps_farther_join_condition;
	r_n_s_p_ps_farther_join_condition.push_back(EqualJoin::JoinPair(supplier->getAttribute("S_SUPPKEY"),partsupp->getAttribute("PS_SUPPKEY")));
	LogicalOperator* r_n_s_p_ps_farther_join=new EqualJoin(r_n_s_p_ps_farther_join_condition,r_n_s_farther_join,p_ps_farther_join);


	LogicalOperator* root_father=new LogicalQueryPlanRoot(0,r_n_s_p_ps_farther_join,LogicalQueryPlanRoot::PERFORMANCE);
	BlockStreamIteratorBase* final_physical_iterator_tree=root_father->getIteratorTree(64*1024-sizeof(unsigned));
//
//	final_physical_iterator_tree->open();
//	while(final_physical_iterator_tree->next(0));
//	final_physical_iterator_tree->close();



	printf("Q2: execution time: %4.4f second.\n",getSecond(start));


	final_physical_iterator_tree->~BlockStreamIteratorBase();
	root->~LogicalOperator();
	root_father->~LogicalOperator();



}
static void query_3(){
	unsigned long long int start=curtick();

	TableDescriptor* customer=Environment::getInstance()->getCatalog()->getTable("CUSTOMER");	//490
	TableDescriptor* orders=Environment::getInstance()->getCatalog()->getTable("ORDERS");		//3000
	TableDescriptor* lineitem=Environment::getInstance()->getCatalog()->getTable("LINEITEM");	//15000



	LogicalOperator* c_scan=new LogicalScan(customer->getProjectoin(0));

	LogicalOperator* o_scan=new LogicalScan(orders->getProjectoin(0));

	LogicalOperator* l_scan=new LogicalScan(lineitem->getProjectoin(0));




	Filter::Condition c_filter_condition;
	c_filter_condition.add(customer->getAttribute("C_MKTSEGMENT"),FilterIterator::AttributeComparator::EQ,std::string("BUILDING"));
	LogicalOperator* c_filter=new Filter(c_filter_condition,c_scan);

	Filter::Condition o_filter_condition;
	o_filter_condition.add(orders->getAttribute("O_ORDERDATE"),FilterIterator::AttributeComparator::L,std::string("1995-3-15"));
	LogicalOperator* o_filter=new Filter(o_filter_condition,o_scan);


	std::vector<EqualJoin::JoinPair> c_o_join_condition;
	c_o_join_condition.push_back(EqualJoin::JoinPair(customer->getAttribute("C_CUSTKEY"),orders->getAttribute("O_CUSTKEY")));
	LogicalOperator* c_o_join=new EqualJoin(c_o_join_condition,c_filter,o_filter);

	Filter::Condition l_filter_condition;
	l_filter_condition.add(lineitem->getAttribute("L_SHIPDATE"),FilterIterator::AttributeComparator::GEQ,std::string("1995-3-15"));
	LogicalOperator* l_filter=new Filter(l_filter_condition,l_scan);

	std::vector<EqualJoin::JoinPair> c_o_l_join_condition;
	c_o_l_join_condition.push_back(EqualJoin::JoinPair(orders->getAttribute("O_ORDERKEY"),lineitem->getAttribute("L_ORDERKEY")));
	LogicalOperator* c_o_l_join=new EqualJoin(c_o_l_join_condition,c_o_join,l_filter);


	std::vector<Attribute> groupby_attributes;
	groupby_attributes.push_back(c_o_l_join->getDataflow().getAttribute("L_ORDERKEY"));
	groupby_attributes.push_back(c_o_l_join->getDataflow().getAttribute("O_ORDERDATE"));
	groupby_attributes.push_back(c_o_l_join->getDataflow().getAttribute("O_SHIPPRIORITY"));
	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(c_o_l_join->getDataflow().getAttribute("L_EXTENDEDPRICE"));
	aggregation_attributes.push_back(c_o_l_join->getDataflow().getAttribute("L_DISCOUNT"));
	std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;
	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
	aggregation_function.push_back(BlockStreamAggregationIterator::State::sum);
	LogicalOperator* aggregation=new Aggregation(groupby_attributes,aggregation_attributes,aggregation_function,c_o_l_join);



	LogicalOperator* root=new LogicalQueryPlanRoot(0,aggregation,LogicalQueryPlanRoot::PERFORMANCE);
	BlockStreamIteratorBase* final_physical_iterator_tree=root->getIteratorTree(64*1024-sizeof(unsigned));

	final_physical_iterator_tree->open();
	while(final_physical_iterator_tree->next(0));
	final_physical_iterator_tree->close();

	printf("Q3: execution time: %4.4f second.\n",getSecond(start));


	final_physical_iterator_tree->~BlockStreamIteratorBase();
	root->~LogicalOperator();



}


static void load_tpc_h_4_partition(){
	Environment::getInstance(true);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();


	/////////////////////////////// PART TABLE //////////////////////////////////
	TableDescriptor* table_1=new TableDescriptor("PART",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
//	TableDescriptor* table_1=new TableDescriptor("PART",0);
	table_1->addAttribute("row_id", data_type(t_u_long));
	table_1->addAttribute("P_PARTKEY",data_type(t_u_long));  				//0
	table_1->addAttribute("P_NAME",data_type(t_string),55);
	table_1->addAttribute("P_MFGR",data_type(t_string),25);
	table_1->addAttribute("P_BRAND",data_type(t_string),10);
	table_1->addAttribute("P_TYPE",data_type(t_string),25);
	table_1->addAttribute("P_SIZE",data_type(t_int));
	table_1->addAttribute("P_CONTAINER",data_type(t_string),10);
	table_1->addAttribute("P_RETAILPRICE",data_type(t_decimal),55);
	table_1->addAttribute("P_COMMENT",data_type(t_string),23);

	table_1->createHashPartitionedProjectionOnAllAttribute("P_PARTKEY",4);
	///////////////////////////////////////////////////////////////////////////////
	std::vector<std::string> partnames;
	partnames.push_back("/home/imdb/data/tpc-h/source/SF-5/part.tbl");
	HdfsLoader* Hl = new HdfsLoader('|','\n',partnames,table_1);
	Hl->load();
	Hl->~HdfsLoader();
	partnames.clear();
	printf("part load succeed!\n");
	sleep(5);
//	return ;
//
//
	/////////////////////////////// SUPPLIER TABLE //////////////////////////////////
	TableDescriptor* table_2=new TableDescriptor("SUPPLIER",1);
	table_2->addAttribute("row_id", data_type(t_u_long));
	table_2->addAttribute("S_SUPPKEY",data_type(t_u_long));  				//0
	table_2->addAttribute("S_NAME",data_type(t_string),55);
	table_2->addAttribute("S_ADDRESS",data_type(t_string),40);
	table_2->addAttribute("S_NATIONKEY",data_type(t_u_long));
	table_2->addAttribute("S_PHONE",data_type(t_string),15);
	table_2->addAttribute("S_ACCTBAL",data_type(t_decimal));
	table_2->addAttribute("S_COMMENT",data_type(t_string),101);

	table_2->createHashPartitionedProjectionOnAllAttribute("S_SUPPKEY",4);
	///////////////////////////////////////////////////////////////////////////////
	std::vector<std::string> suppliernames;
	suppliernames.push_back("/home/imdb/data/tpc-h/source/SF-5/supplier.tbl");
	Hl = new HdfsLoader('|','\n',suppliernames,table_2);
	Hl->load();
	Hl->~HdfsLoader();
	suppliernames.clear();
	printf("supplier load succeed!\n");
	sleep(5);
////	return ;
//
//
	/////////////////////////////// PARTSUPP TABLE //////////////////////////////////
	TableDescriptor* table_3=new TableDescriptor("PARTSUPP",2);
	table_3->addAttribute("row_id", data_type(t_u_long));
	table_3->addAttribute("PS_PARTKEY",data_type(t_u_long));  				//0
	table_3->addAttribute("PS_SUPPKEY",data_type(t_u_long));
	table_3->addAttribute("PS_AVAILQTY",data_type(t_int));
	table_3->addAttribute("PS_SUPPLYCOST",data_type(t_decimal));
	table_3->addAttribute("PS_COMMENT",data_type(t_string),199);

	table_3->createHashPartitionedProjectionOnAllAttribute("PS_PARTKEY",4);
	///////////////////////////////////////////////////////////////////////////////
	std::vector<std::string> partsuppnames;
	partsuppnames.push_back("/home/imdb/data/tpc-h/source/SF-5/partsupp.tbl");
	Hl = new HdfsLoader('|','\n',partsuppnames,table_3);
	Hl->load();
	Hl->~HdfsLoader();
	partsuppnames.clear();
	printf("partsupp load succeed!\n");
	sleep(5);
////	return ;


	/////////////////////////////// CUSTOM TABLE //////////////////////////////////
	TableDescriptor* table_4=new TableDescriptor("CUSTOMER",3);
	table_4->addAttribute("row_id", data_type(t_u_long));
	table_4->addAttribute("C_CUSTKEY",data_type(t_u_long));  				//0
	table_4->addAttribute("C_NAME",data_type(t_string),25);
	table_4->addAttribute("C_ADDRESS",data_type(t_string),40);
	table_4->addAttribute("C_NATIONKEY",data_type(t_u_long));
	table_4->addAttribute("C_PHONE",data_type(t_string),15);
	table_4->addAttribute("C_ACCTBAL",data_type(t_decimal));
	table_4->addAttribute("C_MKTSEGMENT",data_type(t_string),10);
	table_4->addAttribute("C_COMMENT",data_type(t_string),117);

	table_4->createHashPartitionedProjectionOnAllAttribute("C_CUSTKEY",4);
	///////////////////////////////////////////////////////////////////////////////
	std::vector<std::string> customernames;
	customernames.push_back("/home/imdb/data/tpc-h/source/SF-5/customer.tbl");
	Hl = new HdfsLoader('|','\n',customernames,table_4);
	Hl->load();
	Hl->~HdfsLoader();
	customernames.clear();
	printf("custom load succeed!\n");
	sleep(5);
////	return ;


	/////////////////////////////// ORDERS TABLE //////////////////////////////////
	TableDescriptor* table_5=new TableDescriptor("ORDERS",4);
	table_5->addAttribute("row_id", data_type(t_u_long));
	table_5->addAttribute("O_ORDERKEY",data_type(t_u_long));  				//0
	table_5->addAttribute("O_CUSTKEY",data_type(t_u_long));
	table_5->addAttribute("O_ORDERSTATUS",data_type(t_string),1);
	table_5->addAttribute("O_TOTALPRICE",data_type(t_decimal));
	table_5->addAttribute("O_ORDERDATE",data_type(t_date),15);
	table_5->addAttribute("O_ORDERPRIORITY",data_type(t_string),15);
	table_5->addAttribute("O_CLERK",data_type(t_string),15);
	table_5->addAttribute("O_SHIPPRIORITY",data_type(t_int));
	table_5->addAttribute("O_COMMENT",data_type(t_string),79);

	table_5->createHashPartitionedProjectionOnAllAttribute("O_ORDERKEY",4);
	///////////////////////////////////////////////////////////////////////////////
	std::vector<std::string> ordersnames;
	ordersnames.push_back("/home/imdb/data/tpc-h/source/SF-5/orders.tbl");
	Hl = new HdfsLoader('|','\n',ordersnames,table_5);
	Hl->load();
	Hl->~HdfsLoader();
	ordersnames.clear();
	printf("orders load succeed!\n");
	sleep(5);
////	return ;

	/////////////////////////////// NATION TABLE //////////////////////////////////
	TableDescriptor* table_7=new TableDescriptor("NATION",6);
	table_7->addAttribute("row_id", data_type(t_u_long));
	table_7->addAttribute("N_NATIONKEY",data_type(t_u_long));  				//0
	table_7->addAttribute("N_NAME",data_type(t_string),25);
	table_7->addAttribute("N_REGIONKEY",data_type(t_u_long));
	table_7->addAttribute("N_COMMENT",data_type(t_string),152);

	table_7->createHashPartitionedProjectionOnAllAttribute("N_NATIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////
	std::vector<std::string> nationnames;
	nationnames.push_back("/home/imdb/data/tpc-h/source/SF-5/nation.tbl");
	Hl = new HdfsLoader('|','\n',nationnames,table_7);
	Hl->load();
	Hl->~HdfsLoader();
	nationnames.clear();
	printf("nation load succeed!\n");
	sleep(5);
//	return ;

	/////////////////////////////// REGION TABLE //////////////////////////////////
	TableDescriptor* table_8=new TableDescriptor("REGION",7);
	table_8->addAttribute("row_id", data_type(t_u_long));
	table_8->addAttribute("R_REGIONKEY",data_type(t_u_long));  				//0
	table_8->addAttribute("R_NAME",data_type(t_string),25);
	table_8->addAttribute("R_COMMENT",data_type(t_string),152);

	table_8->createHashPartitionedProjectionOnAllAttribute("R_REGIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////
	std::vector<std::string> regionnames;
	regionnames.push_back("/home/imdb/data/tpc-h/source/SF-5/region.tbl");
	Hl = new HdfsLoader('|','\n',regionnames,table_8);
	Hl->load();
	Hl->~HdfsLoader();
	regionnames.clear();
	printf("region load succeed!\n");
	sleep(5);
//	return ;


	/////////////////////////////// LINEITEM TABLE //////////////////////////////////
	TableDescriptor* table_6=new TableDescriptor("LINEITEM",5);
	table_6->addAttribute("row_id", data_type(t_u_long));
	table_6->addAttribute("L_ORDERKEY",data_type(t_u_long));  				//0
	table_6->addAttribute("L_PARTKEY",data_type(t_u_long));
	table_6->addAttribute("L_SUPPKEY",data_type(t_u_long));
	table_6->addAttribute("L_LINENUMBER",data_type(t_u_long));
	table_6->addAttribute("L_QUANTITY",data_type(t_decimal));
	table_6->addAttribute("L_EXTENDEDPRICE",data_type(t_decimal));
	table_6->addAttribute("L_DISCOUNT",data_type(t_decimal));
	table_6->addAttribute("L_TEX",data_type(t_decimal));
	table_6->addAttribute("L_RETURNFLAG",data_type(t_string),1);
	table_6->addAttribute("L_LINESTATUS",data_type(t_string),1);
	table_6->addAttribute("L_SHIPDATE",data_type(t_date));
	table_6->addAttribute("L_COMMITDATE",data_type(t_date));
	table_6->addAttribute("L_RECEIPTDATE",data_type(t_date));
	table_6->addAttribute("L_SHIPINSTRUCT",data_type(t_string),25);
	table_6->addAttribute("L_SHIPMODE",data_type(t_string),10);
	table_6->addAttribute("L_COMMENT",data_type(t_string),44);

	table_6->createHashPartitionedProjectionOnAllAttribute("L_ORDERKEY",4);
	///////////////////////////////////////////////////////////////////////////////
	std::vector<std::string> lineitemnames;
	lineitemnames.push_back("/home/imdb/data/tpc-h/source/SF-5/lineitem.tbl");
	Hl = new HdfsLoader('|','\n',lineitemnames,table_6);
	Hl->load();
	Hl->~HdfsLoader();
	lineitemnames.clear();
	printf("lineitem load succeed!\n");
	sleep(5);
//	return ;
}


static void init_single_node_tpc_h_envoriment(bool master=true){
	Environment::getInstance(master);
	printf("Press any key to continue!\n");
	int input;
	scanf("%d",&input);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();


	/////////////////////////////// PART TABLE //////////////////////////////////
//	TableDescriptor* table_1=new TableDescriptor("PART",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	TableDescriptor* table_1=new TableDescriptor("PART",0);
	table_1->addAttribute("row_id", data_type(t_u_long),0,true);
	table_1->addAttribute("P_PARTKEY",data_type(t_u_long),0,true);  				//0
	table_1->addAttribute("P_NAME",data_type(t_string),55);
	table_1->addAttribute("P_MFGR",data_type(t_string),25);
	table_1->addAttribute("P_BRAND",data_type(t_string),10);
	table_1->addAttribute("P_TYPE",data_type(t_string),25);
	table_1->addAttribute("P_SIZE",data_type(t_int));
	table_1->addAttribute("P_CONTAINER",data_type(t_string),10);
	table_1->addAttribute("P_RETAILPRICE",data_type(t_decimal),4);
	table_1->addAttribute("P_COMMENT",data_type(t_string),23);

	table_1->createHashPartitionedProjectionOnAllAttribute("P_PARTKEY",1);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// SUPPLIER TABLE //////////////////////////////////
	TableDescriptor* table_2=new TableDescriptor("SUPPLIER",1);
	table_2->addAttribute("row_id", data_type(t_u_long),0,true);
	table_2->addAttribute("S_SUPPKEY",data_type(t_u_long),0,true);  				//0
	table_2->addAttribute("S_NAME",data_type(t_string),55);
	table_2->addAttribute("S_ADDRESS",data_type(t_string),40);
	table_2->addAttribute("S_NATIONKEY",data_type(t_u_long));
	table_2->addAttribute("S_PHONE",data_type(t_string),15);
	table_2->addAttribute("S_ACCTBAL",data_type(t_decimal),4);
	table_2->addAttribute("S_COMMENT",data_type(t_string),101);

	table_2->createHashPartitionedProjectionOnAllAttribute("S_SUPPKEY",1);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// PARTSUPP TABLE //////////////////////////////////
	TableDescriptor* table_3=new TableDescriptor("PARTSUPP",2);
	table_3->addAttribute("row_id", data_type(t_u_long),0,true);
	table_3->addAttribute("PS_PARTKEY",data_type(t_u_long),0,true);  				//0
	table_3->addAttribute("PS_SUPPKEY",data_type(t_u_long));
	table_3->addAttribute("PS_AVAILQTY",data_type(t_int));
	table_3->addAttribute("PS_SUPPLYCOST",data_type(t_decimal),2);
	table_3->addAttribute("PS_COMMENT",data_type(t_string),199);

	table_3->createHashPartitionedProjectionOnAllAttribute("PS_PARTKEY",1);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// CUSTOM TABLE //////////////////////////////////
	TableDescriptor* table_4=new TableDescriptor("CUSTOMER",3);
	table_4->addAttribute("row_id", data_type(t_u_long),0,true);
	table_4->addAttribute("C_CUSTKEY",data_type(t_u_long),0,true);  				//0
	table_4->addAttribute("C_NAME",data_type(t_string),25);
	table_4->addAttribute("C_ADDRESS",data_type(t_string),40);
	table_4->addAttribute("C_NATIONKEY",data_type(t_u_long));
	table_4->addAttribute("C_PHONE",data_type(t_string),15);
	table_4->addAttribute("C_ACCTBAL",data_type(t_decimal),4);
	table_4->addAttribute("C_MKTSEGMENT",data_type(t_string),10);
	table_4->addAttribute("C_COMMENT",data_type(t_string),117);

	table_4->createHashPartitionedProjectionOnAllAttribute("C_CUSTKEY",1);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// ORDERS TABLE //////////////////////////////////
	TableDescriptor* table_5=new TableDescriptor("ORDERS",4);
	table_5->addAttribute("row_id", data_type(t_u_long),0,true);
	table_5->addAttribute("O_ORDERKEY",data_type(t_u_long),0,true);  				//0
	table_5->addAttribute("O_CUSTKEY",data_type(t_u_long));
	table_5->addAttribute("O_ORDERSTATUS",data_type(t_string),1);
	table_5->addAttribute("O_TOTALPRICE",data_type(t_decimal),4);
	table_5->addAttribute("O_ORDERDATE",data_type(t_date),15);
	table_5->addAttribute("O_ORDERPRIORITY",data_type(t_string),15);
	table_5->addAttribute("O_CLERK",data_type(t_string),15);
	table_5->addAttribute("O_SHIPPRIORITY",data_type(t_int));
	table_5->addAttribute("O_COMMENT",data_type(t_string),79);

	table_5->createHashPartitionedProjectionOnAllAttribute("O_ORDERKEY",1);//should be 4
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// LINEITEM TABLE //////////////////////////////////
	TableDescriptor* table_6=new TableDescriptor("LINEITEM",5);
	table_6->addAttribute("row_id", data_type(t_u_long),0,true);			//0
	table_6->addAttribute("L_ORDERKEY",data_type(t_u_long),0,true);
	table_6->addAttribute("L_PARTKEY",data_type(t_u_long));
	table_6->addAttribute("L_SUPPKEY",data_type(t_u_long));
	table_6->addAttribute("L_LINENUMBER",data_type(t_u_long));
	table_6->addAttribute("L_QUANTITY",data_type(t_decimal)); 		//5
	table_6->addAttribute("L_EXTENDEDPRICE",data_type(t_decimal),4);
	table_6->addAttribute("L_DISCOUNT",data_type(t_decimal));
	table_6->addAttribute("L_TEX",data_type(t_decimal),4);
	table_6->addAttribute("L_RETURNFLAG",data_type(t_string),1);
	table_6->addAttribute("L_LINESTATUS",data_type(t_string),1);	//10
	table_6->addAttribute("L_SHIPDATE",data_type(t_date));
	table_6->addAttribute("L_COMMITDATE",data_type(t_date));
	table_6->addAttribute("L_RECEIPTDATE",data_type(t_date));
	table_6->addAttribute("L_SHIPINSTRUCT",data_type(t_string),25);
	table_6->addAttribute("L_SHIPMODE",data_type(t_string),10);
	table_6->addAttribute("L_COMMENT",data_type(t_string),44);

	table_6->createHashPartitionedProjectionOnAllAttribute("L_ORDERKEY",1); // the number of partition should be 4.
	///////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// NATION TABLE //////////////////////////////////
	TableDescriptor* table_7=new TableDescriptor("NATION",6);
	table_7->addAttribute("row_id", data_type(t_u_long),0,true);
	table_7->addAttribute("N_NATIONKEY",data_type(t_u_long),0,true);  				//0
	table_7->addAttribute("N_NAME",data_type(t_string),25);
	table_7->addAttribute("N_REGIONKEY",data_type(t_u_long));
	table_7->addAttribute("N_COMMENT",data_type(t_string),152);

	table_7->createHashPartitionedProjectionOnAllAttribute("N_NATIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// REGION TABLE //////////////////////////////////
	TableDescriptor* table_8=new TableDescriptor("REGION",7);
	table_8->addAttribute("row_id", data_type(t_u_long),0,true);
	table_8->addAttribute("R_REGIONKEY",data_type(t_u_long),0,true);  				//0
	table_8->addAttribute("R_NAME",data_type(t_string),25);
	table_8->addAttribute("R_COMMENT",data_type(t_string),152);

	table_8->createHashPartitionedProjectionOnAllAttribute("R_REGIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////



	catalog->add_table(table_1);
	catalog->add_table(table_2);
	catalog->add_table(table_3);
	catalog->add_table(table_4);
	catalog->add_table(table_5);
	catalog->add_table(table_6);
	catalog->add_table(table_7);
	catalog->add_table(table_8);


	//T0
	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
	}

	for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}

	for(unsigned i=0;i<table_3->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(2)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,15);
	}
	//T3
	for(unsigned i=0;i<table_4->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(3)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
	}

	for(unsigned i=0;i<table_5->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(4)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,19);
	}

	//T6
	for(unsigned i=0;i<table_6->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_6->getProjectoin(0)->getPartitioner()->RegisterPartition(i,93);
	}
	for(unsigned i=0;i<table_7->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_7->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
	for(unsigned i=0;i<table_8->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_8->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
}

static void init_multi_node_tpc_h_envoriment(bool master=true){
	Environment::getInstance(master);
	printf("Press any key to continue!\n");
	int input;
	scanf("%d",&input);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();


	/////////////////////////////// PART TABLE //////////////////////////////////
//	TableDescriptor* table_1=new TableDescriptor("PART",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	TableDescriptor* table_1=new TableDescriptor("PART",0);
	table_1->addAttribute("row_id", data_type(t_u_long),0,true);
	table_1->addAttribute("P_PARTKEY",data_type(t_u_long),0,true);  				//0
	table_1->addAttribute("P_NAME",data_type(t_string),55);
	table_1->addAttribute("P_MFGR",data_type(t_string),25);
	table_1->addAttribute("P_BRAND",data_type(t_string),10);
	table_1->addAttribute("P_TYPE",data_type(t_string),25);
	table_1->addAttribute("P_SIZE",data_type(t_int));
	table_1->addAttribute("P_CONTAINER",data_type(t_string),10);
	table_1->addAttribute("P_RETAILPRICE",data_type(t_decimal),4);
	table_1->addAttribute("P_COMMENT",data_type(t_string),23);

	table_1->createHashPartitionedProjectionOnAllAttribute("P_PARTKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// SUPPLIER TABLE //////////////////////////////////
	TableDescriptor* table_2=new TableDescriptor("SUPPLIER",1);
	table_2->addAttribute("row_id", data_type(t_u_long),0,true);
	table_2->addAttribute("S_SUPPKEY",data_type(t_u_long),0,true);  				//0
	table_2->addAttribute("S_NAME",data_type(t_string),55);
	table_2->addAttribute("S_ADDRESS",data_type(t_string),40);
	table_2->addAttribute("S_NATIONKEY",data_type(t_u_long));
	table_2->addAttribute("S_PHONE",data_type(t_string),15);
	table_2->addAttribute("S_ACCTBAL",data_type(t_decimal),4);
	table_2->addAttribute("S_COMMENT",data_type(t_string),101);

	table_2->createHashPartitionedProjectionOnAllAttribute("S_SUPPKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// PARTSUPP TABLE //////////////////////////////////
	TableDescriptor* table_3=new TableDescriptor("PARTSUPP",2);
	table_3->addAttribute("row_id", data_type(t_u_long),0,true);
	table_3->addAttribute("PS_PARTKEY",data_type(t_u_long),0,true);  				//0
	table_3->addAttribute("PS_SUPPKEY",data_type(t_u_long));
	table_3->addAttribute("PS_AVAILQTY",data_type(t_int));
	table_3->addAttribute("PS_SUPPLYCOST",data_type(t_decimal),2);
	table_3->addAttribute("PS_COMMENT",data_type(t_string),199);

	table_3->createHashPartitionedProjectionOnAllAttribute("PS_PARTKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// CUSTOM TABLE //////////////////////////////////
	TableDescriptor* table_4=new TableDescriptor("CUSTOMER",3);
	table_4->addAttribute("row_id", data_type(t_u_long),0,true);
	table_4->addAttribute("C_CUSTKEY",data_type(t_u_long),0,true);  				//0
	table_4->addAttribute("C_NAME",data_type(t_string),25);
	table_4->addAttribute("C_ADDRESS",data_type(t_string),40);
	table_4->addAttribute("C_NATIONKEY",data_type(t_u_long));
	table_4->addAttribute("C_PHONE",data_type(t_string),15);
	table_4->addAttribute("C_ACCTBAL",data_type(t_decimal),4);
	table_4->addAttribute("C_MKTSEGMENT",data_type(t_string),10);
	table_4->addAttribute("C_COMMENT",data_type(t_string),117);

	table_4->createHashPartitionedProjectionOnAllAttribute("C_CUSTKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// ORDERS TABLE //////////////////////////////////
	TableDescriptor* table_5=new TableDescriptor("ORDERS",4);
	table_5->addAttribute("row_id", data_type(t_u_long),0,true);
	table_5->addAttribute("O_ORDERKEY",data_type(t_u_long),0,true);  				//0
	table_5->addAttribute("O_CUSTKEY",data_type(t_u_long));
	table_5->addAttribute("O_ORDERSTATUS",data_type(t_string),1);
	table_5->addAttribute("O_TOTALPRICE",data_type(t_decimal),4);
	table_5->addAttribute("O_ORDERDATE",data_type(t_date),15);
	table_5->addAttribute("O_ORDERPRIORITY",data_type(t_string),15);
	table_5->addAttribute("O_CLERK",data_type(t_string),15);
	table_5->addAttribute("O_SHIPPRIORITY",data_type(t_int));
	table_5->addAttribute("O_COMMENT",data_type(t_string),79);

	table_5->createHashPartitionedProjectionOnAllAttribute("O_ORDERKEY",4);//should be 4
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// LINEITEM TABLE //////////////////////////////////
	TableDescriptor* table_6=new TableDescriptor("LINEITEM",5);
	table_6->addAttribute("row_id", data_type(t_u_long),0,true);			//0
	table_6->addAttribute("L_ORDERKEY",data_type(t_u_long),0,true);
	table_6->addAttribute("L_PARTKEY",data_type(t_u_long));
	table_6->addAttribute("L_SUPPKEY",data_type(t_u_long));
	table_6->addAttribute("L_LINENUMBER",data_type(t_u_long));
	table_6->addAttribute("L_QUANTITY",data_type(t_decimal)); 		//5
	table_6->addAttribute("L_EXTENDEDPRICE",data_type(t_decimal),4);
	table_6->addAttribute("L_DISCOUNT",data_type(t_decimal));
	table_6->addAttribute("L_TEX",data_type(t_decimal),4);
	table_6->addAttribute("L_RETURNFLAG",data_type(t_string),1);
	table_6->addAttribute("L_LINESTATUS",data_type(t_string),1);	//10
	table_6->addAttribute("L_SHIPDATE",data_type(t_date));
	table_6->addAttribute("L_COMMITDATE",data_type(t_date));
	table_6->addAttribute("L_RECEIPTDATE",data_type(t_date));
	table_6->addAttribute("L_SHIPINSTRUCT",data_type(t_string),25);
	table_6->addAttribute("L_SHIPMODE",data_type(t_string),10);
	table_6->addAttribute("L_COMMENT",data_type(t_string),44);

	table_6->createHashPartitionedProjectionOnAllAttribute("L_ORDERKEY",4); // the number of partition should be 4.
	///////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// NATION TABLE //////////////////////////////////
	TableDescriptor* table_7=new TableDescriptor("NATION",6);
	table_7->addAttribute("row_id", data_type(t_u_long),0,true);
	table_7->addAttribute("N_NATIONKEY",data_type(t_u_long),0,true);  				//0
	table_7->addAttribute("N_NAME",data_type(t_string),25);
	table_7->addAttribute("N_REGIONKEY",data_type(t_u_long));
	table_7->addAttribute("N_COMMENT",data_type(t_string),152);

	table_7->createHashPartitionedProjectionOnAllAttribute("N_NATIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// REGION TABLE //////////////////////////////////
	TableDescriptor* table_8=new TableDescriptor("REGION",7);
	table_8->addAttribute("row_id", data_type(t_u_long),0,true);
	table_8->addAttribute("R_REGIONKEY",data_type(t_u_long),0,true);  				//0
	table_8->addAttribute("R_NAME",data_type(t_string),25);
	table_8->addAttribute("R_COMMENT",data_type(t_string),152);

	table_8->createHashPartitionedProjectionOnAllAttribute("R_REGIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////



	catalog->add_table(table_1);
	catalog->add_table(table_2);
	catalog->add_table(table_3);
	catalog->add_table(table_4);
	catalog->add_table(table_5);
	catalog->add_table(table_6);
	catalog->add_table(table_7);
	catalog->add_table(table_8);


	//T0
	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
	}

	for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}

	for(unsigned i=0;i<table_3->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(2)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,15);
	}
	//T3
	for(unsigned i=0;i<table_4->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(3)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,3);
	}

	for(unsigned i=0;i<table_5->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(4)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,19);
	}

	//T6
	for(unsigned i=0;i<table_6->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_6->getProjectoin(0)->getPartitioner()->RegisterPartition(i,93);
	}
	for(unsigned i=0;i<table_7->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_7->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
	for(unsigned i=0;i<table_8->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		table_8->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
}







static int tcp_h_test_single_node(){
//	load_tpc_h_4_partition();

	unsigned repeated_times=1;

	init_single_node_tpc_h_envoriment();
	for(unsigned i=0;i<repeated_times;i++){
		query_1();
	}
	for(unsigned i=0;i<repeated_times;i++){
		query_2();
	}
	for(unsigned i=0;i<repeated_times;i++){
		query_3();
	}


	Environment::getInstance()->~Environment();

}
static int tcp_h_test_multi_nodes(){

	unsigned repeated_times=3;
	printf("Master or slave?\n");
	int input;
	scanf("%d",&input);
	if(input==0){
		init_multi_node_tpc_h_envoriment(true);
		for(unsigned i=0;i<repeated_times;i++){
			query_1();
		}
		for(unsigned i=0;i<repeated_times;i++){
			query_2();
		}
		for(unsigned i=0;i<repeated_times;i++){
			query_3();
		}
	}
	else{
		init_multi_node_tpc_h_envoriment(false);
	}


	Environment::getInstance()->~Environment();

}

#endif /* TPC_H_TEST_CPP_ */
