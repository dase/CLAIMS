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

static void query_1(){
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


	LogicalOperator* root=new LogicalQueryPlanRoot(0,aggregation,LogicalQueryPlanRoot::PRINT);

	BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024-sizeof(unsigned));

	physical_iterator_tree->open();
	while(physical_iterator_tree->next(0));
	physical_iterator_tree->close();

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
	partnames.push_back("/home/imdb/data/tpc-h/SF-1/part.tbl");
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
	table_2->addAttribute("S_PARTKEY",data_type(t_u_long));  				//0
	table_2->addAttribute("S_NAME",data_type(t_string),55);
	table_2->addAttribute("S_ADDRESS",data_type(t_string),40);
	table_2->addAttribute("S_NATIONKEY",data_type(t_u_long));
	table_2->addAttribute("S_PHONE",data_type(t_string),15);
	table_2->addAttribute("S_ACCTBAL",data_type(t_decimal));
	table_2->addAttribute("S_COMMENT",data_type(t_string),101);

	table_2->createHashPartitionedProjectionOnAllAttribute("S_PARTKEY",4);
	///////////////////////////////////////////////////////////////////////////////
	std::vector<std::string> suppliernames;
	suppliernames.push_back("/home/imdb/data/tpc-h/SF-1/supplier.tbl");
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
	partsuppnames.push_back("/home/imdb/data/tpc-h/SF-1/partsupp.tbl");
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
	customernames.push_back("/home/imdb/data/tpc-h/SF-1/customer.tbl");
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
	ordersnames.push_back("/home/imdb/data/tpc-h/SF-1/orders.tbl");
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
	nationnames.push_back("/home/imdb/data/tpc-h/SF-1/nation.tbl");
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
	regionnames.push_back("/home/imdb/data/tpc-h/SF-1/region.tbl");
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
	lineitemnames.push_back("/home/imdb/data/tpc-h/SF-1/lineitem.tbl");
	Hl = new HdfsLoader('|','\n',lineitemnames,table_6);
	Hl->load();
	Hl->~HdfsLoader();
	lineitemnames.clear();
	printf("lineitem load succeed!\n");
	sleep(5);
//	return ;
}


static void init_single_node_tpc_h_envoriment(){
	Environment::getInstance(true);
	sleep(1);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();


	/////////////////////////////// PART TABLE //////////////////////////////////
//	TableDescriptor* table_1=new TableDescriptor("PART",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	TableDescriptor* table_1=new TableDescriptor("PART",0);
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


	/////////////////////////////// SUPPLIER TABLE //////////////////////////////////
	TableDescriptor* table_2=new TableDescriptor("SUPPLIER",1);
	table_2->addAttribute("row_id", data_type(t_u_long));
	table_2->addAttribute("S_PARTKEY",data_type(t_u_long));  				//0
	table_2->addAttribute("S_NAME",data_type(t_string),55);
	table_2->addAttribute("S_ADDRESS",data_type(t_string),40);
	table_2->addAttribute("S_NATIONKEY",data_type(t_u_long));
	table_2->addAttribute("S_PHONE",data_type(t_string),15);
	table_2->addAttribute("S_ACCTBAL",data_type(t_decimal));
	table_2->addAttribute("S_COMMENT",data_type(t_string),101);

	table_2->createHashPartitionedProjectionOnAllAttribute("S_PARTKEY",4);
	///////////////////////////////////////////////////////////////////////////////


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

	/////////////////////////////// NATION TABLE //////////////////////////////////
	TableDescriptor* table_7=new TableDescriptor("NATION",6);
	table_7->addAttribute("row_id", data_type(t_u_long));
	table_7->addAttribute("N_NATIONKEY",data_type(t_u_long));  				//0
	table_7->addAttribute("N_NAME",data_type(t_string),25);
	table_7->addAttribute("N_REGIONKEY",data_type(t_u_long));
	table_7->addAttribute("N_COMMENT",data_type(t_string),152);

	table_7->createHashPartitionedProjectionOnAllAttribute("N_NATIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// REGION TABLE //////////////////////////////////
	TableDescriptor* table_8=new TableDescriptor("REGION",7);
	table_8->addAttribute("row_id", data_type(t_u_long));
	table_8->addAttribute("R_REGIONKEY",data_type(t_u_long));  				//0
	table_8->addAttribute("R_NAME",data_type(t_string),25);
	table_8->addAttribute("R_COMMENT",data_type(t_string),152);

	table_8->createHashPartitionedProjectionOnAllAttribute("R_REGIONKEY",1);
	///////////////////////////////////////////////////////////////////////////////


	/////////////////////////////// LINEITEM TABLE //////////////////////////////////
	TableDescriptor* table_6=new TableDescriptor("LINEITEM",5);
	table_6->addAttribute("row_id", data_type(t_u_long));			//0
	table_6->addAttribute("L_ORDERKEY",data_type(t_u_long));
	table_6->addAttribute("L_PARTKEY",data_type(t_u_long));
	table_6->addAttribute("L_SUPPKEY",data_type(t_u_long));
	table_6->addAttribute("L_LINENUMBER",data_type(t_u_long));
	table_6->addAttribute("L_QUANTITY",data_type(t_decimal)); 		//5
	table_6->addAttribute("L_EXTENDEDPRICE",data_type(t_decimal));
	table_6->addAttribute("L_DISCOUNT",data_type(t_decimal));
	table_6->addAttribute("L_TEX",data_type(t_decimal));
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

	for(unsigned i=0;i<table_6->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(5)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,94);
	}
	//T6
	for(unsigned i=0;i<table_7->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(6)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}

	for(unsigned i=0;i<table_8->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(7)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}






}









static int tcp_h_test(){
//	load_tpc_h_4_partition();

	init_single_node_tpc_h_envoriment();
//	sleep(1);
	query_1();
//	sleep(1);
	Environment::getInstance()->~Environment();
}


#endif /* TPC_H_TEST_CPP_ */
