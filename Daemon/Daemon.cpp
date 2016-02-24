/*
 * Daemon.cpp
 *
 *  Created on: Feb 20, 2014
 *      Author: wangli
 */

#include "Daemon.h"

#include <pthread.h>
#include <glog/logging.h>

#include "./Executing.h"
#include "../Client/ClaimsServer.h"
#include "../stmt_handler/stmt_handler.h"

using claims::stmt_handler::StmtHandler;
#define WORK_THREAD_COUNT 1

Daemon* Daemon::instance_ = 0;
Lock* Daemon::lock_ = new Lock();

Daemon* Daemon::getInstance() {
  /*
   *  new Daemon() will create worker threads that call Daemon::getInstance(),
   *  but at that time, instance_ may still be 0/NULL, so instance_ will
   *  be assigned many times in different threads.
   *  A lock helps instance_ be assigned before child threads arrive here.
   */
  lock_->acquire();
  if (instance_ == 0) {
    instance_ = new Daemon();
  }
  lock_->release();
  return instance_;
}

Daemon::Daemon() {
  // TODO Auto-generated constructor stub
  // TODO: create work threads
  for (unsigned i = 0; i < WORK_THREAD_COUNT; i++) {
    pthread_t tid;
    const int error = pthread_create(&tid, NULL, worker, NULL);
    if (error != 0) {
      std::cout << "cannot create thread!" << std::endl;
      return;
    }
  }
}

Daemon::~Daemon() {
  // TODO Auto-generated destructor stub
}

/*
 void* Daemon::worker(void* para) {
 Daemon* pthis = (Daemon*) para;
 while (true) {

 remote_command rc = Daemon::getInstance()->getRemoteCommand();

 //assume all commands are sql commands.
 ExecutedResult result;
 std::string error_info;
 Node* node;
 ResultSet* result_set = Executing::ru    delete
 stmt_handler;n_sql(std::string(rc.cmd),error_info);

 if(result_set==0){
 printf("-Worker: add error result!\n");
 result.error_info = error_info;
 result.result = 0;
 result.status_ = EXECUTED_RESULT_STATUS_ERROR;
 result.fd = rc.socket_fd;
 }
 else{
 printf("-Worker: add ok result!\n");
 result.status_ = EXECUTED_RESULT_STATUS_OK;
 result.result = result_set;
 result.fd = rc.socket_fd;
 }


 printf("-Worker add result into the queue!\n");
 pthis->addExecutedResult(result);

 //		pthis->;
 }
 }
 */

void* Daemon::worker(void* para) {
  //  Daemon* pthis = (Daemon*) para;
  while (true) {
    remote_command rc = Daemon::getInstance()->getRemoteCommand();

    // assume all commands are sql commands.
    ExecutedResult result;
    result.fd_ = rc.socket_fd;
    result.status_ = true;

    // result is a pointer, which now is NULL and should be assigned in
    // function.

    ClientListener::checkFdValid(result.fd_);
#ifndef NEWSQLINTERFACE
    Executing::run_sql(rc.cmd, result.result, result.status_, result.error_info,
                       result.info_, result.fd);
    ClientLogging::log(
        "after running sql, the result is : status_-%d, err-%s, info-%s",
        result.status_, result.error_info.c_str(), result.info_.c_str());
#else
    StmtHandler* stmt_handler = new StmtHandler(rc.cmd);
    stmt_handler->Execute(&result);
    LOG(INFO) << "the result of after running sql: " << rc.cmd
              << " status_: " << result.status_
              << "error info: " << result.error_info_
              << " info: " << result.info_ << endl;

#endif
    ClientListener::checkFdValid(result.fd_);
    printf("-Worker add result into the queue!\n");
    Daemon::getInstance()->addExecutedResult(result);
    delete stmt_handler;
  }
  return NULL;
}

void Daemon::addRemoteCommand(const remote_command& rc) {
  lock_->acquire();
  remote_command_queue_.push_back(rc);
  lock_->release();
  semaphore_command_queue_.post();
  //  cout<<"post command queue semaphore"<<endl;
}
remote_command Daemon::getRemoteCommand() {
  semaphore_command_queue_.wait();
  //  cout<<"minus command queue semaphore "<<endl;
  remote_command ret;
  lock_->acquire();
  ret = remote_command_queue_.front();
  remote_command_queue_.pop_front();
  lock_->release();
  return ret;
}

ExecutedResult Daemon::getExecutedResult() {
  semaphore_result_queue_.wait();
  LOG(INFO) << "semaphore_result_queue_ waited" << endl;
  ExecutedResult ret;
  lock_->acquire();
  ret = executed_result_queue_.front();
  executed_result_queue_.pop_front();
  LOG(INFO) << "got result into result queue and deleted" << endl;
  lock_->release();
  return ret;
}

void Daemon::addExecutedResult(const ExecutedResult& item) {
  lock_->acquire();
  executed_result_queue_.push_back(item);
  LOG(INFO) << "pushed result into result queue" << endl;
  lock_->release();
  semaphore_result_queue_.post();
  LOG(INFO) << "semaphore_result_queue_ posted" << endl;
}
