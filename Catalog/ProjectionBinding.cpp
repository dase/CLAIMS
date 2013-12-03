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

bool ProjectionBinding::BindingEntireProjection(Partitioner* part,const StorageLevel& desriable_storage_level){
	if(part->get_bing_mode_()==OneToOne){
		std::vector<std::pair<unsigned,NodeID> > partition_id_to_nodeid_list;
		ResourceManagerMaster* rmm=Environment::getInstance()->getResourceManagerMaster();
		std::vector<NodeID> node_id_list=rmm->getSlaveIDList();
		unsigned allocate_cur=0;
//		allocate_cur=rand()%node_id_list.size();
		for(unsigned i=0;i<part->getNumberOfPartitions();i++){
			NodeID target=node_id_list[allocate_cur];

			/*check whether target node has enough resource*/
			bool check_passed=false;
			unsigned buget=part->getPartitionDataSize(i);
			unsigned failures=0;
			while(!check_passed){
				switch(desriable_storage_level){
					case MEMORY:{
						if(rmm->ApplyMemoryBuget(target,buget)==true){
							check_passed=true;
							failures=0;
						}
						break;
					}
					case DISK:{
						if(rmm->ApplyDiskBuget(target,buget)==true){
							check_passed=true;
							failures=0;
						}
						break;
					}
					case HDFS:{
						check_passed=true;
						failures=0;
						break;
					}
					default:{
						break;
					}
				}

				if(!check_passed){
					allocate_cur=(allocate_cur+1)%node_id_list.size();
					target=node_id_list[allocate_cur];
					failures++;
					if(failures>node_id_list.size()){
						/* none of the available Slave could meet the budget;
						 * TODO: 1.Roll back the allocated budget for other partitions.
						 * 		 2.The partitioner may need to consider OneToMany mode so
						 * 		  that a single partition can be bound to multiple nodes.
						 * */
						printf("binding fails! not enough resource!\n");
						return false;
					}
				}

			}

			/* store the binding information in the list*/
			partition_id_to_nodeid_list.push_back(std::pair<unsigned,NodeID>(i,target));

			allocate_cur=(allocate_cur+1)%node_id_list.size();
			/*bind*/
//			part->bindPartitionToNode(i,node_id_list[allocate_cur]);
//
//
//
//
//			BlockManagerMaster::getInstance()->SendBindingMessage(partition_id,number_of_chunks,MEMORY,target);
		}
		/* conduct the binding according to the bingding information list*/
		for(unsigned i=0;i<partition_id_to_nodeid_list.size();i++){
			const unsigned partition_off=partition_id_to_nodeid_list[i].first;
			const NodeID node_id=partition_id_to_nodeid_list[i].second;
			/* update the information in Catalog*/
			part->bindPartitionToNode(partition_off,node_id);

			/* notify the StorageManger of the target node*/
			PartitionID partition_id(part->getProejctionID(),partition_off);
			const unsigned number_of_chunks=part->getPartitionChunks(partition_off);
			BlockManagerMaster::getInstance()->SendBindingMessage(partition_id,number_of_chunks,desriable_storage_level,node_id);
		}
		return true;
	}

	/**
	 * TODO: OneToMany mode where one partition is allowed to be bound to multiple nodes.
	 */
	return false;
}
