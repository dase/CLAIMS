/*
 * RandomMemAccessIterator.h
 *
 *  Created on: 2013年8月27日
 *      Author: SCDONG
 */

#ifndef RANDOMMEMACCESSITERATOR_H_
#define RANDOMMEMACCESSITERATOR_H_

#include <vector>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>

#include "../iterator.hpp"
#include "../common/Schema/Schema.h"
#include "../common/Schema/SchemaFix.h"

class RandomMemAccessIterator :public Iterator {
public:
	struct State
	{
		State(std::string file_name, Iterator* child, Schema* input, Schema* output)
		:file_name(file_name), child(child), input(input), output(output)
		{};
		State(){};
		std::string file_name;
		Iterator* child;
		Schema* input;
		Schema* output;
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			ar & file_name & child & input & output;
		}
	};
public:
	RandomMemAccessIterator(RandomMemAccessIterator::State state);
	RandomMemAccessIterator():base(NULL), length(0), fd(0), oid(0) {};
	virtual ~RandomMemAccessIterator();
	bool open();
	inline bool next(void *desc)
	{
//		if(state.child->next(&oid)){
		if(true){
			oid=rand()&0b111111111111111111111111111;
//			printf("\nrowid: %d\n", oid);
//			state.input->copyTuple(base+oid*state.input->getTupleActualSize(base), desc);
//			state.input->copyTuple((char*)base+oid*8, desc);
			*(long*)desc=*(long*)((char*)base+oid*8);
//			printf("tuple: %d\t%d\n", *(int*)desc, *((int*)desc+1));
			return true;
		}
		return false;
	}
	bool close();
public:
	RandomMemAccessIterator::State state;	//should be serialized
private:
	void* base;
	char* data;
	long length;
	int fd;
	long oid;
private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar &boost::serialization::base_object<Iterator>(*this) & state;
	}
};


#endif /* RANDOMMEMACCESSITERATOR_H_ */
