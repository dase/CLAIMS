/*
 * SequencialDiskAccessIterator.h
 *
 *  Created on: 2013年8月27日
 *      Author: SCDONG
 */

#ifndef SEQUENCIALDISKACCESSITERATOR_H_
#define SEQUENCIALDISKACCESSITERATOR_H_

#include <vector>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>

#include "../iterator.hpp"
#include "../Schema/Schema.h"
#include "../Schema/SchemaFix.h"

class SequencialDiskAccessIterator :public Iterator {
public:
	struct State
	{
		State(std::string file_name, Schema* input, Schema* output)
		:file_name(file_name), input(input), output(output)
		{};
		State(){};
		std::string file_name;
		Schema* input;
		Schema* output;
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			ar & file_name & input & output;
		}
	};
public:
	SequencialDiskAccessIterator(SequencialDiskAccessIterator::State state);
	SequencialDiskAccessIterator():fd(NULL), length(0), cursor(0) {};
	~SequencialDiskAccessIterator();
	bool open();
	inline bool next(void* desc)
	{
		if (cursor >= length)
			return false;
		cursor += fread(desc, 1, state.input->getTupleMaxSize(), fd);

		return true;
	}
	bool close();
public:
	SequencialDiskAccessIterator::State state;	//should be serialized
private:
	FILE* fd;
	long length;
	long cursor;
private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Iterator>(*this) & state;
	}
};


#endif /* SEQUENCIALDISKACCESSITERATOR_H_ */
