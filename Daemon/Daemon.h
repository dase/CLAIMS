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
#include "../common/Block/ResultSet.h"
#include "../utility/lock.h"
#include "../utility/lock_guard.h"

#define EXECUTED_RESULT_STATUS_OK 0
#define EXECUTED_RESULT_STATUS_ERROR 1
#define NEWSQLINTERFACE

using claims::utility::LockGuard;

struct remote_command {
  std::string cmd;
  int socket_fd;
};
struct ExecutedResult {
  enum { kWarningShowMaxCount = 64, kWarningShowMaxLength = 70000 };
  bool status_;  // ture is OK
  int fd_;
  ResultSet* result_;
  std::string error_info_;
  std::string info_;
  std::string warning_;
  int warning_count_;

  SpineLock append_lock_;

  ExecutedResult()
      : status_(0),
        fd_(-1),
        result_(NULL),
        error_info_(""),
        info_(""),
        warning_(""),
        warning_count_(0) {}
  void SetError(string err_info) {
    status_ = false;
    result_ = NULL;
    if ("" == error_info_) error_info_ = err_info;
    info_ = "";
  }

  void SetResult(string info, ResultSet* result) {
    status_ = true;
    result_ = result;
    info_ = info;
  }
  void AppendWarning(string warning_info) {
    if (++warning_count_ < kWarningShowMaxCount &&
        warning_.length() + warning_info.length() < kWarningShowMaxLength)
      warning_ += warning_info;
  }

  bool HasEnoughWarning() {
    return warning_count_ >= kWarningShowMaxCount ||
           warning_.length() >= kWarningShowMaxLength;
  }

  void AtomicAppendWarning(string warning_info) {
    LockGuard<SpineLock> guard(append_lock_);
    if (++warning_count_ < kWarningShowMaxCount &&
        warning_.length() + warning_info.length() < kWarningShowMaxLength)
      warning_ += warning_info;
  }
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
