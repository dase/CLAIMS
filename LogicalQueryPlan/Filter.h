/*
 * Filter.h
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#ifndef FILTER_H_
#define FILTER_H_
#include <vector>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "LogicalOperator.h"
#include "../common/AttributeComparator.h"
#include "../common/ExpressionCalculator.h"
#include "../common/ExpressionItem.h"

#include <map>
#include "../common/Expression/qnode.h"
class Filter:public LogicalOperator {
public:
	class Condition{
	public:
		friend class Filter;
//		typedef AttributeComparator::comparison comparison;
		Condition(){}
		Condition(const Condition& r);
		~Condition();
		void add(const Attribute& attr,const AttributeComparator::comparison& com,const void*const& const_value);
		void add(const Attribute& attr,const AttributeComparator::comparison& com,std::string str_exp);
		void print(int level=0)const;
	private:
		unsigned getCompaisonNumber()const;
	private:
		std::vector<Attribute> attribute_list_;
		std::vector<AttributeComparator::comparison> comparison_list_;
		std::vector<void*> const_value_list_;
	};
	Filter(LogicalOperator *child,vector<QNode *>qual);
	Filter(std::vector<AttributeComparator> ComparatorList,LogicalOperator* child );
	Filter(const Condition& condition, LogicalOperator*  child);
	Filter(LogicalOperator *child,std::vector<std::vector<ExpressionItem> > &exprArray);
	virtual ~Filter();
	Dataflow GetDataflow();
	BlockStreamIteratorBase* GetIteratorTree(const unsigned& blocksize);
	bool GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size=4096*1024);
	void Print(int level=0)const;
private:
	bool couldHashPruned(unsigned partition_id,const DataflowPartitioningDescriptor&)const;
	float predictSelectivity()const;
	void generateComparatorList(const Dataflow&);
	bool getcolindex(Dataflow dataflow);//get column index
private:
	LogicalOperator* child_;
	Condition condition_;
	vector<AttributeComparator> comparator_list_;
	std::vector<Expression> exprArray_;

	map<string,int>colindex_;
	vector<QNode *>qual_;
};

#endif /* FILTER_H_ */
