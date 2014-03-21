/*
 * CSBIndexBuilding.cpp
 *
 *  Created on: Mar 17, 2014
 *      Author: scdong
 */

#include "CSBIndexBuilding.h"
#include "../storage/BlockManager.h"
#include <algorithm>

using std::stable_sort;

bottomLayerCollecting::bottomLayerCollecting(State state) :state_(state), partition_reader_iterator_(0), chunk_reader_iterator_(0), chunk_offset_(0), block_offset_(0) {

	std::vector<column_type> column_list;
	column_list.push_back(column_type(t_int));	//chunk offset
	column_list.push_back(state_.schema_->getcolumn(state_.key_indexing_));
	column_list.push_back(column_type(t_u_smallInt));		//block offset
	column_list.push_back(column_type(t_u_smallInt));		//tuple_offset

	output_schema_ = new SchemaFix(column_list);

	initialize_expanded_status();
}

bottomLayerCollecting::~bottomLayerCollecting() {
	// TODO Auto-generated destructor stub
}

bottomLayerCollecting::State::State(ProjectionID projection_id, Schema* schema, unsigned key_indexing, unsigned block_size)
: projection_id_(projection_id), schema_(schema), key_indexing_(key_indexing), block_size_(block_size) {

}

bool bottomLayerCollecting::open(const PartitionOffset& partition_offset)
{

	AtomicPushBlockStream(BlockStreamBase::createBlockWithDesirableSerilaizedSize(state_.schema_, state_.block_size_));
	if(completeForInitializationJob()){
		/* this is the first expanded thread*/
		PartitionStorage* partition_handle_;
		if((partition_handle_=BlockManager::getInstance()->getPartitionHandle(PartitionID(state_.projection_id_,partition_offset)))==0){
			printf("The partition[%s] does not exists!\n",PartitionID(state_.projection_id_,partition_offset).getName().c_str());
			open_ret_=false;
		}
		else{
			partition_reader_iterator_=partition_handle_->createAtomicReaderIterator();
			chunk_reader_iterator_ = partition_reader_iterator_->nextChunk();
		}
		open_ret_=true;
		broadcaseOpenFinishedSignal();
	}
	else{
		/* For other expanded threads just wait for the first thread finishing initialization*/
		waitForOpenFinished();
	}
	return getOpenReturnValue();
}

