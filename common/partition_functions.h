/*
 * partition_functions.h
 *
 *  Created on: Sep 22, 2014
 *      Author: wangli
 */

#ifndef PARTITION_FUNCTIONS_H_
#define PARTITION_FUNCTIONS_H_
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>

struct partition_schema {
  enum partition_mode { hash, broadcast, range };
  partition_mode mode;
  unsigned partition_key_index;
  static partition_schema set_hash_partition(int _partition_key_index = 0) {
    partition_schema ret;
    ret.mode = hash;
    ret.partition_key_index = _partition_key_index;
    return ret;
  }
  static partition_schema set_broadcast_partition() {
    partition_schema ret;
    ret.mode = broadcast;
    ret.partition_key_index = 0;
    return ret;
  }
  static partition_schema set_range_partition(int _number_of_ranges) {
    assert(false);
    return partition_schema();
  }
  bool isHashPartition() const { return mode == hash; }
  bool isBroadcastPartition() const { return mode == broadcast; }

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& mode& partition_key_index;
  }
};

#endif /* PARTITION_FUNCTIONS_H_ */
