/*
 * block_iterator.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: wangli
 */
#include <stdlib.h>
#include "../Schema/SchemaFix.h"
#include "../BlockIterator/SingleColumnScanBlockIterator.h"
#include "../BlockIterator/CopyBasedFilterIteratorBlockFix.h"
#include "../rdtsc.h"

int mainasfasfag(int argc,const char** argv){
	std::vector<column_type> column_list;
	column_list.push_back(column_type(t_int));

	SchemaFix* input=new SchemaFix(column_list);
	SingleColumnScanBlockIteratorFix::State scs_state("/home/imdb/temp/Uniform_0_99.column",input);
	BlockIteratorFix* scs=new SingleColumnScanBlockIteratorFix(scs_state);


	int f=atoi(argv[2]);
	FilterIterator::AttributeComparator fA(column_type(t_int),Comparator::L,0,&f);

	std::vector<FilterIterator::AttributeComparator> ComparatorList;
	ComparatorList.push_back(fA);
//	ComparatorList.push_back(filter2);
	CopyBasedFilterIteratorBlockFix::State filterState(input,scs,ComparatorList);
	BlockIteratorFix* filter=new CopyBasedFilterIteratorBlockFix(filterState);

	FilterIterator::AttributeComparator fA2(column_type(t_int),Comparator::L,0,&f);
	std::vector<FilterIterator::AttributeComparator> ComparatorList2;
	ComparatorList2.push_back(fA2);
	CopyBasedFilterIteratorBlockFix::State filterState2(input,filter,ComparatorList2);
	BlockIteratorFix* filter2=new CopyBasedFilterIteratorBlockFix(filterState2);
	unsigned blocksize=atoi(argv[1]);
	BlockFix * block=new BlockFix(blocksize,input);
	block->setEmpty();
	int choice=0;

	while(choice==0){


		filter->open();
		unsigned long long int start=curtick();
		while(filter->next(*block)){
			block->setEmpty();
		}
		printf("Time=%f Throughput=%f.\n",getSecond(start),1024/getSecond(start));
		filter->close();
		printf("Continue(0) or Not(1) ?\n");

		scanf("%d",&choice);

	}


}

