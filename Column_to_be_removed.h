/*
 * Column.h
 *
 *  Created on: May 7, 2013
 *      Author: wangli
 */

#ifndef COLUMN_TOBEREMOVED_H_
#define COLUMN_TOBEREMOVED_H_
#include "ColumnAppenderBase.h"


#define oid_t unsigned
class Column_TOBEREMOVED {
public:
	Column_TOBEREMOVED(unsigned _table_id,unsigned column_id, unsigned type_length);
	virtual ~Column_TOBEREMOVED();
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
