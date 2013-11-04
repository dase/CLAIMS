/*
 * BlockStreamSingleColumnScan.h
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMSINGLECOLUMNSCAN_H_
#define BLOCKSTREAMSINGLECOLUMNSCAN_H_
#include <string>
#include "BlockStreamIteratorBase.h"
#include "../Schema/Schema.h"
class BlockStreamSingleColumnScan:public BlockStreamIteratorBase {
public:
	class State{
	friend class BlockStreamSingleColumnScan;
	public:
		State(std::string file_name,Schema* schema);
	private:
		Schema* schema_;
		std::string filename_;
	};
	BlockStreamSingleColumnScan(State state);
	virtual ~BlockStreamSingleColumnScan();
	bool open();
	bool next(BlockStreamBase *block);
	bool close();
private:
	int fd_;
	State state_;
	unsigned file_length_;
	char* base_;
	char* data_;
	char* cursor_;
};

#endif /* BLOCKSTREAMSINGLECOLUMNSCAN_H_ */
