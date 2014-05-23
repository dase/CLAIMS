/*
 * AggregationIterator.h
 *
 *  Created on: Jun 18, 2013
 *      Author: wangli
 */

#ifndef AGGREGATIONITERATOR_H_
#define AGGREGATIONITERATOR_H_

#include <vector>
#include <map>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "../iterator.hpp"
#include "../common/Schema/Schema.h"
#include "../common/hash.h"
#include "../common/hashtable.h"

class AggregationIterator:public Iterator {
public:
	struct State
	{
		enum aggregation{sum,min,max,count};		//TODO: average,mean
		State(Schema* input, Schema* output,
				Iterator* child,
				std::vector<unsigned> groupByIndex,
				std::vector<unsigned> aggregationIndex,
				std::vector<aggregation> aggregations,
				unsigned nbuckets,
				unsigned bucketsize)
		:input(input), output(output),child(child), groupByIndex(groupByIndex),
		 aggregationIndex(aggregationIndex), aggregations(aggregations),
		 nbuckets(nbuckets),bucketsize(bucketsize)
		{}
		State(){};
		Schema* input;
		Schema* output;
		Iterator* child;
		std::vector<unsigned> groupByIndex;
		std::vector<unsigned> aggregationIndex;
		std::vector<aggregation> aggregations;
		unsigned nbuckets;
		unsigned bucketsize;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & input & output & child & groupByIndex & aggregationIndex & aggregations & nbuckets & bucketsize;
		}
	};
	AggregationIterator(AggregationIterator::State state);
	AggregationIterator(){};
	virtual ~AggregationIterator();
	bool open();
	inline bool next(void* desc)
	{
		void* tuple;
		if((tuple=it.readnext())!=0)
		{
			memcpy(desc,tuple,state.output->getTupleActualSize(tuple));
			return true;
		}
		bucket_cur++;
		if(hashtable->placeIterator(it,bucket_cur)==false)
		{
			return false;
		}
		return next(desc);
	}
	bool close();
public:
	AggregationIterator::State state; //should be serialized.
private:

	PartitionFunction* hash;
	BasicHashTable* hashtable;
	BasicHashTable::Iterator it;
	std::map<unsigned,unsigned> inputGroupByToOutput;
	std::map<unsigned,unsigned> inputAggregationToOutput;
	std::vector<fun> aggregationFunctions;
	unsigned bucket_cur;
	void* tuple;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar &boost::serialization::base_object<Iterator>(*this) & state;
	}
};

#endif /* AGGREGATIONITERATOR_H_ */
