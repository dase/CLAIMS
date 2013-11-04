/*
 * JoinIterator.cpp
 *
 *  Created on: 2013-6-20
 *      Author: zhanglei
 */

#include "JoinIterator.h"

JoinIterator::JoinIterator(JoinIterator::State state)
:state(state),hash(0),hashtable(0),bucket_cur(0),lTuple(0),rTuple(0),joinedTuple(0){
	// TODO Auto-generated constructor stub

}

JoinIterator::~JoinIterator() {
	// TODO Auto-generated destructor stub
}

bool JoinIterator::open(){
	unsigned outputindex=0;

	for(unsigned i=0;i<state.joinIndexl.size();i++){
		inputlJoinToOutput[i]=outputindex++;
	}

	for(unsigned i=0;i<state.linkIndexl.size();i++){
		inputlLinkToOutput[i]=outputindex++;
	}

	for(unsigned i=0;i<state.linkIndexr.size();i++){
		inputrLinkToOutput[i]=outputindex++;
	}

	lTuple=memalign(cacheline_size,state.inputl->getTupleMaxSize());
	rTuple=memalign(cacheline_size,state.inputr->getTupleMaxSize());
	joinedTuple=memalign(cacheline_size,state.output->getTupleMaxSize());;
	hash=PartitionFunctionFactory::createModuloFunction(state.nbuckets);
	//hashtable size is the same as the tuple in the left schema
	hashtable=new BasicHashTable(state.nbuckets,state.bucketsize,state.output->getTupleMaxSize());
	state.childl->open();
	state.childr->open();

	unsigned bn;
	BasicHashTable::Iterator tmp_it=hashtable->CreateIterator();

	void* key_in_input_tuple;
	void* key_in_hash_table;
	void* value_in_input_tuple;
	void* value_in_hash_table;

	int i=0;
	while(state.childl->next(lTuple)){
		//Currently, only one group-by column is supported.
		//TODO: support multiple group-by columns

		//Currently, only support int as the group-by column
		//TODO: support various data type, e.g., double, string, etc.

		bn=hash->get_partition_value(*(int*)(state.inputl->getColumnAddess(state.joinIndexl[0],lTuple)));
		hashtable->placeIterator(tmp_it,bn);

		void *new_tuple_in_hash_table;
		new_tuple_in_hash_table=hashtable->allocate(bn);

		//copy join columns
		for(unsigned i=0;i<state.joinIndexl.size();i++){
			key_in_input_tuple=state.inputl->getColumnAddess(state.joinIndexl[i],lTuple);
			key_in_hash_table=state.output->getColumnAddess(inputlJoinToOutput[i],new_tuple_in_hash_table);
			state.inputl->getcolumn(state.joinIndexl[i]).operate->assignment(key_in_input_tuple,key_in_hash_table);
		}

		//copy link columns
		for(unsigned i=0;i<state.linkIndexl.size();i++){
			value_in_input_tuple=state.inputl->getColumnAddess(state.linkIndexl[i],lTuple);
			value_in_hash_table=state.output->getColumnAddess(inputlLinkToOutput[i],new_tuple_in_hash_table);
			state.inputl->getcolumn(state.linkIndexl[i]).operate->assignment(value_in_input_tuple,value_in_hash_table);
		}

	}

	//prepare for the next primitive.
	it=hashtable->CreateIterator();
	hashtable->placeIterator(it,0);
	return true;
}

bool JoinIterator::close()
{
	free(lTuple);
	free(rTuple);
	free(joinedTuple);
	hash->~PartitionFunction();
	hashtable->~BasicHashTable();
	return true;
}
