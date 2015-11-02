/*
 * physical_operator.h
 *
 *  Created on: Mar 8, 2014
 *      Author: wangli
 */

#ifndef EXPANDABLEBLOCKSTREAMITERATORBASE_H_
#define EXPANDABLEBLOCKSTREAMITERATORBASE_H_
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <string>

#include <assert.h>
#include <boost/unordered/unordered_map.hpp>

#include "../physical_operator/physical_operator_base.h"
#include "../common/Block/BlockStream.h"
#include "../Catalog/Partitioner.h"
#include "../utility/lock.h"
#include "../common/hashtable.h"

using boost::unordered::unordered_map;
namespace claims {
namespace physical_operator {
class ThreadContext {
 public:
  virtual ~ThreadContext() {}
  int32_t get_locality_() const { return locality_; }
  void set_locality_(int32_t locality) { locality_ = locality; }

 private:
  /* the id of the core who creates the context*/
  int32_t locality_;
};
typedef int barrier_number;
typedef int serialized_section_number;
class PhysicalOperator : public PhysicalOperatorBase {
 public:
  /*
   * A thread can choose one from the following strategies when initializing a
   * context
   * (1) crm_no_reuse: the thread always creates a new context whenever.
   * (2) crm_core_sensitive: the thread first try to find and reuse a free
   * context allocated by a thread located
   * 		on the same core. If there isn't any, a new context is created.
   * (3) crm_numa_sensitive: a free context can only be reused by a thread if
   * the context is created by a core
   *  		within the same NUMA socket.
   * (4) crm_anyway: a free context will be reused whenever possible.
   */
  enum context_reuse_mode {
    crm_no_reuse,
    crm_core_sensitive,
    crm_numa_sensitive,
    crm_anyway
  };

  PhysicalOperator(unsigned number_of_barrier = 1,
                   unsigned number_of_seriliazed_section = 1);
  virtual ~PhysicalOperator();
  /**
   * Because that boost::serialization will allocate an instance of this class
   * when deserializing, and hence the following three virtual method cannot be
   * pure.
   */
  virtual bool Open(const PartitionOffset& part_off = 0) { assert(false); };
  virtual bool Next(BlockStreamBase*) { assert(false); };
  virtual bool Close() { assert(false); };
  virtual void Print() { printf("??\n"); };

  /** As different elastic iterators differs from each other in the structure of
   * the context
   * and the way to construct the context, this function should be implemented
   * explicitly
   * by each elastic iterator..
   */
  virtual ThreadContext* CreateContext() { assert(false); };

 protected:
  ThreadContext* CreateOrReuseContext(context_reuse_mode crm);

  /* this function initialize the state_ of expanded iterator.
   * Should be called in the constructor and close() of the expanded iterator
   * implementation.
   */
  void InitExpandedStatus();

  /* Return true, if not any thread has obtain the entry into the serialized
   * section
   * with id=tserialized_section_id; otherwise, return false*/
  bool TryEntryIntoSerializedSection(unsigned serialized_section_id = 0);

  /*
   * Register to all the barriers that a new thread has been registered.
   * Accordingly, barriers
   * could increase the expected number of threads by 1.
   * Note: all iterator using barrier should call this function at the beginner
   * of open()
   */
  void RegisterExpandedThreadToAllBarriers();

  /*
   * When an expanded thread is about to exit, call this method before exit to
   * remove the count
   * in barriers.
   * Please note that
   */
  void UnregisterExpandedThreadToAllBarriers(unsigned barrier_index = 0);

  void BarrierArrive(unsigned barrier_index = 0);

  void InitContext(ThreadContext* tc);
  ThreadContext* GetContext();

  /*
   * This method is called when a thread wants its and all the others' context
   */
  void DestoryAllContext();

  //	/*
  //	 * This method is call when a thread wants to destroy its own context
  //	 */
  //	void destorySelfContext();

  bool CheckTerminateRequest();

  /*
   * This function may be called by multiple expanded threads to update the
   * return value
   * for the open function.
   * If any error happens at any error, the open_ret_ should be false.
   */
  void SetReturnStatus(bool ret);

  bool GetReturnStatus() const;

 private:
  ThreadContext* GetFreeContext(context_reuse_mode crm);

 protected:
  unsigned number_of_registered_expanded_threads_;

 private:
  /* the return value of open() */
  volatile bool open_ret_;

  pthread_mutex_t sync_lock_;
  pthread_cond_t sync_cv_;

  /* the semaphore used to guarantee that only the first thread does the real
   * initialization work.*/
  semaphore* seriliazed_section_entry_key_;

  Barrier* barrier_;

  unsigned number_of_barrier_;
  unsigned number_of_seriliazed_section_;

  Lock lock_number_of_registered_expanded_threads_;

  /* this list maintain the contexts that are currently in use. */
  boost::unordered_map<pthread_t, ThreadContext*> context_list_;

  std::vector<ThreadContext*> free_context_list_;

  Lock context_lock_;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& boost::serialization::base_object<PhysicalOperatorBase>(*this);
  }
};
}  // namespace physical_operator
}  // namespace claims
#endif  //  EXPANDABLEBLOCKSTREAMITERATORBASE_H_
