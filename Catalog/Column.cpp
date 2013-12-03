/*
 * Column.cpp
 *
 *  Created on: Nov 8, 2013
 *      Author: wangli
 */

#include "Column.h"


Column::~Column() {
	// TODO Auto-generated destructor stub
}

Column::Column(Attribute& attr,ColumnID column_id)
:Attribute(attr),column_id_(column_id){

}
