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
#include "../../catalog/attribute.h"
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
typedef void (*AvgDivide)(void *sum_value,unsigned long tuple_number,void *result);

enum qnodetype
{
	t_qnode,t_qexpr_cal,t_qexpr_cmp,t_qexpr,t_qexpr_unary,t_qexpr_ternary,t_qcolcumns,t_qexpr_func,t_qexpr_case_when,t_qexpr_in,
	t_qexpr_date_add_sub,
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
	oper_date_add_day,oper_date_add_week,oper_date_add_month,oper_date_add_year,//quanter type have been changed to month*3 in transfromqual()
	oper_date_sub_day,oper_date_sub_week,oper_date_sub_month,oper_date_sub_year,
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
		if(value!=NULL) {
			free(value);
			value = NULL;
		}
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
	virtual ~QExpr_unary()
	{
		if(next!=NULL)
			next->~QNode();
	};
	QExpr_unary(QNode * arg,data_type a_type,oper_type op_types,qnodetype q_type,char *t_alias);
	QExpr_unary(QExpr_unary *node);
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<QNode>(*this) & op_type & next ;
	}
};
class QExpr_binary:public QNode//
{
public:
	oper_type op_type;
	QNode *lnext,*rnext;
	ExecFunc function_call;// 通过函数可以知道具体的node类型，因此不需要进行nodetype的判断
	QExpr_binary(){};
	virtual ~QExpr_binary()
	{
		if(lnext!=NULL)
			lnext->~QNode();
		if(rnext!=NULL)
			rnext->~QNode();
	};
	QExpr_binary(QNode *l_arg,QNode *r_arg,data_type a_type,oper_type op_types,qnodetype q_type,char *t_alias);
	QExpr_binary(QExpr_binary *node);
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
	virtual ~QExpr_ternary()
	{
		if(next0!=NULL)
			next0->~QNode();
		if(next1!=NULL)
			next1->~QNode();
		if(next2!=NULL)
			next2->~QNode();
	};
	QExpr_ternary(QNode *arg0,QNode *arg1,QNode *arg2,data_type a_type,oper_type op_types,qnodetype q_type,char *t_alias);
	QExpr_ternary(QExpr_ternary *node);
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
	/* tab_index represents the logical index of table this column belongs to.
	 * This index is used to distinguish the source of the column if expression
	 * contains more than one table. For instance, if the expression is T1.a =T2.b
	 * , which is used in an equal join condition, then tab_index in QColumn
	 * indicates the target table. If express only reads data from one table, the
	 * default value for tab_index should be 0.
	 */
	int tab_index; // represent the logical index of the table this column belongs to.
	QColcumns(const char *tbl,const char *coln,data_type a_type,const char *t_alias);
	QColcumns(QColcumns *node);
	QColcumns():tab_index(0){};
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
	QExpr(QExpr *node);
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
	QExpr_case_when(QExpr_case_when *node);
	QExpr_case_when(){};
	virtual ~QExpr_case_when()
	{
		for(int i=0;i<qual.size();i++)
		{
			if(qual[i]!=NULL)
				qual[i]->~QNode();
		}
		for(int i=0;i<ans.size();i++)
		{
			if(ans[i]!=NULL)
				ans[i]->~QNode();
		}
	};
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
	QExpr_in(QExpr_in *node);
	QExpr_in(){};
	virtual ~QExpr_in()
	{
		for(int i=0;i<cmpnode.size();i++)
		{
			if(cmpnode[i]!=NULL)
				cmpnode[i]->~QNode();
		}
		for(int i=0;i<rnode.size();i++)
		{
//			if(rnode[i]!=NULL)
			{
				for(int j=0;j<rnode[i].size();j++)
				{
					if(rnode[i][j]!=NULL)
						rnode[i][j]->~QNode();
				}
			}
		}
	};
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<QNode>(*this) & cmpnode & rnode;
	}
};
class QExpr_date_add_sub:public QNode
{
public:
	oper_type op_type;
	QNode *lnext,*rnext;
	data_type rnext_type;//the return type of the rnext
	ExecFunc function_call;
	QExpr_date_add_sub(){};
	virtual ~QExpr_date_add_sub()
	{
		if(lnext!=NULL)
		{
			lnext->~QNode();
		}
		if(rnext!=NULL)
		{
			rnext->~QNode();
		}
	};
	QExpr_date_add_sub(QNode *l_arg,QNode *r_arg,data_type a_type,oper_type op_types,qnodetype q_type,data_type rr_type,char *t_alias);
	QExpr_date_add_sub(QExpr_date_add_sub *node);
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<QNode>(*this) & lnext &rnext & op_type &rnext_type;
	}
};
#endif /* QNode_H_ */
