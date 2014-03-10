/*
 * ExpandableBlockStreamSingleColumnScan.cpp
 *
 *  Created on: Aug 27, 2013
 *      Author: wangli
 */
#include <memory.h>
#include <malloc.h>
#include <sys/mman.h>
#include <errno.h>
#include "../../rename.h"
#include "ExpandableBlockStreamSingleColumnScan.h"

ExpandableBlockStreamSingleColumnScan::ExpandableBlockStreamSingleColumnScan(State state)
:state_(state), fd_(-1), file_length_(0), base_(0), data_(0), cursor_(0),
 open_finished_(false) {
	sema_open_.set_value(1);
	sema_open_finished_.set_value(0);
}

ExpandableBlockStreamSingleColumnScan::ExpandableBlockStreamSingleColumnScan()
:fd_(-1), file_length_(0), base_(0), data_(0), cursor_(0),open_finished_(false){
	sema_open_.set_value(1);
	sema_open_finished_.set_value(0);
	}

ExpandableBlockStreamSingleColumnScan::~ExpandableBlockStreamSingleColumnScan() {
	// TODO Auto-generated destructor stub
}

ExpandableBlockStreamSingleColumnScan::State::State(std::string file_name,
		Schema* schema, unsigned block_size) :
		schema_(schema), filename_(file_name), block_size_(block_size) {
	used_size_control_ = false;
}

ExpandableBlockStreamSingleColumnScan::State::State(std::string file_name,
		Schema* schema, unsigned block_size, unsigned long used_length) :
		schema_(schema), filename_(file_name), block_size_(block_size), used_length_(used_length) {
	used_size_control_ = true;
}

bool ExpandableBlockStreamSingleColumnScan::open(const PartitionOffset& part_off) {
	if (sema_open_.try_wait()) {
//		printf("Scan open!\n");

		/* the winning thread does the read job in the open function*/
		fd_ = FileOpen(state_.filename_.c_str(), O_RDONLY);
		if (fd_ == -1) {
			printf("Cannot open file %s! Reason: %s\n",
					state_.filename_.c_str(), strerror(errno));
			return false;
		}
		file_length_ = lseek(fd_, 0, SEEK_END);
		if (state_.used_size_control_ && (state_.used_length_ < file_length_))
		{
			file_length_ = state_.used_length_;
			cout << "scan iterator in, scan size: " << file_length_ << endl;
		}

		base_ = (char*) mmap(0, file_length_, PROT_READ, MAP_PRIVATE, fd_, 0);

		if (base_ == 0) {
			printf("mmap errors!\n");
			return false;
		}

		data_=base_;
//		data_ = (char*) malloc(file_length_);
//
//		if (data_ == 0) {
//			printf("malloc errors!\n");
//			return false;
//		}
//		memcpy(data_, base_, file_length_);
		if (data_ != 0) {
			cursor_ = (char*) data_;
//			printf("Open is successful!\n");
			/* Set a value that is much lager than the number of parallel threads even in an extreme case.*/
//			sema_open_finished_.set_value(10000);
			open_finished_ = true;

			return true;
		} else
			return false;
	} else {
		while (!open_finished_) {
			usleep(1);
		}
//		sema_open_finished_.wait();
		return true;
	}
}

bool ExpandableBlockStreamSingleColumnScan::next(BlockStreamBase* block) {
	allocated_block allo_block_temp;
	if (file_length_ == 0)
	{
		cout << "scan iterator return false!\n";
		return false;
	}

//	if (cursor_ + state_.block_size_ <= data_ + file_length_) {
//		cout << "full!\n";
//	}
//	else if (cursor_ == data_ + file_length_)
//		cout << "nothing!\n";
//	else if (cursor_ <= data_ + file_length_)
//		cout << "part!\n";

//	if (atomicIncreaseCursor(state_.block_size_, allo_block_temp)) {
	if (atomicIncreaseCursor(block->getSerializedBlockSize(), allo_block_temp)) {
//		block->copyBlock(allo_block_temp.start, allo_block_temp.length);
		Block fake_block(allo_block_temp.length,allo_block_temp.start);
		block->constructFromBlock(fake_block);
		return true;
	}
//	cout << "scan iterator return false!\n";
	return false;
}

bool ExpandableBlockStreamSingleColumnScan::close() {
	sema_open_.post();

	open_finished_ = false;
//	free(data_);
	data_ = 0;
	munmap(base_, file_length_);
	if (FileClose(fd_) == 0)
		return true;
	else
		return false;
}

bool ExpandableBlockStreamSingleColumnScan::atomicIncreaseCursor(unsigned bytes,
		allocated_block &allo_block) {
	bool ret;
	cursor_lock_.acquire();
	if (cursor_ + bytes <= data_ + file_length_) {
		allo_block.start = cursor_;
		allo_block.length = bytes;
		cursor_ += allo_block.length;
//		cout << "a full block returned!\n";
		ret = true;
	}
	/* there some data remaining, but the data is smaller than the expected bytes.*/
	else if (cursor_ < data_ + file_length_) {
		allo_block.start = cursor_;
		allo_block.length = data_ + file_length_ - cursor_;
		cursor_ += allo_block.length;
//		cout << "rest data returned!\n";
		ret = true;
	} else {
//		cout << "no block remained!\n";
		ret = false;
	}
	cursor_lock_.release();
	return ret;
}
