/*
 * LogicalCSBIndexBuilding.h
 *
 *  Created on: Mar 21, 2014
 *      Author: scdong
 */

#ifndef LOGICALCSBINDEXBUILDING_H_
#define LOGICALCSBINDEXBUILDING_H_
#include "../LogicalQueryPlan/LogicalOperator.h"
#include "../Scheduler/Dataflow.h"
#include "../Catalog/Attribute.h"
#include "../ids.h"
#include "../Catalog/table.h"

class LogicalCSBIndexBuilding : public LogicalOperator {
public:
	//target projection id and the column attribute which will be indexed
	LogicalCSBIndexBuilding(ProjectionID projection_id, Attribute index_attr, std::string index_name);
	virtual ~LogicalCSBIndexBuilding();

	Dataflow getDataflow();
	BlockStreamIteratorBase* getIteratorTree(const unsigned &);
	bool GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size=4096*1024);
private:
	void print(int level = 0) const;
private:
//	unsigned index_offset_;
//	std::vector<Attribute> scan_attribute_list_;
	ProjectionID projection_id_;
	Attribute index_attr_;
	std::string index_name_;
	ProjectionDescriptor* scan_projection_;
	Dataflow blc_dataflow_;
	Dataflow bls_dataflow_;
};

#endif /* LOGICALCSBINDEXBUILDING_H_ */