bool bottomLayerCollecting::next(BlockStreamBase* block) {
	remaining_block rb;
	void* original_tuple;
	void* tuple_new;

	// There are blocks which haven't been completely processed
	if (atomicPopRemainingBlock(rb))
	{
		while ((original_tuple = rb.iterator->currentTuple()) > 0)
		{
			const unsigned bytes = output_schema_->getTupleMaxSize();
			if ((tuple_new = block->allocateTuple(bytes)) > 0)
			{
				// construct tuple_new <chunk_offset, key_index, block_offset, tuple_offset>
				output_schema_->getcolumn(0).operate->assignment((void*)(& rb.chunk_offset), tuple_new);
				output_schema_->getcolumn(1).operate->assignment(state_.schema_->getColumnAddess(state_.key_indexing_, original_tuple), output_schema_->getColumnAddess(1, tuple_new));
				output_schema_->getcolumn(2).operate->assignment((void*)(& rb.block_offset), output_schema_->getColumnAddess(2, tuple_new));
				output_schema_->getcolumn(3).operate->assignment((void*)(& rb.tuple_offset), output_schema_->getColumnAddess(3, tuple_new));
				rb.iterator->increase_cur_();
				rb.tuple_offset++;

///*for testing*/				state_.schema_->displayTuple(original_tuple, " | ");
///*for testing*/				output_schema_->displayTuple(tuple_new, " | ");
///*for testing*/				sleep(1);
			}
			else
			{
				atomicPushRemainingBlock(rb);
				return true;
			}
		}
		AtomicPushBlockStream(rb.block);
	}

	// When the program arrivals here, it means that there is no remaining block or the remaining block is
	// exhausted. What we should do is to ask a new block from the chunk_reader_iterator (or prartition_reader_iterator)
	BlockStreamBase* block_for_asking = AtomicPopBlockStream();
	block_for_asking->setEmpty();
	rb.block=block_for_asking;
	while (askForNextBlock(block_for_asking, rb))
	{
//		BlockStreamBase::BlockStreamTraverseIterator* traverse_iterator = block_for_asking->createIterator();
		rb.iterator=block_for_asking->createIterator();
		while ((original_tuple = rb.iterator->currentTuple()) > 0)
		{
			const unsigned bytes = output_schema_->getTupleMaxSize();
			if ((tuple_new = block->allocateTuple(bytes)) > 0)
			{
				// construct tuple_new <chunk_offset, key_index, block_offset, tuple_offset>
				output_schema_->getcolumn(0).operate->assignment((void*)(& rb.chunk_offset), tuple_new);
				output_schema_->getcolumn(1).operate->assignment(state_.schema_->getColumnAddess(state_.key_indexing_, original_tuple), output_schema_->getColumnAddess(1, tuple_new));
				output_schema_->getcolumn(2).operate->assignment((void*)(& rb.block_offset), output_schema_->getColumnAddess(2, tuple_new));
				output_schema_->getcolumn(3).operate->assignment((void*)(& rb.tuple_offset), output_schema_->getColumnAddess(3, tuple_new));
				rb.iterator->increase_cur_();
				rb.tuple_offset++;

///*for testing*/				state_.schema_->displayTuple(original_tuple, " | ");
///*for testing*/				output_schema_->displayTuple(tuple_new, " | ");
///*for testing*/				sleep(1);
			}
			else
			{
				atomicPushRemainingBlock(rb);
				return true;
			}
		}
//		traverse_iterator->~BlockStreamTraverseIterator();
		block_for_asking->setEmpty();
	}
	AtomicPushBlockStream(block_for_asking);
	if (!block->Empty())
		return true;
	return false;
}

bool bottomLayerCollecting::close() {
	initialize_expanded_status();
	partition_reader_iterator_->~PartitionReaderItetaor();
//	chunk_reader_iterator_->~ChunkReaderIterator();
	remaining_block_list_.clear();
	block_stream_list_.clear();
	open_finished_ = false;

	return true;
}

void bottomLayerCollecting::atomicPushRemainingBlock(remaining_block rb)
{
	lock_.acquire();
	remaining_block_list_.push_back(rb);
	lock_.release();
}

bool bottomLayerCollecting::atomicPopRemainingBlock(remaining_block& rb)
{
	lock_.acquire();
	if (remaining_block_list_.size() > 0)
	{
		rb = remaining_block_list_.front();
		remaining_block_list_.pop_front();
		lock_.release();
		return true;
	}
	lock_.release();
	return false;
}

void bottomLayerCollecting::AtomicPushBlockStream(BlockStreamBase* block)
{
	lock_.acquire();
	block_stream_list_.push_back(block);
	lock_.release();
}

BlockStreamBase* bottomLayerCollecting::AtomicPopBlockStream()
{
	assert(!block_stream_list_.empty());
	lock_.acquire();
	BlockStreamBase* block = block_stream_list_.front();
	block_stream_list_.pop_front();
	lock_.release();
	return block;
}

