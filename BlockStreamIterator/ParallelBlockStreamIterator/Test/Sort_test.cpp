/*
 * Sort_test.cpp
 *
 *  Created on: 2014-2-17
 *      Author: casa
 */

#include <vector>
#include <iostream>
#include "../BlockStreamSortIterator.h"
#include "../ExpandableBlockStreamProjectionScan.h"

#include "../../BlockStreamPrint.h"

#include "../../../Environment.h"

#include "../../../common/ids.h"
#include "../../../common/Block/BlockStream.h"

#include "../../../Catalog/ProjectionBinding.h"

#include "../../../LogicalQueryPlan/logical_scan.h"
#include "../../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../../LogicalQueryPlan/logical_equal_join.h"
#include "../../../LogicalQueryPlan/Filter.h"
#include "../../../LogicalQueryPlan/logical_aggregation.h"

using namespace std;

static int sort_iterator_test(){
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


	BlockStreamSortIterator::State sort_state;
	sort_state.block_size_=64*1024-sizeof(unsigned);
	sort_state.child_=scan;
	vector<unsigned> orderbyKey_;
	sort_state.orderbyKey_.push_back(0);
	sort_state.orderbyKey_.push_back(1);
	sort_state.orderbyKey_.push_back(2);
	sort_state.input_=new SchemaFix(column_list);
	BlockStreamSortIterator *sort=new BlockStreamSortIterator(sort_state);

	/*******************print******************/
	BlockStreamPrint::State print_state;
	print_state.block_size_=64*1024-sizeof(unsigned);
	print_state.child_=sort;
	print_state.schema_=sort_state.input_;
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
