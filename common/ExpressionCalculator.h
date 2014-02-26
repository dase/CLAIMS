/*
 * ExpressionCalculator.h
 *
 *  Created on: Feb 25, 2014
 *      Author: wangli
 */

#ifndef EXPRESSIONCALCULATOR_H_
#define EXPRESSIONCALCULATOR_H_
#include <stack>
#include <list>
#include <vector>
#include "ExpressionItem.h"

typedef std::stack<ExpressionItem> ExpressionItemStack;
typedef std::vector<ExpressionItem> Expression;
class ExpressionCalculator {
public:
	ExpressionCalculator();
	virtual ~ExpressionCalculator();
	static void calcuate(Expression &exp){
		if(exp.size()<=1){
			return;
		}
		else{
			ExpressionItemStack stack;
			calcualte(exp,stack);
			assert(stack.size()==1);
			printf("result:\n");
			stack.top().print();
		}
	}
	static void calcualte(Expression &exp,ExpressionItemStack& stack){
		for(unsigned i=0;i<exp.size();i++){
			if(exp[i].type!=ExpressionItem::operator_type){
				stack.push(exp[i]);

			}
			else{
				compute(exp[i],stack);
			}
		}
	}
	static void compute(ExpressionItem operator_item,ExpressionItemStack& stack);
};

#endif /* EXPRESSIONCALCULATOR_H_ */
