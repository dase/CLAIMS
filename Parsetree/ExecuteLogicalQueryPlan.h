/*
 * ExecuteLogicalQueryPlan.h
 *
 *  Created on: 2014-3-4
 *      Author: imdb
 */

#ifndef EXECUTELOGICALQUERYPLAN_H_
#define EXECUTELOGICALQUERYPLAN_H_
#include <string>
#include "./sql_node_struct.h"
#include "../catalog/catalog.h"
#include "../common/Block/ResultSet.h"
#include "../common/data_type.h"
#include "../catalog/table.h"
#include "../logical_operator/logical_operator.h"

using claims::catalog::Catalog;
using claims::catalog::TableDescriptor;
using claims::logical_operator::LogicalOperator;

struct query_result {
  query_result() : result_set(0) {}
  ~query_result() { delete result_set; }
  string msg;
  ResultSet *result_set;
};

bool query(const string &sql, query_result &result_set);

void ExecuteLogicalQueryPlan();

void ExecuteLogicalQueryPlan(const string &sql, ExecutedResult *result);

void CreateTable(Catalog *catalog, Node *node, ExecutedResult *result);

void CreateTable(Catalog *catalog, Node *node);

void CreateProjection(Catalog *catalog, Node *node, ExecutedResult *result);

void CreateProjection(Catalog *catalog, Node *node);

void Query(Catalog *catalog, Node *node, ExecutedResult *result,
           const bool local_mode  // true表示将结果输出到本机标准输出，非C/S模式
           );

void Query(Catalog *catalog, Node *node);

void LoadData(Catalog *catalog, Node *node, ExecutedResult *result);

void LoadData(Catalog *catalog, Node *node);

void InsertData(Catalog *catalog, Node *node, ExecutedResult *result);

void InsertData(Catalog *catalog, Node *node);

void ShowTable(Catalog *catalog, Node *node, ExecutedResult *result);

void ShowTable(Catalog *catalog, Node *node);

void DropTable(Catalog *catalog, Node *node, ExecutedResult *result);

bool InsertValueToStream(Insert_vals *insert_value, TableDescriptor *table,
                         unsigned position, ostringstream &ostr);
bool CheckType(const column_type *col_type, Expr *expr);

LogicalOperator *convert_sql_to_logical_operator_tree(const char *sql);

#endif /* EXECUTELOGICALQUERYPLAN_H_ */
