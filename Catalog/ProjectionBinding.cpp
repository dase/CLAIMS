/*
 * ProjectionBinding.cpp
 *
 *  Created on: Nov 2, 2013
 *      Author: wangli
 */
#include <vector>
#include "ProjectionBinding.h"
#include "../Environment.h"
ProjectionBinding::ProjectionBinding() {
	// TODO Auto-generated constructor stub

}

ProjectionBinding::~ProjectionBinding() {
	// TODO Auto-generated destructor stub
}

bool ProjectionBinding::BindingEntireProjection(Partitioner* part){
	if(part->get_bing_mode_()==OneToOne){
		ResourceManagerMaster* rmm=Environment::getInstance()->getResourceManagerMaster();
		std::vector<NodeID> node_id_list=rmm->getSlaveIDList();
		unsigned allocate_cur=0;
		for(unsigned i=0;i<part->getNumberOfPartitions();i++){
			NodeID target=node_id_list[allocate_cur];

			/*check whether target node has enough resource*/
			bool check_passed=false;
			unsigned buget=part->getPartitionDataSize(i);
			unsigned failures=0;
			while(!check_passed){

				if(rmm->ApplyDiskBuget(target,buget)==true){
					check_passed=true;
				}
				else{
					allocate_cur=(allocate_cur+1)%node_id_list.size();
					target=node_id_list[allocate_cur];
					failures++;
				}
				if(failures>node_id_list.size()){
					/* none of the available Slave could meet the budget;
					 * TODO: 1.Roll back the allocated budget for other partitions.
					 * 		 2.The partiitoner may need to consider OneToMany mode so
					 * 		  that a single partition can be bound to multiple nodes.
					 * */
					return false;
				}
			}

			/*bind*/
			part->bindPartitionToNode(i,node_id_list[allocate_cur]);
			allocate_cur=(allocate_cur+1)%node_id_list.size();
		}
	}
	return true;
	/**
	 * TODO: OneToMany mode where one partition is allowed to be bound to multiple nodes.
	 */
}
