/*
 * Serialization_Schema.h
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#ifndef SERIALIZATION_SCHEMA_H_
#define SERIALIZATION_SCHEMA_H_
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


class Serialization_Schema:public Schema
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & columns;
	}
};


#endif /* SERIALIZATION_SCHEMA_H_ */
