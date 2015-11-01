/*
 * ExecuteLogicalQueryPlan.cpp
 *
 *  Created on: 2014-3-3
 *      Author: imdb
 */

#ifndef __EXECLOGICALQUERYPLAN__
#define __EXECLOGICALQUERYPLAN__

#include <iostream>
#include <cstring>
#include <string>
#include "../Environment.h"

#include "../Catalog/stat/Analyzer.h"

#include "../Catalog/ProjectionBinding.h"

#include "../Parsetree/sql_node_struct.h"
#include "../Parsetree/parsetree2logicalplan.cpp"
#include "../Parsetree/runparsetree.h"
#include "../Parsetree/ExecuteLogicalQueryPlan.h"

#include "../logical_operator/logical_scan.h"
#include "../logical_operator/logical_equal_join.h"
#include "../logical_operator/logical_aggregation.h"

#include "../logical_operator/logical_filter.h"
#include "../utility/rdtsc.h"

#include "../Loader/Hdfsloader.h"

#include "../Client/ClaimsServer.h"
#include "../logical_operator/logical_limit.h"
#include "../logical_operator/logical_query_plan_root.h"
#define SQL_Parser
using namespace std;
#define SQL_Parser

const int INT_LENGTH = 10;
const int FLOAT_LENGTH = 10;
const int SMALLINT_LENGTH = 4;
#define SQL_Parser
timeval start_time;	//2014-5-4---add---by Yu

void ExecuteLogicalQueryPlan(const string &sql,ResultSet *&result_set,bool &result_flag,string &error_msg, string &info, int fd)
{
	Environment::getInstance(true);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();
	string tablename;
    vector<Node *>allnode;
	struct ParseResult presult={NULL,NULL,sql.c_str(),0,&allnode};
	Node* oldnode=getparsetreeroot(&presult,sql.c_str());
	if(oldnode == NULL)
	{
		FreeAllNode(presult.node_pointer);
		error_msg="There are some errors during parsing time";
		result_flag=false;
		result_set = NULL;
		return;
	}
	Stmt *stmtList = (Stmt *)oldnode;
	while (stmtList != NULL)
	{
		Node *node = (Node *)stmtList->data;
		switch(node->type)
		{
		case t_create_table_stmt:
		{
			CreateTable(catalog, node, result_set, result_flag, error_msg, info);
			break;
		}
		case t_create_projection_stmt:
		{
			CreateProjection(catalog, node, result_set, result_flag, error_msg, info);
			break;
		}
		case t_query_stmt:
		{
			Query(catalog, node, result_set, result_flag, error_msg, info, false);
			break;
		}
		case t_load_table_stmt:
		{
			LoadData(catalog, node, result_set, result_flag, error_msg, info);
			break;
		}
		case t_insert_stmt:	// 2014-4-19---add---by Yu	// 2014-5-1---modify---by Yu
		{
			InsertData(catalog, node, result_set, result_flag, error_msg, info);
			break;
		}
		case t_show_stmt:
		{
			ShowTable(catalog, node, result_set, result_flag, error_msg, info);
			break;
		}
		case t_drop_stmt:
		{
			DropTable(catalog, node, result_set, result_flag, error_msg, info);
			break;
		}
		default:
		{
			cout<<node->type<<endl;
			puts("nothing matched!\n");
			error_msg = "no sentence matched";
			result_flag = false;
			result_set = NULL;
		}
		}
		if(result_flag==false){
			FreeAllNode(&allnode);	// -Yu 2015-3-2
		}
		stmtList = (Stmt *)stmtList->next;
	}
}


void ExecuteLogicalQueryPlan()
{
	Environment::getInstance(true);
	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();

	while(1)
	{
		//cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SQL is begginning~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;;
		string tablename;
	    vector<Node *>allnode;
		struct ParseResult presult={NULL,NULL,NULL,0,&allnode};
		Node* oldnode=getparsetreeroot(&presult);
		// get parser time	//2014-5-4---add---by Yu
		timeval finish_parser_time;
		gettimeofday(&finish_parser_time, NULL);
		cout<<"parser use "<<(double)(finish_parser_time.tv_usec - start_time.tv_usec)/1000+(finish_parser_time.tv_sec - start_time.tv_sec)*1000<<" ms"<<endl;

		if(oldnode == NULL)	// 2014-2-24---增加node为空的判断---by余楷
		{
			printf("[ERROR]there are some wrong in statement! please try again!!\n");
			FreeAllNode(presult.node_pointer);	//释放SQL解析过程忠所有申请的内存		// 2014-3-6---增加解析错误后的处理---by余楷
			//			printf("Continue(1) or not (others number)?\n");
			//			scanf("%d",&count);
			//			getchar();	// 2014-3-4---屏蔽换行符对后面的影响---by余楷
			//			setbuf(stdin, NULL);	//关闭缓冲
			continue;
		}

		ResultSet *result_set = NULL;
		bool result_flag = true;
		string error_msg = "";
		string info = "";

		Stmt *stmtList = (Stmt *)oldnode;

		while (stmtList != NULL)
		{
			Node *node = (Node *)stmtList->data;

			// 语义分析
			switch(node->type)
			{
			case t_create_table_stmt:	// 创建表的语句
			{
				CreateTable(catalog, node, result_set, result_flag, error_msg, info);
			}
			break;
			case t_create_projection_stmt:	// 创建projection的语句
			{
				CreateProjection(catalog, node, result_set, result_flag, error_msg, info);
			}
			break;
			case t_query_stmt: // 2014-3-4---修改为t_query_stmt,添加对查询语句的处理---by余楷
			{
				Query(catalog, node, result_set, result_flag, error_msg, info, true);
			}
			break;
			case t_load_table_stmt:	//	导入数据的语句
			{
				LoadData(catalog, node, result_set, result_flag, error_msg, info);
			}
			break;
			case t_insert_stmt:	// 2014-4-19---add---by Yu	// 2014-5-1---modify---by Yu
			{
				InsertData(catalog, node, result_set, result_flag, error_msg, info);
			}
			break;
			case t_show_stmt:
			{
				ShowTable(catalog, node, result_set, result_flag, error_msg, info);
			}
			break;
			default:
			{
				ASTParserLogging::log("%s", node->type);
				puts("nothing matched!\n");
			}
			}//end switch
			if(result_flag==false){
				ASTParserLogging::elog("%s", error_msg.c_str());
				FreeAllNode(&allnode);// -Yu 2015-3-2
			}
			else {
				cout<<info<<endl;
			}
//			malloc_stats();
			stmtList = (Stmt *)stmtList->next;
		}

		FreeAllNode(&allnode);

		//		SQLParse_log("SQL Complete! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		//		printf("Continue(1) or not (0)?\n");
		//		scanf("%d",&count);
		//		getchar();	// 2014-3-4---屏蔽换行符对后面的影响---by余楷
	}
}

