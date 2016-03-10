/*
 * IteratorExecutorMaster.h
 *
 *  Created on: Jun 21, 2013
 *      Author: wangli
 */

#ifndef ITERATOREXECUTORMASTER_H_
#define ITERATOREXECUTORMASTER_H_

#include <libconfig.h++>
#include <vector>
#include <string>
#include "../physical_operator/physical_operator_base.h"
#include "../common/Logging.h"
#include "../utility/Timer.h"

using claims::physical_operator::PhysicalOperatorBase;
class IteratorExecutorMaster {
 public:
  IteratorExecutorMaster();
  virtual ~IteratorExecutorMaster();
  static IteratorExecutorMaster* getInstance();
  bool ExecuteBlockStreamIteratorsOnSites(PhysicalOperatorBase* it,
                                          std::vector<std::string> ip_list);
  bool ExecuteBlockStreamIteratorsOnSite(PhysicalOperatorBase* it,
                                         NodeID target_ip);
  bool Propogation(const int, std::string target);

 private:
  static IteratorExecutorMaster* _instance;
};

#endif /* ITERATOREXECUTORMASTER_H_ */
