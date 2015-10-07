/*
 * ExpandableBlockStreamHdfsScan.cpp
 *
 *  Created on: 2013-10-12
 *      Author: casa
 */

#include "../physical_query_plan/ExpandableBlockStreamHdfsScan.h"

ExpandableBlockStreamHdfsScan::ExpandableBlockStreamHdfsScan(State state)
:state_(state),open_finished_(false){
	// TODO 自动生成的构造函数存根
	sema_open_.set_value(1);
}

ExpandableBlockStreamHdfsScan::ExpandableBlockStreamHdfsScan()
:open_finished_(false){
	sema_open_.set_value(1);
}

ExpandableBlockStreamHdfsScan::~ExpandableBlockStreamHdfsScan() {
	// TODO 自动生成的析构函数存根
}

ExpandableBlockStreamHdfsScan::State::State(string partition_file_name,Schema *partition_schema,unsigned block_size)
:partition_file_name_(partition_file_name),
 partition_schema_(partition_schema),
 block_size_(block_size){

}

bool ExpandableBlockStreamHdfsScan::Open(const PartitionOffset&){
	if(sema_open_.try_wait()){
		/*open every block of a certain partition*/
//		MemoryStore *ms=new MemoryStore();
//		vector<HdfsBlock> hdfsblock_array=ms->getFromMem(state_.partition_file_name_);
//		cout<<hdfsblock_array.size()<<endl;
		BlockManager::BlockManagerWorkerActor *bmk=0;
		BlockManager *bm=BlockManager::getInstance();
/*		vector<ChunkInfo> vci=bm->loadFromHdfs(state_.partition_file_name_.c_str());*/
		// 在此将所有的指针都存进去，非空即存
		int offset=0;
		void *chunkptr;
		bool bl=true;
		string filename=bm->askForMatch(state_.partition_file_name_.c_str(),*(bm->getId()));
		cout<<"get filename is filename: "<<filename.c_str()<<endl;
		while(bl){
			ostringstream chunkid;
			chunkid<<filename.c_str()<<"$"<<offset;
			cout<<"chunkid: "<<chunkid.str().c_str()<<endl;
			chunkptr=bm->getLocal(chunkid.str().c_str());
			if(chunkptr==0)
				break;
			file_set_.push_back((char *)chunkptr);
			offset++;
		}
		cb_.file_index=0;
		cb_.cursor=cb_.base=file_set_[0];
		cb_.length=CHUNK_SIZE;
		open_finished_=true;
		return true;
	}
	else{
		while(!open_finished_){
			usleep(1);
		}
		return true;
	}
}

bool ExpandableBlockStreamHdfsScan::Next(BlockStreamBase *block){
//	void *toTuple=memalign(cacheline_size,state_.partition_schema_->getTupleMaxSize());
//	void *column_in_toTuple;
	allocated_block allo_block_temp;
	if(cb_.file_index<file_set_.size()){
		lock_.acquire();
		if(cb_.cursor>=cb_.base+cb_.length){
			cb_.file_index++;
			cb_.cursor=cb_.base=file_set_[cb_.file_index];
			cb_.length=64*1024*1024;
		}
		lock_.release();
		if(cb_.file_index<file_set_.size()){
			if(atomicIncreaseCursor(state_.block_size_,allo_block_temp)){
				block->copyBlock(allo_block_temp.start, allo_block_temp.length);
				return true;
			}
		}
		else{
			return false;
		}
	}
	return false;
//	void *tuple=0;
//	while((tuple=block->allocateTuple(state_.partition_schema_->getTupleMaxSize()))>0){
//		if(cb_.cursor<cb_.sublock_size){
//			for(unsigned i=0;i<state_.partition_schema_->getncolumns();i++){
//				key_in_hashtable=state_.input_schema_left->getColumnAddess(state_.joinIndex_left[i],cur_in_ht_itr);
//				column_in_toTuple=state_.partition_schema_->getColumnAddess(i,toTuple);
//				state_.input_schema_left->getcolumn(i).operate->assignment(key_in_hashtable,column_in_joinedTuple);
//			}
//		}
//		else{
//			cb_.sublock_index++;
//			if(cb_.sublock_index==(cb_.length/cb_.sublock_size-1)){
//				cb_.file_index++;
//				cb_.base=cb_.cursor=file_set_[cb_.file_index];
//				cb_.sublock_index=0;
//				cb_.sublock_size=SUBLOCK_SIZE;
//				cb_.length=BLOCK_SIZE;
//			}
//		}
//	}
//	return true;
}

bool ExpandableBlockStreamHdfsScan::Close(){
	sema_open_.post();
	open_finished_ = false;
	cb_.base=cb_.cursor=0;
	cb_.file_index=0;
	cb_.length=0;
	file_set_.clear();
	return true;
}
void ExpandableBlockStreamHdfsScan::Print(){
	printf("Scan!\n");
	printf("----------------\n");
}
bool ExpandableBlockStreamHdfsScan::atomicIncreaseCursor(unsigned bytes,allocated_block &allo_block) {
	bool ret;
	lock_.acquire();
	if (cb_.cursor+bytes<=cb_.base+cb_.length){
		allo_block.start =cb_.cursor;
		allo_block.length =bytes;
		cb_.cursor+=allo_block.length;
		ret=true;
	}
	/* there some data remaining, but the data is smaller than the expected bytes.*/
	else if(cb_.cursor<cb_.base+cb_.length){
		allo_block.start=cb_.cursor;
		allo_block.length=cb_.base+cb_.length-cb_.cursor;
		cb_.cursor+=allo_block.length;
		ret=true;
	}else{
		ret=false;
	}
	lock_.release();
	return ret;
}

