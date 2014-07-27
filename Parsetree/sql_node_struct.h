/*
 *
 *
 */

#ifndef NODES_H_
#define NODES_H_
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <set>
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" void emit(char *s, ...);
extern "C" void yyerror(const char *s, ...);
extern int yylineno;

/*
 * enum data_type{
 * 	t_smallInt,t_int,t_u_long,t_float,t_double,t_string, t_date, t_time, t_datetime, t_decimal, t_boolean, t_u_smallInt};
 * from data_type.h
 */

enum nodetype
{
	t_none,
 	t_name,t_uservar,t_name_name,t_stringval,t_intnum,t_approxnum,t_bool,   t_dateval,t_timeval,t_datatimeval,//---6.21fzh---
	t_table,t_column,t_join,t_subquery,t_condition,
	t_query_stmt,t_stmt,t_do_stmt,t_truncate_stmt,
	t_select_list,t_select_opts,t_select_expr,t_expr_list_header,
	t_expr,t_expr_list,t_expr_cal,t_expr_func,t_expr_relation,
	t_from_list,t_from_expr,t_from_table,
	t_where_list,
	t_groupby_list,t_groupby_expr,
	t_having_list,
	t_orderby_list,t_orderby_expr,
	t_limit_list,t_limit_expr,
	t_insert_stmt,	t_insert_val_list, t_insert_vals, t_insert_assign, // 2014-4-17---增加---by Yu
	t_create_database_stmt, t_create_table_stmt, t_create_col_list, t_create_def, t_create_projection_stmt,// 2014-2-24---新增t_create_projection_stmt类型---by余楷
	t_alter_database_stmt, t_alter_table_stmt, t_alter_def,
	t_create_select_stmt,t_column_atts, t_opt_csc,
	t_datatype,t_length,t_enum_list,
	t_create_index_stmt,	t_index_col_list,	t_drop_index,	// 2014-3-24---增加---by Yu
	t_drop_database_stmt,t_drop_table_stmt, t_table_list,	// 2014-3-24---增加---by Yu
	t_load_table_stmt,	// 2014-3-24---add---by Yu
	t_show_stmt,	// 2014-5-4---add---by Yu
};

// 2014-4-14---delete union dataval, because only char* is need---by Yu

struct Node//基本节点
{
	nodetype type;
};

struct Stmt	//语句列表 2014-3-4---增加语句列表结构体---by余楷
{
	nodetype type;
	Node *data;
	Node *next;
	Node *last;	//指向列表的最后一个节点，即最后一个语句
};

struct Expr//常量表达式
{
	nodetype type;
	char *data;
	//Node *next;
};

struct Expr_list_header
{
	nodetype type;
	Node * header;
	Node * tail;
};

struct Expr_list
{
	nodetype type;
	Node * data;
	Node * next;
};

struct Columns//列
{
	nodetype type;
	char * parameter1;
	char *parameter2;
	Node * next;
};

struct Expr_cal//计算表达式,二元表达式
{
	nodetype type;
	char * str;//5.23---by fzh---
	char * sign,*parameter;
	int cmp;
	Node *lnext,*rnext;
};

struct Expr_func //函数表达式，将is null/exist等判断抽象成函数
{
	nodetype type;
	char * str;//5.23---by fzh---
	char * funname;
	Node *args;
	Node * parameter1,*parameter2;//函数中的参数列表，处理between...and.../case...when...then...end等
	Node *next;
};

struct Table //table
{
	nodetype type;
	char * dbname,*tablename,*astablename;
	int issubquery;
	Node *subquery;
	Node * whcdn;
};
struct Join //join
{
	nodetype type;
	int jointype;
	Node *lnext,*rnext,*condition;
};
struct Condition
{
	nodetype type;
	int conditiontype;
	Node * args;
};
struct Subquery //subquery
{
	nodetype type;
	char * querystring;
	Node *next;
};

/*****************select子句开始*********************************************************************************************************************/
struct Query_stmt
{
	nodetype type;
	char * querystring;
	int select_opts;
	Node *select_list;
	Node *from_list;
	Node *where_list;
	Node *groupby_list;
	Node *having_list;
	Node *orderby_list;
	Node *limit_list;
	Node *into_list;
};

/*
select 子句前面有些修饰符

select 子句中每个选项的类型如下：
1.column
2.函数
3.计算式
4.关系式
*/
struct Select_list//选项列表
{
	nodetype type;
	int isall;
	Node * args;
	Node *next;
};


struct Select_expr //选项表达式
{
	nodetype type;
	char *ascolname;
	Node * colname;
};

