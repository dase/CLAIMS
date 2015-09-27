/*
 * testSort.cpp
 *
 *  Created on: 2014-3-9
 *      Author: casa
 */

#include <vector>
#include <iostream>
#include "../../BlockStreamIterator/BlockStreamPrint.h"

#include "../../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamSortIterator.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamProjectionScan.h"

#include "../../Environment.h"

#include "../../common/ids.h"
#include "../../common/Mapping.h"
#include "../../common/ExpressionItem.h"
#include "../../common/ExpressionCalculator.h"
#include "../../common/TypeCast.h"
#include "../../common/TypePromotionMap.h"
#include "../../common/AttributeComparator.h"

#include "../../common/Block/BlockStream.h"

#include "../../utility/test_tool.h"
#include "../../utility/rdtsc.h"

#include "../../Catalog/ProjectionBinding.h"

#include "../../LogicalQueryPlan/logical_scan.h"
#include "../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../LogicalQueryPlan/logical_equal_join.h"
#include "../../LogicalQueryPlan/logical_sort.h"
#include "../../LogicalQueryPlan/Filter.h"
#include "../../LogicalQueryPlan/logical_aggregation.h"

static int testSort(){
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
		const int partition_key_index_1=0;
		table_1->createHashPartitionedProjection(cj_proj0,"row_id",1);	//G0
		catalog->add_table(table_1);

		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
		}

		LogicalOperator* scan=new LogicalScan(table_1->getProjectoin(0));

		LogicalFilter::Condition filter_condition_1;

		filter_condition_1.add(table_1->getAttribute("row_id"),AttributeComparator::L,std::string("100000"));


		LogicalOperator* filter_1=new LogicalFilter(filter_condition_1,scan);

		vector<LogicalSort::OrderByAttr *> oas;
		LogicalSort::OrderByAttr *od0=new LogicalSort::OrderByAttr("cj","row_id");
		LogicalSort::OrderByAttr *od1=new LogicalSort::OrderByAttr("cj","order_no");
		LogicalSort::OrderByAttr *od2=new LogicalSort::OrderByAttr("cj","sec_code");
		oas.push_back(od2);
		oas.push_back(od1);
		oas.push_back(od0);
		LogicalOperator* sort=new LogicalSort(scan,oas);

		BlockStreamIteratorBase *sort_=sort->GetIteratorTree(64*1024-sizeof(unsigned));

		BlockStreamPrint::State print_state;
		print_state.block_size_=64*1024-sizeof(unsigned);
		print_state.child_=sort_;
		vector<column_type> column_list;
		column_list.push_back(column_type(t_u_long));
		column_list.push_back(column_type(t_int));
		column_list.push_back(column_type(t_u_long));
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
