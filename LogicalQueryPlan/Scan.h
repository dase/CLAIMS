/*
 * Scan.h
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */

#ifndef SCAN_H_
#define SCAN_H_
#include "LogicalOperator.h"
#include "../Schema/LogicalProjection.h"
#include "../ids.h"
class LogicalScan: public LogicalOperator {
public:
	LogicalScan(std::vector<Attribute> attribute_list);
	LogicalScan(const TableID&);
	LogicalScan(ProjectionDescriptor* projection,const float sample_rate_=1);
	LogicalScan(const TableID&,const std::vector<unsigned>& selected_attribute_index_list);
	virtual ~LogicalScan();
	LogicalProjection getLogcialProjection()const;
	Dataflow getDataflow();
	BlockStreamIteratorBase* getIteratorTree(const unsigned &);

private:
	/**check whether all the involved attributes are in the same projection.*/
	bool checkInASingleProjection()const;
private:
	LogicalProjection logical_projection_;
//	Partitioner* partition_info_;
//	Dataflow* dataflow_;
	std::vector<Attribute> scan_attribute_list_;
	ProjectionDescriptor* target_projection_;
	Dataflow dataflow_;
	float sample_rate_;

};

#endif /* SCAN_H_ */
