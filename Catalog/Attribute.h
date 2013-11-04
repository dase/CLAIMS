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
struct Attribute
{
	Attribute(unsigned pos,const std::string& name, data_type type, unsigned size = 0)
	{
		index=pos;
		attrName = name;
		attrType = new column_type(type, size);

	}
	Attribute(const Attribute& att){
		attrName=att.attrName;
		attrType=new column_type(*att.attrType);
		index=att.index;

	}
	std::string attrName;
	column_type* attrType;
	/*the position in the table*/
	unsigned index;
};

#endif /* ATTRIBUTE_H_ */