/*
from子句选项类型：
1.表.别名
2.子查询 .别名
3.join
*/
struct From_list
{
	nodetype type;
	Node * args;
	Node * next;
	Node * whcdn;
};

struct From_expr //from子句
{
	nodetype type;
	char * astablename;
	Node *next;
};

/*
 where子句类型
 1.逻辑表达式
 2.关系表达式
 用expr表示即可
 */

struct Where_list
{
	nodetype type;
	char * wherestring;
	Node *next;
};

/*
groupby子句
1.总体是否有with rollup?
2.每一项可能有asc/desc
3.选项可能是expr,如rand()

*/
struct Groupby_list
{
	nodetype type;
	char * groupbystring;
	Node *next;
	int with_rollup;
};

struct Groupby_expr
{
	nodetype type;
	Node *args;
	int sorttype;
	Node *next;
};

/*
having 子句同where子句
*/

struct Having_list
{
	nodetype type;
	char * havingstring;
	Node *next;
};

/*
orderby子句
{col_name | expr | position} [ASC | DESC] , ...
类似于groupby
*/

struct Orderby_list
{
	nodetype type;
	char * orderbystring;
	Node *next;
};

struct Orderby_expr
{
	nodetype type;
	Node *args;
	char * sorttype;
	Node *next;
};

/*
limit子句
1.num
2.num,num
*/
/*
struct Limit_list//没什么用，因为limit只有一项
{
	nodetype type;
	char * limitstring;
	Node *next;
};*/

struct Limit_expr
{
	nodetype type;
	Node * offset;
	Node *row_count;
};

// 2014-4-17---add ---by Yu
struct Insert_stmt
{
	nodetype type;
	int insert_opt;	// 2014-4-16---not used in this version ---by Yu
	char *tablename;
	Node *col_list;
	Node *insert_val_list;
	Node *insert_assign_list;	// 2014-4-16---not used in this version ---by Yu
	Node *insert_assign_list_from_set;	// 2014-4-17---not used in this version ---by Yu
	Node *select_stmt;	// 2014-4-17---not used in this version ---by Yu
};

// 2014-4-17---add ---by Yu
struct Insert_val_list
{
	nodetype type;
	Node *insert_vals;
	Node *next;
};

// 2014-4-17---add ---by Yu
struct Insert_vals
{
	nodetype type;
	int value_type;
	Node *expr;
	Node *next;
};

// 2014-4-17---add ---by Yu
struct Insert_assign_list
{
	nodetype type;
	char *col_name;
	int value_type;
	Node *expr;
	Node *next;
};


/*****************select子句结束*********************************************************************************************************************/

/******************DDL语句开始***************************************
******************************************************************/


struct Create_database_stmt
{
	nodetype type;
	int create_type;
	int check;
	char * name;
};

struct Create_table_stmt
{
	nodetype type;
	int create_type;	//opt_temporary
	int check;			//opt_if_not_exists
	char * name1;
	char * name2;
	Node * list;
	Node * select_stmt;
};

struct Create_col_list
{
	nodetype type;
	Node * data;
	Node * next;
};

struct Create_def
{
	nodetype type;
	int deftype;
	char * name;	// 2-18---增加name属性---by余楷
	Node * datatype;
	Node * col_atts;
	Node * col_list;
};

// 2014-4-14---change the struct---by Yu
struct Column_atts
{
	nodetype type;
	int datatype;	//若 datatype & 0111100 != 0，则还有内容存在data中
	int num1;
	double num2;
	char *s;
	Node * col_list;
};

struct Create_select_stmt
{
	nodetype type;
	int ignore_replace;
	int temporary;
	Node * select_stmt;
};

// 2014-2-24---增加该结构体---by余楷
struct Create_projection_stmt
{
	nodetype type;
	char *tablename;
	Node *column_list;
	int partition_num;
	char *partition_attribute_name;
};

struct Do_stmt
{
	nodetype type;
	Node * data;
};

struct Truncate_stmt
{
	nodetype type;
	char * name;
};

struct Alterdatabase_stmt
{
	nodetype type;
	int createtype;
	char * name;
	Node * opt;
};

struct Opt_csc
{
	nodetype type;
	int datatype;
	char * s1;
	char * s2;
};

struct Altertable_stmt
{
	nodetype type;
	int isignore;
	char * name;
	Node * parameter;
};

struct Alter_def
{
	nodetype type;
	int altertype;
	char * name1;
	char * name2;
	int datatype;
	int coltype;
	Node * parameter;	//create_col_list or column_list
	Node * next;
};

