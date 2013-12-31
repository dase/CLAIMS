/*
 * Schema.h
 *
 *  Created on: Jun 7, 2013
 *      Author: wangli
 */

#ifndef SCHEMA_H_
#define SCHEMA_H_
#include <vector>
#include <assert.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "../data_type.h"


class Schema {
public:
	enum schema_type{fixed,varaible};
	Schema(std::vector<column_type> columns);
	Schema(){};
	virtual ~Schema();
	virtual unsigned getTupleMaxSize()=0;

	inline virtual unsigned getTupleActualSize(void* tuple) const=0;
	virtual void getColumnValue(unsigned index,void* src, void* desc)=0;
	inline virtual void* getColumnAddess(const unsigned& index,const void* const & column_start) const __attribute__((always_inline)) =0;
	inline virtual unsigned copyTuple(void* src, void* desc) const =0;
	unsigned getncolumns();
	inline column_type getcolumn(const unsigned index) const {
			return columns[index];
		}
	std::vector<column_type> columns;
	virtual schema_type getSchemaType()=0;
	virtual void displayTuple(const void* tuple_start_address,const char* spliter="|")const;
protected:

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & columns;
	}
};

#endif /* SCHEMA_H_ */
