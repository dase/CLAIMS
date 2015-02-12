/*
 * runparsetree.cpp
 *
 *  Created on: 2014年1月21日
 *      Author: imdb
 */

#include "runparsetree.h"

/*
 * 利用StreamBuffer类作为缓冲，获取标准输入，处理后放入desc中，送给解析器解析
 * @param stream_buffer: StreamBuffer类的一个实例
 * @param desc 存放即将交给解析器的sql语句
 */
void GetInputSQL(StreamBuffer* stream_buffer, char *desc) {
	assert(stream_buffer != NULL && desc != NULL);
	if (stream_buffer->IsEmpty()) {
//		getline(cin, stream_buffer->GetCurrent());
		cin.getline(stream_buffer->GetCurrent(), stream_buffer->GetFreedBufferCount(),'\n');
		stream_buffer->MoveForwardEnd(cin.gcount());
	}
}

Node * getparsetreeroot()
{
	int charnum=0;
	globalReadOffset = 0;

	NodePointer.clear();
	errorNumber = 0;
	parsetreeroot = NULL;
	memset(globalInputText, 0, sizeof(globalInputText));
//	printf("stdin buffer has:%d char\n", cin.rdbuf()->in_avail());
	printf("Claims>");
	while(1)
	{
		int c=getchar();
//		if (c == '\n')
//		isspace();

		globalInputText[charnum++]=c;
		if(c==';')
		{
			globalInputText[charnum++]='\0';
//			getchar();	// eliminate '\n'
			break;
		}
//		setbuf(stdin, NULL);
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

//	strcpy(globalInputText,"show tables;show");
	SQLParse_log("globalInputText:\n%s\n",globalInputText);

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

Node * getparsetreeroot(const char *sql)
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