bool InsertValueToStream(Insert_vals *insert_value, TableDescriptor *table, unsigned position, ostringstream &ostr)
{
	bool has_warning = true;
	if (insert_value->value_type == 0)	// 指定具体的值
	{
		// check whether the column type match value type
		has_warning = CheckType(table->getAttribute(position).attrType, (Expr*)insert_value->expr);

		switch (insert_value->expr->type)	// 2014-4-17---only these type are supported now---by Yu
		{
		case t_stringval: case t_intnum: case t_approxnum: case t_bool:
		{
			Expr *expr = (Expr *)insert_value->expr;
			ostr<<expr->data;
		}
		break;
		default:{}
		}
	}
	else if(insert_value->type == 1) {}	// 设置为default, 暂不支持

	return has_warning;
}
/*
bool query(const string& sql, query_result& result_set) {
	bool ret;
	string msg;
	string err;
	ExecuteLogicalQueryPlan(sql,result_set.result_set,ret,err,msg);
	if(ret){
		result_set.msg = msg;
	}
	else {
		result_set.msg = err;
	}
	return ret;
}
*/
bool CheckType(const column_type *col_type, Expr *expr)		// check whether the string is digit, can use strtol()
{
	nodetype insert_value_type = expr->type;
	//TODO	对类型为int的列不能插入字符串等。。。
	switch(col_type->type)
	{
	case t_int: return (insert_value_type != t_intnum) || strlen(expr->data) > INT_LENGTH;
	case t_float: return !(insert_value_type == t_approxnum) || strlen(expr->data) > FLOAT_LENGTH;
	case t_double: return (insert_value_type != t_approxnum);
	case t_string: return (insert_value_type != t_stringval) || strlen(expr->data) > col_type->get_length();//---5.27fzh---
	//case t_u_long: return (insert_value_type != t_intnum) || strlen(expr->data) > INT_LENGTH || (expr->data[0] == '-');	// =='-' 实际不可行，‘-’不会被识别进expr中
	case t_date: return false;
	case t_time: return false;
	case t_datetime: return false;
	case t_smallInt: return (insert_value_type != t_intnum) || strlen(expr->data) > SMALLINT_LENGTH;
	case t_decimal: return (insert_value_type != t_intnum);
	//case t_u_smallInt: return (insert_value_type != t_intnum) || (expr->data[0] == '-') || length(expr->data) > INT_LENGTH;
	//	strtol();
	default: return true;
	}
}

