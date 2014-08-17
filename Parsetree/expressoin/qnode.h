/*
 * QNode.h
 *
 *  Created on: Aug 4, 2014
 *      Author: imdb
 */

#ifndef QNode_H_
#define QNode_H_
#include<stdio.h>
#include<iostream>
#include<malloc.h>
#include<string.h>
using namespace std;
#define MAX_ARGS 10
#include "../../Parsetree/sql_node_struct.h"
#include "../../common/data_type.h"
#include "../../Catalog/Attribute.h"
#include "../../common/Schema/Schema.h"
#include "qnode.h"
typedef unsigned long int Datum;
typedef struct FuncCallInfoData
{
	int nargs;
	void * args[MAX_ARGS];
	void *results;
}*FuncCallInfo;
typedef void (*ExecFunc)(FuncCallInfo  fcinfo);
typedef void *(*FuncCall)(Node *qinfo,void *tuple,Schema *schema);
typedef void *(*TypeCastFunc)(void * oldvalue,data_type totype);
enum qnodetype
{
	t_qnode,t_qexpr_cal,t_qexpr,t_qcolcumns,t_qexpr_func,t_qname,t_qstring,t_qint,
};
class QNode:public Node
{
public:
	qnodetype type;
	FuncCall FuncId;
	data_type actual_type,return_type;
	QNode(){};
	virtual ~QNode(){};
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar&type &actual_type ;
	}
};
class QExpr_cal:public QNode//二元计算表达式，先做个测试exec_cal()
{
public:
	string sign,parameter;
	int cmp;
	QNode *lnext,*rnext;
	ExecFunc function_call;// 通过函数可以知道具体的node类型，因此不需要进行nodetype的判断
	QExpr_cal(){};
	~QExpr_cal(){};
	QExpr_cal(QNode *l_arg,QNode *r_arg,data_type r_type,char *tsign,char *para,int tcmp);
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<QNode>(*this) & lnext &rnext & cmp &sign & parameter ;
	}
};
class QColcumns:public QNode//getscalarvar
{
public:
	int id;//表示在dataflow_schema中的第几个
	string table,col;
	QColcumns(char *tbl,char *coln,data_type r_type);
	QColcumns(){};
	~QColcumns(){};
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<QNode>(*this) & table & col ;
	}
};
class QExpr:public QNode//getconst
{
public:
	string value;
	TypeCastFunc CastFunc;
	QExpr(char *val,data_type r_type);
	QExpr(){};
	~QExpr(){};
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<QNode>(*this)&value;
	}
};



#endif /* QNode_H_ */
