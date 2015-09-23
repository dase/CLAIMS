/*
 * Expanded_iterators_test.cpp
 *
 *  Created on: Mar 9, 2014
 *      Author: wangli
 */
#ifndef __EXPANDED_ITERATORS_TEST__
#define __EXPANDED_ITERATORS_TEST__
#include <vector>
#include <iostream>
#include "../../../Environment.h"
#include "../../../LogicalQueryPlan/Scan.h"
#include "../../../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../../../LogicalQueryPlan/logical_equal_join.h"
#include "../../../Catalog/ProjectionBinding.h"
#include "../../../LogicalQueryPlan/Filter.h"
#include "../../../LogicalQueryPlan/Aggregation.h"
#include "../../../LogicalQueryPlan/Buffer.h"
#include "../../../utility/rdtsc.h"
#include "../../../Executor/ExpanderTracker.h"
#include "../../../common/AttributeComparator.h"

#include "../../BlockStreamPerformanceMonitorTop.h"

#include "../BlockStreamExpander.h"

using namespace std;
static int expanded_iterators_test(){
	int master;
//	cout<<"Master(0) or Slave(others)"<<endl;
//	cin>>master;
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
		table_1->addAttribute("order_no",data_type(t_u_long),0,true);
		table_1->addAttribute("sec_code",data_type(t_int));
		table_1->addAttribute("trade_dir",data_type(t_int));
		table_1->addAttribute("order_type",data_type(t_int));				//5
		table_1->addAttribute("trade_no",data_type(t_int));
		table_1->addAttribute("trade_time",data_type(t_int));
		table_1->addAttribute("trade_time_dec",data_type(t_u_long));
		table_1->addAttribute("order_time",data_type(t_int));
		table_1->addAttribute("order_time_dec",data_type(t_u_long));		//10
		table_1->addAttribute("trade_price",data_type(t_double));
		table_1->addAttribute("trade_amt",data_type(t_double));
		table_1->addAttribute("trade_vol",data_type(t_double));
		table_1->addAttribute("pbu_id",data_type(t_int));
		table_1->addAttribute("acct_id",data_type(t_int));					//15
		table_1->addAttribute("order_prtfil_code",data_type(t_int));
		table_1->addAttribute("tran_type",data_type(t_int));
		table_1->addAttribute("trade_type",data_type(t_int));
		table_1->addAttribute("proc_type",data_type(t_int));

		vector<ColumnOffset> cj_proj0_index;
		cj_proj0_index.push_back(0);
		cj_proj0_index.push_back(1);
		cj_proj0_index.push_back(2);
		cj_proj0_index.push_back(3);
		cj_proj0_index.push_back(4);
		cj_proj0_index.push_back(5);
		const int partition_key_index_1=2;
//		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G0
		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",1);	//G0
//		catalog->add_table(table_1);
		vector<ColumnOffset> cj_proj1_index;
		cj_proj1_index.push_back(0);
		cj_proj1_index.push_back(6);
		cj_proj1_index.push_back(7);
		cj_proj1_index.push_back(8);
		cj_proj1_index.push_back(9);
		cj_proj1_index.push_back(10);
		cj_proj1_index.push_back(11);
		cj_proj1_index.push_back(12);
		cj_proj1_index.push_back(13);
		cj_proj1_index.push_back(14);
		cj_proj1_index.push_back(15);
		cj_proj1_index.push_back(16);
		cj_proj1_index.push_back(17);
		cj_proj1_index.push_back(18);
		cj_proj1_index.push_back(18);

		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",1);	//G1

		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G2
		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G3

		//1 month
		// 4 partitions
		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G4
		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",4);	//G5
		// 18 partitions
		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G6
		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G7

		// 5 days
		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",4);	//G8
		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",4);	//G9

		// 1 month 8 partitions
		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G10
		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G11

		// 5 days 8 partitions
		table_1->createHashPartitionedProjection(cj_proj0_index,"order_no",8);	//G12
		table_1->createHashPartitionedProjection(cj_proj1_index,"row_id",8);	//G13


		// 1 day 4 partitions by row_id
		table_1->createHashPartitionedProjection(cj_proj0_index,"row_id",4);	//G14
		catalog->add_table(table_1);

		////////////////////////////////////Create table right//////////////////////////
		TableDescriptor* table_2=new TableDescriptor("sb",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_2->addAttribute("row_id",data_type(t_u_long));
		table_2->addAttribute("order_no",data_type(t_u_long),0,true);
		table_2->addAttribute("entry_date",data_type(t_int));
		table_2->addAttribute("sec_code",data_type(t_int));
		table_2->addAttribute("order_type",data_type(t_int));
		table_2->addAttribute("entry_dir",data_type(t_int));			//5
		table_2->addAttribute("tran_maint_code",data_type(t_int));
		table_2->addAttribute("Last_upd_date",data_type(t_int));
		table_2->addAttribute("Last_upd_time",data_type(t_int));
		table_2->addAttribute("Last_upd_time_dec",data_type(t_u_long));
		table_2->addAttribute("entry_time",data_type(t_int));			//10
		table_2->addAttribute("entry_time_dec",data_type(t_double));
		table_2->addAttribute("order_price",data_type(t_double));
		table_2->addAttribute("order_exec_vol",data_type(t_double));
		table_2->addAttribute("order_vol",data_type(t_double));
		table_2->addAttribute("pbu_id",data_type(t_int));				//15
		table_2->addAttribute("acct_id",data_type(t_int));
		table_2->addAttribute("acct_attr",data_type(t_int));
		table_2->addAttribute("branch_id",data_type(t_int));
		table_2->addAttribute("pbu_inter_order_no",data_type(t_int));
		table_2->addAttribute("pub_inter_txt",data_type(t_int));		//20
		table_2->addAttribute("aud_type",data_type(t_int));
		table_2->addAttribute("trade_restr_type",data_type(t_int));
		table_2->addAttribute("order_star",data_type(t_int));
		table_2->addAttribute("order_restr_type",data_type(t_int));
		table_2->addAttribute("short_sell_flag",data_type(t_int));		//25















		vector<ColumnOffset> sb_proj0_index;
		sb_proj0_index.push_back(0);
		sb_proj0_index.push_back(1);
		sb_proj0_index.push_back(2);
		sb_proj0_index.push_back(3);
		sb_proj0_index.push_back(4);
		sb_proj0_index.push_back(5);

//		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G0
		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",1);	//G0



		vector<ColumnOffset> sb_proj1_index;
		sb_proj1_index.push_back(0);
		sb_proj1_index.push_back(6);
		sb_proj1_index.push_back(7);
		sb_proj1_index.push_back(8);
		sb_proj1_index.push_back(9);
		sb_proj1_index.push_back(10);
		sb_proj1_index.push_back(11);
		sb_proj1_index.push_back(12);
		sb_proj1_index.push_back(13);
		sb_proj1_index.push_back(14);
		sb_proj1_index.push_back(15);
		sb_proj1_index.push_back(16);
		sb_proj1_index.push_back(17);
		sb_proj1_index.push_back(18);
		sb_proj1_index.push_back(19);
		sb_proj1_index.push_back(20);
		sb_proj1_index.push_back(21);
		sb_proj1_index.push_back(22);
		sb_proj1_index.push_back(23);
		sb_proj1_index.push_back(24);
		sb_proj1_index.push_back(25);




		table_2->createHashPartitionedProjection(sb_proj1_index,"order_no",1);	//G1

		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G2
		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G3

		// 1 month
		// 4 partitions
		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G4
		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",4);	//G5

		// 18 partitions
		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G6
		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G7


		//5 days
		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",4);	//G8
		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",4);	//G9

		// 1 month 8 partitions
		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G10
		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G11

		// 5 days 8 partitions
		table_2->createHashPartitionedProjection(sb_proj0_index,"order_no",8);	//G12
		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",8);	//G13

		// 1 day 4 partitions by row_id
		table_2->createHashPartitionedProjection(sb_proj0_index,"row_id",4);	//G14


		catalog->add_table(table_2);
		///////////////////////////////////////////////////////////








		///////////////////////////////////////


		////////////////////////////////////////
		/* the following codes should be triggered by Load module*/
		//////////////////ONE DAY////////////////////////////////////////////////
		//cj_table
		// 4 partitions partitioned by order_no
		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_1->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}
		//partitioned by row_id
		for(unsigned i=0;i<table_1->getProjectoin(14)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(14)->getPartitioner()->RegisterPartition(i,2);
		}

		// 8 partitions
		for(unsigned i=0;i<table_1->getProjectoin(2)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(2)->getPartitioner()->RegisterPartition(i,1);
		}

		for(unsigned i=0;i<table_1->getProjectoin(3)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(3)->getPartitioner()->RegisterPartition(i,3);
		}

		//sb_table
		for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_2->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}
		for(unsigned i=0;i<table_2->getProjectoin(2)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(2)->getPartitioner()->RegisterPartition(i,1);
		}

		for(unsigned i=0;i<table_2->getProjectoin(3)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(3)->getPartitioner()->RegisterPartition(i,3);
		}

		//partitioned by row_id
		for(unsigned i=0;i<table_2->getProjectoin(14)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(14)->getPartitioner()->RegisterPartition(i,2);
		}

		////////////////////////////////////////

		///////////////////ONE MONTH/////////////////////////////////////////////////////////////
		//CJ
		// 4 partition
		for(unsigned i=0;i<table_1->getProjectoin(4)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(4)->getPartitioner()->RegisterPartition(i,40);
		}

		for(unsigned i=0;i<table_1->getProjectoin(5)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(5)->getPartitioner()->RegisterPartition(i,104);
		}
		//8 partitions
		for(unsigned i=0;i<table_1->getProjectoin(10)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(10)->getPartitioner()->RegisterPartition(i,20);
		}

		for(unsigned i=0;i<table_1->getProjectoin(11)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(11)->getPartitioner()->RegisterPartition(i,52);
		}
		// 18 partitions
		for(unsigned i=0;i<table_1->getProjectoin(6)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(6)->getPartitioner()->RegisterPartition(i,10);
		}

		for(unsigned i=0;i<table_1->getProjectoin(7)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(7)->getPartitioner()->RegisterPartition(i,24);
		}

		//SB
		// 4 partition
		for(unsigned i=0;i<table_2->getProjectoin(4)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(4)->getPartitioner()->RegisterPartition(i,39);
		}

		for(unsigned i=0;i<table_2->getProjectoin(5)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(5)->getPartitioner()->RegisterPartition(i,131);
		}
		// 8 partitions
		for(unsigned i=0;i<table_2->getProjectoin(10)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(10)->getPartitioner()->RegisterPartition(i,20);
		}

		for(unsigned i=0;i<table_2->getProjectoin(11)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(11)->getPartitioner()->RegisterPartition(i,66);
		}
		// 18 partitions
		for(unsigned i=0;i<table_2->getProjectoin(6)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(6)->getPartitioner()->RegisterPartition(i,10);
		}

		for(unsigned i=0;i<table_2->getProjectoin(7)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(7)->getPartitioner()->RegisterPartition(i,30);
		}
		////////////////////////////////


		///////////// FIVE DAYS //////////////////
		//// cj////
		// 4 partitions
		for(unsigned i=0;i<table_1->getProjectoin(8)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(8)->getPartitioner()->RegisterPartition(i,14);
		}

		for(unsigned i=0;i<table_1->getProjectoin(9)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(9)->getPartitioner()->RegisterPartition(i,36);
		}
		// 8 partitions
		for(unsigned i=0;i<table_1->getProjectoin(12)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(12)->getPartitioner()->RegisterPartition(i,7);
		}

		for(unsigned i=0;i<table_1->getProjectoin(13)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(13)->getPartitioner()->RegisterPartition(i,19);
		}

		//// sb ////
		// 4 partitions//
		for(unsigned i=0;i<table_2->getProjectoin(8)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(8)->getPartitioner()->RegisterPartition(i,14);
		}

		for(unsigned i=0;i<table_2->getProjectoin(9)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(9)->getPartitioner()->RegisterPartition(i,131);
		}
		// 8 partitions//
		for(unsigned i=0;i<table_2->getProjectoin(12)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(12)->getPartitioner()->RegisterPartition(i,7);
		}

		for(unsigned i=0;i<table_2->getProjectoin(13)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(13)->getPartitioner()->RegisterPartition(i,23);
		}


		/////////////////////////////////////////

	//	sleep(1);
