/*
 * JoinIterator.h
 *
 *  Created on: 2013-6-20
 *      Author: zhanglei
 */

#ifndef JOINITERATOR_H_
#define JOINITERATOR_H_

#include <vector>
#include <map>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "../iterator.hpp"
#include "../Schema/Schema.h"
#include "../hash.h"
#include "../hashtable.h"

class JoinIterator:public Iterator {
public:
	struct State{
		explicit State(Schema *inputl,Schema *inputr,
				Schema *output,Iterator *childl,Iterator *childr,
		std::vector<unsigned> joinIndexl,std::vector<unsigned> joinIndexr,
		std::vector<unsigned> linkIndexl,std::vector<unsigned> linkIndexr,
		unsigned nbuckets,unsigned bucketsize)
		:inputl(inputl),inputr(inputr),output(output),
		 childl(childl),childr(childr),joinIndexl(joinIndexl),joinIndexr(joinIndexr),
		 linkIndexl(linkIndexl),linkIndexr(linkIndexr),nbuckets(nbuckets),
		 bucketsize(bucketsize)
		{}
		State(){};
		Schema *inputl,*inputr;
		Schema *output;
		Iterator *childl,*childr;
		std::vector<unsigned> joinIndexl;
		std::vector<unsigned> joinIndexr;
		std::vector<unsigned> linkIndexl;
		std::vector<unsigned> linkIndexr;
		unsigned nbuckets;
		unsigned bucketsize;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version){
			ar & inputl & inputr & output & childl & childr & joinIndexl & joinIndexr & linkIndexl & linkIndexr & nbuckets & bucketsize;
		}
	};
public:
	JoinIterator(JoinIterator::State state);
	JoinIterator(){};
	virtual ~JoinIterator();
	bool open();
	inline bool next(void *desc){
		bool key_exist;
		void* it_cur;
		unsigned bn;

		void* key_in_input_tuple;
		void* key_in_joined_tuple;
		void* attr_in_input_tuple;
		void* key_in_hash_table;

		while((it_cur=it.readnext())!=0){
			for(unsigned i=0;i<state.joinIndexr.size();i++){
				key_in_input_tuple=state.inputr->getColumnAddess(state.joinIndexr[i],rTuple);
				key_in_hash_table=state.output->getColumnAddess(inputlJoinToOutput[i],it_cur);
				if(state.inputl->getcolumn(state.joinIndexr[i]).operate->equal(key_in_input_tuple,key_in_hash_table)){
					key_exist=true;
				}
				else{
					key_exist=false;
					break;
				}
			}

			if(key_exist){
				for(unsigned i=0;i<state.joinIndexl.size();i++){
					attr_in_input_tuple=state.inputr->getColumnAddess(state.joinIndexl[i],it_cur);
					key_in_joined_tuple=state.output->getColumnAddess(inputlJoinToOutput[i],joinedTuple);
					state.inputl->getcolumn(state.joinIndexl[i]).operate->assignment(attr_in_input_tuple,key_in_joined_tuple);
				}
				for(unsigned i=0;i<state.linkIndexl.size();i++){
					attr_in_input_tuple=state.inputl->getColumnAddess(state.linkIndexl[i],it_cur);
					key_in_joined_tuple=state.output->getColumnAddess(inputlLinkToOutput[i],joinedTuple);
					state.inputl->getcolumn(state.linkIndexl[i]).operate->assignment(attr_in_input_tuple,key_in_joined_tuple);
				}
				for(unsigned i=0;i<state.linkIndexr.size();i++){
					attr_in_input_tuple=state.inputr->getColumnAddess(state.linkIndexr[i],rTuple);
					key_in_joined_tuple=state.output->getColumnAddess(inputrLinkToOutput[i],joinedTuple);
					state.inputr->getcolumn(state.linkIndexr[i]).operate->assignment(attr_in_input_tuple,key_in_joined_tuple);
				}
				memcpy(desc,joinedTuple,state.output->getTupleActualSize(joinedTuple));
			}
			return true;
		}

		if(!state.childr->next(rTuple))
			return false;
		bn=hash->get_partition_value(*(int*)(state.inputr->getColumnAddess(state.joinIndexr[0],rTuple)));
		hashtable->placeIterator(it,bn);
		return next(desc);
	}
	bool close();
public:
	JoinIterator::State state;
private:
	PartitionFunction *hash;
	BasicHashTable *hashtable;
	BasicHashTable::Iterator it;
	std::map<unsigned,unsigned> inputlJoinToOutput;
	std::map<unsigned,unsigned> inputlLinkToOutput;
	std::map<unsigned,unsigned> inputrLinkToOutput;
	BasicHashTable::Iterator tmp_it;
	unsigned int bucket_cur;
	void *rTuple;
	void *lTuple;
	void *joinedTuple;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar &boost::serialization::base_object<Iterator>(*this) & state;
	}
};

#endif /* JOINITERATOR_H_ */