bool bottomLayerCollecting::askForNextBlock(BlockStreamBase* & block, remaining_block& rb)
{
//	if(chunk_reader_iterator_==0)
//		return false;
	if (chunk_reader_iterator_==0||chunk_reader_iterator_->nextBlock(block) == false)
	{
		chunk_reader_iterator_ = partition_reader_iterator_->nextChunk();

		if (chunk_reader_iterator_ == false){
			printf("Has been falsed!!!!!!!!!!!!!*&S*DF&(SD&F(S<><<<><><><><><>\n");
			return false;
		}
		chunk_reader_iterator_->nextBlock(block);
		lock_.acquire();
		rb.chunk_offset = ++chunk_offset_;
		block_offset_ = 0;
		lock_.release();
		rb.block_offset = 0;
		rb.tuple_offset = 0;
		return true;
	}
	rb.chunk_offset = chunk_offset_;
	lock_.acquire();
	rb.block_offset = ++block_offset_;
	lock_.release();
	rb.tuple_offset = 0;
	return true;
}



bottomLayerSorting::bottomLayerSorting(State state) :state_(state)
{
	std::vector<column_type> column_list;
	column_list.push_back(state_.schema_->getcolumn(1));
	column_list.push_back(column_type(t_u_smallInt));		//block offset
	column_list.push_back(column_type(t_u_smallInt));		//tuple_offset

	vector_schema_ = new SchemaFix(column_list);

	tuples_in_chunk.clear();

	initialize_expanded_status();
}

bottomLayerSorting::~bottomLayerSorting()
{

}

bottomLayerSorting::State::State(Schema* schema, ExpandableBlockStreamIteratorBase* child, unsigned block_size)
: schema_(schema), child_(child), block_size_(block_size) {

}

bool bottomLayerSorting::open(const PartitionOffset& partition_offset)
{
	if (completeForInitializationJob())
	{
		const bool child_open_return = state_.child_->open(partition_offset);
		setOpenReturnValue(child_open_return);
		broadcaseOpenFinishedSignal();
	}
	else
		waitForOpenFinished();

	// Open finished. Buffer all the child create dataset in different group according to their ChunkIDs
	BlockStreamBase* block_for_asking = BlockStreamBase::createBlock(state_.schema_, state_.block_size_);
	block_for_asking->setEmpty();
	BlockStreamBase::BlockStreamTraverseIterator* iterator = NULL;
	void* current_chunk = new ChunkOffset;
	while (state_.child_->next(block_for_asking))
	{
		iterator = block_for_asking->createIterator();
		void* current_tuple = NULL;
		while((current_tuple = iterator->nextTuple()) != 0)
		{
			state_.schema_->getColumnValue(0, current_tuple, current_chunk);
//			if(tuples_in_chunk.find(current_chunk)==tuples_in_chunk.cend()){
//				 vector<void*> tmp;
//				 tuples_in_chunk[current_chunk]=tmp;
//			}
//			tuples_in_chunk.find(current_chunk)->second.push_back(current_tuple+state_.schema_->getcolumn(0).get_length());

			if(tuples_in_chunk_.find(*(ChunkOffset*)current_chunk)==tuples_in_chunk_.cend()){
				 vector<compare_node*> tmp;
				 tuples_in_chunk_[*(ChunkOffset*)current_chunk] = tmp;
			}
			compare_node* c_node = (compare_node*)malloc(sizeof(compare_node));
			c_node->vector_schema_ = vector_schema_;
			c_node->tuple_ = malloc(vector_schema_->getTupleMaxSize());
			vector_schema_->copyTuple(current_tuple+state_.schema_->getcolumn(0).get_length(),c_node->tuple_);
//			c_node->tuple_ = current_tuple+state_.schema_->getcolumn(0).get_length();
			c_node->op_ = state_.schema_->getcolumn(1).operate->duplicateOperator();
			tuples_in_chunk_.find(*(ChunkOffset*)current_chunk)->second.push_back(c_node);

//for testing begin
//			if ((*(ChunkOffset*)current_chunk) == 1)
//			{
//				cout << "current chunk: " << *(ChunkOffset*)current_chunk << " tuple: ";
////				vector_schema_->displayTuple(current_tuple+state_.schema_->getcolumn(0).get_length(), " | ");
//				vector_schema_->displayTuple(tuples_in_chunk_.find(*(ChunkOffset*)current_chunk)->second.back()->tuple_, " | ");
////				sleep(1);
//			}
//for testing end

		}
		block_for_asking->setEmpty();
	}

//for testing begin
//	sleep(10000);
//	cout << "Chunk Num: " << tuples_in_chunk_.size() << endl;
//	sleep(1000);
//for testing end

	// Sorting the tuples in each chunk
	for (boost::unordered_map<ChunkOffset, vector<compare_node*> >::iterator iter = tuples_in_chunk_.begin(); iter != tuples_in_chunk_.end(); iter++)
	{
//		Operate* op_ = state_.schema_->getcolumn(1).operate->duplicateOperator();
///*for testing*/		cout << "chunk id: " << *(unsigned short*)iter->first << endl;
//		sort(iter->second, 0, iter->second.size()-1, op_);
//		cqsort(iter->second, 0, iter->second.size()-1, op_);

//for testing begin
		cout << "Chunk size: " << iter->second.size() << endl;
//		for (unsigned i = 0; i < iter->second.size(); i++)
//			vector_schema_->displayTuple(iter->second[i]->tuple_, "\t");
//		sleep(1000);
//for testing end

		stable_sort(iter->second.begin(), iter->second.end(), compare);

//for testing begin
//		for (unsigned i = 0; i < iter->second.size(); i++)
//			vector_schema_->displayTuple(iter->second[i]->tuple_, "\t");
//		sleep(1000);
//for testing end
	}


	return getOpenReturnValue();
}

