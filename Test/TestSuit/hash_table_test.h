/*
 * hash_table_test.h
 *
 *  Created on: Apr 5, 2014
 *      Author: wangli
 */

#ifndef HASH_TABLE_TEST_H_
#define HASH_TABLE_TEST_H_
#include "../../common/Block/BlockStreamBuffer.h"
#include "../../Catalog/Column.h"
#include <boost/functional/hash.hpp>
#include "../../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamProjectionScan.h"
#include "../../storage/PartitionStorage.h"
#include "../../storage/BlockManager.h"
#include "../../common/ids.h"
#include <string.h>
#include "../../common/hash.h"
#define block_size (1024*1024)
void init_alloc_destory();
static void startup_catalog(){
//	int master;
	int master=0;
//	printf("Master(0) or Slave(others)??\n");
//	scanf("%d",&master);
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
		table_1->createHashPartitionedProjection(cj_proj0_index,"row_id",1);	//G0
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


		catalog->add_table(table_1);

		////////////////////////////////////Create table right//////////////////////////
		TableDescriptor* table_2=new TableDescriptor("sb",Environment::getInstance()->getCatalog()->allocate_unique_table_id());
		table_2->addAttribute("row_id",data_type(t_u_long));
		table_2->addAttribute("order_no",data_type(t_u_long));
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

		table_2->createHashPartitionedProjection(sb_proj0_index,"row_id",1);	//G0



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




		table_2->createHashPartitionedProjection(sb_proj1_index,"row_id",1);	//G1


		catalog->add_table(table_2);
		for(unsigned i=0;i<table_1->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_1->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(0)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}


		//sb_table
		for(unsigned i=0;i<table_2->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
		}

		for(unsigned i=0;i<table_2->getProjectoin(1)->getPartitioner()->getNumberOfPartitions();i++){

			catalog->getTable(1)->getProjectoin(1)->getPartitioner()->RegisterPartition(i,6);
		}
	}
}
struct Arg{
	BlockStreamBuffer* buffer;
	BasicHashTable ** hash_table;
	Schema* schema;
	PartitionFunction* hash;
	BlockStreamIteratorBase* iterator;
	PartitionStorage::PartitionReaderItetaor* partition_reader;
	Barrier* barrier;
	unsigned tid;
};

BasicHashTable* generate_hashtable(unsigned tuple_size, unsigned nbuckets, unsigned bucketsize, unsigned nthreads=1);
void* insert_into_hash_table_from_projection(void * argment){
	unsigned long count=0;
	unsigned long long force=0;
	Arg arg=*(Arg*)argment;
	if(arg.tid==0){
		const unsigned nbuckets=1024;
		const unsigned bucketsize=256-8;
		TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("sb");
		Schema* schema=table->getProjectoin(1)->getSchema();
		*arg.hash_table=generate_hashtable(schema->getTupleMaxSize(),nbuckets,bucketsize);
	}

	arg.barrier->Arrive();

	BlockStreamBase* fetched_block=BlockStreamBase::createBlockWithDesirableSerilaizedSize(arg.schema,block_size);
	Operate* op=arg.schema->columns[0].operate->duplicateOperator();
	unsigned nbuckets=arg.hash->getNumberOfPartitions();
	unsigned long long int start=curtick();
	printf("tuple length=%d\n",arg.schema->getTupleMaxSize());
	while(arg.partition_reader->nextBlock(fetched_block)){
		void* tuple;
		BlockStreamBase::BlockStreamTraverseIterator* it=fetched_block->createIterator();
		while(tuple=it->nextTuple()){
			count++;
//			if(*(unsigned long *)tuple==-1){
//				force++;
//				continue;
//			}
//			continue;
//			if(boost::hash_value((*(int*)tuple)%1342342)==0){
//				break;
//			}
//			if(op->getPartitionValue(tuple,arg.hash)==0){
//				break;
//			}
//			unsigned bn=op->getPartitionValue(tuple,arg.hash);
			unsigned bn=boost::hash_value(*(unsigned long*)tuple)%nbuckets;
//			if(bn!=0){
//				continue;
//			}
//			else{
//				force++;
//			}
			void* new_tuple_in_hashtable=(*arg.hash_table)->allocate(bn);

			arg.schema->copyTuple(tuple,new_tuple_in_hashtable);
		}
	}

	printf("%d cycles per tuple\n",(curtick()-start)/count,force);
}

