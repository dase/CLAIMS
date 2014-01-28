/*
 * SchemaFix.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: wangli
 */
#include <memory.h>

#include "SchemaFix.h"


SchemaFix::SchemaFix(std::vector<column_type> col):Schema(col) {

//	accum_offsets=new unsigned[columns.size()];
	totalsize=0;
	unsigned accumu=0;
	for(unsigned i=0;i<col.size();i++)
	{
		totalsize+=col[i].get_length();
		accum_offsets.push_back(accumu);
		accumu+=col[i].get_length();
	}

}

SchemaFix::~SchemaFix() {
}

unsigned SchemaFix::getTupleMaxSize()
{
	return totalsize;
}

void SchemaFix::getColumnValue(unsigned index, void* src, void* desc)
{
	assert(index<columns.size());
	assert(src!=0&&desc!=0);

	columns[index].operate->assignment(accum_offsets[index]+(char*)src,desc);
}

unsigned SchemaFix::getColumnOffset(unsigned index)
{
	return accum_offsets[index];
}
Schema* SchemaFix::getSubSchema(std::vector<unsigned> index)const{
	std::vector<column_type> col;
	for(unsigned i=0;i<index.size();i++){
		col.push_back(columns[index[i]]);
	}
	return new SchemaFix(col);
}