void CreateTable(Catalog *catalog, Node *node, ResultSet *&result_set, bool &result_flag, string &error_msg, string &info)
{
	/* nodetype type, int create_type, int check, char * name1, char * name2, Node * list, Node * select_stmt */
	Create_table_stmt * ctnode = (Create_table_stmt *)node;
	string tablename;
	if(ctnode->name2 != NULL) {
		tablename = ctnode->name2;
	}
	else if(ctnode->name1 != NULL) {
		tablename = ctnode->name1;
	}
	else {
		error_msg="No table name during creating table!";
		result_flag=false;
		result_set = NULL;
		return;
	}

	TableDescriptor *new_table = Environment::getInstance()->getCatalog()->getTable(tablename);
	if (new_table != NULL)
	{
		error_msg="The table "+tablename +" has existed during creating table!";
		result_flag=false;
		result_set = NULL;
		return;
	}

	new_table = new TableDescriptor(tablename,Environment::getInstance()->getCatalog()->allocate_unique_table_id());
	new_table->addAttribute("row_id",data_type(t_u_long),0,true);

	Create_col_list *list = (Create_col_list*)ctnode->list;
	string primaryname;
	int colNum = 0;
	while (list)
	{
		Create_def *data = (Create_def*) list->data;
		if (data->deftype == 1)
		{
			++colNum;
			string colname = data->name;
			primaryname = colname;
			Column_atts *column_atts = (Column_atts*)data->col_atts;

			/* TODO: Whether column is unique or has default value is not finished,
			 *  because there are no supports
			 */
			Datatype * datatype = (Datatype *)data->datatype;
			switch (datatype->datatype)	// add more type --- 2014-4-2
			{
			case 1:
			{
				if (column_atts && (column_atts->datatype & 01)){	// not null
					new_table->addAttribute(colname, data_type(t_boolean), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype & 02)){	// can be null
					new_table->addAttribute(colname, data_type(t_boolean), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_boolean), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 3:
			{
				if (datatype->opt_uz & 01 != 0){
					if (column_atts && (column_atts->datatype & 01)){
						new_table->addAttribute(colname, data_type(t_u_smallInt), 0, true, false);
					}
					else if (column_atts && (column_atts->datatype & 02)){
						new_table->addAttribute(colname, data_type(t_u_smallInt), 0, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_u_smallInt), 0, true);
					}
				}
				else{
					if (column_atts && (column_atts->datatype & 01)){
						new_table->addAttribute(colname, data_type(t_smallInt), 0, true, false);
					}
					else if (column_atts && (column_atts->datatype & 02)){
						new_table->addAttribute(colname, data_type(t_smallInt), 0, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_smallInt), 0, true);
					}
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 5: case 6:
			{
				if (column_atts && (column_atts->datatype & 01)){
					new_table->addAttribute(colname, data_type(t_int), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype & 02)){
					new_table->addAttribute(colname, data_type(t_int), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_int), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 7:
			{
				if (datatype->opt_uz & 01 != 0)
				{
					if (column_atts && (column_atts->datatype & 01)){
						new_table->addAttribute(colname, data_type(t_u_long), 0, true, false);
					}
					else if (column_atts && (column_atts->datatype & 02)){
						new_table->addAttribute(colname, data_type(t_u_long), 0, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_u_long), 0, true);
					}
					cout<<colname<<" is created"<<endl;
				}
				else
				{
					//TODO:no supports
					error_msg="This type is not supported during creating table!";
					result_flag=false;
					result_set = NULL;
				}
				break;
			}
			case 9:
			{
				if (column_atts && (column_atts->datatype & 01)){
					new_table->addAttribute(colname, data_type(t_double), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype & 02)){
					new_table->addAttribute(colname, data_type(t_double), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_double), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 10:
			{
				if (column_atts && (column_atts->datatype & 01)){
					new_table->addAttribute(colname, data_type(t_float), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype & 02)){
					new_table->addAttribute(colname, data_type(t_float), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_float), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 11:
			{
				if(datatype->length){
					Length * l = (Length*)datatype->length;

					if (column_atts && (column_atts->datatype & 01)){
						new_table->addAttribute(colname, data_type(t_decimal), l->data1, true, false);
					}
					else if (column_atts && (column_atts->datatype & 02)){
						new_table->addAttribute(colname, data_type(t_decimal), l->data1, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_decimal), l->data1, true);
					}
					cout<<colname<<" is created"<<endl;
				}
				else{
					if (column_atts && (column_atts->datatype & 01)){
						new_table->addAttribute(colname, data_type(t_decimal), 0, true, false);
					}
					else if (column_atts && (column_atts->datatype & 02)){
						new_table->addAttribute(colname, data_type(t_decimal), 0, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_decimal), 0, true);
					}
					cout<<colname<<" is created"<<endl;
				}
				break;
			}
			case 12:	// DATE --- 2014-4-1
			{
				if (column_atts && (column_atts->datatype & 01)){
					new_table->addAttribute(colname, data_type(t_date), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype & 02)){
					new_table->addAttribute(colname, data_type(t_date), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_date), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 13:	// TIME --- 2014-4-1
			{
				if (column_atts && (column_atts->datatype & 01)){
					new_table->addAttribute(colname, data_type(t_time), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype & 02)){
					new_table->addAttribute(colname, data_type(t_time), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_time), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 15:	// DATETIME --- 2014-4-1
			{
				if (column_atts && (column_atts->datatype & 01)){
					new_table->addAttribute(colname, data_type(t_datetime), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype & 02)){
					new_table->addAttribute(colname, data_type(t_datetime), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_datetime), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}

			case 17: case 18:
			{
				if (datatype->length)
				{
					Length * l = (Length*)datatype->length;

					if (column_atts && (column_atts->datatype & 01)){
						new_table->addAttribute(colname, data_type(t_string), l->data1, true, false);
					}
					else if (column_atts && (column_atts->datatype & 02)){
						new_table->addAttribute(colname, data_type(t_string), l->data1, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_string), l->data1, true);
					}
				}
				else
				{
					if (column_atts && (column_atts->datatype & 01)){
						new_table->addAttribute(colname, data_type(t_string), 1, true, false);
					}
					else if (column_atts && (column_atts->datatype & 02)){
						new_table->addAttribute(colname, data_type(t_string), 1, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_string), 1, true);
					}
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			default:
			{
				error_msg="This type is not supported now during creating table!";
				result_flag=false;
				result_set = NULL;
			}
			}
		}
		list = (Create_col_list* )list->next;
	}
	if(result_flag==false)
		return;

//	cout<<"the first attribute Name:"<<new_table->getAttribute(0).getName()<<endl;

//	new_table->createHashPartitionedProjectionOnAllAttribute(new_table->getAttribute(0).getName(), 18);
	catalog->add_table(new_table);
	//				TableID table_id=catalog->getTable(tablename)->get_table_id();

	//				for(unsigned i=0;i<catalog->getTable(table_id)->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){
	//					catalog->getTable(table_id)->getProjectoin(catalog->getTable(table_id)->getNumberOfProjection()-1)->getPartitioner()->RegisterPartition(i,2);
	//					catalog->getTable(table_id)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
	//				}

	catalog->saveCatalog();
	//			catalog->restoreCatalog();// commented by li to solve the dirty read after insert
	result_flag=true;
	info = "create table successfully";
	result_set=NULL;
	return;
}
/*

void CreateTable(Catalog *catalog, Node *node) {
	string tablename;
	 nodetype type, int create_type, int check, char * name1, char * name2, Node * list, Node * select_stmt
	Create_table_stmt * ctnode = (Create_table_stmt *)node;
	if(ctnode->name2 != NULL) {
		tablename = ctnode->name2;
	}
	else if(ctnode->name1 != NULL) {
		tablename = ctnode->name1;
	}
	else {
		ASTParserLogging::elog("No table name!");
		return;
	}

	TableDescriptor *new_table = Environment::getInstance()->getCatalog()->getTable(tablename);
	if (new_table != NULL) {
		ASTParserLogging::elog("The table %s has existed!", tablename.c_str());
		return;
	}
	new_table = new TableDescriptor(tablename,Environment::getInstance()->getCatalog()->allocate_unique_table_id());

	new_table->addAttribute("row_id",data_type(t_u_long),0,true);

	Create_col_list *list = (Create_col_list*)ctnode->list;
	string primaryname;
	int colNum = 0;
	while (list)
	{
		Create_def *data = (Create_def*) list->data;
		if (data->deftype == 1)
		{
			++colNum;
			string colname = data->name;
			primaryname = colname;
			Column_atts *column_atts = (Column_atts*)data->col_atts;

			 TODO: Whether column is unique or has default value is not finished,
			 *  because there are no supports

			Datatype * datatype = (Datatype *)data->datatype;
			switch (datatype->datatype)	// add more type --- 2014-4-2
			{
			case 1:
			{
				if (column_atts && (column_atts->datatype && 01)){	// not null
					new_table->addAttribute(colname, data_type(t_boolean), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype && 02)){	// can be null
					new_table->addAttribute(colname, data_type(t_boolean), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_boolean), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 3:
			{
				if (datatype->opt_uz & 01 != 0){
					if (column_atts && (column_atts->datatype && 01)){
						new_table->addAttribute(colname, data_type(t_u_smallInt), 0, true, false);
					}
					else if (column_atts && (column_atts->datatype && 02)){
						new_table->addAttribute(colname, data_type(t_u_smallInt), 0, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_u_smallInt), 0, true);
					}
				}
				else{
					if (column_atts && (column_atts->datatype && 01)){
						new_table->addAttribute(colname, data_type(t_smallInt), 0, true, false);
					}
					else if (column_atts && (column_atts->datatype && 02)){
						new_table->addAttribute(colname, data_type(t_smallInt), 0, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_smallInt), 0, true);
					}
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 5: case 6:
			{
				if (column_atts && (column_atts->datatype && 01)){
					new_table->addAttribute(colname, data_type(t_int), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype && 02)){
					new_table->addAttribute(colname, data_type(t_int), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_int), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 7:
			{
				if (datatype->opt_uz & 01 != 0)
				{
					if (column_atts && (column_atts->datatype && 01)){
						new_table->addAttribute(colname, data_type(t_u_long), 0, true, false);
					}
					else if (column_atts && (column_atts->datatype && 02)){
						new_table->addAttribute(colname, data_type(t_u_long), 0, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_u_long), 0, true);
					}
					cout<<colname<<" is created"<<endl;
				}
				else
				{
					//TODO:no supports
					ASTParserLogging::log("This type is not supported!");
				}
				break;
			}
			case 9:
			{
				if (column_atts && (column_atts->datatype && 01)){
					new_table->addAttribute(colname, data_type(t_double), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype && 02)){
					new_table->addAttribute(colname, data_type(t_double), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_double), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 10:
			{
				if (column_atts && (column_atts->datatype && 01)){
					new_table->addAttribute(colname, data_type(t_float), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype && 02)){
					new_table->addAttribute(colname, data_type(t_float), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_float), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 11:
			{
				if (column_atts && (column_atts->datatype && 01)){
					new_table->addAttribute(colname, data_type(t_decimal), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype && 02)){
					new_table->addAttribute(colname, data_type(t_decimal), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_decimal), 0, true);
				}
				cout<<colname<<" is created"<<endl;
			}
			case 12:	// DATE --- 2014-4-1
			{
				if (column_atts && (column_atts->datatype && 01)){
					new_table->addAttribute(colname, data_type(t_date), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype && 02)){
					new_table->addAttribute(colname, data_type(t_date), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_date), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 13:	// TIME --- 2014-4-1
			{
				if (column_atts && (column_atts->datatype && 01)){
					new_table->addAttribute(colname, data_type(t_time), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype && 02)){
					new_table->addAttribute(colname, data_type(t_time), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_time), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			case 15:	// DATETIME --- 2014-4-1
			{
				if (column_atts && (column_atts->datatype && 01)){
					new_table->addAttribute(colname, data_type(t_datetime), 0, true, false);
				}
				else if (column_atts && (column_atts->datatype && 02)){
					new_table->addAttribute(colname, data_type(t_datetime), 0, true, true);
				}
				else{
					new_table->addAttribute(colname, data_type(t_datetime), 0, true);
				}
				cout<<colname<<" is created"<<endl;
				break;
			}

			case 17: case 18:
			{
				if (datatype->length)
				{
					Length * l = (Length*)datatype->length;

					if (column_atts && (column_atts->datatype && 01)){
						new_table->addAttribute(colname, data_type(t_string), l->data1, true, false);
					}
					else if (column_atts && (column_atts->datatype && 02)){
						new_table->addAttribute(colname, data_type(t_string), l->data1, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_string), l->data1, true);
					}
				}
				else
				{
					if (column_atts && (column_atts->datatype && 01)){
						new_table->addAttribute(colname, data_type(t_string), 1, true, false);
					}
					else if (column_atts && (column_atts->datatype && 02)){
						new_table->addAttribute(colname, data_type(t_string), 1, true, true);
					}
					else{
						new_table->addAttribute(colname, data_type(t_string), 1, true);
					}
				}
				cout<<colname<<" is created"<<endl;
				break;
			}
			default:
			{
				ASTParserLogging::log("This type is not supported now!");
			}
			}
		}
		list = (Create_col_list* )list->next;
	}

	// add for  test, create projection default while creating table
	cout<<"Name:"<<new_table->getAttribute(0).getName()<<endl;

	catalog->add_table(new_table);

	TableID table_id=catalog->getTable(tablename)->get_table_id();


	 * note:
	 * after creating a new table,
	 * 	a projection with 18 partition number will be created
	 * 	--Yu,2015-2-9

	new_table->createHashPartitionedProjectionOnAllAttribute(new_table->getAttribute(0).getName(), 18);
	//				for(unsigned i=0;i<catalog->getTable(table_id)->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){
	////					catalog->getTable(table_id)->getProjectoin(catalog->getTable(table_id)->getNumberOfProjection()-1)->getPartitioner()->RegisterPartition(i,2);
	//					catalog->getTable(table_id)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
	//				}

	catalog->saveCatalog();
}
*/

void CreateProjection(Catalog *catalog, Node *node, ResultSet *&result_set, bool & result_flag,  string &error_msg, string &info) {
	bool is_correct = true;
	Create_projection_stmt *newnode = (Create_projection_stmt *)node;
	int partition_num = newnode->partition_num;
	string tablename = newnode->tablename;
	TableDescriptor *table = NULL;

	if((table = catalog->getTable(tablename)) == NULL)	// 2014-4-30---add check---by Yu
	{
		error_msg="There is no table named "+ tablename+" during creating projection";
		result_flag=false;
		result_set = NULL;
		return;
		is_correct = false;
		return;
	}
	TableID table_id=catalog->getTable(tablename)->get_table_id();
	string partition_attribute_name = newnode->partition_attribute_name;

	std::vector<ColumnOffset> index;
	index.push_back(0);		// add by scdong: add row_id column to each projection automatically
	Columns *col_list = (Columns *)newnode->column_list;
	string colname;
	while(col_list)
	{
		if (col_list->parameter2 != NULL)
		{
			colname = col_list->parameter2;
		}
		else if (col_list->parameter1 != NULL)
		{
			colname = col_list->parameter1;
		}
		else
		{
			error_msg="NO column name during creating projection!";
			result_flag=false;
			result_set = NULL;
			return;
			is_correct = false;
			break;
		}
		cout<<tablename+"."+colname<<endl;
		if(table->isExist(tablename+"."+colname))	// 2014-4-30---add check---by Yu
			index.push_back(table->getAttribute(colname).index);
		else
		{
			error_msg="The column "+ colname+" is not existed! during creating projection";
			result_flag=false;
			result_set = NULL;
			return;
			is_correct = false;
			break;
		}
		col_list = (Columns *)col_list->next;
	}
	if(result_flag==false)
		return;
	if(!is_correct)
		return;

	catalog->getTable(table_id)->createHashPartitionedProjection(index,partition_attribute_name,partition_num);

	int projection_index = catalog->getTable(table_id)->getNumberOfProjection()-1;
	for(unsigned i=0;i<catalog->getTable(table_id)->getProjectoin(projection_index)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(table_id)->getProjectoin(projection_index)->getPartitioner()->RegisterPartition(i,0);
	}

	catalog->saveCatalog();
	//			catalog->restoreCatalog();// commented by li to solve the dirty read after insert

	result_flag=true;
	result_set = NULL;
	info = "create projection successfully";
	result_set=NULL;
	return;
}
/*

void CreateProjection(Catalog *catalog, Node *node) {
	ASTParserLogging::log("this is create projection");
	Create_projection_stmt *newnode = (Create_projection_stmt *)node;
	int partition_num = newnode->partition_num;
	string tablename = newnode->tablename;
	TableDescriptor *table = NULL;

	if((table = catalog->getTable(tablename)) == NULL)	// 2014-4-30---add check---by Yu
	{
		ASTParserLogging::elog("There is no table named %s", tablename.c_str());
		return;
	}
	TableID table_id=catalog->getTable(tablename)->get_table_id();
	string partition_attribute_name = newnode->partition_attribute_name;

	std::vector<ColumnOffset> index;
	index.push_back(0);		// add by scdong: add row_id column to each projection automatically
	Columns *col_list = (Columns *)newnode->column_list;
	string colname;
	while(col_list)
	{
		if (col_list->parameter2 != NULL) {
			colname = col_list->parameter2;
		}
		else if (col_list->parameter1 != NULL) {
			colname = col_list->parameter1;
		}
		else {
			cout<<"[ERROR]NO column name!"<<endl;
			return;
		}
		cout<<colname<<endl;
		if(table->isExist(tablename+"."+colname))	{// 2014-4-30---add check---by Yu
			index.push_back(table->getAttribute(colname).index);
		}
		else {
			ASTParserLogging::elog("The column %s is not existed!", colname.c_str());
			return;
		}
		col_list = (Columns *)col_list->next;
	}

	catalog->getTable(table_id)->createHashPartitionedProjection(index,partition_attribute_name,partition_num);
	int projection_index = catalog->getTable(table_id)->getNumberOfProjection()-1;
	for(unsigned i=0;i<catalog->getTable(table_id)->getProjectoin(projection_index)->getPartitioner()->getNumberOfPartitions();i++){

		catalog->getTable(table_id)->getProjectoin(projection_index)->getPartitioner()->RegisterPartition(i,0);
	}

	catalog->saveCatalog();
}
*/

void Query(Catalog *catalog, Node *node, ResultSet *&result_set, bool& result_flag, string &error_msg, string &info, const bool local_mode) {
	if (!semantic_analysis(node,false))
	{
		error_msg="semantic analysis error";
		result_flag=false;
		result_set = NULL;
		return;
	}
	output(node,0);
	preprocess(node);
	Query_stmt *querynode=(Query_stmt *)node;
	if(querynode->from_list!=NULL)
		int fg=solve_join_condition(querynode->from_list);
	if(querynode->where_list!=NULL)
	{
		struct Where_list * curt=(struct Where_list *)(querynode->where_list);
		struct Node *cur=(struct Node *)(curt->next);
		departwc(cur,querynode->from_list);
	}
#ifdef SQL_Parser
	output(node,0);
#endif

	LogicalOperator* plan=parsetree2logicalplan(node);

	LogicalOperator* root=NULL;
	if(querynode->limit_list!=NULL)
	{
		Limit_expr *lexpr=(Limit_expr *)querynode->limit_list;
		if(lexpr->offset==NULL)
		{
			root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::kResultCollector,LimitConstraint(atoi(((Expr *)lexpr->row_count)->data)));
		}
		else
		{
			root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::kResultCollector,LimitConstraint(atoi(((Expr *)lexpr->row_count)->data),atoi(((Expr *)lexpr->offset)->data)));
		}
	}
	else
	{
		root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::kResultCollector);
	}

#ifdef SQL_Parser
	root->Print(0);
#endif

	PhysicalOperatorBase* physical_iterator_tree=root->GetPhysicalPlan(64*1024);
	//					puts("+++++++++++++++++++++begin time++++++++++++++++");
	unsigned long long start=curtick();
	physical_iterator_tree->Print();

	physical_iterator_tree->Open();

	while(physical_iterator_tree->Next(0));
	physical_iterator_tree->Close();
	//					printf("++++++++++++++++Q1: execution time: %4.4f second.++++++++++++++\n",getSecond(start));
	result_set=physical_iterator_tree->GetResultSet();
	cout<<"execute "<<result_set->query_time_<<" s"<<endl;
	result_flag=true;

	if (local_mode){
		result_set->print();
		delete physical_iterator_tree;
		delete root;
		delete result_set;
	}
	return;
}
/*

void Query(Catalog *catalog, Node *node) {
	SQLParse_log("this is query stmt!!!!!!!!!!!!!!!!!!");
	if (!semantic_analysis(node,false))//---3.22fzh---
	{
		SQLParse_elog("semantic_analysis error");
		//					assert(false);
		return;
	}
	preprocess(node);
	//#ifdef SQL_Parser
	//				output(node,0);
	//#endif
	Query_stmt *querynode=(Query_stmt *)node;
	if(querynode->from_list!=NULL)
		int fg=solve_join_condition(querynode->from_list);
	if(querynode->where_list!=NULL)
	{
		struct Where_list * curt=(struct Where_list *)(querynode->where_list);
		struct Node *cur=(struct Node *)(curt->next);
		departwc(cur,querynode->from_list);
	}
#ifdef SQL_Parser
	output(node,0);
#endif
	LogicalOperator* plan=parsetree2logicalplan(node);//现在由于没有投影，所以只把from_list传输进去。因此在完善之后，需要在parsetree2logicalplan()中
	//进行判断，对于不同的语句，比如select,update等选择不同的操作。

	LogicalOperator* root=NULL;
	if(querynode->limit_list!=NULL)
	{
		Limit_expr *lexpr=(Limit_expr *)querynode->limit_list;
		if(lexpr->offset==NULL)
		{
			root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::RESULTCOLLECTOR,LimitConstraint(atoi(((Expr *)lexpr->row_count)->data)));
		}
		else
		{
			root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::RESULTCOLLECTOR,LimitConstraint(atoi(((Expr *)lexpr->row_count)->data),atoi(((Expr *)lexpr->offset)->data)));
		}
	}
	else
	{
		root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::RESULTCOLLECTOR);
	}
	#ifdef SQL_Parser
	root->print(0);
	cout<<"performance is ok!the data will come in,please enter any char to continue!!"<<endl;
	getchar();
	getchar();
	#endif

	BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024);
	#ifdef SQL_Parser
	cout<<"~~~~~~~~~physical plan~~~~~~~~~~~~~~"<<endl;
	physical_iterator_tree->print();
	cout<<"~~~~~~~~~physical plan~~~~~~~~~~~~~~"<<endl;
	puts("+++++++++++++++++++++begin time++++++++++++++++");
	#endif
	unsigned long long start=curtick();
	physical_iterator_tree->open();
	while(physical_iterator_tree->next(0));
	physical_iterator_tree->close();

	ResultSet* result_set=physical_iterator_tree->getResultSet();
	result_set->print();

	delete physical_iterator_tree;
	delete root;
	delete result_set;
	//				printf("++++++++++++++++Q1: execution time: %4.4f second.++++++++++++++\n",getSecond(start));
}
*/

void LoadData(Catalog *catalog, Node *node, ResultSet *&result_set, bool &result_flag, string &error_msg, string &info) {
	Loadtable_stmt *new_node = (Loadtable_stmt*)node;

	string table_name(new_node->table_name);
	TableDescriptor *table = catalog->getTable(table_name);

	// 2014-4-17---check the exist of table---by Yu
	if(table == NULL)
	{
		error_msg="the table "+table_name +" does not exist during loading!";
		result_flag=false;
		result_set = NULL;
		return;
	}
	string column_separator(new_node->column_separator);
	string tuple_separator(new_node->tuple_separator);
	//			printf("wef:%s\n",new_node->tuple_separator);
	Expr_list *path_node = (Expr_list*)new_node->path;

	ASTParserLogging::log("load file\'s name:");
	vector<string> path_names;	// save the name of files which should be loaded
	//for test: the path name is:	/home/imdb/data/tpc-h/part.tbl
	while(path_node)
	{
		Expr *data = (Expr*)path_node->data;
		ASTParserLogging::log("%s",data->data);
		path_names.push_back(string(data->data));
		path_node = (Expr_list*)path_node->next;
	}

	// split sign should be considered carefully, in case of it may be "||" or "###"
	ASTParserLogging::log("The separator are :%c,%c, The sample is %lf, mode is %d\n",
			column_separator[0], tuple_separator[0], new_node->sample, new_node->mode);
	HdfsLoader *loader = new HdfsLoader(column_separator[0], tuple_separator[0], path_names, table, (open_flag)new_node->mode);
	loader->load(new_node->sample);

	result_flag=true;
	result_set = NULL;
	info = "load data successfully";
	result_set=NULL;

	catalog->saveCatalog();
	//	catalog->restoreCatalog();// commented by li to solve the dirty read after insert
}
/*

void LoadData(Catalog *catalog, Node *node) {
	Loadtable_stmt *new_node = (Loadtable_stmt*)node;

	string table_name(new_node->table_name);
	TableDescriptor *table = catalog->getTable(table_name);

	// 2014-4-17---check the exist of table---by Yu
	if(table == NULL)
	{
		ASTParserLogging::elog("the table %s does not exist!", table_name.c_str());
		return;
	}
	string column_separator(new_node->column_separator);
	string tuple_separator(new_node->tuple_separator);
	//				printf("wef:%s\n",new_node->tuple_separator);
	Expr_list *path_node = (Expr_list*)new_node->path;

	ASTParserLogging::log("load file\'s name:");
	vector<string> path_names;	// save the name of files which should be loaded
	//for test: the path name is:	/home/claims/data/tpc-h/part.tbl
	while(path_node)
	{
		Expr *data = (Expr*)path_node->data;
		ASTParserLogging::log("%s",data->data);
		path_names.push_back(string(data->data));
		path_node = (Expr_list*)path_node->next;
	}

	// split sign should be considered carefully, in case of it may be "||" or "###"
	ASTParserLogging::log("The separator are :%c,%c", column_separator[0], tuple_separator[0]);
	HdfsLoader *loader = new HdfsLoader(column_separator[0], tuple_separator[0], path_names, table);
	loader->load();
	catalog->saveCatalog();
}
*/

void InsertData(Catalog *catalog, Node *node, ResultSet *&result_set, bool &result_flag, string &error_msg, string &info) {
	bool has_warning = false;
	bool is_correct = true;
	bool is_all_col = false;
	Insert_stmt *insert_stmt = (Insert_stmt *)node;

	string table_name(insert_stmt->tablename);
	TableDescriptor *table = Environment::getInstance()->getCatalog()->getTable(table_name);
	if(table == NULL)
	{
		//				ASTParserLogging::elog("The table %s does not exist!", table_name.c_str());
		error_msg = "The table "+table_name+" does not exist!";
		result_flag = false;
		result_set = NULL;
		return;
	}

	unsigned col_count = 0;
	Columns *col = (Columns *)insert_stmt->col_list;
	if (col == NULL) {	// insert all columns
		is_all_col = true;
	}
	else {	// get insert column count
		++col_count;
		while(col = (Columns *)col->next) ++col_count;
	}

	Insert_val_list *insert_value_list = (Insert_val_list*)insert_stmt->insert_val_list;
	if (insert_value_list == NULL) {
		//				ASTParserLogging::elog("No value!");
		error_msg = "No value!";
		result_flag = false;
		result_set = NULL;
		return;
	}

	ostringstream ostr;
	int changed_row_num = 0;
	while(insert_value_list)	// 循环获得 （……），（……），（……）中的每一个（……）
	{
		// make sure: the insert column count = insert value count = used column count = used value count

		// init
		Insert_vals *insert_value = (Insert_vals *)insert_value_list->insert_vals;
		col = (Columns *)insert_stmt->col_list;

		if (is_all_col)	// insert all columns
		{
			// by scdong: Claims adds a default row_id attribute for all tables which is attribute(0), when inserting tuples we should begin to construct the string_tuple from the second attribute.
			for(unsigned int position = 1; position < table->getNumberOfAttribute(); position++)
			{
				// check value count
				if (insert_value == NULL)
				{
					//							ASTParserLogging::elog("Value count is too few");
					is_correct = false;
					error_msg = "Value count is too few";
					result_flag = false;
					result_set = NULL;
					break;
				}

				// insert value to ostringstream and if has warning return 1;	look out the order!
				has_warning = InsertValueToStream(insert_value, table, position, ostr) || has_warning;

				// move back
				insert_value = (Insert_vals*)insert_value->next;
				ostr<<"|";
			}

			if (!is_correct) break;

			// check insert value count
			if (insert_value)
			{
				//						ASTParserLogging::elog("Value count is too many");
				error_msg = "Value count is too many";
				result_flag = false;
				result_set = NULL;
				is_correct = false;
				break;
			}
		}
		else	//insert part of columns
		{
			// get insert value count and check whether it match column count
			unsigned insert_value_count = 0;
			while (insert_value)
			{
				++insert_value_count;
				insert_value = (Insert_vals*)insert_value->next;
			}
			if (insert_value_count != col_count)
			{
				//						ASTParserLogging::elog("Column count doesn't match value count");
				error_msg = "Column count doesn't match value count";
				result_flag = false;
				result_set = NULL;
				is_correct = false;
				break;
			}
			unsigned int used_col_count = 0;

			// by scdong: Claims adds a default row_id attribute for all tables which is attribute(0), when inserting tuples we should begin to construct the string_tuple from the second attribute.
			for(unsigned int position = 1; position < table->getNumberOfAttribute(); position++)
			{
				// find the matched column and value by name
				col = (Columns*)insert_stmt->col_list;
				Insert_vals *insert_value = (Insert_vals *)insert_value_list->insert_vals;

				// take attention that attrName is tablename.colname
				while (col && (table->getAttribute(position).attrName).compare(table->getTableName() +"."+ col->parameter1))
				{
					col = (Columns*)col->next;
					insert_value = (Insert_vals*)insert_value->next;
				}

				// if find
				// the column count is proved to match the insert value count, so column exist, then insert_value exist
				if (col && insert_value)
				{
					++used_col_count;

					// insert value to ostringstream and if has warning return 1; look out the order!
					has_warning = InsertValueToStream(insert_value, table, position, ostr) || has_warning;
				}

				ostr<<"|";
			}//end for

			// check if every insert column is existed
			if (used_col_count != col_count)
			{
				//						ASTParserLogging::elog("Some columns don't exist");
				error_msg = "Some columns don't exist";
				result_flag = false;
				result_set = NULL;
				is_correct = false;
				break;
			}
		}//end else

		if (!is_correct) break;

		insert_value_list = (Insert_val_list*)insert_value_list->next;
		if(insert_value_list != NULL)
			ostr<<"\n";

		++changed_row_num;
	}// end while

	if (!is_correct) return;
	if (has_warning) ASTParserLogging::log("[WARNING]: The type is not matched!\n");
	ASTParserLogging::log("the insert content is \n%s\n",ostr.str().c_str());

	HdfsLoader* Hl = new HdfsLoader(table);
	string tmp = ostr.str();
	Hl->append(ostr.str());

	catalog->saveCatalog();
	//			catalog->restoreCatalog(); // commented by li to solve the dirty read after insert

	result_flag=true;
	ostr.clear();
	ostr.str("");
	ostr<<"insert data successfully. " <<changed_row_num <<" rows changed.";
	info = ostr.str();
	result_set=NULL;
}
/*

void InsertData(Catalog *catalog, Node *node) {

	bool has_warning = false;
	bool is_all_col = false;
	Insert_stmt *insert_stmt = (Insert_stmt *)node;

	string table_name(insert_stmt->tablename);
	TableDescriptor *table = Environment::getInstance()->getCatalog()->getTable(table_name);
	if(table == NULL)
	{
		ASTParserLogging::elog("The table %s does not exist!", table_name.c_str());
		return;
	}

	unsigned col_count = 0;
	Columns *col = (Columns *)insert_stmt->col_list;
	if (col == NULL) {	// insert all columns
		is_all_col = true;
	}
	else {	// get insert column count
		++col_count;
		while(col = (Columns *)col->next) ++col_count;
	}

	Insert_val_list *insert_value_list = (Insert_val_list*)insert_stmt->insert_val_list;
	if (insert_value_list == NULL) {
		ASTParserLogging::elog("No value!");
		return;
	}

	ostringstream ostr;
	while(insert_value_list)	// 循环获得 （……），（……），（……）中的每一个（……）
	{
		// make sure: the insert column count = insert value count = used column count = used value count

		// init
		Insert_vals *insert_value = (Insert_vals *)insert_value_list->insert_vals;
		col = (Columns *)insert_stmt->col_list;

		if (is_all_col)	// insert all columns
		{
			// by scdong: Claims adds a default row_id attribute for all tables which is attribute(0), when inserting tuples we should begin to construct the string_tuple from the second attribute.
			for(unsigned int position = 1; position < table->getNumberOfAttribute(); position++)
			{
				// check value count
				if (insert_value == NULL)
				{
					ASTParserLogging::elog("Value count is too few");
					return;
				}

				// insert value to ostringstream and if has warning return 1;	look out the order!
				has_warning = InsertValueToStream(insert_value, table, position, ostr) || has_warning;

				// move back
				insert_value = (Insert_vals*)insert_value->next;
				ostr<<"|";
			}

			// check insert value count
			if (insert_value)
			{
				ASTParserLogging::elog("Value count is too many");
				return;
			}
		}
		else	//insert part of columns
		{	//TODO:必须确认 不能为空的列有数据插入
			// get insert value count and check whether it match column count
			unsigned insert_value_count = 0;
			while (insert_value)
			{
				++insert_value_count;
				insert_value = (Insert_vals*)insert_value->next;
			}
			if (insert_value_count != col_count)
			{
				ASTParserLogging::elog("Column count doesn't match value count");
				return;
			}

			unsigned int used_col_count = 0;

			// by scdong: Claims adds a default row_id attribute for all tables which is attribute(0), when inserting tuples we should begin to construct the string_tuple from the second attribute.
			for(unsigned int position = 1; position < table->getNumberOfAttribute(); position++)
			{
				// find the matched column and value by name
				col = (Columns*)insert_stmt->col_list;
				Insert_vals *insert_value = (Insert_vals *)insert_value_list->insert_vals;

				// take attention that attrName is tablename.colname
				while (col && (table->getAttribute(position).attrName).compare(table->getTableName() +"."+ col->parameter1))
				{
					col = (Columns*)col->next;
					insert_value = (Insert_vals*)insert_value->next;
				}

				// if find
				// the column count is proved to match the insert value count, so column exist, then insert_value exist
				if (col && insert_value)
				{
					++used_col_count;
					// insert value to ostringstream and if has warning return 1; look out the order!
					has_warning = InsertValueToStream(insert_value, table, position, ostr) || has_warning;
				}

				ostr<<"|";
			}//end for

			// check if every insert column is existed
			if (used_col_count != col_count)
			{
				ASTParserLogging::elog("Some columns don't exist");
				return;
			}
		}//end else

		insert_value_list = (Insert_val_list*)insert_value_list->next;
		if(insert_value_list != NULL)
			ostr<<"\n";
	}// end while

	if (has_warning)
		ASTParserLogging::log("[WARNING]: The type is not matched!\n");
	ASTParserLogging::log("the insert content is \n%s\n",ostr.str().c_str());

	HdfsLoader* Hl = new HdfsLoader(table);
	string tmp = ostr.str().c_str();
	Hl->append(ostr.str().c_str());

	catalog->saveCatalog();
}
*/

void ShowTable(Catalog *catalog, Node *node, ResultSet *&result_set, bool &result_flag, string &error_msg, string &info) {
	Show_stmt *show_stmt = (Show_stmt *)node;
	ostringstream ostr;
	switch(show_stmt->show_type)
	{
	case 1:
	{
		ostr<<"TABLES:"<<endl;
		for (unsigned i = 0; i < catalog->getTableCount(); ++i) {
			ostr<<catalog->getTable(i)->getTableName()<<endl;
		}
		info = ostr.str();
		result_flag=true;
		result_set=NULL;
	}
	break;
	default:{
		ASTParserLogging::elog("Sorry, not supported now!");
		error_msg = "not supported now!";
		result_flag = false;
		result_set = NULL;
	}
	}
}
/*

void ShowTable(Catalog *catalog, Node *node) {
	Show_stmt *show_stmt = (Show_stmt *)node;
	switch(show_stmt->show_type)
	{
	case 1:
	{
		cout<<"Tables:"<<endl;
		for (unsigned i = 0; i < catalog->getTableCount(); ++i) {
			cout<<catalog->getTable(i)->getTableName()<<endl;
		}
	}
	break;
	default:{
		ASTParserLogging::elog("Sorry, not supported now!");
	}
	}
}
*/

void DropTable(Catalog *catalog, Node *node, ResultSet *&result_set, bool&result_flag, string &error_msg, string &info) {
	assert(node != NULL);
	Droptable_stmt *drop_stmt = (Droptable_stmt*)node;
	Tablelist *table_list = (Tablelist *)(drop_stmt->table_list);
	//TODO
}

#endif