static double projection_scan(unsigned degree_of_parallelism){
//	const unsigned nbuckets=1024*1024*16;
	const unsigned nbuckets=1024;
	const unsigned bucketsize=256-8;
	const unsigned nthreads=degree_of_parallelism;
	double ret;
	printf("nthread=%d\n",nthreads);
	TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("sb");
	Schema* schema=table->getProjectoin(1)->getSchema();
//	BasicHashTable* hashtable=generate_hashtable(schema->getTupleMaxSize(),nbuckets,bucketsize);


	LogicalScan* scan=new LogicalScan(table->getProjectoin(1));
	scan->getDataflow();
	BlockStreamIteratorBase* warm_up_iterator=scan->getIteratorTree(1024*64);

	ExpandableBlockStreamProjectionScan::State ps_state;
	ps_state.block_size_=1024*64;
	ps_state.projection_id_=table->getProjectoin(1)->getProjectionID();
	ps_state.schema_=schema;

	BlockStreamBase* block_for_asking=BlockStreamBase::createBlockWithDesirableSerilaizedSize(schema,64*1024);
	warm_up_iterator->open(0);
	while(warm_up_iterator->next(block_for_asking)){

	}
	warm_up_iterator->close();

	Arg arg;
	arg.hash=PartitionFunctionFactory::createBoostHashFunction(nbuckets);
//	arg.hash_table=hashtable;
	arg.schema=schema;
	arg.partition_reader=BlockManager::getInstance()->getPartitionHandle(PartitionID(table->getProjectoin(1)->getProjectionID(),0))->createAtomicReaderIterator();
	arg.barrier=new Barrier(nthreads);
	pthread_t pid[1000];
	unsigned long long int start=curtick();
	for(unsigned i=0;i<nthreads;i++){
		arg.tid=i;
		pthread_create(&pid[i],0,insert_into_hash_table_from_projection,&arg);
	}

	for(unsigned i=0;i<nthreads;i++){
		pthread_join(pid[i],0);
	}
//	hashtable->report_status();
	printf("time:%4.4f\n",getSecond(start));
	ret=getSecond(start);
	int cycles_per_allocate=((double)curtick()-start)/(65536000)*nthreads;
	printf("Avg. %d cycles per allocation!\n",cycles_per_allocate);
	(*arg.hash_table)->~BasicHashTable();
	schema->~Schema();
	return ret;
}
Schema* generate_schema(unsigned tuple_length){
	std::vector<column_type> columns;
	for(unsigned i=0;i<tuple_length/4;i++){
		columns.push_back(column_type(t_int));
	}
	Schema* schema=new SchemaFix(columns);
	return schema;
}

BasicHashTable* generate_hashtable(unsigned tuple_size, unsigned nbuckets, unsigned bucketsize,unsigned nthreads){
	BasicHashTable* hashtable=new BasicHashTable(nbuckets,bucketsize,tuple_size,nthreads);
	return hashtable;
}

BlockStreamBuffer* initial_input_date(Schema* schema,unsigned long total_data_size_in_MB){

	unsigned block_num=total_data_size_in_MB*1024*1024/(block_size);

	BlockStreamBuffer* buffer=new BlockStreamBuffer(block_size,block_num,schema);
	int value=0;
	BlockStreamBase* new_block=BlockStreamBase::createBlockWithDesirableSerilaizedSize(schema,block_size);
	for(unsigned i=0;i<block_num;i++){
		void* tuple;
		new_block->setEmpty();
		while(tuple=new_block->allocateTuple(schema->getTupleMaxSize())){
			schema->columns[0].operate->assignment(&value,tuple);
			value=random();
//			value++;
		}
		buffer->insertBlock(new_block);
	}
	new_block->~BlockStreamBase();
	return buffer;

}
void* insert_into_hash_table(void * argment){
	Arg arg=*(Arg*)argment;
	printf("pid=%d\n",arg.tid);
	if(arg.tid==0){
//		printf("hash table initilizing!\n");
//		const unsigned nbuckets=1024*1024;
//		const unsigned bucketsize=256-8;
//		TableDescriptor* table=Environment::getInstance()->getCatalog()->getTable("sb");
//		Schema* schema=table->getProjectoin(1)->getSchema();
//		*arg.hash_table=generate_hashtable(schema->getTupleMaxSize(),nbuckets,bucketsize);
	}

	arg.barrier->Arrive();
	printf("pass!\n");

	if(arg.tid==0){
		arg.barrier->reset();
	}

	BlockStreamBase* fetched_block=BlockStreamBase::createBlockWithDesirableSerilaizedSize(arg.schema,block_size);
	Operate* op=arg.schema->columns[0].operate->duplicateOperator();
	unsigned nbuckets=arg.hash->getNumberOfPartitions();
	while(arg.buffer->getBlock(*fetched_block)){
		void* tuple;
		BlockStreamBase::BlockStreamTraverseIterator* it=fetched_block->createIterator();
		while(tuple=it->nextTuple()){
//			if(boost::hash_value((*(int*)tuple)%(1024*1024))!=0){
////				continue;
//			}
//			if(op->getPartitionValue(tuple,arg.hash)==0){
//				break;
//			}
			unsigned bn=arg.schema->columns[0].operate->getPartitionValue(tuple,arg.hash);
			void* new_tuple_in_hashtable=(*arg.hash_table)->atomicAllocate(bn,arg.tid);
			arg.schema->copyTuple(tuple,new_tuple_in_hashtable);
		}
	}
	arg.barrier->Arrive();
	if(arg.tid==0){
//		(*arg.hash_table)->~BasicHashTable();
//		printf("I can release the hash table!\n");
	}
}

void create_thread(unsigned nthreads){

}