bool bottomLayerSorting::next(BlockStreamBase* block)
{
	for (boost::unordered_map<ChunkOffset, vector<compare_node*> >::iterator iter = tuples_in_chunk_.begin(); iter != tuples_in_chunk_.end(); iter++)
	{
		switch (vector_schema_->getcolumn(0).type)
		{
		case t_int:
		{
			CSBPlusTree<int>* csb_tree = indexBuilding<int>(iter->second);

//for testing the search result
			vector<search_result*> ret;
			while (true)
			{
				ret.clear();
				int sec_code = 0;
				cout << "\nPlease input the sec_code for searching: ";
				cin >> sec_code;
				ret = csb_tree->Search(sec_code);
				if (ret.size() == 0)
				{
					cout << "The result set is NULL!\n";
					continue;
				}
				cout << "The result set size is: " << ret.size() << "\nHow many to print? ";
				cin >> sec_code;
				for (int i = 0; i < ret.size() && i < sec_code; i++)
					cout << "<" << ret[i]->_block_off << ", " << ret[i]->_tuple_off << ">\t";
				cout << endl;
			}
//for testing end

			//register csb_tree into index_manager

			break;
		}
		default:
		{
			cout << "[ERROR: (CSBIndexBuilding.cpp->bottomLayerSorting->next()]: The data type is not defined!\n";
			break;
		}
		}
	}
	return false;
}

bool bottomLayerSorting::close()
{
	initialize_expanded_status();
	state_.child_->close();
	return true;
}

void bottomLayerSorting::sort(vector<void*> chunk_tuples, int begin, int end, Operate* op)
{
//for testing
//	for (unsigned i = 0; i < 10; i++)
//		cout << *(int*)(vector_schema_->getColumnAddess(0, chunk_tuples[i])) << endl;
//	sleep(1000);
//for testing

//	printf("Sort begins!\n");
	cout << "Sort begins! left: " << begin << " right: " << end << endl;
	if (begin >= end)
		return;
	int left = begin+1;
	int right = end;
	const void* compare_key = vector_schema_->getColumnAddess(0, chunk_tuples[begin]);

	while (left < right)
	{
		const void* left_key = vector_schema_->getColumnAddess(0, chunk_tuples[left]);
		if (op->greate(left_key, compare_key))
		{
			const void* right_key = vector_schema_->getColumnAddess(0, chunk_tuples[right]);
			while (op->greate(right_key, compare_key))
			{
				right--;
				right_key = vector_schema_->getColumnAddess(0, chunk_tuples[right]);
			}
			if (left >= right)
				break;
			swap (chunk_tuples[left], chunk_tuples[right]);
			right--;
		}
		else
			left++;
	}
	const void* left_key = vector_schema_->getColumnAddess(0, chunk_tuples[left]);
	if (op->greate(left_key, compare_key))
		left--;
	swap(chunk_tuples[begin], chunk_tuples[left]);
	sort(chunk_tuples, begin, left, op);
	sort(chunk_tuples, right, end, op);
	printf("Sort ends!\n");
}

