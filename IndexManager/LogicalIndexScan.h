/*
 * LogicalIndexScan.h
 *
 *  Created on: Apr 1, 2014
 *      Author: scdong
 */

#ifndef LOGICALINDEXSCAN_H_
#define LOGICALINDEXSCAN_H_

#include "../common/ids.h"
#include "IndexScanIterator.h"
#include "../Catalog/table.h"
#include "../logical_operator/logical_operator.h"
using namespace claims::logical_operator;

class LogicalIndexScan :public LogicalOperator{
public:
	LogicalIndexScan();
	LogicalIndexScan(ProjectionID projection_id, Attribute index_attr, vector<IndexScanIterator::query_range> q_range);
	virtual ~LogicalIndexScan();

	PlanContext GetPlanContext();
	PhysicalOperatorBase* GetPhysicalPlan(const unsigned& blocksize);
	bool GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size=4096*1024);
	void Print(int level=0)const;

private:
	ProjectionID projection_id_;
	Attribute index_attr_;
	vector<IndexScanIterator::query_range> query_range_;
	ProjectionDescriptor* scan_projection_;
	PlanContext dataflow_;
};

#endif /* LOGICALINDEXSCAN_H_ */
