/*
 * EqualJoin.h
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#ifndef EQUALJOIN_H_
#define EQUALJOIN_H_
#include <vector>
#include "LogicalOperator.h"
#include "../Catalog/Attribute.h"
class EqualJoin:public LogicalOperator {
	struct JoinPair{
		JoinPair(Attribute &a,Attribute & b):first(a),second(b){};
		Attribute first;
		Attribute second;
	};
public:
	EqualJoin(std::vector<JoinPair>,LogicalOperator* left_input,LogicalOperator* right_input);
	virtual ~EqualJoin();
	Dataflow getDataflow();

private:
	bool isHashOnLeftKey(const Partitioner& part,const Attribute& key)const;
private:
	std::vector<JoinPair> joinkey_pair_list_;
	LogicalOperator* left_child_;
	LogicalOperator* right_child_;
};

#endif /* EQUALJOIN_H_ */
