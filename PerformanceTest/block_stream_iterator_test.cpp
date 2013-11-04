/*
 * block_stream_iterator_test.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>

#include "../BlockStreamIterator/BlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/BlockStreamFilter.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamFilter.h"
#include "../Serialization/RegisterDerivedClass.h"
#include "../Schema/SchemaFix.h"
#include "../rdtsc.h"

int mainafasf234(int argc,const char** argv){

	const unsigned block_size=atoi(argv[1]);
	const unsigned thread_count=atoi(argv[2]);
	int f=atoi(argv[3]);
	const unsigned expander_buffer=atoi(argv[4]);
	std::vector<column_type> column_list,column_list_;
	column_list.push_back(column_type(t_int));

	Schema* schema=new SchemaFix(column_list);
	ExpandableBlockStreamSingleColumnScan::State ebssc_state("/home/imdb/temp/Uniform_0_99.column",schema,block_size);
	BlockStreamIteratorBase* ebssc=new ExpandableBlockStreamSingleColumnScan(ebssc_state);


	FilterIterator::AttributeComparator filter1(column_type(t_int),Comparator::L,0,&f);
	std::vector<FilterIterator::AttributeComparator> ComparatorList;
	ComparatorList.push_back(filter1);
	ExpandableBlockStreamFilter::State ebsf_state(schema,ebssc,ComparatorList,block_size);
	BlockStreamIteratorBase* bbsf=new ExpandableBlockStreamFilter(ebsf_state);


	BlockStreamExpander::State bse_state(schema,bbsf,thread_count,block_size,expander_buffer);
	BlockStreamIteratorBase* bse=new BlockStreamExpander(bse_state);

	BlockStreamBase *block=new BlockStreamFix(block_size,4);
	int choice=0;


	std::ostringstream ostr;
	boost::archive::text_oarchive oa(ostr);
	oa.register_type(static_cast<ExpandableBlockStreamFilter *>(NULL));
	oa.register_type(static_cast<BlockStreamExpander *>(NULL));
	oa.register_type(static_cast<ExpandableBlockStreamSingleColumnScan *>(NULL));
	Register_Schemas<boost::archive::text_oarchive>(oa);
//	Register_Iterators(oa);
	oa<<bse;

	std::cout<<"Serialization Result:"<<ostr.str()<<std::endl;

	std::istringstream istr(ostr.str());
	boost::archive::text_iarchive ia(istr);
	BlockStreamIteratorBase* des;

	ia.register_type(static_cast<ExpandableBlockStreamFilter *>(NULL));
	ia.register_type(static_cast<BlockStreamExpander *>(NULL));
	ia.register_type(static_cast<ExpandableBlockStreamSingleColumnScan *>(NULL));
	Register_Schemas<boost::archive::text_iarchive>(ia);
	ia>>des;
//	return 1;
	while(choice==0){


//		bsf->open();
		des->open();
		unsigned long long int start=curtick();

		while(des->next(block)){
			block->setEmpty();
		}
		printf("Time=%f Throughput=%f.\n",getSecond(start),1024/getSecond(start));
		des->close();
		printf("Continue(0) or Not(1) ?\n");

		scanf("%d",&choice);

	}

}


