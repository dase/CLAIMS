/*
 * test.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */

#include "../LogicalOperator.h"
#include "../../Environment.h"
#include "../../Catalog/ProjectionBinding.h"
int main(){
	Environment::getInstance(true);

	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	rmms->RegisterNewSlave("192.168.1.1");
	rmms->RegisterNewSlave("192.168.1.2");
	rmms->RegisterNewSlave("192.168.1.3");
	rmms->RegisterNewSlave("192.168.1.4");
	rmms->RegisterNewSlave("192.168.1.5");
	rmms->RegisterDiskBuget(0,10000);
	rmms->RegisterDiskBuget(1,10000);
	rmms->RegisterDiskBuget(2,10000);
	rmms->RegisterDiskBuget(3,10000);
	rmms->RegisterDiskBuget(4,10000);

	TableDescriptor* table=new TableDescriptor("Student",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	table->addAttribute("Name",data_type(t_string),10);
	table->addAttribute("Age",data_type(t_int));
	table->addAttribute("Gender",data_type(t_int));
	table->addAttribute("Score",data_type(t_float));

	vector<ColumnOffset> index;
	index.push_back(0);
	index.push_back(1);
	index.push_back(3);
	const int partition_key_index=3;
	table->createHashPartitionedProjection(index,partition_key_index,4);
	Catalog* catalog=Environment::getInstance()->getCatalog();
	catalog->add_table(table);
	///////////////////////////////////////


	////////////////////////////////////////
	/* the following codes should be triggered by Load module*/

	for(unsigned i=0;i<table->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,"Partition_"+i,12);
	}
	////////////////////////////////////////






	ProjectionBinding *pb=new ProjectionBinding();
	pb->BindingEntireProjection(catalog->getTable(0)->getProjectoin(0)->getPartitioner());





	printf("Waiting~\n");
	while(true){
		sleep(1);

	}


}

