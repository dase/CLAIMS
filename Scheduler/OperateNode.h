/*
 * OperateNode.h
 *
 *  Created on: 2013-9-26
 *      Author: casa
 */

#ifndef OPERATENODE_H_
#define OPERATENODE_H_

#include <string>
#include <pair>
#include <vector>
using namespace std;

#include "../Schema/Schema.h"
#include "../Schema/SchemaFix.h"
#include "../Catalog/table.h"

typedef bool cut_t;

class OperateNode {
public:
	OperateNode(){};

	OperateNode(string operate_name,OperateNode *left_child,OperateNode *right_child)
	:operate_name_(operate_name),left_child_(left_child),right_child_(right_child),parent_(0){
		iscut_=false;
	}

	virtual ~OperateNode(){};

	inline string getOperateName(){
		return operate_name_;
	}

	inline void setCut(){
		iscut_=true;
	}

	inline bool isCut(){
		return iscut_==true;
	}

	inline bool hashleftchild(){
		return left_child_!=0;
	}

	inline bool hasrightchild(){
		return right_child_!=0;
	}

	inline OperateNode *getLeftchild(){
		return left_child_;
	}

	inline OperateNode *getRightchild(){
		return right_child_;
	}

	//for debug
	inline OperateNode *getParent(){
		return parent_;
	}

	inline void setLeftchild(OperateNode *leftchild){
		left_child_=leftchild;
	}

	inline void setParent(OperateNode *node){
		parent_=node;
	}

	virtual Schema *getOutputSchema()=0;

	virtual Schema *getLeftInputSchema()=0;

	virtual Schema *getRightInputSchema(){
		return 0;
	};

	virtual vector<pair<string,string> > getIppath(){
		return (vector<pair<string,string> >)(0);
	};

protected:
	//TODO: every node has two child?
	OperateNode *left_child_;
	OperateNode *right_child_;

	//TODO: check if the parent point must be existed! modified by 0928
	OperateNode *parent_;
private:
	string operate_name_;
	cut_t iscut_;

};

class ScanNode:public OperateNode{
public:
	ScanNode(OperateNode *left_child,OperateNode *right_child,Schema *column_schema_scan,vector<pair<string,string> > columns_ip_path_scan)
	:OperateNode("scan",left_child,right_child),column_schema_scan_(column_schema_scan),columns_ip_path_scan_(columns_ip_path_scan){
	}

	virtual ~ScanNode(){};

	Schema *getOutputSchema(){
		return column_schema_scan_;
	}

	Schema *getLeftInputSchema(){
		return getOutputSchema();
	}

	vector<pair<string,string> > getIppath(){
		return columns_ip_path_scan_;
	}

private:
	//TODO: 在这里schema和ip_path是原子对应的
	Schema *column_schema_scan_;
	//TODO: 这里以后怎么办？怎么样有多个输入呢？重载state的第一个参数，使其能够输入多个文件
	vector<pair<string,string> > columns_ip_path_scan_;
};

class FilterNode:public OperateNode{
public:
	FilterNode(OperateNode *left_child,OperateNode *right_child)
	:OperateNode("filter",left_child,right_child){
	}

	virtual ~FilterNode(){};

	Schema *getOutputSchema(){
		return left_child_->getOutputSchema();
	}

	Schema *getLeftInputSchema(){
		return left_child_->getOutputSchema();
	}

private:

};

class BroadcastJoinNode: public OperateNode{
public:
	BroadcastJoinNode(OperateNode *left_child,OperateNode *right_child,vector<unsigned> left_index,vector<unsigned> right_index,vector<unsigned> left_payload,vector<unsigned> right_payload)
	:OperateNode("broadcastjoin",left_child,right_child),left_index_(left_index),right_index_(right_index),left_payload_(left_payload),right_payload_(right_payload){
	}

	virtual ~BroadcastJoinNode(){};

	//this join is definitely different from other operators, equijoin
//	Schema *getOutputSchema(){
//		Schema *rt=0;
//		vector<column_type> columns;
//		Schema *sl=getLeftInputSchema();
//		Schema *sr=getRightInputSchema();
//		for(unsigned i=0;i<sl->columns.size();i++){
//			columns.push_back(sl->columns[i]);
//		}
//
//		for(unsigned j=0;j<sr->columns.size();j++){
//			bool exist=false;
//			for(unsigned k=0;k<right_index_.size();k++){
//				if(j!=right_index_[k])
//					exist=false;
//				else{
//					exist=true;
//					break;
//				}
//			}
//			if(exist==false){
//				columns.push_back(sr->columns[j]);
//			}
//		}
//		rt=new SchemaFix(columns);
//		return rt;
//	}

	Schema *getOutputSchema(){
		Schema *rt=0;
		vector<column_type> columns;
		Schema *sl=getLeftInputSchema();
		Schema *sr=getRightInputSchema();
		for(unsigned i=0;i<left_index_.size();i++){
			columns.push_back(sl->columns[left_index_[i]]);
		}
		for(unsigned i=0;i<left_payload_.size();i++){
			columns.push_back(sl->columns[left_payload_[i]]);
		}
		for(unsigned i=0;i<right_payload_.size();i++){
			columns.push_back(sr->columns[right_payload_[i]]);
		}
		rt=new SchemaFix(columns);
		return rt;
	}

	Schema *getLeftInputSchema(){
		return left_child_->getOutputSchema();
	}

	Schema *getRightInputSchema(){
		return right_child_->getOutputSchema();
	}

	vector<unsigned> getLeftIndex(){
		return left_index_;
	}

	vector<unsigned> getRightIndex(){
		return right_index_;
	}

	vector<unsigned> getLeftPayload(){
		return left_payload_;
	}

	vector<unsigned> getRightPayload(){
		return right_payload_;
	}

private:
	//TODO: columns to the index of integer
	vector<unsigned> left_index_;
	vector<unsigned> right_index_;
	//TODO: given by select list
	vector<unsigned> left_payload_;
	vector<unsigned> right_payload_;
};

class ExchangeUpperNode:public OperateNode{
public:
	ExchangeUpperNode(OperateNode *left_child,OperateNode *right_child)
	:OperateNode("exchange",left_child,right_child){
	}

	virtual ~ExchangeUpperNode(){};

	Schema *getOutputSchema(){
		return left_child_->getOutputSchema();
	}

	Schema *getLeftInputSchema(){
		return left_child_->getOutputSchema();
	}

private:
	//TODO: support exchangeid
};

class ExpandNode:public OperateNode{
public:
	ExpandNode(OperateNode *left_child,OperateNode *right_child)
	:OperateNode("expand",left_child,right_child){
	}

	virtual ~ExpandNode(){};

	Schema *getOutputSchema(){
		return left_child_->getOutputSchema();
	}

	Schema *getLeftInputSchema(){
		return left_child_->getOutputSchema();
	}

private:
	//TODO: support parallel degree
};

#endif /* OPERATENODE_H_ */
