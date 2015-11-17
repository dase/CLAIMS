/*
 * LogicalIndexScan.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: scdong
 */

#include "LogicalIndexScan.h"
#include "IndexManager.h"
#include "../Catalog/Catalog.h"

LogicalIndexScan::LogicalIndexScan() {
	// TODO Auto-generated constructor stub

}

LogicalIndexScan::LogicalIndexScan(ProjectionID projection_id, Attribute index_attr, vector<IndexScanIterator::query_range> q_range)
: projection_id_(projection_id), index_attr_(index_attr), query_range_(q_range)
{
	scan_projection_ = Catalog::getInstance()->getTable(index_attr_.table_id_)->getProjectoin(projection_id_.projection_off);
	assert(scan_projection_);
}

LogicalIndexScan::~LogicalIndexScan() {
	// TODO Auto-generated destructor stub
}

PlanContext LogicalIndexScan::GetPlanContext()
{
	if(!scan_projection_->AllPartitionBound()){
		printf("Binded!\n");
		Catalog::getInstance()->getBindingModele()->BindingEntireProjection(scan_projection_->getPartitioner(),DESIRIABLE_STORAGE_LEVEL);
	}

	dataflow_.attribute_list_ = scan_projection_->getAttributeList();
	Partitioner* par = scan_projection_->getPartitioner();
	dataflow_.plan_partitioner_ = PlanPartitioner(*par);
	dataflow_.commu_cost_ = 0;

	return dataflow_;
}

PhysicalOperatorBase* LogicalIndexScan::GetPhysicalPlan(const unsigned & blocksize)
{
	IndexScanIterator::State state;
	state.schema_ = GetSchema(dataflow_.attribute_list_);
	state.projection_id_ = projection_id_;
	state.block_size_ = blocksize;
	state.query_range_ = query_range_;
	state.index_id_ = IndexManager::getInstance()->getIndexID(index_attr_);

	return new IndexScanIterator(state);
}

bool LogicalIndexScan::GetOptimalPhysicalPlan(Requirement requirement, PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size)
{
	assert(false);
	return false;
}

void LogicalIndexScan::Print(int level) const
{
	cout << "level "  << level << ": Logical Index Scan\n";
}
