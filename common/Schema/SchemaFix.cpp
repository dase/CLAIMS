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
SchemaFix::SchemaFix(const SchemaFix& r):Schema(r){
	this->accum_offsets=r.accum_offsets;
	this->totalsize=r.totalsize;
}
SchemaFix::~SchemaFix() {
}

unsigned SchemaFix::getTupleMaxSize()const
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
Schema* SchemaFix::duplicateSchema()const{
	return new SchemaFix(columns);
}

void SchemaFix::toValue(std::string text_tuple, void* binary_tuple, const char attr_separator)
{
	string::size_type prev_pos = 0;
	string::size_type pos = 0;

	for (int i = 0; i < columns.size(); i++)
	{
		for(; (attr_separator != text_tuple[pos]) && (pos<text_tuple.length()); pos++);
		if(prev_pos <= pos)
		{
			columns[i].operate->toValue(binary_tuple+accum_offsets[i],text_tuple.substr(prev_pos,pos-prev_pos).c_str());
			pos++;
			prev_pos = pos;
		}
	}
}
void SchemaFix::addColumn(column_type  ct,unsigned size)
{
	accum_offsets.push_back(totalsize);
	columns.push_back(ct);
	totalsize+=size;
}
