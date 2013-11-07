/*
 * LogicalOperator.h
 *
 *  Created on: Nov 7, 2013
 *      Author: wangli
 */

#ifndef LOGICALOPERATOR_H_
#define LOGICALOPERATOR_H_
#include <stdio.h>
class LogicalOperator {
public:
	LogicalOperator();
	virtual ~LogicalOperator();
	virtual void getSchema()=0;
	virtual LogicalOperator* getchild()=0;
};
class ColoredLogicalOperator{
public:
	ColoredLogicalOperator(){};
	virtual void getSchema()=0;
	virtual int getColor()=0;
//	LogicalOperator* getchind(){return getchild()};
	virtual ColoredLogicalOperator* getchild()=0;
};
class join:public ColoredLogicalOperator,public LogicalOperator{
public:
	join(){};
	void getSchema(){
		printf("join::getSchema()\n");
	}
	int getColor(){
		printf("join::getColor\n");
	}
	join* getchild(){
		printf("join::getchind\n");
		return this;
	}


};


class tryit{
public:
	void doit(){
		LogicalOperator* o=new join();
		o->getchild();
		ColoredLogicalOperator* co=(join*)o;//(ColoredLogicalOperator*)o;
		co->getColor();
	}
};
#endif /* LOGICALOPERATOR_H_ */
