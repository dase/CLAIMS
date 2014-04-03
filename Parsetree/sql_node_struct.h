/*
 *
 * 14.02.15 已有select语句和create语句，包括select语句连接系统所需的函数
 *
 */

#ifndef NODES_H_
#define NODES_H_
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;


extern "C" int yylex();
extern "C" int yyparse();
extern "C" void emit(char *s, ...);
extern "C" void yyerror(const char *s, ...);
extern int yylineno;


enum nodetype
{
	t_none,
 	t_name,t_uservar,t_name_name,t_stringval,t_intnum,t_approxnum,t_bool,
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
	t_create_database_stmt, t_create_table_stmt, t_create_col_list, t_create_def, t_create_projection_stmt,// 2014-2-24---新增t_create_projection_stmt类型---by余楷
	t_alter_database_stmt, t_alter_table_stmt, t_alter_def,
	t_create_select_stmt,t_column_atts, t_opt_csc,
	t_datatype,t_length,t_enum_list
};

union dataval
{
	int int_val;
	char * string_val;
	double double_val;
	bool bool_val;
};

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
	dataval data;
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
	char * sign,*parameter;
	int cmp;
	Node *lnext,*rnext;
};

struct Expr_func //函数表达式，将is null/exist等判断抽象成函数
{
	nodetype type;
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

struct Column_atts
{
	nodetype type;
	int datatype;	//若 datatype & 0111100 != 0，则还有内容存在num1或num2或s中
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
	char * name1;
	int index_type;
	char * name2;
	Node * index_col_name;
};

struct Index_col_name
{
	nodetype type;
	char * name;
	int length;
	int asc_desc;
	Node * next;
};

struct Dropindex_stmt
{
	nodetype type;
	char * name1;
	char * name2;
};

struct Dropdatabase_stmt
{
	nodetype type;
	int droptype;
	char * name;
};

struct Droptable_stmt
{
	nodetype type;
	int istemp;
	int opt_rc;
	Node * tablelist;
};

struct Tablelist
{
	nodetype type;
	char * name1;
	char * name2;
	Node * next;
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

//////////////////////////未完待续////////////////////////////////////////


/******************DDL语句结束********************************************
*************************************************************************/

/******************函数声明***********************************************
*************************************************************************/

void FreeAllNode();		// 2014-3-6---增加释放所有节点函数---by余楷

void output(Node * n, int floor);

void outputSpace(int f);

struct Node *newStmt(nodetype t, Node *list, Node *newNode);	// 2014-3-4---增加新建语句列表函数---by余楷

struct Node* newExpr(nodetype t, dataval d);

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

/*******　DDL语句  *****/
struct Node * newCreateDatabaseStmt(nodetype type, int create_type, int check, char * name);

struct Node * newCreateTableStmt( nodetype type, int create_type, int check, char * name1, char * name2, Node * list, Node * select_stmt);

struct Node * newCreateColList(nodetype type, Node * data, Node * next);

struct Node * newCreateDef(nodetype type, int deftype, char * name, Node * datatype, Node * col_atts, Node * col_list);	// 2-18---增加name属性---by余楷

struct Node * newColumnAtts(nodetype type, int datatype, int num1, double num2, char *s, Node * col_list);

// 2014-2-24---增加该结构体---by余楷
struct Node *newCreateProjectionStmt(nodetype type, char *tablename, Node *column_list, int partition_num, char *partition_attribute_name);

struct Node * newAlterDatabaseStmt(nodetype type, int createtype, char * name, Node* opt);

struct Node * newOptCsc(nodetype type, int datatype, char * s1, char * s2);

struct Node * newAlterTableStmt(nodetype type, int isignore, char * name, Node * parameter);

struct Node * newAlterDef (nodetype type, int altertype, char * name1, char * name2, int datatype, int coltype, Node * parameter, Node * next);

struct Node * newCreateSelectStmt(nodetype type, int ignore_replace, int temporary, Node * select_stmt);

struct Node * newDatatype (nodetype type, int datatype, Node* length, int opt_uz, Node * opt_csc, int isbinary, Node * enum_list);

struct Node * newLength (nodetype type, int data1, int data2);

struct Node * newEnumList (nodetype type, char * s, Node * next);

struct Node * newDoStmt(nodetype type, Node * data);

struct Node * newTruncateStmt(nodetype type, char * name);

//////////////////////////////////////////////////////////////////////////////////////////


int judgepos(struct Node *args,set<string>*st);

void setwcposition(struct Node *wccur,set<string>*st,struct Node *flcur);

void getwctable(struct Node *cur,set<string>*st);

void solvewc(struct Node * wcexpr,struct Node *fromlist);

void departwc(struct Node *  wherecondition,struct Node * fromlist);

bool semantic_analysis(Node *parsetree,bool issubquery);//---3.22fzh--

int solve_join_condition(Node * fromnode);//---3.22fzh---
#endif /* NODES_H_ */
