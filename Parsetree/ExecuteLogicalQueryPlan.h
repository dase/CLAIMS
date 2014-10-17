/*
 * ExecuteLogicalQueryPlan.h
 *
 *  Created on: 2014-3-4
 *      Author: imdb
 */

#ifndef EXECUTELOGICALQUERYPLAN_H_
#define EXECUTELOGICALQUERYPLAN_H_
#include <string>
#include "../common/Block/ResultSet.h"
#include "sql_node_struct.h"
#include "../common/data_type.h"
#include "../Catalog/table.h"
#include <iosfwd>

struct query_result{
	query_result():result_set(0){};
	~query_result(){
		delete result_set;
	}
	string msg;
	ResultSet* result_set;
};

bool query(const string & sql,query_result &result_set);


void ExecuteLogicalQueryPlan();

void ExecuteLogicalQueryPlan(
		const string &sql,
		ResultSet *&result_set,
		bool &result_flag,
		string &error_msg,
		string &info,
		int fd = 0
		);

void CreateTable(
		Catalog *catalog,
		Node *node,
		ResultSet *&result_set,
		bool result_flag,
		string &error_msg,
		string &info
		);

void CreateProjection(
		Catalog *catalog,
		Node *node,
		ResultSet *&result_set,
		bool result_flag,
		string &error_msg,
		string &info
		);


void Query(
		Catalog *catalog,
		Node *node,
		ResultSet *&result_set,
		bool result_flag,
		string &error_msg,
		string &info
		);

void LoadData(
		Catalog *catalog,
		Node *node,
		ResultSet *&result_set,
		bool result_flag,
		string &error_msg,
		string &info
		);

void InsertData(
		Catalog *catalog,
		Node *node,
		ResultSet *&result_set,
		bool result_flag,
		string &error_msg,
		string &info
		);

void ShowTable(
		Catalog *catalog,
		Node *node,
		ResultSet *&result_set,
		bool result_flag,
		string &error_msg,
		string &info
		);

void DropTable(
		Catalog *catalog,
		Node *node,
		ResultSet *&result_set,
		bool result_flag,
		string &error_msg,
		string &info
		);



bool InsertValueToStream(Insert_vals *insert_value, TableDescriptor *table, unsigned position, ostringstream &ostr);
bool CheckType(const column_type *col_type, Expr *expr);

LogicalOperator* convert_sql_to_logical_operator_tree(const char* sql);

#endif /* EXECUTELOGICALQUERYPLAN_H_ */
