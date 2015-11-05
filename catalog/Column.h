/*
 * Column.h
 *
 *  Created on: Nov 8, 2013
 *      Author: wangli
 */

#ifndef COLUMN_H_
#define COLUMN_H_

#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../catalog/Attribute.h"
#include "../common/ids.h"
class Column:public Attribute {
public:
	Column(){};
	Column(Attribute&,ColumnID);
	Column(const Column& c):Attribute(c){
		this->column_id_=c.column_id_;
	}
	virtual ~Column();
	ColumnID column_id_;

	/* for boost::serialization*/
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
    	ar & boost::serialization::base_object<Attribute>(*this) & column_id_;
    }
};

#endif /* COLUMN_H_ */