struct Createindex_stmt
{
	nodetype type;
	int index_att;
	char * index_name;
	int index_type;
	char * table_name;
	Node * index_col_name;
};

struct Index_col_list
{
	nodetype type;
	char * name;
	Node* length;
	int asc_desc;
	Node * next;
};

struct Dropindex_stmt
{
	nodetype type;
	char* index_name;
	char* table_name;
};

struct Dropdatabase_stmt	// 2014-3-24---增加---by Yu
{
	nodetype type;
	int droptype;
	int check;	// actually, it has no mean
	char * name;
};

struct Droptable_stmt	// 2014-3-24---增加---by Yu
{
	nodetype type;
	int is_temp;
	int is_check;
	int option_rc;
	Node * table_list;
};

struct Tablelist	// 2014-3-24---增加---by Yu
{
	nodetype type;
	char * name1;
	char * name2;
	Node * next;
};

struct Loadtable_stmt{	// 2014-4-1---修改---by Yu
	nodetype type;
	char *table_name;
	Node *path;
	char *column_separator;
	char *tuple_separator;
};

struct Rename_stmt
{
	nodetype type;
	char * oldname1;
	char * oldname2;
	char * newname1;
	char * newname2;
	Node * next;
};

struct Describe_stmt
{
	nodetype type;
	char * name;
};

struct Use_stmt
{
	nodetype type;
	char * name;
};

struct Starttrans_stmt
{
	nodetype type;
};

struct Commit_stmt
{
	nodetype type;
	int chain;
	int release;
};

struct Rollback_stmt
{
	nodetype type;
	int chain;
	int release;
};

struct Setauto_stmt
{
	nodetype type;
	int isautocommit;
};

struct Savepoint_stmt
{
	nodetype type;
	char * name;
};

struct Rollbacktosavep_stmt
{
	nodetype type;
	char * name;
};

struct Datatype
{
	nodetype type;
	int datatype;
	Node * length;
	int opt_uz;
	Node * opt_csc;
	int isbinary;
	Node * enum_list;
};

struct Length
{
	nodetype type;
	int data1;
	int data2;
};

struct enum_list
{
	nodetype type;
	char * s;
	Node * next;
};

struct Show_stmt	//2014-5-4---add ---by Yu
{
	nodetype type;
	bool full;
	int show_type;
	char *database_name;
	char *like_string;
};


//////////////////////////未完待续////////////////////////////////////////


/******************DDL语句结束********************************************
*************************************************************************/

/******************函数声明***********************************************
*************************************************************************/


struct Node *newStmt(nodetype t, Node *list, Node *newNode);	// 2014-3-4---增加新建语句列表函数---by余楷

struct Node* newExpr(nodetype t, char *data);

struct Node * newExprList(nodetype t, Node * data, Node * next);

struct Node *newExprlistheader(nodetype type,Node * header,Node * tail);

struct Node * newColumn(nodetype t, char * parameter1, char *parameter2, Node * next);

struct Node * newExprCal(nodetype type, char * sign, char *parameter,
	int cmp, Node *lnext, Node *rnext);

struct Node * newExprFunc(nodetype type, char * funname, Node *args,
	Node * parameter1, Node *parameter2, Node *next);

struct Node * newTable(nodetype type, char * dbname, char *tablename,
	char *astablename, int issubquery, Node *subquery);

struct Node * newJoin(nodetype type, int jointype,Node *lnext, Node *rnext, Node *condition);

struct Node * newCondition(nodetype type, int conditiontype, Node * args);

struct Node * newSubquery(nodetype type, char * querystring, Node *next);

struct Node * newQueryStmt(nodetype t, char * querystring, int select_opts,
	Node *select_list, Node *from_list, Node *where_list, Node *groupby_list,
	Node *having_list, Node *orderby_list, Node *limit_list, Node *into_list);

struct Node * newSelectList(nodetype type,int isall,Node * args,Node *next);

struct Node * newSelectExpr(nodetype type, char *ascolname, Node * colname);

struct Node *  newFromList(nodetype type, Node * args, Node *next);

struct Node * newFromExpr( nodetype type, char * astablename, Node *next);

struct Node * newWhereList(nodetype type, char * wherestring, Node *next);

struct Node * newGroupbyList(nodetype type, char * groupbystring, Node *next, int with_rollup);

struct Node * newGroupbyExpr(nodetype type, Node *args, int sorttype, Node *next);

struct Node * newHavingList(nodetype type, char * havingstring, Node *next);

struct Node * newOrderbyList(nodetype type,char * orderbystring, Node *next);

struct Node * newOrderbyExpr(nodetype type, Node *args, char * sorttype, Node *next);

