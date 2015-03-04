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
#include <string.h>
#include "sql.tab.hpp"
#include "sql.lex.h"
extern timeval start_time;
extern int yyparse (struct ParseResult* result);

static Node * getparsetreeroot()
{
	char InputStr[10000];
	puts("SQL!!!!!");
	int charnum=0;
    while(1)
    {
        char c=getchar();
        InputStr[charnum++]=c;
        if(c==';')
        {
            InputStr[charnum++]='\0';
            break;
        }
    }
    vector<Node *>allnode;
	struct ParseResult presult={NULL,NULL,InputStr,0,&allnode};
//reference to the example of "purecal"
	if(yylex_init_extra(&presult, &presult.yyscan_info_))
	{
    		perror("init alloc failed");
    		return NULL;
  	}
	YY_BUFFER_STATE bp;
	int len = strlen(InputStr);
	printf("%s  %d\n",InputStr,len);
	  //bp = yy_scan_string(buf, p->yyscan_info_);
	bp = yy_scan_bytes(InputStr, len, presult.yyscan_info_);
	yy_switch_to_buffer(bp,presult.yyscan_info_);
  	if(!yyparse(&presult))
  	{
		return presult.ast;
	}
  	else
  	{
   	 	printf("SQL parse failed\n");
   	 	return NULL;
	}
	yy_delete_buffer(bp, presult.yyscan_info_);
}
static Node * getparsetreeroot(struct ParseResult * presult)
{
	char InputStr[10000];
	puts("SQL!!!!!");
	int charnum=0;
    while(1)
    {
        char c=getchar();
        InputStr[charnum++]=c;
        if(c==';')
        {
            InputStr[charnum++]='\0';
            break;
        }
    }
    presult->sql_clause=InputStr;
//reference to the example of "purecal"
	if(yylex_init_extra(presult, &presult->yyscan_info_))
	{
    		perror("init alloc failed");
    		return NULL;
  	}
	YY_BUFFER_STATE bp;
	int len = strlen(InputStr);
	printf("%s  %d\n",InputStr,len);
	  //bp = yy_scan_string(buf, p->yyscan_info_);
	bp = yy_scan_bytes(InputStr, len, presult->yyscan_info_);
	yy_switch_to_buffer(bp,presult->yyscan_info_);
  	if(!yyparse(presult))
  	{
		return presult->ast;
	}
  	else
  	{
   	 	printf("SQL parse failed\n");
   	 	return NULL;
	}
	yy_delete_buffer(bp, presult->yyscan_info_);
}

/*
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
*/
static Node * getparsetreeroot(const char * InputStr)
{
/*	char InputStr[10000];
	puts("SQL!!!!!");
	int charnum=0;
    while(1)
    {
        char c=getchar();
        InputStr[charnum++]=c;
        if(c==';')
        {
            InputStr[charnum++]='\0';
            break;
        }
    }

	strcpy(InputStr,str);
*/
    vector<Node *>allnode;
	struct ParseResult presult={NULL,NULL,InputStr,0,&allnode};
//reference to the example of "purecal"
	if(yylex_init_extra(&presult, &presult.yyscan_info_))
	{
    		perror("init alloc failed");
    		return NULL;
  	}
	YY_BUFFER_STATE bp;
	int len = strlen(InputStr);
	printf("%s  %d\n",InputStr,len);
	  //bp = yy_scan_string(buf, p->yyscan_info_);
	bp = yy_scan_bytes(InputStr, len, presult.yyscan_info_);
	yy_switch_to_buffer(bp,presult.yyscan_info_);
  	if(!yyparse(&presult))
  	{
		return presult.ast;
	}
  	else
  	{
   	 	printf("SQL parse failed\n");
   	 	return NULL;
	}
	yy_delete_buffer(bp, presult.yyscan_info_);
}
static Node * getparsetreeroot(struct ParseResult * presult,const char * InputStr)
{
	if(yylex_init_extra(presult, &presult->yyscan_info_))
	{
    		perror("init alloc failed");
    		return NULL;
  	}
	YY_BUFFER_STATE bp;
	int len = strlen(InputStr);
	printf("%s  %d\n",InputStr,len);
	  //bp = yy_scan_string(buf, p->yyscan_info_);
	bp = yy_scan_bytes(InputStr, len, presult->yyscan_info_);
	yy_switch_to_buffer(bp,presult->yyscan_info_);
  	if(!yyparse(presult))
  	{
		return presult->ast;
	}
  	else
  	{
   	 	printf("SQL parse failed\n");
   	 	return NULL;
	}
	yy_delete_buffer(bp, presult->yyscan_info_);
}
#endif
