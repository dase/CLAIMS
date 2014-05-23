/*
 * AggregationIterator.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: wangli
 */

#include "AggregationIterator.h"
#include "../configure.h"

AggregationIterator::AggregationIterator(AggregationIterator::State state)
:state(state),hashtable(0),hash(0),bucket_cur(0),tuple(0)
{}

AggregationIterator::~AggregationIterator()
{}
/**
 * In the aggregation iterator, the open method initializes the hash table and
 *  then builds the hash table by read the entire input data.
 */
bool AggregationIterator::open()
{
	unsigned outputindex=0;
	for(unsigned i=0;i<state.groupByIndex.size();i++)
	{
		inputGroupByToOutput[i]=outputindex++;
	}
	for(unsigned i=0;i<state.aggregationIndex.size();i++)
	{
		inputAggregationToOutput[i]=outputindex++;
	}

	for(unsigned i=0;i<state.aggregations.size();i++)
	{
		switch(state.aggregations[i])
		{
			case AggregationIterator::State::count:
			{
				aggregationFunctions.push_back(state.output->getcolumn(inputAggregationToOutput[i]).operate->GetIncreateByOneFunction());
				break;
			}
			case AggregationIterator::State::min:
			{
				aggregationFunctions.push_back(state.output->getcolumn(inputAggregationToOutput[i]).operate->GetMINFunction());
				break;
			}
			case AggregationIterator::State::max:
			{
				aggregationFunctions.push_back(state.output->getcolumn(inputAggregationToOutput[i]).operate->GetMAXFunction());
				break;
			}
			case AggregationIterator::State::sum:
			{
				aggregationFunctions.push_back(state.output->getcolumn(inputAggregationToOutput[i]).operate->GetADDFunction());
				break;
			}
			default:
			{
				printf("invalid aggregation function!\n");
			}
		}

	}

	tuple=memalign(cacheline_size,state.input->getTupleMaxSize());
	hash=PartitionFunctionFactory::createModuloFunction(state.nbuckets);
	hashtable=new BasicHashTable(state.nbuckets,state.bucketsize,state.output->getTupleMaxSize());
	state.child->open();


	unsigned bn;
	BasicHashTable::Iterator tmp_it=hashtable->CreateIterator();
	bool key_exist;
	void* it_cur;
	void* key_in_input_tuple;
	void* key_in_hash_table;
	void* value_in_input_tuple;
	void* value_in_hash_table;
	void* new_tuple_in_hash_table;

	unsigned long long one=1;
	while(state.child->next(tuple))
	{
		//Currently, only one group-by column is supported.
		//TODO: support multiple group-by columns

		//Currently, only support int as the group-by column
		//TODO: support various data type, e.g., double, string, etc.

		bn=hash->get_partition_value(*(int*)(state.input->getColumnAddess(state.groupByIndex[0],tuple)));

		hashtable->placeIterator(tmp_it,bn);

		key_exist=false;
		while((it_cur=tmp_it.readnext())!=0)
		{


			for(unsigned i=0;i<state.groupByIndex.size();i++)
			{
				key_in_input_tuple=state.input->getColumnAddess(state.groupByIndex[i],tuple);
				key_in_hash_table=state.output->getColumnAddess(inputGroupByToOutput[i],it_cur);
				if(state.input->getcolumn(state.groupByIndex[i]).operate->equal(key_in_input_tuple,key_in_hash_table))
				{
					key_exist=true;
				}
				else
				{
					key_exist=false;
					break;
				}
			}


			if(key_exist)
			{
				for(unsigned i=0;i<state.aggregationIndex.size();i++)
				{
					value_in_input_tuple=state.input->getColumnAddess(state.aggregationIndex[i],tuple);
					value_in_hash_table=state.output->getColumnAddess(inputAggregationToOutput[i],it_cur);

					hashtable->UpdateTuple(bn,value_in_hash_table,value_in_input_tuple,aggregationFunctions[i]);
				}
//				key_exist=true;
				break;

			}
		}
		if(key_exist)
			continue;
		new_tuple_in_hash_table=hashtable->allocate(bn);

		//copy group-by columns
		for(unsigned i=0;i<state.groupByIndex.size();i++)
		{
			key_in_input_tuple=state.input->getColumnAddess(state.groupByIndex[i],tuple);
			key_in_hash_table=state.output->getColumnAddess(inputGroupByToOutput[i],new_tuple_in_hash_table);
			state.input->getcolumn(state.groupByIndex[i]).operate->assignment(key_in_input_tuple,key_in_hash_table);
		}

		//copy aggregation columns
		for(unsigned i=0;i<state.aggregationIndex.size();i++)
		{
			/**
			 * use if-else here is a kind of ugly.
			 * TODO: use a function which is initialized according to the aggregation function.
			 */
			if(state.aggregations[i]==State::count)
			{
				value_in_input_tuple=&one;
			}
			else
			{
				value_in_input_tuple=state.input->getColumnAddess(state.aggregationIndex[i],tuple);
			}
			value_in_hash_table=state.output->getColumnAddess(inputAggregationToOutput[i],new_tuple_in_hash_table);

			state.input->getcolumn(state.aggregationIndex[i]).operate->assignment(value_in_input_tuple,value_in_hash_table);
		}

	}

	//prepare for the next primitive.
	it=hashtable->CreateIterator();
	hashtable->placeIterator(it,0);
	return true;

}
bool AggregationIterator::close()
{
	free(tuple);
	hash->~PartitionFunction();
	hashtable->~BasicHashTable();
	return true;
}
