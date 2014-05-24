/*
 * in_iterator_test.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: scdong
 */

#include "../BlockStreamInIterator.h"
#include "../ExpandableBlockStreamSingleColumnScan.h"
#include "../ExpandableBlockStreamFilter.h"
#include "../BlockStreamInIterator.h"
#include "../../BlockStreamPrint.h"
#include "../../../common/Schema/SchemaFix.h"
#include "../../../common/Message.h"
#include "../../../common/AttributeComparator.h"

static int in_iterator_test ()
{
	cout << "test in\n";
	std::vector<column_type> column_list;
	column_list.push_back(column_type(t_u_long));
	column_list.push_back(column_type(t_int));
	column_list.push_back(column_type(t_u_long));
	column_list.push_back(column_type(t_int));
	column_list.push_back(column_type(t_int));
	column_list.push_back(column_type(t_int));

	Schema* input=new SchemaFix(column_list);
	Schema* output=new SchemaFix(column_list);

	unsigned block_size = 64*1024-sizeof(unsigned);

	ExpandableBlockStreamSingleColumnScan::State ebsscs1_state("/home/imdb/data/wangli/T0G0P0",input, block_size);
	BlockStreamIteratorBase* ebssc1=new ExpandableBlockStreamSingleColumnScan(ebsscs1_state);

	unsigned long f = 20000;
	AttributeComparator fA(column_type(t_u_long),Comparator::L,0,&f);
	std::vector<AttributeComparator> ComparatorList;
	ComparatorList.push_back(fA);
	ExpandableBlockStreamFilter::State ebsf_state(input, ebssc1, ComparatorList, block_size);
	BlockStreamIteratorBase* ebfs = new ExpandableBlockStreamFilter(ebsf_state);


	ExpandableBlockStreamSingleColumnScan::State ebsscs2_state("/home/imdb/data/wangli/T0G0P0",input, block_size);
	BlockStreamIteratorBase* ebssc2=new ExpandableBlockStreamSingleColumnScan(ebsscs1_state);


	BlockStreamInIterator::State bsii_state(ebfs, ebssc2, input, input, 0, 0, block_size);
	BlockStreamIteratorBase* bsii = new BlockStreamInIterator(bsii_state);

	std::vector<string> attr_name;
	attr_name.push_back("rowid");
	attr_name.push_back("Trade_Date");
	attr_name.push_back("Order_No");
	attr_name.push_back("Sec_Code");
	attr_name.push_back("Trade_Dir");
	attr_name.push_back("Order_Type");

	BlockStreamPrint::State bsp1_state(input, ebfs, block_size, attr_name, "\t");
	BlockStreamIteratorBase* bsp1 = new BlockStreamPrint(bsp1_state);

	bsp1->print();
	bsp1->open(0);
	BlockStreamBase* block = BlockStreamBase::createBlock(input, block_size);
	while(bsp1->next(block));
	{
	}
	bsp1->close();

	BlockStreamPrint::State bsp_state(input, bsii, block_size, attr_name, "\t");
	BlockStreamIteratorBase* bsp = new BlockStreamPrint(bsp_state);

	bsp->open(0);
//	BlockStreamBase* block = BlockStreamBase::createBlock(input, block_size);
	while(bsp->next(block));
	{
	}
	bsp->close();

	IteratorMessage IM(bsp);
	Message4K M4K = IteratorMessage::serialize4K(IM);
	IteratorMessage tmp = IteratorMessage::deserialize4K(M4K);
	tmp.run();


	return 0;
}


