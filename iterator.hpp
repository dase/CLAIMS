/*
 * iterator
 *
 *  Created on: May 7, 2013
 *      Author: wangli
 */

#ifndef ITERATOR_
#define ITERATOR_
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

/**
 * Base class of iterator
 */
class Iterator
{
public:
	virtual ~Iterator(){};
	virtual bool open()=0;
	inline virtual bool next(void* desc)=0;
	virtual bool close()=0;
private://serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{

	}

};


#endif /* ITERATOR_ */
