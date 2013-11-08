/*
 * Column.cpp
 *
 *  Created on: May 7, 2013
 *      Author: wangli
 */

#include "Column_to_be_removed.h"

Column_TOBEREMOVED::Column_TOBEREMOVED(unsigned _table_id,unsigned _column_id,unsigned type_length)
:table_id(_table_id),column_id(column_id),type_length(type_length),record_count(0)
{
	// TODO Auto-generated constructor stub

}

Column_TOBEREMOVED::~Column_TOBEREMOVED() {
	// TODO Auto-generated destructor stub
}

unsigned Column_TOBEREMOVED::get_type_length()
{
	return type_length;
}

