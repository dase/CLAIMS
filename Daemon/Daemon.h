/*
 * Daemon.h
 *
 *  Created on: Feb 20, 2014
 *      Author: wangli
 */

#ifndef DAEMON_H_
#define DAEMON_H_
#include <vector>
#include <string>
#include <list>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../common/Block/ResultSet.h"
#include "../utility/lock.h"

#define EXECUTED_RESULT_STATUS_OK 0
#define EXECUTED_RESULT_STATUS_ERROR 1

struct remote_command {
  std::string cmd;
  int socket_fd;
};
struct ExecutedResult {
  bool status_;  // ture is OK
  int fd_;
  ResultSet* result_;
  std::string error_info_;
  std::string info_;
  std::string warning_;
  void SetError(string err_info, string warning_info = "") {
    status_ = false;
    result_ = NULL;
    if ("" == err_info) error_info_ = err_info;
    info_ = "";
    warning_ = warning_info;
  }

  void SetResult(string info, ResultSet* result) {
    status_ = true;
    result_ = result;
    info_ = info;
  }
  void AppendWarning(string warning_info) { warning_ += warning_info; }
};
class Daemon {
 public:
  static Daemon* getInstance();
  static void* worker(void*);

  void addRemoteCommand(const remote_command& rc);

  ExecutedResult getExecutedResult();

 private:
  Daemon();
  virtual ~Daemon();
  remote_command getRemoteCommand();
  void addExecutedResult(const ExecutedResult& item);

 private:
  static Daemon* instance_;
  std::list<remote_command> remote_command_queue_;
  semaphore semaphore_command_queue_;

  std::list<ExecutedResult> executed_result_queue_;
  semaphore semaphore_result_queue_;

  static Lock* lock_;
};

#endif /* DAEMON_H_ */
