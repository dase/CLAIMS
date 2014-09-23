/*
 * runparsetree.cpp
 *
 *  Created on: 2014年1月21日
 *      Author: imdb
 */
#ifndef __RUNPARSER__
#define __RUNPARSER__
#include <sys/time.h>
#include "sql_node_struct.h"
#include <algorithm>
#include <vector>
#include "../common/Logging.h"
//#include "../Environment.h"
extern "C" int yylex();
extern "C" int yyparse();
//extern "C" int yyclearin();
//extern "C" int yyerrok();
extern Node * parsetreeroot;
extern char globalInputText[10000];
extern int globalReadOffset;
extern int errorNumber;
extern timeval start_time;

extern vector<Node*> NodePointer;

static Node * getparsetreeroot()
{
	int charnum=0;
	globalReadOffset = 0;


	NodePointer.clear();
	errorNumber = 0;
	parsetreeroot = NULL;
	memset(globalInputText, 0, sizeof(globalInputText));
	printf("Claims>");
	while(1)
	{
		char c=getchar();
		globalInputText[charnum++]=c;
		if(c==';')
		{
			globalInputText[charnum++]='\0';
//			getchar();
			break;
		}
	}
	// 2014-3-4---测试数据---by余楷
//	strcpy(globalInputText,
//			"select row_id from cj;\n"
//			"create table tt(num int not null, s int, ss int primary key, c int, vc varchar(15));\n"
//			"create projection on tt(num, s, c) partitioned on num;\n"
//			"select s from tt;\n"
//			"create projection on tt(ss, c) partitioned on ss;\n"
//			"select ss from tt;\n"
//			"create table t(num int, d double, f float);\n"
//			"create table tt(num int, d double, f float);\n"
//			"create table ttt(num int, d double, f float);\n"
//			"create projection on t(num, f, d) partitioned on num;\n"
//			"select f,d from t;\n"
//			);
	SQLParse_log("globalInputText:\n%s\n",globalInputText);

//	yyclearin;
//	yychar = YYEMPTY;
//	yyerrok;
//	cleanSQLparse();

	gettimeofday(&start_time, NULL);//2014-5-4---add---by Yu

	if(!yyparse())
	{
		return parsetreeroot;
	}
	else
	{
		printf("SQL parse failed\n");
		return NULL;
	}
}

static Node * getparsetreeroot(const char *sql)
{
	assert(sql != NULL);
	ASTParserLogging::log("sql argument is %s", sql);
	int charnum=0;
	globalReadOffset = 0;

	NodePointer.clear();	// 2014-3-7---初始化---by余楷	// 2014-4-2---存放节点指针的数组改为vector---by Yu
	errorNumber = 0;	// 2014-3-6---初始化SQL解析错误个数---by余楷
	parsetreeroot = NULL;	// 2014-3-4---增加初始化语句---by余楷
	memset(globalInputText, 0, sizeof(globalInputText));	// 2014-3-4---增加初始化语句---by余楷
//	ASTParserLogging::log("befor copying, globalInputText: %s", globalInputText);

	// 2014-3-4---测试数据---by余楷
	strcpy(globalInputText,	sql);
	ASTParserLogging::log("after copying, globalInputText: %s", globalInputText);


	if(!yyparse())
	{
		return parsetreeroot;
	}
	else
	{
		printf("SQL parse failed\n");
		return NULL;
	}
}


#endif
