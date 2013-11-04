/*
 * Column.cpp
 *
 *  Created on: May 7, 2013
 *      Author: wangli
 */

#include "Column.h"

Column::Column(unsigned _table_id,unsigned _column_id,unsigned type_length)
:table_id(_table_id),column_id(column_id),type_length(type_length),record_count(0)
{
	// TODO Auto-generated constructor stub

}

Column::~Column() {
	// TODO Auto-generated destructor stub
}

unsigned Column::get_type_length()
{
	return type_length;
}

