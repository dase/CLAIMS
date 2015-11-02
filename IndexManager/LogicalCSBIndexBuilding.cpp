/*
 * LogicalCSBIndexBuilding.cpp
 *
 *  Created on: Mar 21, 2014
 *      Author: scdong
 */

#include "LogicalCSBIndexBuilding.h"
#include "../Catalog/Catalog.h"
#include "CSBIndexBuilding.h"

LogicalCSBIndexBuilding::LogicalCSBIndexBuilding(ProjectionID projection_id, Attribute index_attr, std::string index_name)
: projection_id_(projection_id), index_attr_(index_attr), index_name_(index_name){
	scan_projection_ = Catalog::getInstance()->getTable(index_attr_.table_id_)->getProjectoin(projection_id_.projection_off);
	assert(scan_projection_);
}

LogicalCSBIndexBuilding::~LogicalCSBIndexBuilding() {
	// TODO Auto-generated destructor stub
}

PlanContext LogicalCSBIndexBuilding::GetPlanContext()
{
	if(!scan_projection_->AllPartitionBound()){
		Catalog::getInstance()->getBindingModele()->BindingEntireProjection(scan_projection_->getPartitioner(),DESIRIABLE_STORAGE_LEVEL);
	}

	blc_dataflow_.attribute_list_ = scan_projection_->getAttributeList();
	Partitioner* par = scan_projection_->getPartitioner();
	blc_dataflow_.plan_partitioner_=PlanPartitioner(*par);
	blc_dataflow_.commu_cost_=0;
	return blc_dataflow_;
}

PhysicalOperatorBase* LogicalCSBIndexBuilding::GetPhysicalPlan(const unsigned &block_size)
{
	bottomLayerCollecting::State blc_state;
	blc_state.schema_ = GetSchema(blc_dataflow_.attribute_list_);
	blc_state.projection_id_ = projection_id_;
	for (unsigned i = 0; i < blc_dataflow_.attribute_list_.size(); i++)
	{
		if (blc_dataflow_.attribute_list_[i] == index_attr_)
		{
			blc_state.key_indexing_ = i;
			break;
		}
	}
	blc_state.block_size_ = block_size;
	PhysicalOperatorBase* blc = new bottomLayerCollecting(blc_state);

	bottomLayerSorting::State bls_state;

	vector<column_type> bls_column_list;
	bls_column_list.push_back(t_int);			//chunk offset
	bls_column_list.push_back(blc_state.schema_->getcolumn(blc_state.key_indexing_));			//sec_code
	bls_column_list.push_back(t_u_smallInt);	//chunk offset
	bls_column_list.push_back(t_u_smallInt);	//chunk offset

	bls_state.schema_ = new SchemaFix(bls_column_list);
	bls_state.child_ = blc;
	bls_state.block_size_ = block_size;
	bls_state.projection_id_ = projection_id_;
	bls_state.key_indexing_ = blc_state.key_indexing_;
	bls_state.index_name_ = index_name_;

	return new bottomLayerSorting(bls_state);

}
bool LogicalCSBIndexBuilding::GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size)
{
	assert(false);
	return false;
}
void LogicalCSBIndexBuilding::Print(int level) const
{
	printf("%*.CSBIndexBuilding: %s\n",level*8," ",Catalog::getInstance()->getTable(scan_projection_->getProjectionID().table_id)->getTableName().c_str());
}
