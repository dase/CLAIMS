/*
 * Filter.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: wangli
 */

#include "Filter.h"

Filter::Filter(std::vector<FilterIterator::AttributeComparator> ComparatorList,LogicalOperator* child )
:comparator_list_(ComparatorList),child_(child){

}

Filter::~Filter() {
	// TODO Auto-generated destructor stub
}

Dataflow Filter::getDataflow(){
	return child_->getDataflow();
}