//		cout<<"ready(?)"<<endl;
		printf("ready(?)\n");
		int input;
		scanf("%d",&input);


		LogicalOperator* cj_join_key_scan=new LogicalScan(table_1->getProjectoin(0));
		LogicalOperator* sb_join_key_scan=new LogicalScan(table_2->getProjectoin(0));



		LogicalOperator* cj_payload_scan=new LogicalScan(table_1->getProjectoin(1));

		LogicalOperator* sb_payload_scan=new LogicalScan(table_2->getProjectoin(1));

		Filter::Condition filter_condition_1;
		const int order_type=1;
		filter_condition_1.add(table_1->getAttribute(5),AttributeComparator::EQ,std::string("1"));
		const int trade_date=20101008;
		filter_condition_1.add(table_1->getAttribute(1),AttributeComparator::GEQ,std::string("20101008"));
		const int sec_code=600036;
//		filter_condition_1.add(table_1->getAttribute(3),AttributeComparator::L,std::string("600036"));
		LogicalOperator* filter_1=new Filter(filter_condition_1,cj_join_key_scan);
		std::vector<AttributeComparator> tmp;



		Filter::Condition filter_condition_2;
		const int order_type_=1;
		filter_condition_2.add(table_2->getAttribute(4),AttributeComparator::EQ,std::string("1"));
		const int entry_date=20101008;
		filter_condition_2.add(table_2->getAttribute(2),AttributeComparator::GEQ,std::string("20101008"));
		const int sec_code_=600036;
