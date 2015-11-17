///*
// * LogicalIndexFilter.h
// *
// *  Created on: Mar 28, 2014
// *      Author: scdong
// */
//
//#ifndef LOGICALINDEXFILTER_H_
//#define LOGICALINDEXFILTER_H_
//
//#include "../logical_operator/logical_operator.h"
//#include "../iterator/FilterIterator.h"
//#include "CSBPlusTree.h"
//
//class LogicalIndexFilter :public LogicalOperator {
//	public:
//	class Condition{
//	public:
//		friend class LogicalIndexFilter;
//		Condition(){}
//		Condition(const Condition& r);
//		~Condition();
//		void add(const Attribute& attr,const comparison& com,const void*const& const_value);
//		void add(const Attribute& attr,const comparison& com,std::string str_exp);
//		void print(int level=0)const;
//	private:
//		unsigned getCompaisonNumber()const;
//	private:
//		std::vector<Attribute> attribute_list_;
//		std::vector<comparison> comparison_list_;
//		std::vector<void*> const_value_list_;
//	};
//
//public:
//	LogicalIndexFilter(const Condition& condition, LogicalOperator* child);
//	virtual ~LogicalIndexFilter();
//
//	Dataflow getDataflow();
//	BlockStreamIteratorBase* getIteratorTree(const unsigned& blocksize);
//	bool GetOptimalPhysicalPlan(Requirement requirement,PhysicalPlanDescriptor& physical_plan_descriptor, const unsigned & block_size=4096*1024);
//	void print(int level=0)const;
//
//private:
//	void generateComparatorList(const Dataflow& dataflow);
//
//private:
//	LogicalOperator* child_;
//	Condition condition_;
//	vector<FilterIterator::AttributeComparator> comparator_list_;
//};
//
//#endif /* LOGICALINDEXFILTER_H_ */
