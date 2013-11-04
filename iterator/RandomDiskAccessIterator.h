/*
 * RandomDiskAccessIterator.h
 *
 *  Created on: 2013年8月26日
 *      Author: SCDONG
 */

#ifndef RANDOMDISKACCESSITERATOR_H_
#define RANDOMDISKACCESSITERATOR_H_

#include <vector>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>

#include "../iterator.hpp"
#include "../Schema/Schema.h"
#include "../Schema/SchemaFix.h"

class RandomDiskAccessIterator :public Iterator {
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
	RandomDiskAccessIterator(RandomDiskAccessIterator::State state);
	RandomDiskAccessIterator():fd(NULL), length(0), oid(0) {};
	virtual ~RandomDiskAccessIterator();
	bool open();
	inline bool next(void *desc)
	{
		if(state.child->next(&oid)){
			fseek(fd, oid*state.input->getTupleMaxSize(), SEEK_SET);
			for (int i = 0; i < state.input->getncolumns();i++)
			{
				fread(desc, 1, state.input->getcolumn(i).get_length(), fd);
				desc += state.input->getcolumn(i).get_length();
			}
			return true;
		}
		return false;
	}
	bool close();
public:
	RandomDiskAccessIterator::State state;	//should be serialized
private:
//	void* base;
//	char* cursor;
	long length;
	FILE* fd;
	int oid;
private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar &boost::serialization::base_object<Iterator>(*this) & state;
	}
};


#endif /* RANDOMDISKACCESSITERATOR_H_ */
