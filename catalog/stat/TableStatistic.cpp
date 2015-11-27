/*
 * TableStatistic.cpp
 *
 *  Created on: Feb 2, 2014
 *      Author: wangli
 */

#include "../../catalog/stat/TableStatistic.h"

TableStatistic::TableStatistic() {
	// TODO Auto-generated constructor stub

}

TableStatistic::~TableStatistic() {
	// TODO Auto-generated destructor stub
}
void TableStatistic::print()const{
	printf("Number of tuples:%d\n",number_of_tuples_);
}
void TableStatistic::addAttributeStatistics(const AttributeID& attr_id,AttributeStatistics* stat){
	if(att_stat_list_.find(attr_id)!=att_stat_list_.cend()){
		assert(false);
	}
	att_stat_list_[attr_id]=stat;
}
AttributeStatistics* TableStatistic::getAttributeStatistics(const AttributeID& attr_id)const{
	if(att_stat_list_.find(attr_id)==att_stat_list_.cend()){
		return 0;
	}
	return att_stat_list_.at(attr_id);
}
unsigned long TableStatistic::getCardinality()const{
	return number_of_tuples_;
}