//		filter_condition_2.add(table_2->getAttribute(3),AttributeComparator::L,std::string("600036"));
		LogicalOperator* filter_2=new Filter(filter_condition_2,sb_join_key_scan);


		Filter::Condition filter_condition_cj_payload;
		long tmp1=0;
		filter_condition_cj_payload.add(table_1->getAttribute(0),AttributeComparator::EQ,&tmp1);
		LogicalOperator* filter_cj_payload=new Filter(filter_condition_cj_payload,cj_payload_scan);

		Filter::Condition filter_condition_sb_payload;
		long tmp2=0;
		filter_condition_sb_payload.add(table_2->getAttribute(0),AttributeComparator::EQ,&tmp2);
		LogicalOperator* filter_sb_payload=new Filter(filter_condition_sb_payload,sb_payload_scan);



		std::vector<EqualJoin::JoinPair> sb_cj_join_pair_list;
		sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("order_no"),table_2->getAttribute("order_no")));
		sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("trade_date"),table_2->getAttribute("entry_date")));
		sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("trade_dir"),table_2->getAttribute("entry_dir")));
//		sb_cj_join_pair_list.push_back(EqualJoin::JoinPair(table_1->getAttribute("row_id"),table_2->getAttribute("row_id")));
		LogicalOperator* sb_cj_join=new EqualJoin(sb_cj_join_pair_list,filter_1,filter_2);
