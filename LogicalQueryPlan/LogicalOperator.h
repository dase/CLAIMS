/*
 * LogicalOperator.h
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */

#ifndef LOGICALOPERATOR_H_
#define LOGICALOPERATOR_H_
#include <stdio.h>
#include "../Schema/LogicalProjection.h"


class LogicalOperator{
	virtual LogicalProjection getLogicalProjection()=0;
};

//class Operator{
//
//};
//class LogicalOperator :public Operator{
//public:
//	LogicalOperator();
//	virtual ~LogicalOperator();
//	virtual void getSchema()=0;
//	virtual LogicalOperator* getchild()=0;
//	virtual void setchild(Operator*)=0;
//};
//class ColoredLogicalOperator :public Operator{
//public:
//	ColoredLogicalOperator(){};
//	virtual void getSchema()=0;
//	virtual int getColor()=0;
////	LogicalOperator* getchind(){return getchild()};
//	virtual ColoredLogicalOperator* getchild()=0;
//	virtual void setchild(Operator*)=0;
//};
//class join:public ColoredLogicalOperator,public LogicalOperator{
//public:
//	join(){};
//	void getSchema(){
//		printf("join::getSchema()\n");
//	}
//	int getColor(){
//		printf("join::getColor\n");
//		if(child!=0){
//			printf("Child->");
//			((ColoredLogicalOperator*)child)->getColor();
//		}
//	}
//	join* getchild(){
//		printf("join::getchind\n");
//		return this;
//	}
//	void setchild(Operator* op){
//		child=op;
//	}
//public:
//	Operator* child;
//
//
//};
//
//
//class tryit{
//public:
//	void doit(){
//		LogicalOperator* o=new join();
//		o->getchild();
//		ColoredLogicalOperator* co=(join*)o;//(ColoredLogicalOperator*)o;
//
////		co->setchild(o);
//		co->getColor();
//	}
//};
#endif /* LOGICALOPERATOR_H_ */
