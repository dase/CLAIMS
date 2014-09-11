/*
 * ExecuteLogicalQueryPlan.h
 *
 *  Created on: 2014-3-4
 *      Author: imdb
 */

#ifndef EXECUTELOGICALQUERYPLAN_H_
#define EXECUTELOGICALQUERYPLAN_H_
#include <string>


void ExecuteLogicalQueryPlan();



bool InsertValueToStream(Insert_vals *insert_value, TableDescriptor *table, unsigned position, ostringstream &ostr);

bool CheckType(const column_type *col_type, Expr *expr);
LogicalOperator* convert_sql_to_logical_operator_tree(const char* sql);
#endif /* EXECUTELOGICALQUERYPLAN_H_ */
