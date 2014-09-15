/*
 * ExecuteLogicalQueryPlan.h
 *
 *  Created on: 2014-3-4
 *      Author: imdb
 */

#ifndef EXECUTELOGICALQUERYPLAN_H_
#define EXECUTELOGICALQUERYPLAN_H_
#include "../common/Block/ResultSet.h"
#include "sql_node_struct.h"
#include "../common/data_type.h"
#include "../Catalog/table.h"
#include <iosfwd>


void ExecuteLogicalQueryPlan();
bool InsertValueToStream(Insert_vals *insert_value, TableDescriptor *table, unsigned position, ostringstream &ostr);
bool CheckType(const column_type *col_type, Expr *expr);
void ExecuteLogicalQueryPlan(string sql,ResultSet *&result_set,bool &result_flag,string &error_msg);
#endif /* EXECUTELOGICALQUERYPLAN_H_ */
