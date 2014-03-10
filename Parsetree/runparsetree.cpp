/*
 * runparsetree.cpp
 *
 *  Created on: 2014年1月21日
 *      Author: imdb
 */
#ifndef __RUNPARSER__
#define __RUNPARSER__
#include "sql_node_struct.h"
//#include "../Environment.h"
extern "C" int yylex();
extern "C" int yyparse();
//extern "C" int yyclearin();
//extern "C" int yyerrok();
extern Node * parsetreeroot;
extern char globalInputText[10000];
extern int globalReadOffset;
extern int errorNumber;		// 2014-3-6---声明errorNumber变量---by余楷

static Node * getparsetreeroot()
{
	puts("SQL!!!!!");
	int charnum=0;
	globalReadOffset = 0;

	errorNumber = 0;	// 2014-3-6---初始化SQL解析错误个数---by余楷
	parsetreeroot = NULL;	// 2014-3-4---增加初始化语句---by余楷
	memset(globalInputText, 0, sizeof(globalInputText));	// 2014-3-4---增加初始化语句---by余楷
	cout<<"globalInputText:"<<globalInputText;

	while(1)
	{
		char c=getchar();
		globalInputText[charnum++]=c;
		if(c==';')
		{
			globalInputText[charnum++]='\0';
			getchar();
			break;
		}
	}

	// 2014-3-4---测试数据---by余楷
//	strcpy(globalInputText,
//			"select row_id from cj;"
//			"create table tt(num int not null, s int, ss int primary key, c int, vc varchar(15));"
//			"create projection on tt(num, s, c) partitioned on num;"
////			"select s from tt;"
//			"create projection on tt(ss, c) partitioned on ss;"
////			"select ss from tt;"
//			"create table t(num int, d double, f float);"
//			"create table tt(num int, d double, f float);"
//			"create table ttt(num int, d double, f float);"
//			"create projection on t(num, f, d) partitioned on num;"
//			"select f,d from t;"
//			);
	cout<<"globalInputText:"<<globalInputText<<endl;

//	yyclearin;
////	yychar = YYEMPTY;
//	yyerrok;
//	cleanSQLparse();

	if(!yyparse())	// 2014-3-4---注释冗余代码---by余楷
	{
//		printf("SQL parse worked\n");
//		if (parsetreeroot == NULL)
//			return NULL;

//		output(parsetreeroot, 1);
//		if(parsetreeroot->type==t_query_stmt)
//		{
//			Query_stmt *node=(Query_stmt *)parsetreeroot;
//			puts("select_stmt2>>>>>>>>");
//			if(node->where_list!=NULL)
//			{
//				struct Where_list * curt=(struct Where_list *)(node->where_list);
//				struct Node *cur=(struct Node *)(curt->next);
//				puts("wc2tb");
//				departwc(cur,node->from_list);
//				puts("partree complete!!!");
//			}
//		}
		return parsetreeroot;
	}
	else
	{
		printf("SQL parse failed\n");
		return NULL;
	}
	//return 0;
}

static Node * getparsetreeroot(const char *sql)
{
	puts("SQL!!!!!");
	int charnum=0;
	globalReadOffset = 0;

	errorNumber = 0;	// 2014-3-6---初始化SQL解析错误个数---by余楷
	parsetreeroot = NULL;	// 2014-3-4---增加初始化语句---by余楷
	memset(globalInputText, 0, sizeof(globalInputText));	// 2014-3-4---增加初始化语句---by余楷
	cout<<"globalInputText:"<<globalInputText;


	// 2014-3-4---测试数据---by余楷
	strcpy(globalInputText,sql );
	cout<<"globalInputText:"<<globalInputText<<endl;

	if(!yyparse())	// 2014-3-4---注释冗余代码---by余楷
	{
		return parsetreeroot;
	}
	else
	{
		printf("SQL parse failed\n");
		return NULL;
	}
	//return 0;
}

#endif
