/*
 * Filter.h
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#ifndef FILTER_H_
#define FILTER_H_
#include <vector>
#include "LogicalOperator.h"
#include "../iterator/FilterIterator.h"
class Filter:public LogicalOperator {
public:
	class Condition{
	public:
		friend class Filter;
//		typedef FilterIterator::AttributeComparator::comparison comparison;
		Condition(){}
		Condition(const Condition& r);
		~Condition();
		void add(const Attribute& attr,const FilterIterator::AttributeComparator::comparison& com,const void*const& const_value);
		void add(const Attribute& attr,const FilterIterator::AttributeComparator::comparison& com,std::string str_exp);
		void print(int level=0)const;
	private:
		unsigned getCompaisonNumber()const;
	private:
		std::vector<Attribute> attribute_list_;
		std::vector<FilterIterator::AttributeComparator::comparison> comparison_list_;
		std::vector<void*> const_value_list_;
	};
	Filter(std::vector<FilterIterator::AttributeComparator> ComparatorList,LogicalOperator* child );
	Filter(const Condition& condition, LogicalOperator*  child);
	virtual ~Filter();
	Dataflow getDataflow();
	BlockStreamIteratorBase* getIteratorTree(const unsigned& blocksize);
	bool GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size=4096*1024);
	void print(int level=0)const;
private:
	bool couldHashPruned(unsigned partition_id,const DataflowPartitioningDescriptor&)const;
	float predictSelectivity()const;
	void generateComparatorList(const Dataflow&);
private:
	LogicalOperator* child_;
	Condition condition_;
	vector<FilterIterator::AttributeComparator> comparator_list_;


};

#endif /* FILTER_H_ */
