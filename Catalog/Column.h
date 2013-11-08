/*
 * Column.h
 *
 *  Created on: Nov 8, 2013
 *      Author: wangli
 */

#ifndef COLUMN_H_
#define COLUMN_H_
#include "Attribute.h"
#include "../ids.h"
class Column:public Attribute {
public:
	Column(Attribute&,ColumnID);
	Column(const Column& c):Attribute(c){
		this->column_id_=c.column_id_;
	}
	virtual ~Column();
	ColumnID column_id_;
};

#endif /* COLUMN_H_ */
