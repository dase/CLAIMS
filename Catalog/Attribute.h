/*
 * Attribute.h
 *
 *  Created on: Oct 30, 2013
 *      Author: wangli
 */

#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_
#include <string>
#include "../data_type.h"
typedef unsigned TableID;
struct Attribute
{
	Attribute(TableID tableid,unsigned pos,const std::string& name, data_type type, unsigned size = 0)
	{
		table_id_=tableid;
		index=pos;
		attrName = name;
		attrType = new column_type(type, size);

	}
	Attribute(const Attribute& att){
		table_id_=att.table_id_;
		attrName=att.attrName;
		attrType=new column_type(*att.attrType);
		index=att.index;

	}
	std::string attrName;
	column_type* attrType;
	/*the position in the table*/
	unsigned index;
	TableID table_id_;
};

#endif /* ATTRIBUTE_H_ */
