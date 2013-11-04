/*
 * CombinedIterator.h
 *
 *  Created on: Jun 17, 2013
 *      Author: wangli
 */

#ifndef COMBINEDITERATOR_H_
#define COMBINEDITERATOR_H_

#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "../iterator.hpp"
#include "../Schema/Schema.h"

class CombinedIterator:public Iterator {
public:
	struct State
	{

		State(std::vector<Schema*> inputs, Schema* output,std::vector<Iterator*> children)
		:inputs(inputs),output(output),children(children)
		{}
		State(){};
		std::vector<Schema*> inputs;
		Schema* output;
		std::vector<Iterator*> children;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & inputs & output & children;
		}
	};
	CombinedIterator(CombinedIterator::State state);
	CombinedIterator():tuple(0){};
	virtual ~CombinedIterator();
	bool open();
	bool next(void* desc);
	bool close();
public:
	CombinedIterator::State state;//should be serialized.
private:

	void* tuple;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar &boost::serialization::base_object<Iterator>(*this) & state;
	}
};

#endif /* COMBINEDITERATOR_H_ */
