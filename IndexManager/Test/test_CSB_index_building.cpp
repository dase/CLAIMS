/*
 * test_CSB_index_building.cpp
 *
 *  Created on: Mar 19, 2014
 *      Author: scdong
 */

#include <vector>
#include <iostream>
#include <stdio.h>
#include "../../Environment.h"
#include "../../Resource/ResourceManagerMaster.h"
#include "../../Catalog/Catalog.h"
#include "../../Catalog/table.h"
#include "../../physical_operator/physical_projection_scan.h"
#include "../CSBIndexBuilding.h"
#include "test_index_manager.cpp"

using namespace std;

static int test_CSBIndexBuilding ()
{
	int master;
		printf("~!OKOKO!!!!!\n");
		printf("Master(0) or Slave(others)??\n");
		scanf("%d",&master);
		if(master!=0){
			Environment::getInstance(false);
		}
		else{

			Environment::getInstance(true);

			ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
			Catalog* catalog=Environment::getInstance()->getCatalog();

			TableDescriptor* table_1=new TableDescriptor("cj",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
			table_1->addAttribute("row_id",data_type(t_u_long));  				//0
			table_1->addAttribute("trade_date",data_type(t_int));
			table_1->addAttribute("order_no",data_type(t_u_long));
			table_1->addAttribute("sec_code",data_type(t_int));
			table_1->addAttribute("trade_dir",data_type(t_int));
			table_1->addAttribute("order_type",data_type(t_int));				//5

			vector<ColumnOffset> cj_proj0_index;
			cj_proj0_index.push_back(0);
			cj_proj0_index.push_back(1);
			cj_proj0_index.push_back(2);
			cj_proj0_index.push_back(3);
			cj_proj0_index.push_back(4);
			cj_proj0_index.push_back(5);
			const int partition_key_index_1=2;
			table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G0
			catalog->add_table(table_1);


			////////////////////////////////////////
			/* the following codes should be triggered by Load module*/
			//////////////////ONE DAY////////////////////////////////////////////////
			//cj_table
			// 4 partitions partitioned by order_no
			for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

				catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
			}
			/////////////////////////////////////////

			ProjectionBinding *pb=new ProjectionBinding();
			pb->BindingEntireProjection(catalog->getTable(0)->getProjectoin(0)->getPartitioner());

			printf("ready(?)\n");
			int input;
			scanf("%d",&input);

			vector<column_type> blc_column_list;
			blc_column_list.push_back(column_type(t_u_long));
			blc_column_list.push_back(column_type(t_int));
			blc_column_list.push_back(column_type(t_u_long));
			blc_column_list.push_back(column_type(t_int));		//sec_code for indexing
			blc_column_list.push_back(column_type(t_int));
			blc_column_list.push_back(column_type(t_int));

/*for testing the original data*/
//			Schema* ps_schema = new SchemaFix(blc_column_list);
//			ExpandableBlockStreamProjectionScan::State ps_state(catalog->getTable(0)->getProjectoin(0)->getProjectionID(), ps_schema, 64*1024);
//			ExpandableBlockStreamIteratorBase* ps = new ExpandableBlockStreamProjectionScan(ps_state);
//			ps->open();
//			BlockStreamBase* block = BlockStreamBase::createBlockWithDesirableSerilaizedSize(ps_schema, 64*1024);
//			void* tuple;
//			while (ps->next(block))
//			{
//				BlockStreamBase::BlockStreamTraverseIterator* iterator = block->createIterator();
//				while((tuple = iterator->nextTuple()) != 0)
//				{
//					ps_schema->displayTuple(tuple, " | ");
//					sleep(1);
//				}
//
//			}
//			ps->close();
/*for testing the original data*/


			Schema* blc_schema = new SchemaFix(blc_column_list);
			unsigned block_size = 64*1024;
			bottomLayerCollecting::State blc_state(catalog->getTable(0)->getProjectoin(0)->getProjectionID(), blc_schema, 3, block_size);
			PhysicalOperatorBase* blc = new bottomLayerCollecting(blc_state);

			vector<column_type> bls_column_list;
			bls_column_list.push_back(t_int);	//chunk offset
			bls_column_list.push_back(t_int);			//sec_code
			bls_column_list.push_back(t_u_smallInt);	//chunk offset
			bls_column_list.push_back(t_u_smallInt);	//chunk offset

			Schema* bls_schema = new SchemaFix(bls_column_list);
			bottomLayerSorting::State bls_state(bls_schema, blc, block_size, catalog->getTable(0)->getProjectoin(0)->getProjectionID(), 3, "sec_code_index");
			PhysicalOperatorBase* bls = new bottomLayerSorting(bls_state);

			bls->Open();
			BlockStreamBase* block;
			while(bls->Next(block))
			{

			}
			bls->Close();

			//following for test the index manager~
			test_index_manager_();
		}
		cout<<"Waiting~~~!~"<<endl;
		while(true){
			sleep(1);
		}
		return 0;
}