struct Node * newLimitExpr(nodetype type, Node * offset, Node * row_count);


//2014-4-17---add---by Yu
Node* newInsertStmt(int insert_opt, char *tablename, Node *col_list,
		Node *insert_val_list, Node *insert_assign_list, Node *insert_assign_list_from_set, Node *select_stmt);

//2014-4-17---add---by Yu
Node* newInsertValueList(Node *insert_vals, Node *next);

//2014-4-17---add---by Yu
Node* newInsertVals(int value_type, Node *expr, Node *next);

//2014-4-17---add---by Yu
Node* newInsertAssignList(char *col_name, int value_type, Node *expr, Node *next);

/*******　DDL语句  *****/
struct Node * newCreateDatabaseStmt(nodetype type, int create_type, int check, char * name);

struct Node * newCreateTableStmt( nodetype type, int create_type,
		int check, char * name1, char * name2, Node * list, Node * select_stmt);

struct Node * newCreateColList(nodetype type, Node * data, Node * next);

struct Node * newCreateDef(nodetype type, int deftype, char * name,
		Node * datatype, Node * col_atts, Node * col_list);	// 2-18---增加name属性---by余楷

struct Node * newColumnAtts(nodetype type, int datatype,
		int num1, double num2, char *s, Node * col_list);

// 2014-2-24---增加该结构体---by余楷
struct Node *newCreateProjectionStmt(nodetype type, char *tablename,
		Node *column_list, int partition_num, char *partition_attribute_name);

struct Node * newAlterDatabaseStmt(nodetype type, int createtype, char * name, Node* opt);

struct Node * newOptCsc(nodetype type, int datatype, char * s1, char * s2);

struct Node * newAlterTableStmt(nodetype type, int isignore, char * name, Node * parameter);

struct Node * newAlterDef (nodetype type, int altertype, char * name1,
		char * name2, int datatype, int coltype, Node * parameter, Node * next);

struct Node * newCreateSelectStmt(nodetype type, int ignore_replace, int temporary, Node * select_stmt);

struct Node * newDatatype (nodetype type, int datatype, Node* length,
		int opt_uz, Node * opt_csc, int isbinary, Node * enum_list);

struct Node * newLength (nodetype type, int data1, int data2);

struct Node * newEnumList (nodetype type, char * s, Node * next);

struct Node * newDoStmt(nodetype type, Node * data);

struct Node * newTruncateStmt(nodetype type, char * name);

// 2014-3-24---增加---by Yu
struct Node* newIndexColList(nodetype type, char * name, Node* length, int asc_desc, Node * next);

// 2014-3-24---增加---by Yu
struct Node* newCreateIndex(nodetype type, int index_att, char* index_name, int index_type, char* table_name, Node* index_col_name);

// 2014-3-24---增加---by Yu
struct Node* newDropIndex(nodetype type, char* index_name, char* table_name);

// 2014-3-24---增加---by Yu
struct Node* newDropDatabase(nodetype type, int drop_type, int check, char* name);

// 2014-3-24---增加---by Yu
struct Node* newDropTable(nodetype type, int is_temp, int is_check, int opt_rc, Node * table_list);

// 2014-3-24---增加---by Yu
struct Node* newTableList(nodetype type, char * name1, char * name2, Node * next);

// 2014-3-27---增加---by Yu
struct Node* newLoadTable(nodetype type, char *table_name, Node *path, char *column_separator, char *tuple_separator);

Node *newShowStmt(int show_type, bool full, char *database_name, char *like_string);	//2014-5-4---add ---by Yu

//////////////////////////////////////////////////////////////////////////////////////////


inline void insertNodePointer(Node * a);	// 2014-3-7---增加将节点指针存入指针数组的函数---by余楷

void FreeAllNode();		// 2014-3-6---增加释放所有节点函数---by余楷

void output(Node * n, int floor);

void outputSpace(int f);

void GetCorrectString(char *dest, const char *src);	// 2014-4-2---增加该函数---by Yu


int judgepos(struct Node *args,set<string>*st);

void setwcposition(struct Node *wccur,set<string>*st,struct Node *flcur);

void getwctable(struct Node *cur,set<string>*st);

void solvewc(struct Node * wcexpr,struct Node *fromlist);

void departwc(struct Node *  wherecondition,struct Node * fromlist);

bool semantic_analysis(Node *parsetree,bool issubquery);//---3.22fzh--

int solve_join_condition(Node * fromnode);//---3.22fzh---
void preprocess(Node *node);//---5.23fzh---
#endif /* NODES_H_ */
