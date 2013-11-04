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
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamCombinedIterator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamJoinIterator.h"
#include "../Serialization/RegisterDerivedClass.h"
#include "../Schema/SchemaFix.h"
#include "../rdtsc.h"

//int main(int argc,const char** argv){
int main_combine(int argc,const char** argv){
	const unsigned block_size=BLOCK_SIZE_CAO;
	const unsigned thread_count=4;
	const unsigned expander_buffer=4;
	std::vector<column_type> column_list,column_list_;
	column_list.push_back(column_type(t_int));

	Schema* schema=new SchemaFix(column_list);
	ExpandableBlockStreamSingleColumnScan::State ebssc_state1("/home/imdb/temp/Uniform_0_99.column",schema,block_size);
	BlockStreamIteratorBase* ebssc1=new ExpandableBlockStreamSingleColumnScan(ebssc_state1);
	ExpandableBlockStreamSingleColumnScan::State ebssc_state2("/home/imdb/temp/Uniform_0_99.column",schema,block_size);
	BlockStreamIteratorBase* ebssc2=new ExpandableBlockStreamSingleColumnScan(ebssc_state2);

	std::vector<Schema *> inputs;
	inputs.push_back(schema);
	inputs.push_back(schema);

	column_list_.push_back(column_type(t_int));
	column_list_.push_back(column_type(t_int));
	Schema* output=new SchemaFix(column_list_);

	std::vector<BlockStreamIteratorBase *> children_;
	children_.push_back(ebssc1);
	children_.push_back(ebssc2);

	BlockStreamCombinedIterator::State bsci_state(inputs,output,children_);
	BlockStreamCombinedIterator *bsc=new BlockStreamCombinedIterator(bsci_state);

	BlockStreamExpander::State bse_state(schema,bsc,thread_count,block_size,expander_buffer);
	BlockStreamIteratorBase* bse=new BlockStreamExpander(bse_state);

	BlockStreamBase *block=new BlockStreamFix(block_size,8);
	int choice=0;


	std::ostringstream ostr;
	boost::archive::text_oarchive oa(ostr);
	oa.register_type(static_cast<BlockStreamCombinedIterator *>(NULL));
	oa.register_type(static_cast<BlockStreamExpander *>(NULL));
	oa.register_type(static_cast<ExpandableBlockStreamSingleColumnScan *>(NULL));
	Register_Schemas<boost::archive::text_oarchive>(oa);
//	Register_Iterators(oa);
	oa<<bse;

	std::cout<<"Serialization Result:"<<ostr.str()<<std::endl;

	std::istringstream istr(ostr.str());
	boost::archive::text_iarchive ia(istr);
	BlockStreamIteratorBase* des;

	ia.register_type(static_cast<BlockStreamCombinedIterator *>(NULL));
	ia.register_type(static_cast<BlockStreamExpander *>(NULL));
	ia.register_type(static_cast<ExpandableBlockStreamSingleColumnScan *>(NULL));
	Register_Schemas<boost::archive::text_iarchive>(ia);
	ia>>des;
//	return 1;
	while(choice==0){


//		bsf->open();
		des->open();
		cout<<"after open!"<<endl;
		unsigned long long int start=curtick();

		cout<<"ready for the next"<<endl;

		unsigned tuple_count=0;
		while(des->next(block)){
			BlockStreamBase::BlockStreamTraverseIterator* it=block->createIterator();
			while(it->nextTuple()){
				tuple_count++;
			}
			block->setEmpty();
		}
		printf("Time=%f Throughput=%f.\n tuple=%d",getSecond(start),1024/getSecond(start),tuple_count);
		des->close();
		printf("Continue(0) or Not(1) ?\n");

		scanf("%d",&choice);

	}

}
