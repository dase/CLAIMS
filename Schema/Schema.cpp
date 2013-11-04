/*
 * Schema.cpp
 *
 *  Created on: Jun 7, 2013
 *      Author: wangli
 */

#include "Schema.h"

Schema::Schema(std::vector<column_type> columns):columns(columns) {
	// TODO Auto-generated constructor stub

}

Schema::~Schema() {
	// TODO Auto-generated destructor stub
}

unsigned Schema::getncolumns()
{
	return columns.size();
}

