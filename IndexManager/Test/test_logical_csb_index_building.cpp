/*
 * test_logical_csb_index_building.cpp
 *
 *  Created on: Mar 21, 2014
 *      Author: scdong
 */

#include "../LogicalCSBIndexBuilding.h"
#include "../CSBPlusTree.h"
#include "../IndexManager.h"
#include "../IndexScanIterator.h"
#include "../LogicalIndexScan.h"

#include "../../Environment.h"
#include "../../Resource/ResourceManagerMaster.h"
#include "../../Catalog/Catalog.h"
#include "../../logical_operator/LogicalQueryPlanRoot.h"

static int test_logical_csb_index_building()
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
			table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",1);	//G0

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



			printf("ready(?)\n");
			int input;
			scanf("%d",&input);

			LogicalOperator* csb_building = new LogicalCSBIndexBuilding(table_1->getProjectoin(0)->getProjectionID(), table_1->getAttribute(3), "sec_code_index");
			const NodeID collector_node_id=0;
			LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,csb_building,LogicalQueryPlanRoot::RESULTCOLLECTOR);
<<<<<<< HEAD
			root->Print();
=======
			root->Print();
>>>>>>> master-yk-150927
			PhysicalOperatorBase* executable_query_plan=root->GetPhysicalPlan(1024*64);
//			executable_query_plan->print();
//			IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(executable_query_plan,"127.0.0.1");
			executable_query_plan->Open();
			cout << "open finished!\n";
			while (executable_query_plan->Next(0));
			cout << "next finished!\n";
			executable_query_plan->Close();
			cout << "close finished!\n";

//			ResultSet* result_set = executable_query_plan->getResultSet();

			executable_query_plan->~PhysicalOperatorBase();
			root->~LogicalOperator();
cout << "index building finished!\n";
/********************************* Logical Index Scan *********************************/
			vector<IndexScanIterator::query_range> q_range;
			q_range.clear();
			int value_low = 10107;
			int value_high = 10110;

			IndexScanIterator::query_range q1;
			q1.value_low = malloc(sizeof(int));		//newmalloc
			q1.value_low = (void*)(&value_low);
			q1.comp_low = EQ;
			q1.value_high = malloc(sizeof(int));		//newmalloc
			q1.value_high = (void*)(&value_low);
			q1.comp_high = EQ;
			q1.c_type.type = t_int;
			q1.c_type.operate = new OperateInt();
			q1.valuebytes_low.clear();
			q1.valuebytes_high.clear();
			q_range.push_back(q1);

			IndexScanIterator::query_range q2;
			q2.value_low = malloc(sizeof(int));		//newmalloc
			q2.value_low = (void*)(&value_high);
			q2.comp_low = EQ;
			q2.value_high = malloc(sizeof(int));		//newmalloc
			q2.value_high = (void*)(&value_high);
			q2.comp_high = EQ;
			q2.c_type.type = t_int;
			q2.c_type.operate = new OperateInt();
			q_range.push_back(q2);

			LogicalOperator* index_scan = new LogicalIndexScan(table_1->getProjectoin(0)->getProjectionID(), table_1->getAttribute(3), q_range);
			root=new LogicalQueryPlanRoot(collector_node_id,index_scan,LogicalQueryPlanRoot::PRINT);
<<<<<<< HEAD
			root->Print();
=======
			root->Print();
>>>>>>> master-yk-150927
			executable_query_plan=root->GetPhysicalPlan(1024*64);
//			executable_query_plan->print();
//			IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(executable_query_plan,"127.0.0.1");
			executable_query_plan->Open();
			while (executable_query_plan->Next(0));
			executable_query_plan->Close();

			executable_query_plan->~PhysicalOperatorBase();
			root->~LogicalOperator();

		}
		cout<<"Waiting~~~!~"<<endl;
		while(true){
			sleep(1);
		}
		return 0;
}