//		LogicalOperator* sb_cj_join=new EqualJoin(sb_cj_join_pair_list,cj_join_key_scan,sb_join_key_scan);
//		LogicalOperator* sb_cj_join=new EqualJoin(sb_cj_join_pair_list,buffer1,buffer2);

		std::vector<Attribute> group_by_attributes;
		group_by_attributes.push_back(table_1->getAttribute("order_no"));


		std::vector<Attribute> aggregation_attributes;


//		aggregation_attributes.push_back(table_1->getAttribute("trade_date"));

		aggregation_attributes.push_back(table_1->getAttribute("order_no"));
		aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
//		aggregation_attributes.push_back(table_1->getAttribute("sec_code"));
		std::vector<BlockStreamAggregationIterator::State::aggregation> aggregation_function;

		aggregation_function.push_back(BlockStreamAggregationIterator::State::min);
		aggregation_function.push_back(BlockStreamAggregationIterator::State::count);
//		LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,filter_1);
//		LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,cj_join_key_scan);

		LogicalOperator* aggregation=new Aggregation(group_by_attributes,aggregation_attributes,aggregation_function,sb_cj_join);

//

//		const NodeID collector_node_id=0;
//		LogicalOperator* root=new LogicalQueryPlanRoot(collector_node_id,cj_join_key_scan,LogicalQueryPlanRoot::PERFORMANCE);
//		unsigned long long int timer_start=curtick();
//		root->print();

//		root->getDataflow();

//		BlockStreamIteratorBase* executable_query_plan=root->getIteratorTree(1024-sizeof(unsigned));
		BlockStreamIteratorBase* executable_query_plan;
//		executable_query_plan=root->getIteratorTree(1024*64-sizeof(unsigned));
//		printf("query optimization time :%5.5f\n",getMilliSecond(timer_start));


//		filter_1->getDataflow();
		LogicalOperator* target=aggregation;
		executable_query_plan=target->getIteratorTree((1024*64-sizeof(unsigned)));

		BlockStreamExpander::State expander_state(target->getDataflow().getSchema(),executable_query_plan,500,1024*64-sizeof(unsigned));
		BlockStreamIteratorBase* expander=new BlockStreamExpander(expander_state);

		BlockStreamPerformanceMonitorTop::State perf_state(target->getDataflow().getSchema(),expander,1024*64-
				sizeof(unsigned));
		BlockStreamIteratorBase* top=new BlockStreamPerformanceMonitorTop(perf_state);

		top->print();

		BlockStreamBase* block=BlockStreamBase::createBlockWithDesirableSerilaizedSize(target->getDataflow().getSchema(),1024*64);
		int c=1;
		while(c==1){
//			timer_start=curtick();b
			for (int i = 0; i < 1; i++)
			{
				top->open();
				while(top->next(block)){
					block->setEmpty();
				}
				top->close();

				cout << "filter test: " << i << endl;
			}
//			sleep(1);
			printf("Terminate(0) or continue(others)?\n");
//			sleep()
			scanf("%d",&c);
//			sleep(10);
//			getchar();
//			cout<<"<<<<<<<<<<<<<<<<<<<<<You input ="<<c+1<<endl<<flush;
			printf("you print=%d\n",c);
//			sleep(5);
		}
	}
//	cout<<"Waiting~~~!~"<<endl;
//	while(true){
//		sleep(1);
//	}

}


#endif