double fill_hash_table(unsigned degree_of_parallelism){
//	init_alloc_destory();
	Config::getInstance();
	const unsigned nbuckets=64;
	const unsigned tuple_size=32;
	const unsigned bucketsize=1024-8;
	const unsigned long data_size_in_MB=256;
	const unsigned nthreads=degree_of_parallelism;
	double ret;
	Schema* schema=generate_schema(tuple_size);
	BasicHashTable* hashtable=generate_hashtable(schema->getTupleMaxSize(),nbuckets,bucketsize,nthreads);
	printf("before allocate data!\n");
//	sleep(3);

	BlockStreamBuffer* buffer=initial_input_date(schema,data_size_in_MB);
	printf("after allocate data!\n");
//	sleep(3);
	Arg args[1000];
	pthread_t pid[1000];
	Arg arg;
	arg.buffer=buffer;
	arg.hash=PartitionFunctionFactory::createBoostHashFunction(nbuckets);
	arg.hash_table=&hashtable;
	arg.schema=schema;
	arg.barrier=new Barrier(degree_of_parallelism);
	unsigned long long int start=curtick();
	for(unsigned i=0;i<nthreads;i++){

		args[i]=arg;
		args[i].tid=i;
		pthread_create(&pid[i],0,insert_into_hash_table,&args[i]);
	}

	for(unsigned i=0;i<nthreads;i++){
		pthread_join(pid[i],0);
	}
//	hashtable->report_status();
	printf("time:%4.4f\n",getSecond(start));
	ret=getSecond(start);
	int cycles_per_allocate=((double)curtick()-start)/(data_size_in_MB*1024*1024/tuple_size)*nthreads;
	printf("Avg. %d cycles per allocation!\n",cycles_per_allocate);
	buffer->~BlockStreamBuffer();
	(*arg.hash_table)->~BasicHashTable();
	schema->~Schema();

	return ret;

}
double getAverage(int degree_of_parallelism,int repeated_times=10){
	double ret=0;
	for(unsigned i=0;i<repeated_times;i++){
		ret+=fill_hash_table(degree_of_parallelism);
//		ret+=projection_scan(degree_of_parallelism);
	}
	return ret/repeated_times;
}

void scalability_test(){
//	init_alloc_destory();
	startup_catalog();
	unsigned int max_degree_of_parallelism=Config::max_degree_of_parallelism;
//	max_degree_of_parallelism=1;
	unsigned repeated_times=1;
	double standard_throughput=0;
	for(unsigned i=1;i<=max_degree_of_parallelism;i++){
//		init_alloc_destory();
		double total_time=getAverage(i,repeated_times);
		if(i==1){
			standard_throughput=1/total_time;
			printf("D=%d\ts=%4.4f scale:1\n",i,total_time,1);
		}
		else{
			printf("D=%d\ts=%4.4f scale:%f\n",i,total_time,1/total_time/standard_throughput);
		}
		printf("______________\n");
//		sleep(5);
	}

	Environment::getInstance()->~Environment();
	sleep(100);
}

BasicHashTable* init_hash_table(){
	const unsigned nbuckets=1024*1024;
	const unsigned bucket_size=256-8;
	const unsigned tuple_size=12;
	return generate_hashtable(bucket_size,nbuckets,tuple_size);
}

void init_and_destory(){
	BasicHashTable* hashtable=init_hash_table();
	hashtable->~BasicHashTable();
}

void init_alloc_destory(){
	const unsigned long allocate_count=1024*1024;

	string str = "123456789012";
	BasicHashTable* hashtable=init_hash_table();

//	for(unsigned long i=0;i<allocate_count;i++){
//		void* allocated_tuple=hashtable->allocate(rand()%(1024));
//		memcpy(allocated_tuple, (void*)&str, 12);
////		memset(allocated_tuple,0,12);
//	}


	hashtable->~BasicHashTable();
}

void memory_leak_test(){

	const unsigned repeated_times=10;
//	printf("Init and destroy test.\n");
//	for(unsigned i=0;i<repeated_times;i++){
//		init_and_destory();
//		printf("%d-th round!\n",i);
//		sleep(1);
//	}

	printf("Init, allocate, and destory test.\n");
	for(unsigned i=0;i<repeated_times;i++){
		init_alloc_destory();
		printf("%d-th round!\n",i);
//		sleep(1);
	}






	sleep(1000);



}
int performance_test(){
	const unsigned tuple_size=32;
	unsigned nbuckets=1024*1024;
	unsigned bucket_size=256;
	const unsigned long datasize=1024*1024*1024;

	BasicHashTable* hashtable=generate_hashtable(tuple_size,nbuckets,bucket_size,1);

	unsigned long long int start=curtick();
	for(unsigned i=0,bn=0;i<datasize/tuple_size;i++,bn++){
		hashtable->atomicAllocate(bn);
		if(bn==nbuckets){
			bn=0;
		}
	}
	printf("Avg: %d cycles per allocation!\n",(curtick()-start)/(datasize/tuple_size));
}

int hash_table_test(){
//	performance_test();
	scalability_test();
//	memory_leak_test();
}

#endif /* HASH_TABLE_TEST_H_ */

