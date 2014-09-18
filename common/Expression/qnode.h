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
#include "../../common/TypeCast.h"
#define BASE_SIZE 128
typedef struct FuncCallInfoData
{
	int nargs;
	void * args[MAX_ARGS];
	void *results;
}*FuncCallInfo;
typedef  void (*ExecFunc)(FuncCallInfo  fcinfo);
typedef void* (*FuncCall)(Node *qinfo,void *tuple,Schema *schema);
enum qnodetype
{
	t_qnode,t_qexpr_cal,t_qexpr_cmp,t_qexpr,t_qexpr_unary,t_qexpr_ternary,t_qcolcumns,t_qexpr_func,t_qname,t_qstring,t_qint,t_qexpr_case_when,t_qexpr_in,
};

enum oper_type
{
	oper_none,
	oper_add,oper_minus,oper_multiply,oper_divide,oper_mod,
	oper_and,oper_or,oper_xor,oper_not,
	oper_equal,oper_not_equal,oper_great,oper_great_equal,oper_less,oper_less_equal,
	oper_both_trim,oper_trailing_trim,oper_leading_trim,oper_like,oper_not_like,oper_upper,oper_substring,
	oper_negative,
	oper_case_when,
};
class QNode:public Node
{
public:
	qnodetype type;
	FuncCall FuncId;
	data_type actual_type,return_type;
	void * value;
	int length;
	bool isnull;
	TypeCastF type_cast_func;
	string alias;
	QNode(){};
	virtual ~QNode()
	{
		if(value!=NULL)
			free(value);
	};
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & type & actual_type & return_type & length & isnull &alias;
	}
};
class QExpr_unary:public QNode
{
public:
	oper_type op_type;
	QNode *next;
	ExecFunc function_call;
	QExpr_unary(){};
	virtual ~QExpr_unary(){};
	QExpr_unary(QNode * arg,data_type a_type,oper_type op_types,qnodetype q_type,char *t_alias);
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<QNode>(*this) & op_type & next ;
	}
};
class QExpr_binary:public QNode//二元计算表达式，先做个测试exec_cal()
{
public:
	oper_type op_type;
	QNode *lnext,*rnext;
	ExecFunc function_call;// 通过函数可以知道具体的node类型，因此不需要进行nodetype的判断
	QExpr_binary(){};
	virtual ~QExpr_binary(){};
	QExpr_binary(QNode *l_arg,QNode *r_arg,data_type a_type,oper_type op_types,qnodetype q_type,char *t_alias);
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<QNode>(*this) & lnext &rnext & op_type ;
	}
};
class QExpr_ternary:public QNode
{
public:
	oper_type op_type;
	QNode* next0,*next1,*next2;
	ExecFunc function_call;
	QExpr_ternary(){};
	virtual ~QExpr_ternary(){};
	QExpr_ternary(QNode *arg0,QNode *arg1,QNode *arg2,data_type a_type,oper_type op_types,qnodetype q_type,char *t_alias);
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<QNode>(*this) & op_type & next0 & next1 & next2  ;
	}
};
class QColcumns:public QNode//getscalarvar
{
public:
	int id;//表示在dataflow_schema中的第几个
	string table,col;
	QColcumns(char *tbl,char *coln,data_type a_type,char *t_alias);
	QColcumns(){};
	virtual ~QColcumns(){};
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<QNode>(*this) & table & col & id ;
	}
};
class QExpr:public QNode//getconst
{
public:
	string const_value;
	QExpr(char *val,data_type r_type,char *t_alias);
	QExpr(){};
	virtual ~QExpr(){};
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<QNode>(*this)&const_value;
	}
};

class QExpr_case_when:public QNode
{
public:
	vector<QNode *>qual;
	vector<QNode *>ans;
	QNode *result;
	QExpr_case_when(vector<QNode *>&qual_,vector<QNode *>&ans_,string alias_);
	QExpr_case_when(){};
	virtual ~QExpr_case_when(){};
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<QNode>(*this) & qual & ans & result;
	}
};
class QExpr_in:public QNode
{
public:
	vector<QNode *>cmpnode;
	vector< vector<QNode *>  >rnode;
	QExpr_in(vector<QNode *>&cmpnode_,vector<vector<QNode *> >&rnode_,char * alias_);
	QExpr_in(){};
	virtual ~QExpr_in(){};
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<QNode>(*this) & cmpnode & rnode;
	}
};
#endif /* QNode_H_ */
