/*
 * SchemaFix.h
 *
 *  Created on: Jun 8, 2013
 *      Author: wangli
 */

#ifndef SCHEMAFIX_H_
#define SCHEMAFIX_H_
#include <assert.h>
#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include "Schema.h"
class SchemaFix:public Schema {
public:
	SchemaFix(std::vector<column_type> columns);
	SchemaFix(){};
	virtual ~SchemaFix();
	inline unsigned getTupleMaxSize();
	inline unsigned getTupleActualSize(void* tuple) const{
		return totalsize;
	}
	inline unsigned ingetTupleActualSize(void* tuple) const{
		return totalsize;
	}
	inline unsigned getTupleSize()const{
		return totalsize;
	}
	inline unsigned copy(void* src,void* desc) const{
		memcpy(desc,src,totalsize);
		return totalsize;
	}
	void getColumnValue(unsigned index, void* src, void* desc);
	inline void* getColumnAddess(unsigned index,const void* const & column_start) const __attribute__((always_inline))
	{
		return (char*)column_start+accum_offsets[index];
	}
	inline unsigned copyTuple(void* src, void* desc) const{
		memcpy(desc,src,totalsize);
		return totalsize;
	}
	inline Schema::schema_type getSchemaType(){
		return Schema::fixed;
	}
//	void displayTuple(const void* tuple_start_address,const char* spliter)const;
private:
	unsigned getColumnOffset(unsigned index);

private:
	unsigned totalsize;
	std::vector<unsigned> accum_offsets;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Schema>(*this) & totalsize & accum_offsets;
	}
};

#endif /* SCHEMAFIX_H_ */
