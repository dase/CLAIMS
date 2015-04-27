/*
 * BlockStreamSingleColumnScan.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */
#include <memory.h>
#include <malloc.h>
#include <sys/mman.h>
#include <errno.h>
#include "BlockStreamSingleColumnScan.h"
#include "../common/rename.h"
BlockStreamSingleColumnScan::BlockStreamSingleColumnScan(State state)
:state_(state),data_(0),cursor_(0),file_length_(0),base_(0),fd_(0){

}

BlockStreamSingleColumnScan::~BlockStreamSingleColumnScan() {
	// TODO Auto-generated destructor stub
}

BlockStreamSingleColumnScan::State::State(std::string file_name,Schema* schema)
:schema_(schema),filename_(file_name){

}

bool BlockStreamSingleColumnScan::open(const PartitionOffset& part_off){

	fd_=FileOpen(state_.filename_.c_str(),O_RDONLY);
	if(fd_==-1){
		printf("Cannot open file %s! Reason: %s\n",state_.filename_.c_str(),strerror(errno));
		return false;
	}
	file_length_=lseek(fd_,0,SEEK_END);

	base_=(char*)mmap(0,file_length_,PROT_READ,MAP_PRIVATE,fd_,0);

	if(base_==0){
		printf("mmap errors!\n");
		return false;
	}

	data_=(char*)malloc(file_length_);		//newmalloc

	if(data_==0){
		printf("malloc errors!\n");
		return false;
	}
	memcpy(data_,base_,file_length_);
	if(data_!=0){
		cursor_=(char*)data_;
		printf("Open is successful!\n");
		return true;
	}
	else
		return false;
}

bool BlockStreamSingleColumnScan::next(BlockStreamBase *block){
	void* tuple;
	while(cursor_<data_+file_length_){
		const unsigned kbytes=state_.schema_->getTupleActualSize(cursor_);
		if((tuple=block->allocateTuple(kbytes))>0){
			state_.schema_->columns[0].operate->assign(cursor_,tuple);
			cursor_+=kbytes;
		}
		else{
			return true;
		}
	}
	if(!block->Empty()){
		return true;
	}

	return false;
}
bool BlockStreamSingleColumnScan::close(){
	free(data_);
	data_=0;
//	munmap(base,length);
	if(FileClose(fd_)==0) {
		std::cout<<"in "<<__FILE__<<":"<<__LINE__;printf("-----for debug: close fd %d.\n", fd_);
		return true;
	}
	else {
		return false;
	}
}
