/*
 * runparsetree.cpp
 *
 *  Created on: 2014年1月21日
 *      Author: imdb
 */
#ifndef __RUNPARSER__
#define __RUNPARSER__

#include "runparsetree.h"

/*
 * 利用StreamBuffer类作为缓冲，获取标准输入，处理后放入desc中，送给解析器解析
 * @param stream_buffer: StreamBuffer类的一个实例
 * @param desc 存放即将交给解析器的sql语句
 */
void GetInputSQL(StreamBuffer *stream_buffer, char *desc) {
  assert(stream_buffer != NULL && desc != NULL);
  if (stream_buffer->IsEmpty()) {
    //		getline(cin, stream_buffer->GetCurrent());
    cin.getline(stream_buffer->GetCurrent(),
                stream_buffer->GetFreedBufferCount(), '\n');
    stream_buffer->MoveForwardEnd(cin.gcount());
  }
}

Node *getparsetreeroot() {
#ifdef OLDSQL
  char InputStr[10000];
  puts("SQL!!!!!");
  int charnum = 0;
  while (1) {
    char c = getchar();
    InputStr[charnum++] = c;
    if (c == ';') {
      InputStr[charnum++] = '\0';
      break;
    }
  }
  vector<Node *> allnode;
  struct ParserResult presult = {NULL, NULL, InputStr, 0, &allnode};
  // reference to the example of "purecal"
  if (yylex_init_extra(&presult, &presult.yyscan_info_)) {
    perror("init alloc failed");
    return NULL;
  }
  YY_BUFFER_STATE bp;
  int len = strlen(InputStr);
  printf("%s  %d\n", InputStr, len);
  // bp = yy_scan_string(buf, p->yyscan_info_);
  bp = yy_scan_bytes(InputStr, len, presult.yyscan_info_);
  yy_switch_to_buffer(bp, presult.yyscan_info_);
  if (!yyparse(&presult)) {
    return presult.ast;
  } else {
    printf("SQL parse failed\n");
    return NULL;
  }
  yy_delete_buffer(bp, presult.yyscan_info_);
#else
  cout << "old sql used!" << endl;
  return NULL;
#endif
}
static Node *getparsetreeroot(struct ParserResult *presult) {
#ifdef OLDSQL
  char InputStr[10000];
  puts("SQL!!!!!");
  int charnum = 0;
  while (1) {
    char c = getchar();
    InputStr[charnum++] = c;
    if (c == ';') {
      InputStr[charnum++] = '\0';
      break;
    }
  }
  presult->sql_clause = InputStr;
  // reference to the example of "purecal"
  if (yylex_init_extra(presult, &presult->yyscan_info_)) {
    perror("init alloc failed");
    return NULL;
  }
  YY_BUFFER_STATE bp;
  int len = strlen(InputStr);
  printf("%s  %d\n", InputStr, len);
  // bp = yy_scan_string(buf, p->yyscan_info_);
  bp = yy_scan_bytes(InputStr, len, presult->yyscan_info_);
  yy_switch_to_buffer(bp, presult->yyscan_info_);
  if (!yyparse(presult)) {
    return presult->ast;
  } else {
    printf("SQL parse failed\n");
    return NULL;
  }
  yy_delete_buffer(bp, presult->yyscan_info_);
#else
  cout << "old sql used!" << endl;
  return NULL;
#endif
}

/*
static Node * getparsetreeroot(const char *sql)
=======
Node * getparsetreeroot(const char *sql)
>>>>>>> refs/remotes/origin/master150426
{
        assert(sql != NULL);
        ASTParserLogging::log("sql argument is %s", sql);
        int charnum=0;
        globalReadOffset = 0;

        NodePointer.clear();	// 2014-3-7---初始化---by余楷	//
2014-4-2---存放节点指针的数组改为vector---by Yu
        errorNumber = 0;	// 2014-3-6---初始化SQL解析错误个数---by余楷
        parsetreeroot = NULL;	// 2014-3-4---增加初始化语句---by余楷
        memset(globalInputText, 0, sizeof(globalInputText));	//
2014-3-4---增加初始化语句---by余楷
//	ASTParserLogging::log("befor copying, globalInputText: %s",
globalInputText);

        // 2014-3-4---测试数据---by余楷
        strcpy(globalInputText,	sql);
        ASTParserLogging::log("after copying, globalInputText: %s",
globalInputText);


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
static Node *getparsetreeroot(const char *str) {
#ifdef OLDSQL
  /*	puts("SQL!!!!!");
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

  <<<<<<< HEAD
  */
  char InputStr[10000];
  strcpy(InputStr, str);
  vector<Node *> allnode;
  struct ParserResult presult = {NULL, NULL, InputStr, 0, &allnode};
  // reference to the example of "purecal"
  if (yylex_init_extra(&presult, &presult.yyscan_info_)) {
    perror("init alloc failed");
    return NULL;
  }
  YY_BUFFER_STATE bp;
  int len = strlen(InputStr);
  printf("%s  %d\n", InputStr, len);
  // bp = yy_scan_string(buf, p->yyscan_info_);
  bp = yy_scan_bytes(InputStr, len, presult.yyscan_info_);
  yy_switch_to_buffer(bp, presult.yyscan_info_);
  if (!yyparse(&presult)) {
    return presult.ast;
  } else {
    printf("SQL parse failed\n");
    return NULL;
  }
  yy_delete_buffer(bp, presult.yyscan_info_);
#else
  cout << "old sql used!" << endl;
  return NULL;
#endif
}
static Node *getparsetreeroot(struct ParserResult *presult,
                              const char *InputStr) {
#ifdef OLDSQL
  if (yylex_init_extra(presult, &presult->yyscan_info_)) {
    perror("init alloc failed");
    return NULL;
  }
  YY_BUFFER_STATE bp;
  int len = strlen(InputStr);
  printf("%s  %d\n", InputStr, len);
  // bp = yy_scan_string(buf, p->yyscan_info_);
  bp = yy_scan_bytes(InputStr, len, presult->yyscan_info_);
  yy_switch_to_buffer(bp, presult->yyscan_info_);
  if (!yyparse(presult)) {
    return presult->ast;
  } else {
    printf("SQL parse failed\n");
    return NULL;
  }
  yy_delete_buffer(bp, presult->yyscan_info_);
#else
  cout << "old sql used!" << endl;
  return NULL;
#endif
}
#endif