void bottomLayerSorting::swap(void* &a, void* &b)
{
	void* tmp = a;
	a = b;
	b = tmp;
}

bool bottomLayerSorting::compare(const compare_node* a, const compare_node* b)
{
	const void* left = a->vector_schema_->getColumnAddess(0, a->tuple_);
	const void* right = b->vector_schema_->getColumnAddess(0, b->tuple_);
	return a->op_->less(left, right);

}

template<typename T>
CSBPlusTree<T>* bottomLayerSorting::indexBuilding(vector<compare_node*> chunk_tuples)
{
	data_offset<T>* aray = new data_offset<T> [chunk_tuples.size()];
///*for testing*/	cout << "chunk data size: " << chunk_tuples.size() << endl << endl;
	for (unsigned i = 0; i < chunk_tuples.size(); i++)
	{
		aray[i]._key = *(T*)(vector_schema_->getColumnAddess(0, chunk_tuples[i]->tuple_));
		aray[i]._block_off = *(unsigned short*)(vector_schema_->getColumnAddess(1, chunk_tuples[i]->tuple_));
		aray[i]._tuple_off = *(unsigned short*)(vector_schema_->getColumnAddess(2, chunk_tuples[i]->tuple_));
///*for testing*/		cout << aray[i]._key << "\t" << aray[i]._block_off << "\t" << aray[i]._tuple_off << endl;
	}
	CSBPlusTree<int>* csb_tree = new CSBPlusTree<int> ();
	csb_tree->BulkLoad(aray, chunk_tuples.size());
	cout << "*************************CSB indexing build successfully!*************************\n";
	return csb_tree;
}


void bottomLayerSorting::cqsort(vector<void*> secondaryArray, int left,int right,Operate* op){
	cout << "Sort begins! left: " << left << " right: " << right << endl;
	int front=left+1;
	int end=right;
	//TODO:	orderby key can be any one of the column
	void * key=vector_schema_->getColumnAddess(0,secondaryArray[left]);
	if(left>=right)
		return;
	while(1){
		const void *lg=vector_schema_->getColumnAddess(0,secondaryArray[end]);
		const void *rg=key;
		while(op->greate(lg,rg)){
//			while(state_.input_->getcolumn(state_.orderbyKey_).operate->greate((const void *&)secondaryArray[end],(const void *&)key))
			end--;lg=vector_schema_->getColumnAddess(0,secondaryArray[end]);
		}
		const void *ll=vector_schema_->getColumnAddess(0,secondaryArray[front]);
		const void *rl=key;
		while(op->less(ll,rl)&&front<end){
			front++;ll=vector_schema_->getColumnAddess(0,secondaryArray[front]);
		}
		if(front>=end)
			break;
		void * &x=secondaryArray[front];
		void * &y=secondaryArray[end];
		swap(x,y);
		if(op->equal(vector_schema_->getColumnAddess(0,secondaryArray[front]),key))
			end--;
		else
			front++;
	}
	void *&m=secondaryArray[left];
	void *&n=secondaryArray[end];
	swap(m,n);
	if(left<front-1)
		cqsort(secondaryArray,left,front-1,op);
	if(end+1<right)
		cqsort(secondaryArray,end+1,right,op);
	cout << "Sort ends!\n";

}
