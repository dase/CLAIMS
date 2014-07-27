/*
 * testProject_wl.cpp
 *
 *  Created on: 2014-3-8
 *      Author: casa
 */

/*
 * testProject.cpp
 *
 *  Created on: 2014-3-2
 *      Author: casa
 */

#include <vector>
#include <iostream>
#include "../../Environment.h"

#include "../../common/ids.h"
#include "../../common/Mapping.h"
#include "../../common/ExpressionItem.h"
#include "../../common/ExpressionCalculator.h"
#include "../../common/TypeCast.h"
#include "../../common/TypePromotionMap.h"
#include "../../common/AttributeComparator.h"

#include "../../common/Block/BlockStream.h"

#include "../../BlockStreamIterator/BlockStreamPrint.h"

#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamProjectIterator.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamProjectionScan.h"

#include "../../utility/test_tool.h"
#include "../../utility/rdtsc.h"

#include "../../Catalog/ProjectionBinding.h"

#include "../../LogicalQueryPlan/Scan.h"
#include "../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../LogicalQueryPlan/EqualJoin.h"
#include "../../LogicalQueryPlan/Project.h"
#include "../../LogicalQueryPlan/Filter.h"
#include "../../LogicalQueryPlan/Aggregation.h"


/**
 * @li: I use the first projection of cj for testing.
 * It seems to be some error in the generated mapping.
 * Also, it is very likely that my modification results in
 * errors.
 */
