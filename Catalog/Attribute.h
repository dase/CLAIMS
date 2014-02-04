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
#define ATTRIBUTE_NULL -1
#define ATTRIBUTE_ANY -2

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
		if(att.table_id_<ATTRIBUTE_ANY){
			attrType=new column_type(*att.attrType);
			index=att.index;
		}

	}
	Attribute& operator=(const Attribute& att){
		table_id_=att.table_id_;
		attrName=att.attrName;
		if(att.table_id_<ATTRIBUTE_ANY){
			attrType=new column_type(*att.attrType);
			index=att.index;
		}
		return *this;
	}

	Attribute(unsigned symbol=ATTRIBUTE_NULL):table_id_(symbol),index(-1),attrType(0){
		switch(symbol){
		case ATTRIBUTE_ANY:{
			attrName="*";
			break;
		}
		case ATTRIBUTE_NULL:{
			attrName="NULL";
			break;
		}
		default:{
			assert(false);
		}
		}

	}
	~Attribute(){
		if(table_id_<TableID(ATTRIBUTE_ANY)){
			attrType->~column_type();
		}
	}
	bool operator==(const Attribute& r)const{
		return table_id_==r.table_id_&&index==r.index;
	}
	bool operator!=(const Attribute& r)const{
		return !(table_id_==r.table_id_&&index==r.index);
	}
	bool isNULL()const{
		return table_id_==TableID(ATTRIBUTE_NULL);
	}
	bool isANY()const{
		return table_id_==TableID(ATTRIBUTE_ANY);
	}
	std::string getName()const{
		return attrName;
	}
	std::string attrName;
	column_type* attrType;
	/*the position in the table*/
	unsigned index;
	TableID table_id_;
};

#endif /* ATTRIBUTE_H_ */
