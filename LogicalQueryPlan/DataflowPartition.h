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
	DataflowPartition(unsigned partitoin_id,int cardinality,NodeID location);
	DataflowPartition(const DataflowPartition& part);
	unsigned getDataCardinality()const;
	void setDataCardinality(const unsigned&);
	bool isAvaliable()const;
	void setFiltered();
	NodeID getLocation()const;
	virtual ~DataflowPartition();
protected:
	unsigned partition_id_;
	int cardinality_;
	NodeID location_;
	bool filtered_;

};

#endif /* DATAFLOWBRANCH_H_ */
