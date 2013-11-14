/*
 * Scan.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */
#include <stdio.h>
#include "Scan.h"
#include "../Environment.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamProjectionScan.h"
LogicalScan::LogicalScan(std::vector<Attribute> attribute_list):scan_attribute_list_(attribute_list) {
	// TODO Auto-generated constructor stub

}

LogicalScan::~LogicalScan() {
	// TODO Auto-generated destructor stub
}

LogicalProjection LogicalScan::getLogcialProjection()const{
	return logical_projection_;
}

Dataflow LogicalScan::getDataflow(){
	TableID table_id=scan_attribute_list_[0].table_id_;
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable(table_id);

	ProjectionOffset target_projection_off=-1;
	for(ProjectionOffset projection_off=0;projection_off<table->getNumberOfProjection();projection_off++){
		ProjectionDescriptor* projection=table->getProjectoin(projection_off);
		bool fail=false;
		for(std::vector<Attribute>::iterator it=scan_attribute_list_.begin();it!=scan_attribute_list_.end();it++){
			if(!projection->hasAttribute(*it)){
				/*the attribute *it does not in the projection*/
				fail=true;
				break;
			}
		}
		if(fail==true){
			continue;
		}
		target_projection_off=projection_off;
		break;
	}

	if(target_projection_off==-1){
		/*fail to find a projection that contains all the scan attribute*/
		printf("The current implementation does not support the scanning that involves more than one projection.\n");
		assert(false);
	}
	target_projection_=table->getProjectoin(target_projection_off);
	/*build the data flow*/


	dataflow_.attribute_list_=scan_attribute_list_; /*attribute_list*/

	dataflow_.property_.partitioner=DataflowPartitionDescriptor(*target_projection_->getPartitioner());
	dataflow_.property_.commnication_cost=0;
	return dataflow_;

}
BlockStreamIteratorBase* LogicalScan::getIteratorTree(const unsigned &block_size){
	/* In the current implementation, all the attributes within the involved projection
	 * are read.
	 * TODO: Ideally, the columns in one projection are stored separately and only the
	 * needed columns are touch for a given query.
	 */
	ExpandableBlockStreamProjectionScan::State state;
	state.block_size_=block_size;
	state.projection_id_=target_projection_->getProjectionID();
	state.schema_=getSchema(dataflow_.attribute_list_);
	return new ExpandableBlockStreamProjectionScan(state);
}
