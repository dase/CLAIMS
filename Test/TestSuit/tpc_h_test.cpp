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

#include "../../physical_operator/physical_operator_base.h"

#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamAggregationIterator.h"

#include "../../logical_operator/LogicalQueryPlanRoot.h"
#include "../../logical_operator/logical_aggregation.h"
#include "../../logical_operator/logical_scan.h"
#include "../../logical_operator/Filter.h"
#include "../../logical_operator/logical_equal_join.h"

#include "../../common/AttributeComparator.h"

#include "../../common/types/NValue.hpp"

#include "../../utility/rdtsc.h"

#include "../set_up_environment.h"
#include "../../Executor/IteratorExecutorSlave.h"

static void query_1(){
	unsigned long long int start=curtick();
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("LINEITEM");

	LogicalOperator* scan=new LogicalScan(table->getProjectoin(0));


	LogicalFilter::Condition filter_condition_1;
	filter_condition_1.add(table->getAttribute("LINEITEM.L_SHIPDATE"),AttributeComparator::L,std::string("1998-12-01"));
	LogicalOperator* filter=new LogicalFilter(filter_condition_1,scan);
//	LogicalOperator* filter=new Filter();

	std::vector<Attribute> group_by_attributes;
	group_by_attributes.push_back(table->getAttribute("LINEITEM.L_RETURNFLAG"));
	group_by_attributes.push_back(table->getAttribute("LINEITEM.L_LINESTATUS"));
	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(table->getAttribute("LINEITEM.L_QUANTITY"));
	aggregation_attributes.push_back(table->getAttribute("LINEITEM.L_EXTENDEDPRICE"));
	aggregation_attributes.push_back(table->getAttribute("LINEITEM.L_DISCOUNT"));
	aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
	std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;

	aggregation_function.push_back(PhysicalAggregation::State::kSum);
	aggregation_function.push_back(PhysicalAggregation::State::kSum);
	aggregation_function.push_back(PhysicalAggregation::State::kSum);
	aggregation_function.push_back(PhysicalAggregation::State::kCount);
	LogicalOperator* aggregation=new LogicalAggregation(group_by_attributes,aggregation_attributes,aggregation_function,filter);


	LogicalOperator* root=new LogicalQueryPlanRoot(0,filter,LogicalQueryPlanRoot::kPerformance);

	PhysicalOperatorBase* physical_iterator_tree=root->GetPhysicalPlan(64*1024);
//	physical_iterator_tree->print();
	IteratorExecutorSlave::executePhysicalQueryPlan(PhysicalQueryPlan(physical_iterator_tree));
//	physical_iterator_tree->open();
//	while(physical_iterator_tree->next(0));
//	physical_iterator_tree->close();
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


	LogicalFilter::Condition filter_condition_1;
	filter_condition_1.add(region->getAttribute("REGION.R_NAME"),AttributeComparator::EQ,std::string("EUROPE"));
	LogicalOperator* r_filter=new LogicalFilter(filter_condition_1,r_scan);

	std::vector<LogicalEqualJoin::JoinPair> r_n_join_condition;
	r_n_join_condition.push_back(LogicalEqualJoin::JoinPair(region->getAttribute("REGION.R_REGIONKEY"),nation->getAttribute("NATION.N_REGIONKEY")));
	LogicalOperator* r_n_join=new LogicalEqualJoin(r_n_join_condition,r_scan,n_scan);

	std::vector<LogicalEqualJoin::JoinPair> n_s_join_condition;
	n_s_join_condition.push_back(LogicalEqualJoin::JoinPair(nation->getAttribute("NATION.N_NATIONKEY"),supplier->getAttribute("SUPPLIER.S_NATIONKEY")));
	LogicalOperator* n_s_join=new LogicalEqualJoin(n_s_join_condition,r_n_join,s_scan);

	std::vector<LogicalEqualJoin::JoinPair> s_ps_join_condition;
	s_ps_join_condition.push_back(LogicalEqualJoin::JoinPair(supplier->getAttribute("SUPPLIER.S_SUPPKEY"),partsupp->getAttribute("PARTSUPP.PS_SUPPKEY")));
	LogicalOperator* s_ps_join=new LogicalEqualJoin(s_ps_join_condition,n_s_join,ps_scan);

	std::vector<LogicalEqualJoin::JoinPair> p_ps_join_condition;
	p_ps_join_condition.push_back(LogicalEqualJoin::JoinPair(partsupp->getAttribute("PARTSUPP.PS_PARTKEY"),part->getAttribute("PART.P_PARTKEY")));
	LogicalOperator* s_ps_n_join=new LogicalEqualJoin(p_ps_join_condition,s_ps_join,p_scan);



	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(s_ps_join->GetPlanContext().GetAttribute("PARTSUPP.PS_SUPPLYCOST"));
	std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;
	aggregation_function.push_back(PhysicalAggregation::State::kMin);
	LogicalOperator* aggregation=new LogicalAggregation(std::vector<Attribute>(),aggregation_attributes,aggregation_function,s_ps_n_join);

	LogicalOperator* root=new LogicalQueryPlanRoot(0,s_ps_n_join,LogicalQueryPlanRoot::kResultCollector);

	PhysicalOperatorBase* sub_physical_iterator_tree=root->GetPhysicalPlan(64*1024);

	sub_physical_iterator_tree->Open();
	while(sub_physical_iterator_tree->Next(0));
	sub_physical_iterator_tree->Close();

	ResultSet *result_set=sub_physical_iterator_tree->GetResultSet();

	BlockStreamBase::BlockStreamTraverseIterator* it=result_set->createIterator().atomicNextBlock()->createIterator();
	NValue sub_query_result=*(NValue*)it->nextTuple();
	it->~BlockStreamTraverseIterator();
	sub_physical_iterator_tree->~PhysicalOperatorBase();

	LogicalFilter::Condition p_filter_condition_1;
	p_filter_condition_1.add(part->getAttribute("PART.P_SIZE"),AttributeComparator::EQ,std::string("15"));//randomly 0~50
	//TODO like predicates
	LogicalOperator* p_filter=new LogicalFilter(p_filter_condition_1,p_scan);

	LogicalFilter::Condition ps_filter_condition_1;
	ps_filter_condition_1.add(partsupp->getAttribute("PARTSUPP.PS_SUPPLYCOST"),AttributeComparator::EQ,OperateDecimal::toString(sub_query_result));
//	ps_filter_condition_1.add(partsupp->getAttribute("PS_SUPPLYCOST"),AttributeComparator::EQ,std::string("1.00"));
	LogicalOperator* ps_filter=new LogicalFilter(ps_filter_condition_1,ps_scan);

	std::vector<LogicalEqualJoin::JoinPair> p_ps_farther_join_condition;
	p_ps_farther_join_condition.push_back(LogicalEqualJoin::JoinPair(part->getAttribute("PART.P_PARTKEY"),partsupp->getAttribute("PARTSUPP.PS_PARTKEY")));
	LogicalOperator* p_ps_farther_join=new LogicalEqualJoin(p_ps_farther_join_condition,p_scan,ps_filter);



	///////////////////////////

	LogicalFilter::Condition r_filter_father_condition;
	r_filter_father_condition.add(region->getAttribute("REGION.R_NAME"),AttributeComparator::EQ,std::string("AFRICA"));
	LogicalOperator* r_filter_father=new LogicalFilter(r_filter_father_condition,r_scan);

	std::vector<LogicalEqualJoin::JoinPair> r_n_farther_join_condition;
	r_n_farther_join_condition.push_back(LogicalEqualJoin::JoinPair(region->getAttribute("REGION.R_REGIONKEY"),nation->getAttribute("NATION.N_REGIONKEY")));
	LogicalOperator* r_n_farther_join=new LogicalEqualJoin(r_n_farther_join_condition,r_scan,n_scan);

	std::vector<LogicalEqualJoin::JoinPair> r_n_s_farther_join_condition;
	r_n_s_farther_join_condition.push_back(LogicalEqualJoin::JoinPair(nation->getAttribute("NATION.N_NATIONKEY"),supplier->getAttribute("SUPPLIER.S_NATIONKEY")));
	LogicalOperator* r_n_s_farther_join=new LogicalEqualJoin(r_n_s_farther_join_condition,r_n_farther_join,s_scan);

	std::vector<LogicalEqualJoin::JoinPair> r_n_s_p_ps_farther_join_condition;
	r_n_s_p_ps_farther_join_condition.push_back(LogicalEqualJoin::JoinPair(supplier->getAttribute("SUPPLIER.S_SUPPKEY"),partsupp->getAttribute("PARTSUPP.PS_SUPPKEY")));
	LogicalOperator* r_n_s_p_ps_farther_join=new LogicalEqualJoin(r_n_s_p_ps_farther_join_condition,r_n_s_farther_join,p_ps_farther_join);

<<<<<<< HEAD

	LogicalOperator* root_father=new LogicalQueryPlanRoot(0,r_n_s_p_ps_farther_join,LogicalQueryPlanRoot::PERFORMANCE);
=======
	LogicalOperator* root_father=new LogicalQueryPlanRoot(0,r_n_s_p_ps_farther_join,LogicalQueryPlanRoot::kPerformance);
>>>>>>> master-yk-150927
	PhysicalOperatorBase* final_physical_iterator_tree=root_father->GetPhysicalPlan(64*1024);
//
//	final_physical_iterator_tree->open();
//	while(final_physical_iterator_tree->next(0));
//	final_physical_iterator_tree->close();



	printf("Q2: execution time: %4.4f second.\n",getSecond(start));


	final_physical_iterator_tree->~PhysicalOperatorBase();
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




	LogicalFilter::Condition c_filter_condition;
	c_filter_condition.add(customer->getAttribute("CUSTOMER.C_MKTSEGMENT"),AttributeComparator::EQ,std::string("BUILDING"));
	LogicalOperator* c_filter=new LogicalFilter(c_filter_condition,c_scan);

	LogicalFilter::Condition o_filter_condition;
	o_filter_condition.add(orders->getAttribute("ORDERS.O_ORDERDATE"),AttributeComparator::L,std::string("1995-3-15"));
	LogicalOperator* o_filter=new LogicalFilter(o_filter_condition,o_scan);


	std::vector<LogicalEqualJoin::JoinPair> c_o_join_condition;
	c_o_join_condition.push_back(LogicalEqualJoin::JoinPair(customer->getAttribute("CUSTOMER.C_CUSTKEY"),orders->getAttribute("ORDERS.O_CUSTKEY")));
	LogicalOperator* c_o_join=new LogicalEqualJoin(c_o_join_condition,c_filter,o_filter);

	LogicalFilter::Condition l_filter_condition;
	l_filter_condition.add(lineitem->getAttribute("LINEITEM.L_SHIPDATE"),AttributeComparator::GEQ,std::string("1995-3-15"));
	LogicalOperator* l_filter=new LogicalFilter(l_filter_condition,l_scan);

	std::vector<LogicalEqualJoin::JoinPair> c_o_l_join_condition;
	c_o_l_join_condition.push_back(LogicalEqualJoin::JoinPair(orders->getAttribute("ORDERS.O_ORDERKEY"),lineitem->getAttribute("LINEITEM.L_ORDERKEY")));
	LogicalOperator* c_o_l_join=new LogicalEqualJoin(c_o_l_join_condition,c_o_join,l_filter);


	std::vector<Attribute> groupby_attributes;
	groupby_attributes.push_back(c_o_l_join->GetPlanContext().GetAttribute("LINEITEM.L_ORDERKEY"));
	groupby_attributes.push_back(c_o_l_join->GetPlanContext().GetAttribute("ORDERS.O_ORDERDATE"));
	groupby_attributes.push_back(c_o_l_join->GetPlanContext().GetAttribute("ORDERS.O_SHIPPRIORITY"));
	std::vector<Attribute> aggregation_attributes;
	aggregation_attributes.push_back(c_o_l_join->GetPlanContext().GetAttribute("LINEITEM.L_EXTENDEDPRICE"));
	aggregation_attributes.push_back(c_o_l_join->GetPlanContext().GetAttribute("LINEITEM.L_DISCOUNT"));
	std::vector<PhysicalAggregation::State::Aggregation> aggregation_function;
	aggregation_function.push_back(PhysicalAggregation::State::kSum);
	aggregation_function.push_back(PhysicalAggregation::State::kSum);
	LogicalOperator* aggregation=new LogicalAggregation(groupby_attributes,aggregation_attributes,aggregation_function,c_o_l_join);



<<<<<<< HEAD
	LogicalOperator* root=new LogicalQueryPlanRoot(0,Aggregation,LogicalQueryPlanRoot::PERFORMANCE);
=======
	LogicalOperator* root=new LogicalQueryPlanRoot(0,Aggregation,LogicalQueryPlanRoot::kPerformance);
>>>>>>> master-yk-150927
	PhysicalOperatorBase* final_physical_iterator_tree=root->GetPhysicalPlan(64*1024);

	final_physical_iterator_tree->Open();
	while(final_physical_iterator_tree->Next(0));
	final_physical_iterator_tree->Close();

	printf("Q3: execution time: %4.4f second.\n",getSecond(start));


	final_physical_iterator_tree->~PhysicalOperatorBase();
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
	partnames.push_back("/home/claims/data/tpc-h/source/SF-5/part.tbl");
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
	suppliernames.push_back("/home/claims/data/tpc-h/source/SF-5/supplier.tbl");
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
	partsuppnames.push_back("/home/claims/data/tpc-h/source/SF-5/partsupp.tbl");
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
	customernames.push_back("/home/claims/data/tpc-h/source/SF-5/customer.tbl");
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
	ordersnames.push_back("/home/claims/data/tpc-h/source/SF-5/orders.tbl");
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
	nationnames.push_back("/home/claims/data/tpc-h/source/SF-5/nation.tbl");
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
	regionnames.push_back("/home/claims/data/tpc-h/source/SF-5/region.tbl");
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
	lineitemnames.push_back("/home/claims/data/tpc-h/source/SF-5/lineitem.tbl");
	Hl = new HdfsLoader('|','\n',lineitemnames,table_6);
	Hl->load();
	Hl->~HdfsLoader();
	lineitemnames.clear();
	printf("lineitem load succeed!\n");
	sleep(5);
//	return ;
}







static int tcp_h_test_single_node(){
//	load_tpc_h_4_partition();

	unsigned repeated_times=1;

	startup_single_node_environment_of_tpch();
	for(unsigned i=0;i<repeated_times;i++){
		query_1();
	}
	for(unsigned i=0;i<repeated_times;i++){
		query_2();
	}
	for(unsigned i=0;i<repeated_times;i++){
		query_3();
	}

	sleep(1);

	Environment::getInstance()->~Environment();

}
static int tcp_h_test_multi_nodes(){
	unsigned repeated_times=3;

	printf("Master or slave?\n");
	int input;
	scanf("%d",&input);
	if(input==0){

		startup_multiple_node_environment_of_tpch(true);

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
		startup_multiple_node_environment_of_tpch(false);
	}

	while(true) sleep(1);

	Environment::getInstance()->~Environment();

}

#endif /* TPC_H_TEST_CPP_ */
