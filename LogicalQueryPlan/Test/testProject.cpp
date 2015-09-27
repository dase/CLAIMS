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

#include "../../Environment.h"

#include "../../common/ids.h"
#include "../../common/Mapping.h"
#include "../../common/ExpressionItem.h"
#include "../../common/ExpressionCalculator.h"
#include "../../common/TypeCast.h"
#include "../../common/TypePromotionMap.h"

#include "../../common/Block/BlockStream.h"

#include "../../utility/test_tool.h"
#include "../../utility/rdtsc.h"

#include "../../Catalog/ProjectionBinding.h"

#include "../../LogicalQueryPlan/logical_scan.h"
#include "../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../LogicalQueryPlan/logical_equal_join.h"
#include "../../LogicalQueryPlan/Project.h"
#include "../../LogicalQueryPlan/Filter.h"
#include "../../LogicalQueryPlan/logical_aggregation.h"

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

		TableDescriptor* table_1=new TableDescriptor("cj",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
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

		std::vector<std::vector<ExpressionItem> > vve;
		std::vector<ExpressionItem> ve1;
		ExpressionItem ei1;
		ei1.setVariable("cj","row_id");
		ve1.push_back(ei1);

		std::vector<ExpressionItem> ve2;
		ExpressionItem ei2;
		ei2.setVariable("cj","trade_date");
		ve2.push_back(ei2);

		std::vector<ExpressionItem> ve3;
		ExpressionItem ei3;
		ei3.setVariable("cj","order_no");
		ve3.push_back(ei3);

		std::vector<ExpressionItem> ve4;
		ExpressionItem ei4;
		ei4.setVariable("cj","sec_code");
		ve4.push_back(ei4);

		std::vector<ExpressionItem> ve5;
		ExpressionItem ei5;
		ei5.setVariable("cj","trade_dir");
		ve5.push_back(ei5);

		std::vector<ExpressionItem> ve6;
		ExpressionItem ei6;
		ei6.setVariable("cj","order_type");
		ve6.push_back(ei6);

		vve.push_back(ve1);
		vve.push_back(ve2);
		vve.push_back(ve3);
		vve.push_back(ve4);
		vve.push_back(ve5);
		vve.push_back(ve6);

		LogicalProject *proj=new LogicalProject(scan,vve);
		BlockStreamIteratorBase* prj=proj->GetIteratorTree(1024*64-sizeof(unsigned));

		BlockStreamPrint::State print_state;
		print_state.block_size_=64*1024-sizeof(unsigned);
		print_state.child_=prj;
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
