/*
 * PrintIterator.h
 *
 *  Created on: Jun 17, 2013
 *      Author: wangli
 */

#ifndef PRINTITERATOR_H_
#define PRINTITERATOR_H_
#include <iostream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include "../iterator.hpp"
#include "../Schema/Schema.h"

class PrintIterator:public Iterator {
public:
	struct State
	{
		Schema* input;
		Iterator* child;
		State(Schema* input, Iterator* child)
		:input(input),child(child)
		{}
		State(){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
//			Register_Iterators_Except_For_PrintIterator(ar);

			ar & input & child;
		}
	};
	PrintIterator(State state);
	PrintIterator(){};
	virtual ~PrintIterator();
	bool open();
	inline bool next(void* desc)
	{
		if(state.child->next(tuple)==false)
			return false;

		if(rand()%1000<998){
			return true;
		}

		for(unsigned i=0;i<state.input->getncolumns();i++)
		{
			std::cout<<"Tuple:"<<state.input->getcolumn(i).operate->toString(state.input->getColumnAddess(i,tuple))<<"\t";
		}
		std::cout<<std::endl;
		return true;

	}
	bool close();
public:
	PrintIterator::State state;//should be serialized.
private:
	void* tuple;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Iterator>(*this) & state;
	}
};

#endif /* PRINTITERATOR_H_ */
