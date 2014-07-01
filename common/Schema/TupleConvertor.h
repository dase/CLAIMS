/*
 * TupleConventor.h
 * This class makes varous tuple conversions.
 *
 *  Created on: Jan 26, 2014
 *      Author: wangli
 */

#ifndef TUPLECONVENTOR_H_
#define TUPLECONVENTOR_H_
#include <vector>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "Schema.h"
class TupleConvertor {
public:
	TupleConvertor();
	virtual ~TupleConvertor();

	/*
	 * extract sub-tuple from a tuple. The index for the columns that appear in the desc tuple are specified in
	 * parameter index in form of array. E.g., index={0,3,4} means that the remainning columns are the 1st, 3rd
	 * and 4th.
	 * src_s and des_c describe the schema for tuple and desc tuple.
	 * memory of desc tuple shoulbe be allocated before calling this method.
	 */
	void sub_tuple(const Schema*& src_s,const Schema*& des_c,const void* const& tuple, void* desc, std::vector<unsigned> index);
//	void sub_tuple(const Schema* const src_s,const Schema* const des_s, void*& const tuple,void*& const desc);
};

class SubTuple{
public:
	SubTuple(Schema* srouce, Schema* target, std::vector<unsigned> index);
	void getSubTuple(void*& tuple, void*& target);
private:
	Schema* source_schema_;
	Schema* target_schema_;
	std::vector<unsigned> index_;

};

#endif /* TUPLECONVENTOR_H_ */
