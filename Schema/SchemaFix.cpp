/*
 * SchemaFix.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: wangli
 */
#include <memory.h>

#include "SchemaFix.h"


SchemaFix::SchemaFix(std::vector<column_type> columns):Schema(columns) {
	// TODO Auto-generated constructor stub
//	accum_offsets=new unsigned[columns.size()];
	totalsize=0;
	unsigned accumu=0;
	for(unsigned i=0;i<columns.size();i++)
	{
		totalsize+=columns[i].get_length();
		accum_offsets.push_back(accumu);
		accumu+=columns[i].get_length();
	}

}

SchemaFix::~SchemaFix() {
	// TODO Auto-generated destructor stub
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

