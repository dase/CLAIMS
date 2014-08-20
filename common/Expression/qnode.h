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
typedef struct FuncCallInfoData
{
	int nargs;
	void * args[MAX_ARGS];
	void *results;
}*FuncCallInfo;
typedef  void (*ExecFunc)(FuncCallInfo  fcinfo);
typedef void* (*FuncCall)(Node *qinfo,void *tuple,Schema *schema);
typedef void* (*TypeCastFunc)(void * oldvalue,data_type totype);
enum qnodetype
{
	t_qnode,t_qexpr_cal,t_qexpr_cmp,t_qexpr,t_qcolcumns,t_qexpr_func,t_qname,t_qstring,t_qint,
};

enum oper_type
{
	oper_none,
	oper_add,oper_minus,oper_multi,oper_divide,oper_mod,
	oper_and,oper_or,oper_xor,oper_not,
	oper_equal,oper_not_equal,oper_great,oper_great_equal,oper_less,oper_less_equal,
	oper_both_trim,oper_trailing_trim,oper_leading_trim,oper_like,oper_not_like,
};
class QNode:public Node
{
public:
	qnodetype type;
	FuncCall FuncId;
	data_type actual_type,return_type;
	enum{s_size=100};
	char result_store[s_size];
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
class QExpr_binary:public QNode//二元计算表达式，先做个测试exec_cal()
{
public:
	oper_type op_type;
	QNode *lnext,*rnext;
	ExecFunc function_call;// 通过函数可以知道具体的node类型，因此不需要进行nodetype的判断
	QExpr_binary(){};
	~QExpr_binary(){};
	QExpr_binary(QNode *l_arg,QNode *r_arg,data_type r_type,oper_type op_types,qnodetype q_type);
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<QNode>(*this) & lnext &rnext & op_type ;
	}
};
class QExpr_unary:public QNode
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<QNode>(*this);
	}
};
class QExpr_ternary:public QNode
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<QNode>(*this)  ;
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
