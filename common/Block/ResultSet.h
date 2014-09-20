/*
 * ResultSet.h
 *
 *  Created on: Jan 20, 2014
 *      Author: wangli
 */

#ifndef RESULTSET_H_
#define RESULTSET_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "DynamicBlockBuffer.h"

class ResultSet :public DynamicBlockBuffer{
public:
	ResultSet():DynamicBlockBuffer(),schema_(0){};
	ResultSet(const ResultSet& r);
	virtual ~ResultSet();
	void print()const;
	Schema* schema_;
	std::vector<std::string> column_header_list_;
	double query_time_;
private:
	void printNChar(int n, char c)const;
};

#endif /* RESULTSET_H_ */
