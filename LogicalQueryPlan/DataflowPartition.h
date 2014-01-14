/*
 * DataflowBranch.h
 *
 *  Created on: Nov 11, 2013
 *      Author: wangli
 */

#ifndef DATAFLOWBRANCH_H_
#define DATAFLOWBRANCH_H_
#include "../ids.h"
//#include "../Catalog"



class DataflowPartition {
public:
	friend class DataflowPartitioningDescriptor;
	DataflowPartition():filtered_(false){};
	DataflowPartition(unsigned partitoin_id,int datasize,NodeID location);
	DataflowPartition(const DataflowPartition& part);
	unsigned getDataSize()const;
	void setDataSize(const unsigned&);
	bool isAvaliable()const;
	void setFiltered();
	NodeID getLocation()const;
	virtual ~DataflowPartition();
protected:
	unsigned partition_id_;
	int datasize_;
	NodeID location_;
	bool filtered_;

};

#endif /* DATAFLOWBRANCH_H_ */
