/*
 * Sort_test.cpp
 *
 *  Created on: 2014-2-17
 *      Author: casa
 */

#include <vector>
#include <iostream>
#include "../../BlockStreamPrint.h"
#include "../BlockStreamProjectIterator.h"
#include "../ExpandableBlockStreamProjectionScan.h"
#include "../../../common/ids.h"
#include "../../../Environment.h"
#include "../../../common/Mapping.h"
#include "../../../common/ExpressionItem.h"
#include "../../../common/ExpressionCalculator.h"
#include "../../../common/TypeCast.h"
#include "../../../common/TypePromotionMap.h"
#include "../../../utility/test_tool.h"
#include "../../../Block/BlockStream.h"
#include "../../../LogicalQueryPlan/Scan.h"
#include "../../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../../LogicalQueryPlan/EqualJoin.h"
#include "../../../Catalog/ProjectionBinding.h"
#include "../../../LogicalQueryPlan/Filter.h"
#include "../../../LogicalQueryPlan/Aggregation.h"
using namespace std;

static int Project_test(){
	initialize_arithmetic_type_promotion_matrix();
	initialize_type_cast_functions();
	cout<<"in the sort test!!!"<<endl;
	Environment::getInstance(true);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();
	cout<<"in the sort test!!!"<<endl;

	TableDescriptor* table_1=new TableDescriptor("1",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	cout<<"thread in the sort test!!!"<<endl;
	table_1->addAttribute("1",data_type(t_int));  				//0
	table_1->addAttribute("2",data_type(t_int));
	//首先要定好table中的属性的信息，然后将每个属性加到table中，这样方便等会在下面的projection中定义
	cout<<"in the sort test!!!"<<endl;

	vector<ColumnOffset> cj_proj0_index;
	cj_proj0_index.push_back(0);
	cj_proj0_index.push_back(1);
	const int partition_key_index_1=0;
	cout<<"before binding in the sort test!!!"<<endl;
	table_1->createHashPartitionedProjection(cj_proj0_index,partition_key_index_1,1);	//G0
	//第一个参数是这个projection中的column的index
	//第二个参数是parition的key的index，
	//第三个参数是partition的份数，也就是将整块数据hash为多少份
	//上面这行语句是在添加一个projection，还可以添加另一个projection
	//	cout<<"middle binding in the sort test!!!"<<"ooo"<<partition_key_index_1<<endl;
	catalog->add_table(table_1);

//	---------------------------------------------------------------------------------------------
	for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){
		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,1);
	}
	ProjectionBinding *pb=new ProjectionBinding();
	pb->BindingEntireProjection(catalog->getTable(0)->getProjectoin(0)->getPartitioner(),HDFS);
	cout<<"binding in the sort test!!!"<<endl;
//	---------------------------------------------------------------------------------------------

	/*******************scan******************/
	vector<column_type> column_list;
	column_list.push_back(column_type(t_int));
	column_list.push_back(column_type(t_int));
	ExpandableBlockStreamProjectionScan::State scan_state(catalog->getTable(0)->getProjectoin(0)->getProjectionID(),new SchemaFix(column_list),64*1024-sizeof(unsigned));
	BlockStreamIteratorBase* scan=new ExpandableBlockStreamProjectionScan(scan_state);

	/****************Projection***************/
	vector<column_type> column_list_;
	column_list_.push_back(column_type(t_int));
//	column_list_.push_back(column_type(t_int));
//	vector<int> vi;
//	vi.push_back(1);
//	vi.push_back(0);
//	Expression *expr=new Expression(vi);
//	expr->add_sys_func(add_,200);
	std::vector<ExpressionItem> express_item_list1;
	ExpressionItem ei1;
	ei1.setVariable("T","x");
	express_item_list1.push_back(ei1);

	ExpressionItem ei3;
	ei3.setVariable("T","y");
	express_item_list1.push_back(ei3);

	ExpressionItem ei2;
	ei2.setOperator("+");
	express_item_list1.push_back(ei2);

//	ExpressionItem ei22;
//	ei22.setIntValue("1");
//	express_item_list1.push_back(ei22);
//
//	ExpressionItem ei23;
//	ei23.setOperator("+");
//	express_item_list1.push_back(ei23);
//
//	ExpressionItem ei24;
//	ei24.setIntValue("100");
//	express_item_list1.push_back(ei24);
//
//	ExpressionItem ei25;
//	ei25.setOperator("+");
//	express_item_list1.push_back(ei25);

//	std::vector<ExpressionItem> express_item_list2;
//	ExpressionItem ei4;
//	ei4.setVariable("x");
//	express_item_list2.push_back(ei4);
//
//	ExpressionItem ei6;
//	ei6.setVariable("y");
//	express_item_list2.push_back(ei6);
//
//	ExpressionItem ei5;
//	ei5.setOperator("-");
//	express_item_list2.push_back(ei5);
//
//	ExpressionItem ei221;
//	ei221.setIntValue("1");
//	express_item_list2.push_back(ei221);
//
//	ExpressionItem ei231;
//	ei231.setOperator("-");
//	express_item_list2.push_back(ei231);

	Mapping *map=new Mapping();
	ExpressionMapping v1;
	v1.push_back(0);
	v1.push_back(1);
//	ExpressionMapping v2;
//	v2.push_back(0);
//	v2.push_back(1);
	map->atomicPushExpressionMapping(v1);
//	map->atomicPushExpressionMapping(v2);

	std::vector<std::vector<ExpressionItem> > express_item_list;
	express_item_list.push_back(express_item_list1);
//	express_item_list.push_back(express_item_list2);
	BlockStreamProjectIterator::State project_state(new SchemaFix(column_list),new SchemaFix(column_list_),scan,64*1024-sizeof(unsigned),*map,express_item_list);
	BlockStreamIteratorBase *project=new BlockStreamProjectIterator(project_state);

	/*******************print******************/
	BlockStreamPrint::State print_state;
	print_state.block_size_=64*1024-sizeof(unsigned);
	print_state.child_=project;
	print_state.schema_=project_state.output_;
	print_state.spliter_="-|-";

	BlockStreamIteratorBase* print=new BlockStreamPrint(print_state);

	cout<<"after print of the iterator!!!"<<endl;
	/*******************show******************/
	print->open();
	print->next(0);
	print->close();

	cout<<"end!!!"<<endl;
//	IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(print,"127.0.0.1");
	sleep(10000);
	return 0;
}
