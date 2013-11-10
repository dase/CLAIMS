/*
 * Scan.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */
#include <stdio.h>
#include "Scan.h"
#include "../Environment.h"
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
	TableDescriptor* table=Environment::getInstance()->catalog_->getTable(table_id);
	ProjectionDescriptor* target_projection;
	ProjectionOffset target_projection_off=-1;
	for(ProjectionOffset projection_off=0;projection_off<table->getNumberOfProjection();projection_off++){
		ProjectionDescriptor* projection=table->getProjectoin(projection_off);
		bool fail=false;
		for(std::vector<Attribute>::iterator it;it!=scan_attribute_list_.end();it++){
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
	target_projection=table->getProjectoin(target_projection_off);
	/*build the data flow*/
	Dataflow dataflow;

	dataflow.attribute_list_=scan_attribute_list_; /*attribute_list*/

	dataflow.property_.partitioner=DataflowPartitionDescriptor(target_projection->getPartitioner());
	dataflow.property_.commnication_cost=0;
	return dataflow;

}
