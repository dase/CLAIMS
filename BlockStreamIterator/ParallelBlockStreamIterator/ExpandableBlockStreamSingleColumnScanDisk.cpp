/*
 * ExpandableBlockStreamSingleColumnScanDisk.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: wangli
 */
#include <memory.h>
#include <malloc.h>
#include <sys/mman.h>
#include <errno.h>
#include "../../rename.h"
#include "ExpandableBlockStreamSingleColumnScanDisk.h"

ExpandableBlockStreamSingleColumnScanDisk::ExpandableBlockStreamSingleColumnScanDisk(State state)
:state_(state), fd_(-1), file_length_(0), base_(0), data_(0), open_finished_(false) {
	sema_open_.set_value(1);
	sema_open_finished_.set_value(0);
}

ExpandableBlockStreamSingleColumnScanDisk::ExpandableBlockStreamSingleColumnScanDisk()
:fd_(-1), file_length_(0), base_(0), data_(0), open_finished_(false){
	sema_open_.set_value(1);
	sema_open_finished_.set_value(0);
	}

ExpandableBlockStreamSingleColumnScanDisk::~ExpandableBlockStreamSingleColumnScanDisk() {
	// TODO Auto-generated destructor stub
}

ExpandableBlockStreamSingleColumnScanDisk::State::State(std::string file_name,
		Schema* schema, unsigned block_size) :
		schema_(schema), filename_(file_name), block_size_(block_size) {
	used_size_control_ = false;
}

ExpandableBlockStreamSingleColumnScanDisk::State::State(std::string file_name,
		Schema* schema, unsigned block_size, unsigned long used_length) :
		schema_(schema), filename_(file_name), block_size_(block_size), used_length_(used_length) {
	used_size_control_ = true;
}

bool ExpandableBlockStreamSingleColumnScanDisk::open() {
	block_for_reading_=BlockStreamBase::createBlock(state_.schema_,state_.block_size_);

	if (sema_open_.try_wait()) {
		printf("Scan open!\n");

		/* the winning thread does the read job in the open function*/
		fd_ = FileOpen(state_.filename_.c_str(), O_RDONLY);
		if (fd_ == -1) {
			printf("Cannot open file %s! Reason: %s\n",
					state_.filename_.c_str(), strerror(errno));
			return false;
		}
		file_length_ = lseek(fd_, 0, SEEK_END);
		if (state_.used_size_control_ && (state_.used_length_ < file_length_))
			file_length_ = state_.used_length_;
//		cout << "Disk scan iterator in, scan size: " << file_length_ << endl;
		lseek(fd_,0,SEEK_SET);

		printf("Open is successful!\n");
		open_finished_ = true;
		return true;

	} else {
		while (!open_finished_) {
			usleep(1);
		}
		return true;
	}
}

bool ExpandableBlockStreamSingleColumnScanDisk::next(BlockStreamBase* block) {
	unsigned read_size = 0;
	file_flength_lock_.acquire();
	if (state_.block_size_ <= file_length_)
		read_size=read(fd_,block_for_reading_->getBlockDataAddress(),state_.block_size_);
	else
		read_size = read(fd_,block_for_reading_->getBlockDataAddress(),file_length_);
	if(read_size>0){
		file_length_ -= read_size;
		block->copyBlock(block_for_reading_->getBlockDataAddress(),read_size);
		file_flength_lock_.release();
		return true;
	}
	else
	{
		cout << "disk scan return false\n";
		file_flength_lock_.release();
		return false;
	}
}

bool ExpandableBlockStreamSingleColumnScanDisk::close() {
	sema_open_.post();

	open_finished_ = false;
//	free(data_);
//	data_ = 0;
//	munmap(base_, file_length_);
	if (FileClose(fd_) == 0)
		return true;
	else
		return false;
}
