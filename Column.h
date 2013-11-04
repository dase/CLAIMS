/*
 * Column.h
 *
 *  Created on: May 7, 2013
 *      Author: wangli
 */

#ifndef COLUMN_H_
#define COLUMN_H_
#include "ColumnAppenderBase.h"


#define oid_t unsigned
class Column {
public:
	Column(unsigned _table_id,unsigned column_id, unsigned type_length);
	virtual ~Column();
	virtual unsigned get_type_length();



	virtual ColumnAppenderBase* create_appender()=0;

public:
	unsigned record_count;
private:
	unsigned tuple_length;
	unsigned type_length;

	unsigned table_id;
	unsigned column_id;
};

#endif /* COLUMN_H_ */