static int testProject_wl(){
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

		TableDescriptor* table_1=new TableDescriptor("cj",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		/**
		 * @li: I change the following code such that the scheme matches those in cj's first projection
		 */
		table_1->addAttribute("row_id",data_type(t_u_long));  				//0
		table_1->addAttribute("trade_date",data_type(t_int));
		table_1->addAttribute("order_no",data_type(t_u_long));
		table_1->addAttribute("sec_code",data_type(t_int));
		table_1->addAttribute("trade_dir",data_type(t_int));
		table_1->addAttribute("order_type",data_type(t_int));

		vector<ColumnOffset> cj_proj0;
		cj_proj0.push_back(0);
		cj_proj0.push_back(1);
		cj_proj0.push_back(2);
		cj_proj0.push_back(3);
		cj_proj0.push_back(4);
		cj_proj0.push_back(5);
		const int partition_key_index_1=2;
		table_1->createHashPartitionedProjection(cj_proj0,"row_id",1);	//G0
		catalog->add_table(table_1);

		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
		}

		LogicalOperator* scan=new LogicalScan(table_1->getProjectoin(0));

		Filter::Condition filter_condition_1;

		filter_condition_1.add(table_1->getAttribute("row_id"),AttributeComparator::L,std::string("1000"));


		LogicalOperator* filter_1=new Filter(filter_condition_1,scan);

		std::vector<std::vector<ExpressionItem> > vve;

		Expression express_item_list;

		ExpressionItem ei1;								// case
		ei1.setOperator("case");
		express_item_list.push_back(ei1);

		ExpressionItem ei2;								// x
		ei2.setVariable("cj","sec_code");
		express_item_list.push_back(ei2);

		ExpressionItem ei3;							 	// 60
		ei3.setIntValue("100");
		express_item_list.push_back(ei3);

		ExpressionItem ei4;								// <
		ei4.setOperator("<");
		express_item_list.push_back(ei4);

		ExpressionItem ei5;								// when
		ei5.setOperator("when");
		express_item_list.push_back(ei5);

		ExpressionItem ei6;							 	// value
		ei6.setIntValue("1");
		express_item_list.push_back(ei6);

		ExpressionItem ei7;								// then
		ei7.setOperator("then");
		express_item_list.push_back(ei7);

		ExpressionItem ei8;								// x
		ei8.setIntValue("88");
		express_item_list.push_back(ei8);
//
//		ExpressionItem ei9;							 	// 60
//		ei9.setIntValue(80);
//		express_item_list.push_back(ei3);
//
//		ExpressionItem ei10;								// <
//		ei10.setOperator("<");
//		express_item_list.push_back(ei4);
//
//		ExpressionItem ei11;								// when
//		ei11.setOperator("when");
//		express_item_list.push_back(ei5);
//
//		ExpressionItem ei12;							 	// value
//		ei12.setStringValue("Good");
//		express_item_list.push_back(ei6);
//
//		ExpressionItem ei13;								// then
//		ei13.setOperator("then");
//		express_item_list.push_back(ei7);
//
//		ExpressionItem ei14;							 	// value
//		ei14.setStringValue("PERFECT");
//		express_item_list.push_back(ei14);

		ExpressionItem ei15;								// else
		ei15.setOperator("else");
		express_item_list.push_back(ei15);

		ExpressionItem ei16;								// case
		ei16.setOperator("case");
		express_item_list.push_back(ei16);

		vve.push_back(express_item_list);


//		std::vector<ExpressionItem> ve;
//
//		ExpressionItem ei1;
//		ei1.setVariable("cj","row_id");
//		ve.push_back(ei1);
//
//		ExpressionItem ei2;
//		ei2.setIntValue("1");
//		ve.push_back(ei2);
//
//		ExpressionItem ei3;
//		ei3.setOperator("+");
//		ve.push_back(ei3);
//
//
////		ExpressionItem ei1;
////		ei1.setVariable("cj","sec_code");
////		ve.push_back(ei1);
////
////		ExpressionItem ei2;
////		ei2.setVariable("cj","sec_code");
////		ve.push_back(ei2);
////
////		ExpressionItem ei3;
////		ei3.setOperator("+");
////		ve.push_back(ei3);
//
//		vve.push_back(ve);
//
////		std::vector<ExpressionItem> ve1;
////
////		ExpressionItem ei11;
////		ei11.setVariable("cj","sec_code");
////		ve1.push_back(ei11);
////
////		ExpressionItem ei12;
////		ei12.setVariable("cj","sec_code");
////		ve1.push_back(ei12);
////
//////		ExpressionItem ei12;
//////		ei12.setIntValue("1");
//////		ve1.push_back(ei12);
////
////		ExpressionItem ei13;
////		ei13.setOperator("-");
////		ve1.push_back(ei13);
////
////		ExpressionItem ei14;
////		ei14.setFloatValue("1.23");
////		ve1.push_back(ei14);
////
////		ExpressionItem ei15;
////		ei15.setOperator("+");
////		ve1.push_back(ei15);
////
////		vve.push_back(ve1);
////
////		std::vector<ExpressionItem> ve2;
////
////		ExpressionItem ei21;
////		ei21.setVariable("cj","sec_code");
////		ve2.push_back(ei21);
////
////		ExpressionItem ei22;
////		ei22.setVariable("cj","sec_code");
////		ve2.push_back(ei22);
////
////		ExpressionItem ei23;
////		ei23.setOperator("+");
////		ve2.push_back(ei23);
////
////		ExpressionItem ei24;
////		ei24.setIntValue("100");
////		ve2.push_back(ei24);
////
////		ExpressionItem ei25;
////		ei25.setOperator("-");
////		ve2.push_back(ei25);
////
////		vve.push_back(ve2);
//
////		vve.push_back(ve);

		LogicalOperator* proj=new LogicalProject(filter_1,vve);

//		const NodeID collector_node_id=0;
//		LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,proj,LogicalQueryPlanRoot::PRINT);
////		unsigned long long int timer_start=curtick();
//		root->print();
//////
////		BlockStreamIteratorBase* executable_query_plan=root->getIteratorTree(1024*64-sizeof(unsigned));
////		printf("query optimization time :%5.5f\n",getMilliSecond(timer_start));
//
////		executable_query_plan->print();
//
////		IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(root->getIteratorTree(1024*64-sizeof(unsigned)),"127.0.0.1");
//		BlockStreamIteratorBase* executable_plan=proj->getIteratorTree(1024*64-sizeof(unsigned));
//
//
//		BlockStreamBase* block=BlockStreamBase::createBlockWithDesirableSerilaizedSize(proj->getDataflow().getSchema(),1024*64);
//
//		executable_plan->open();
//		while(executable_plan->next(block));
//		executable_plan->close();

		BlockStreamIteratorBase* prj=proj->getIteratorTree(1024*64-sizeof(unsigned));

		BlockStreamPrint::State print_state;
		print_state.block_size_=64*1024-sizeof(unsigned);
		print_state.child_=prj;
		vector<column_type> column_list;
//		column_list.push_back(column_type(t_u_long));
//		column_list.push_back(column_type(t_int));
		column_list.push_back(column_type(t_int));
//		column_list.push_back(column_type(t_int));
//		column_list.push_back(column_type(t_int));
//		column_list.push_back(column_type(t_float));
//		column_list.push_back(column_type(t_int));
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
