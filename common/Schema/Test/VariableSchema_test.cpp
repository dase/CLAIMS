/*
 * VariableSchema_test.cpp
 *
 *  Created on: 2014-2-17
 *      Author: casa
 */

#include <vector>
#include <iostream>

#include "../SchemaVar.h"

#include "../../ids.h"
#include "../../AttributeComparator.h"

#include "../../../common/Block/BlockStream.h"

#include "../../../BlockStreamIterator/ParallelBlockStreamIterator/physical_projection_scan.h"
#include "../../../BlockStreamIterator/ParallelBlockStreamIterator/physical_filter.h"

#include "../../../BlockStreamIterator/result_printer.h"
#include "../../../logical_operator/LogicalQueryPlanRoot.h"
#include "../../../logical_operator/Filter.h"
#include "../../../Catalog/ProjectionBinding.h"

#include "../../../Environment.h"
#include "../../../logical_operator/logical_aggregation.h"
#include "../../../logical_operator/logical_equal_join.h"
#include "../../../logical_operator/logical_scan.h"
#include "../../../physical_operator/physical_operator_base.h"

using namespace std;

static int variable_schema_test(){
	Environment::getInstance(true);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();

	TableDescriptor* table_1=new TableDescriptor("1",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	table_1->addAttribute("1",data_type(t_int));  				//0
	table_1->addAttribute("2",data_type(t_double));
	table_1->addAttribute("3",data_type(t_string));

	vector<ColumnOffset> cj_proj0_index;
	cj_proj0_index.push_back(0);
	cj_proj0_index.push_back(1);
	cj_proj0_index.push_back(2);
	const int partition_key_index_1=0;
	table_1->createHashPartitionedProjection(cj_proj0_index,partition_key_index_1,1);	//G0

	cout<<"in the main!!!"<<endl;

	catalog->add_table(table_1);

	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}

	ProjectionBinding *pb=new ProjectionBinding();
	cout<<"in ======================================"<<endl;
	pb->BindingEntireProjection(catalog->getTable(0)->getProjectoin(0)->getPartitioner(),HDFS);

	cout<<"in ======================================"<<endl;

	/*******************scan******************/
	vector<column_type> column_list;
	column_list.push_back(column_type(t_int));
	column_list.push_back(column_type(t_double));
	column_list.push_back(column_type(t_string));
	PhysicalProjectionScan::State scan_state(catalog->getTable(0)->getProjectoin(0)->getProjectionID(),new SchemaVar(column_list),64*1024-sizeof(unsigned));
	PhysicalOperatorBase* scan=new PhysicalProjectionScan(scan_state);
	//------------------------------------------------------------------

	/*******************filter******************/
	PhysicalFilter::State filter_state;

	int f0=1;
	AttributeComparator filter0(column_type(t_int),Comparator::EQ,0,&f0);
	char* str="string123";
	AttributeComparator filter1(column_type(t_string),Comparator::EQ,2,str);
	std::vector<AttributeComparator> ComparatorList;
	ComparatorList.push_back(filter1);

	std::vector<column_type> svc;
	svc.push_back(data_type(t_int));
	svc.push_back(data_type(t_double));
	svc.push_back(data_type(t_string));
	Schema *sv=new SchemaVar(svc);

	filter_state.block_size_=64*1024-sizeof(unsigned);
	filter_state.schema_=sv;
	filter_state.comparator_list_=ComparatorList;
	filter_state.child_=scan;

	PhysicalOperatorBase* filter=new PhysicalFilter(filter_state);
	//------------------------------------------------------------------

	/*******************print******************/
	ResultPrinter::State print_state;
	print_state.block_size_=64*1024-sizeof(unsigned);
	print_state.child_=filter;
	print_state.schema_=filter_state.schema_;
	print_state.spliter_="-|-";

	PhysicalOperatorBase* print=new ResultPrinter(print_state);


	/*******************show******************/
	print->Open();
	print->Next(0);
	print->Close();

//	IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(print,"127.0.0.1");
	return 0;
}
