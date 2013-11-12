/*
 * ids.h
 *	Suggested by Liyongfeng and Zhanglei, all the ids are defined in
 *	this file or better code organization purposes.
 *  Created on: Nov 8, 2013
 *      Author: wangli
 */

#ifndef IDS_H_
#define IDS_H_
#include <boost/unordered_map.hpp>

typedef int NodeID;

typedef unsigned TableID;
typedef unsigned AttributeOffset;
typedef unsigned ProjectionOffset;
typedef unsigned ColumnOffset;
typedef unsigned PartitionOffset;

/*the following ids are based on the assumption that the TableOffset is globally unique.*/


/**
 * AttributeID: an attribute in a table has an unique AttributeID*/
struct AttributeID{
	AttributeID(TableID tid,AttributeOffset off):table_id(tid),offset(off){};
	TableID table_id;
	AttributeOffset offset;
	bool operator==(const AttributeID& r)const{
		return table_id==r.table_id&&offset==r.offset;
	}
};
/* for boost::unordered_map*/
//size_t hash_value(const AttributeID& key){
//	size_t seed=0;
//	boost::hash_combine(seed,boost::hash_value(key.offset));
//	boost::hash_combine(seed,boost::hash_value(key.table_id));
//	return seed;
//}


/**
 * ProjectionID: a projection has an unique ProjectionID
 */
struct ProjectionID{
	ProjectionID(){};
	ProjectionID(TableID tid,ProjectionOffset off):table_id(tid),projection_off(off){};
	TableID table_id;
	ProjectionOffset projection_off;
	bool operator==(const ProjectionID& r)const{
		return table_id==r.table_id&& projection_off==r.projection_off;
	}
};
/* for boost::unordered_map*/
//size_t hash_value(const ProjectionID& key){
//	size_t seed=0;
//	boost::hash_combine(seed,boost::hash_value(key.table_id));
//	boost::hash_combine(seed,boost::hash_value(key.projection_off));
//	return seed;
//}


/**
 * ColumnID: a Column corresponds to an attribute and is physically stored in one projection.
 */
struct ColumnID{
	ColumnID(){};
	ColumnID(ProjectionID pid,ColumnOffset off):projection_id(pid),column_off(off){};
	ProjectionID projection_id;
	ColumnOffset column_off;
	bool operator==(const ColumnID &r)const{
		return projection_id==r.projection_id&&column_off==r.column_off;
	}
};
/* for boost::unordered_map*/
//size_t hash_value(const ColumnID& key){
//	size_t seed=0;
//	boost::hash_combine(seed,hash_value(key.projection_id));
//	boost::hash_combine(seed,boost::hash_value(key.column_off));
//	return seed;
//}

/**
 * PartitionID: a partition corresponds to one projection.
 */
struct PartitionID{
	PartitionID(ProjectionID projection_id,PartitionOffset off):projection_id(projection_id),partition_off(off){};
	ProjectionID projection_id;
	PartitionOffset partition_off;
	bool operator==(const PartitionID& r)const{
		return projection_id==r.projection_id&&partition_off==r.partition_off;
	}
};
/* for boost::unordered_map*/
//size_t hash_value(const PartitionID& key){
//	size_t seed=0;
//	boost::hash_combine(seed,hash_value(key.projection_id));
//	boost::hash_combine(seed,boost::hash_value(key.partition_off));
//	return seed;
//}

#endif /* IDS_H_ */
