/*
 * SingleColumnScanIterator.h
 *
 *  Created on: Jun 7, 2013
 *      Author: wangli
 */

#ifndef SINGLECOLUMNSCANITERATOR_H_
#define SINGLECOLUMNSCANITERATOR_H_

#include <vector>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>

#include "../iterator.hpp"
#include "../common/Schema/Schema.h"
#include "../common/Schema/SchemaFix.h"

//#include "../Serialization/s.h"
class SingleColumnScanIterator :public Iterator{
public:
	struct State
	{
		State(std::string file_name,Schema* input, Schema* output)
		:file_name(file_name),input(input),output(output)
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
	SingleColumnScanIterator(SingleColumnScanIterator::State state);
	SingleColumnScanIterator():fd(0),base(0),cursor(0),length(0){};
	virtual ~SingleColumnScanIterator();
	bool open();
	inline bool next(void* desc)
	{

		if(cursor>=(char*)base+length)
			return false;
//		state.input->getcolumn(0).operate->assignment(cursor,desc);
		state.input->copyTuple(cursor,desc);
//		*(int*)desc=*(int*)cursor;
//		state.input->columns[0].operate->assignment(cursor,desc);
		cursor+=(state.input)->getTupleActualSize(cursor);
//		cursor+=4;


//		if(((cursor-(char*)base)/state.input->getTupleMaxSize())%100==0){
//			printf("[Scan]: %d tuples ",(cursor-(char*)base)/state.input->getTupleMaxSize());
//		}

		return true;
	}
	bool close();
public:
	SingleColumnScanIterator::State state;//should be serialized.
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

#endif /* SINGLECOLUMNSCANITERATOR_H_ */
