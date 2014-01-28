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
#include "../ids.h"
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
		if(att.table_id_!=-1){
			attrName=att.attrName;
			attrType=new column_type(*att.attrType);
			index=att.index;
		}

	}
	Attribute& operator=(const Attribute& att){
		table_id_=att.table_id_;
		if(att.table_id_!=-1){
			attrName=att.attrName;
			attrType=new column_type(*att.attrType);
			index=att.index;
		}
		return *this;
	}
	Attribute():table_id_(-1){

	}
	~Attribute(){
		attrType->~column_type();
	}
	bool operator==(const Attribute& r)const{
		return table_id_==r.table_id_&&index==r.index;
	}
	bool operator!=(const Attribute& r)const{
		return !(table_id_==r.table_id_&&index==r.index);
	}
	bool isNULL()const{
		return table_id_==TableID(-1);
	}
	std::string attrName;
	column_type* attrType;
	/*the position in the table*/
	unsigned index;
	TableID table_id_;
};

#endif /* ATTRIBUTE_H_ */
