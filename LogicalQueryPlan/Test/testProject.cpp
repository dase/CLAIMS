/*
 * testProject.cpp
 *
 *  Created on: 2014-3-2
 *      Author: casa
 */

#include <vector>
#include <iostream>
#include "../../BlockStreamIterator/BlockStreamPrint.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamProjectIterator.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamProjectionScan.h"
#include "../../ids.h"
#include "../../Environment.h"
#include "../../common/Mapping.h"
#include "../../common/ExpressionItem.h"
#include "../../common/ExpressionCalculator.h"
#include "../../common/TypeCast.h"
#include "../../common/TypePromotionMap.h"
#include "../../utility/test_tool.h"
#include "../../utility/rdtsc.h"
#include "../../Block/BlockStream.h"
#include "../../LogicalQueryPlan/Scan.h"
#include "../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../LogicalQueryPlan/EqualJoin.h"
#include "../../LogicalQueryPlan/Project.h"
#include "../../Catalog/ProjectionBinding.h"
#include "../../LogicalQueryPlan/Filter.h"
#include "../../LogicalQueryPlan/Aggregation.h"

static int testProject(){
	int master;
	printf("Master(0) or Slave(others)??\n");
	scanf("%d",&master);
	if(master!=0){
		Environment::getInstance(false);
	}
	else{
		Environment::getInstance(true);
		ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
		Catalog* catalog=Environment::getInstance()->getCatalog();

		TableDescriptor* table_1=new TableDescriptor("T",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_1->addAttribute("x",data_type(t_int));  				//0
		table_1->addAttribute("y",data_type(t_int));

		vector<ColumnOffset> cj_proj0;
		cj_proj0.push_back(0);
		cj_proj0.push_back(1);
		const int partition_key_index_1=2;
		table_1->createHashPartitionedProjection(cj_proj0,"x",1);	//G0
		catalog->add_table(table_1);

		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
		}

		LogicalOperator* scan=new LogicalScan(table_1->getProjectoin(0));

		std::vector<std::vector<ExpressionItem> > vve;
		std::vector<ExpressionItem> ve;

		ExpressionItem ei1;
		ei1.setVariable("T","y");
		ve.push_back(ei1);

		ExpressionItem ei2;
		ei2.setVariable("T","x");
		ve.push_back(ei2);

		ExpressionItem ei3;
		ei3.setOperator("+");
		ve.push_back(ei3);

		vve.push_back(ve);

		std::vector<ExpressionItem> ve1;

		ExpressionItem ei11;
		ei11.setVariable("T","y");
		ve1.push_back(ei11);

		ExpressionItem ei12;
		ei12.setVariable("T","x");
		ve1.push_back(ei12);

		ExpressionItem ei13;
		ei13.setOperator("-");
		ve1.push_back(ei13);

//		ExpressionItem ei14;
//		ei14.setDoubleValue("1.23");
//		ve1.push_back(ei14);
//
//		ExpressionItem ei15;
//		ei15.setOperator("+");
//		ve1.push_back(ei15);

		vve.push_back(ve1);

		std::vector<ExpressionItem> ve2;

		ExpressionItem ei21;
		ei21.setVariable("T","y");
		ve2.push_back(ei21);

		ExpressionItem ei22;
		ei22.setVariable("T","x");
		ve2.push_back(ei22);

		ExpressionItem ei23;
		ei23.setOperator("+");
		ve2.push_back(ei23);

//		ExpressionItem ei24;
//		ei24.setIntValue("100");
//		ve2.push_back(ei24);
//
//		ExpressionItem ei25;
//		ei25.setOperator("-");
//		ve2.push_back(ei25);

		vve.push_back(ve2);

//		vve.push_back(ve);

		LogicalOperator* proj=new LogicalProject(scan,vve);

//		const NodeID collector_node_id=0;
//		LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,proj,LogicalQueryPlanRoot::PRINT);
//		unsigned long long int timer_start=curtick();
////		root->print();
////
//		BlockStreamIteratorBase* executable_query_plan=root->getIteratorTree(1024*64-sizeof(unsigned));
//		printf("query optimization time :%5.5f\n",getMilliSecond(timer_start));

//		executable_query_plan->print();

//		IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(proj->getIteratorTree(1024*64-sizeof(unsigned)),"127.0.0.1");
		BlockStreamIteratorBase* prj=proj->getIteratorTree(1024*64-sizeof(unsigned));

		BlockStreamPrint::State print_state;
		print_state.block_size_=64*1024-sizeof(unsigned);
		print_state.child_=prj;
		vector<column_type> column_list;
		column_list.push_back(column_type(t_int));
		column_list.push_back(column_type(t_int));
		column_list.push_back(column_type(t_int));
		print_state.schema_=new SchemaFix(column_list);
		print_state.spliter_="-|-";
		BlockStreamIteratorBase* print=new BlockStreamPrint(print_state);

		print->open();
		print->next(0);
		print->close();

//		executable_query_plan->open();
//		executable_query_plan->next(0);
//		executable_query_plan->close();
	}
	cout<<"Waiting~~~!~"<<endl;
	while(true){
		sleep(1);
	}
	return 0;
}
