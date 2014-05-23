/*
 * RowScanIterator.h
 *
 *  Created on: 2013年8月23日
 *      Author: SCDONG
 */

#ifndef ROWSCANITERATOR_H_
#define ROWSCANITERATOR_H_

#include <vector>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>

#include "../iterator.hpp"
#include "../common/Schema/Schema.h"
#include "../common/Schema/SchemaFix.h"

class RowScanIterator :public Iterator{
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
		template<class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			ar & file_name & input & output;
		}
	};
public:
	RowScanIterator(RowScanIterator::State state);
	RowScanIterator():fd(0),base(0),cursor(0),length(0){};
	virtual ~RowScanIterator();
	bool open();
	inline bool next(void* desc)
	{
		if(cursor >= (char*)base+length)
			return false;

		cursor+=state.input->copyTuple(cursor,desc);

		return true;
	}
	bool close();
public:
	RowScanIterator::State state;//should be serialized.
private:
	void* base;
	char* cursor;
	long length;
	int fd;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar &boost::serialization::base_object<Iterator>(*this)& state;
	}

};


#endif /* ROWSCANITERATOR_H_ */
