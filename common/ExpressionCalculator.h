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

/**
 * print_expression is a good tool for debug.
 * You can :
 * 			1) either insert print_expression function call in your code;
 * 			2) or call the function when debugging in gdb. (p print_expression(stack))
 */

inline void print_expression(Expression& exp){
	printf("__Exp=%d__\n",exp.size());
	for(unsigned i=0;i<exp.size();i++){
		exp[i].print_value();
	}
	printf("_____\n");
}
inline void print_expression(ExpressionItemStack& stack){
	printf("__stack=%d__\n",stack.size());
	ExpressionItemStack tmp=stack;
	while(!stack.empty()){
		stack.top().print_value();
		stack.pop();
	}
	stack=tmp;
	printf("_____\n");
}

class ExpressionSchema {
public:
	ExpressionSchema();
	virtual ~ExpressionSchema();
	static void calcuates(Expression &exp,ExpressionItem& result){
		ExpressionItemStack stack;
		calcualtes(exp,stack);
		assert(stack.size()==1);
		result=stack.top();
	}

	static void calcualtes(Expression &exp,ExpressionItemStack& stack){
		for(unsigned i=0;i<exp.size();i++){
			if(exp[i].type!=ExpressionItem::operator_type){
				stack.push(exp[i]);
			}
			else{
				if(isComposeOperator(exp[i].content.op.op_)){
					op_type compose_op=exp[i].content.op.op_;
					stack.push(exp[i++]);
					unsigned j=i;
					bool processed_compose_op=false;
					for(;j<exp.size();j++){
						if(exp[j].type==ExpressionItem::operator_type&&exp[j].content.op.op_==compose_op)
						{
							unsigned before = stack.size();
							assert(stack.size()==before);
							compute(exp[j],stack);
							i=j+1;
							processed_compose_op=true;
							break;
						}
						else{
							stack.push(exp[j]);
						}
					}
					if(!processed_compose_op){
						printf("No end operator for operator[%d] is found!\n",compose_op);
						assert(false);
					}
				}
				else{
					compute(exp[i],stack);
				}
			}
		}
	}
private:

};

class ExpressionCalculator {
public:
	ExpressionCalculator();
	virtual ~ExpressionCalculator();
	static void calcuate(Expression &exp,ExpressionItem& result){
//		if(exp.size()<=1){
//			return;
//		}
//		else{
			ExpressionItemStack stack;
			calcualte(exp,stack);
			assert(stack.size()==1);
//			printf("result:\n");
//			stack.top().print();
			result=stack.top();
//		}
	}
	static void calcualte(Expression &exp,ExpressionItemStack& stack){
//		print_expression(exp);
		for(unsigned i=0;i<exp.size();i++){
			if(exp[i].type!=ExpressionItem::operator_type){

				stack.push(exp[i]);

			}
			else{
				if(isComposeOperator(exp[i].content.op.op_)){
					op_type compose_op=exp[i].content.op.op_;
					stack.push(exp[i++]);
					unsigned j=i;
					bool processed_compose_op=false;
					for(;j<exp.size();j++){
						if(exp[j].type==ExpressionItem::operator_type&&exp[j].content.op.op_==compose_op)
						{
							unsigned before = stack.size();

//							print_expression(stack);

							assert(stack.size()==before);
							compute(exp[j],stack);
							i=j+1;
							processed_compose_op=true;
							break;
						}
						else{
							stack.push(exp[j]);
						}
					}
					if(!processed_compose_op){
						printf("No end operator for operator[%d] is found!\n",compose_op);
						assert(false);
					}
				}
				else{
					compute(exp[i],stack);
				}

			}
		}
	}
	static void compute(ExpressionItem operator_item,ExpressionItemStack& stack);
private:

	/**
	 * Compose operator refers to a kind of operators, the number of input parameter of witch is decided by the semantic.
	 * e.g., CASE WHEN ... THEN... [WHEN ... THEN...][N] ELSE ..... END
	 */
	static bool isComposeOperator(op_type tp ){
		switch(tp){
		case op_case:{
			return true;
		}
		default:{
			return false;
		}
		}
	}
	static void reverse_stack(ExpressionItemStack& stack){
		ExpressionItemStack tmp;
		while(!stack.empty()){
			tmp.push(stack.top());
			stack.pop();
		}
		stack=tmp;
	}
};

#endif /* EXPRESSIONCALCULATOR_H_ */
