#include <stack>

#include "../common/error_define.h"
#include "../common/Logging.h"
#include "../exec_tracker/segment_exec_status.h"
#include "../physical_operator/segment.h"
/*
 * Copyright [2012-2015] DaSE@ECNU
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * physical_operator/physical_exchange_merger.h
 *
 *  Created on: Aug 29, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "../physical_operator/exchange_merger.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libconfig.h++>
#include <assert.h>
#include <glog/logging.h>
#include <error.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "../common/ids.h"
#include "../physical_operator/physical_operator.h"
#include "../common/rename.h"
#include "../Config.h"
#include "../Environment.h"
#include "../Executor/exchange_tracker.h"
#include "../Executor/expander_tracker.h"
#include "../Executor/PortManager.h"
#include "../utility/maths.h"
#include "../utility/rdtsc.h"
#include "../physical_operator/exchange_sender_pipeline.h"
#include "../physical_operator/exchange_sender_materialized.h"
namespace claims {
namespace physical_operator {
const int kBufferSizeInExchange = 1000;
ExchangeMerger::ExchangeMerger(State state)
    : state_(state),
      all_merged_block_buffer_(NULL),
      block_for_deserialization(NULL),
      block_for_socket_(NULL),
      is_registered_to_tracker_(false),
      receiver_thread_id_(0),
      sock_fd_(-1),
      socket_port_(-1),
      socket_fd_lower_list_(NULL),
      epoll_fd_(-1) {
  set_phy_oper_type(kPhysicalExchangeMerger);
  InitExpandedStatus();

  assert(state.partition_schema_.partition_key_index < 100);
}
ExchangeMerger::ExchangeMerger()
    : all_merged_block_buffer_(NULL),
      block_for_deserialization(NULL),
      block_for_socket_(NULL),
      is_registered_to_tracker_(false),
      receiver_thread_id_(0),
      sock_fd_(-1),
      socket_port_(-1),
      socket_fd_lower_list_(NULL),
      epoll_fd_(-1) {
  InitExpandedStatus();
  set_phy_oper_type(kPhysicalExchangeMerger);
}
ExchangeMerger::~ExchangeMerger() {
  if (NULL != state_.schema_) {
    delete state_.schema_;
    state_.schema_ = NULL;
  }
  if (NULL != state_.child_) {
    delete state_.child_;
    state_.child_ = NULL;
  }
}
/**
 * note the serialized block's size is different from others, it has tail info.
 * exchange merger is at the end of one segment of plan, so it's the "stage_src"
 * for this stage
 */

bool ExchangeMerger::Open(SegmentExecStatus* const exec_status,
                          const PartitionOffset& partition_offset) {
  RETURN_IF_CANCELLED(exec_status);
  unsigned long long int start = curtick();
  RegisterExpandedThreadToAllBarriers();
  if (TryEntryIntoSerializedSection()) {  // first arrived thread dose
    exhausted_lowers = 0;
    this->partition_offset_ = partition_offset;
    lower_num_ = state_.lower_id_list_.size();
    socket_fd_lower_list_ = new int[lower_num_];
    for (int i = 0; i < lower_num_; ++i) {
      socket_fd_lower_list_[i] = -1;
    }
    // buffer all deserialized blocks come from every socket
    all_merged_block_buffer_ = new BlockStreamBuffer(
        state_.block_size_, kBufferSizeInExchange, state_.schema_);

    RETURN_IF_CANCELLED(exec_status);

    ExpanderTracker::getInstance()->addNewStageEndpoint(
        pthread_self(),
        LocalStageEndPoint(stage_src, "Exchange", all_merged_block_buffer_));
    // if one of block_for_socket is full, it will be deserialized into
    // block_for_deserialization and sended to all_merged_data_buffer
    block_for_deserialization =
        BlockStreamBase::createBlock(state_.schema_, state_.block_size_);

    // store block for each socket and the received block is serialized.
    block_for_socket_ = new BlockContainer* [lower_num_];
    for (unsigned i = 0; i < lower_num_; ++i) {
      block_for_socket_[i] = new BlockContainer(
          block_for_deserialization->getSerializedBlockSize());
    }

    RETURN_IF_CANCELLED(exec_status);

    if (PrepareSocket() == false) return false;
    if (SetSocketNonBlocking(sock_fd_) == false) {
      return false;
    }

    LOG(INFO) << "exchange_id = " << state_.exchange_id_
              << " partition_offset = " << partition_offset
              << " Open: exhausted lower senders num = " << exhausted_lowers
              << " lower sender num = " << lower_num_ << endl;

    if (RegisterExchange() == false) {
      LOG(ERROR) << "Register Exchange with ID = " << state_.exchange_id_
                 << " fails!" << endl;
    }
    is_registered_to_tracker_ = true;
    confirm_sender_time = 0;
    frequence = 0;
#ifdef ExchangeSender
    if (IsMaster()) {
      /*  According to a bug reported by dsc, the master exchange upper should
       * check whether other uppers have registered to exchangeTracker.
       * Otherwise, the lower may fail to connect to the exchangeTracker of some
       * uppers when the lower nodes receive the exchange lower, as some uppers
       *  have not register the exchange_id to the exchangeTracker.
       */
      LOG(INFO) << " exchange_id = " << state_.exchange_id_
                << " partition_offset = " << partition_offset
                << "Synchronizing...." << endl;
      IsOtherMergersRegistered();
      LOG(INFO) << " exchange_id = " << state_.exchange_id_
                << " partition_offset = " << partition_offset
                << " Synchronized! Then serialize and send its next segment "
                   "plan to all its lower senders" << endl;
      if (SerializeAndSendPlan() == false) return false;
    }
#endif

    RETURN_IF_CANCELLED(exec_status);

    if (CreateReceiverThread() == false) {
      return false;
    }
    if (!exec_status->is_cancelled()) {
      CreatePerformanceInfo();
    }
  }
  /// A synchronization barrier, in case of multiple expanded threads
  RETURN_IF_CANCELLED(exec_status);

  BarrierArrive();
  return true;
}
/**
 * return block from all_merged_block_buffer
 */
bool ExchangeMerger::Next(SegmentExecStatus* const exec_status,
                          BlockStreamBase* block) {
  while (true) {
    RETURN_IF_CANCELLED(exec_status);

    /*
     * As Exchange merger is a local stage beginner, ExchangeMerger::next will
     * return false in order to shrink the current work thread, if the
     * termination request is detected.
     */
    if (this->CheckTerminateRequest()) {
      LOG(INFO)
          << "<<<<<<<<<<<<<<<<<Exchange detected call back, signal!>>>>>>>>>>"
          << pthread_self() << ">>>>>>>>" << endl;
      return false;
    }

    if (sem_new_block_or_eof_.timed_wait(1)) {
      if (all_merged_block_buffer_->getBlock(*block)) {
        perf_info_->processed_one_block();
        return true;
      }
    }
    /*
     * Fix bug by checking whether the value of sem_new_block_or_eof_ is 0.
     * There is an extreme case that just after timed_wait(1) returned false
     * above, receiver thread executed sem_new_block_or_eof_.post() and
     * nexhausted_lowers++, which leads to nexhausted_lowers = nlowers but
     * actually at least one block is not handled , then this method return
     * false without handing the last block even the last few blocks.
     */
    if (exhausted_lowers == lower_num_ &&
        sem_new_block_or_eof_.get_value() == 0) {
      LOG(INFO) << "the value of sem_new_block_or_eof_ is :"
                << sem_new_block_or_eof_.get_value() << endl;
      LOG(INFO) << "now all lower are exhausted~~~~~~~~~~~" << endl;
      return false;
    }
  }
}

bool ExchangeMerger::Close(SegmentExecStatus* const exec_status) {
  LOG(INFO) << " exchange_merger_id = " << state_.exchange_id_ << " closed!"
            << " exhausted lower senders num = " << exhausted_lowers
            << " lower sender num = " << lower_num_ << endl;

  CancelReceiverThread();
  CloseSocket();
  if (NULL != block_for_socket_) {
    for (unsigned i = 0; i < lower_num_; i++) {
      if (NULL != block_for_socket_[i]) {
        delete block_for_socket_[i];
        block_for_socket_[i] = NULL;
      }
    }
  }
  if (NULL != block_for_deserialization) {
    delete block_for_deserialization;
    block_for_deserialization = NULL;
  }
  if (NULL != all_merged_block_buffer_) {
    delete all_merged_block_buffer_;
    all_merged_block_buffer_ = NULL;
  }

  /* rest the status of this iterator instance, such that the following calling
   * of open() and next() can act correctly.
   */
  ResetStatus();
  if (is_registered_to_tracker_) {
    Environment::getInstance()->getExchangeTracker()->LogoutExchange(
        ExchangeID(state_.exchange_id_, partition_offset_));
  }
  LOG(INFO) << "exchange merger id = " << state_.exchange_id_ << " is closed!"
		    << " CONFIRM frequence:" << frequence
		  	<< " CONFIRM TIME:" << confirm_sender_time;

  return true;
}

void ExchangeMerger::Print() {
  printf("Exchange upper[%ld]:", state_.exchange_id_);
  for (unsigned i = 0; i < state_.upper_id_list_.size(); i++) {
    printf("%d ", state_.upper_id_list_[i]);
  }
  printf("\nlower:");
  for (unsigned i = 0; i < state_.lower_id_list_.size(); i++) {
    printf("%d ", state_.lower_id_list_[i]);
  }
  if (state_.partition_schema_.mode == partition_schema::hash) {
    printf("Hash partition. ");
  } else {
    printf("Broadcast partition. ");
  }
  printf("Partition key index:%d",
         state_.partition_schema_.partition_key_index);
  printf("\n---------\n");
  state_.child_->Print();
}
bool ExchangeMerger::PrepareSocket() {
  struct sockaddr_in my_addr;
  struct sockaddr_in my_addr2;
  socklen_t len = sizeof(my_addr2);
  // sock_fd_ is the socket of this node
  if ((sock_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    LOG(ERROR) << "socket creation error!" << endl;
    return false;
  }
  my_addr.sin_family = AF_INET;

  /* apply for the port dynamically.*/
  /*  if ((socket_port_ = PortManager::getInstance()->applyPort()) == 0) {
      LOG(ERROR) << " exchange_id = " << state_.exchange_id_
                 << " partition_offset = " << partition_offset_
                 << " Fails to apply a port for the socket. Reason: the "
                    " PortManager is exhausted !" << endl;
      return false;
    }
    LOG(INFO) << " exchange_id = " << state_.exchange_id_
              << " partition_offset = " << partition_offset_
              << " succeed applying one port !" << endl;
  */
  my_addr.sin_port = htons(0);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(my_addr.sin_zero), 8);

  // Enable address reuse
  int on = 1;
  setsockopt(sock_fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  if (bind(sock_fd_, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) ==
      -1) {
    PLOG(ERROR) << " exchange_id = " << state_.exchange_id_
                << " partition_offset = " << partition_offset_
                << " bind errors!" << endl;
    return false;
  }

  if (getsockname(sock_fd_, (struct sockaddr*)&my_addr2, &len) == -1) {
    LOG(ERROR) << " exchange_id = " << state_.exchange_id_
               << " partition_offset = " << partition_offset_
               << " getsockname error!" << endl;
    return false;
  }
  socket_port_ = ntohs(my_addr2.sin_port);

  if (listen(sock_fd_, lower_num_) == -1) {
    LOG(ERROR) << " exchange_id = " << state_.exchange_id_
               << " partition_offset = " << partition_offset_
               << " listen errors!\n";
    return false;
  }
  LOG(INFO) << " exchange_id = " << state_.exchange_id_
            << " partition_offset = " << partition_offset_
            << " socket created as: sock_addr = " << inet_ntoa(my_addr.sin_addr)
            << " sock_port = " << socket_port_ << endl;

  return true;
}

void ExchangeMerger::CloseSocket() {
  /* close the epoll fd */
  if (epoll_fd_ > 2) {
    FileClose(epoll_fd_);
  }
  /* colse the sockets of the lowers*/
  if (socket_fd_lower_list_) {
    for (unsigned i = 0; i < lower_num_; i++) {
      if (socket_fd_lower_list_[i] > 2) {
        FileClose(socket_fd_lower_list_[i]);
      }
    }
  }

  for ( auto & fd : lower_sock_fd_list_ ) {
		LOG(INFO) << " exchange_id = " << state_.exchange_id_
					<< " partition_offset = " << partition_offset_
					<< "CloseSocket:" << fd;
		FileClose(fd);
	}
  /* close the socket of this exchange*/
  if (sock_fd_ > 2) {
    FileClose(sock_fd_);
  }
  /* return the applied port to the port manager*/
  //  if (socket_port_ > 0) {
  //    PortManager::getInstance()->returnPort(socket_port_);
  //  }
}

bool ExchangeMerger::RegisterExchange() {
  ExchangeTracker* et = Environment::getInstance()->getExchangeTracker();
  std::ostringstream port_str;
  port_str << socket_port_;
  return et->RegisterExchange(
      ExchangeID(state_.exchange_id_, partition_offset_), port_str.str());
}
/**
 * make sure each exchange merger at the same segment is registered, otherwise
 * result into bugs, one lower sender would fail to connect to one upper merger
 * due to it hasn't prepared.
 */
bool ExchangeMerger::IsOtherMergersRegistered() {
  ExchangeTracker* et = Environment::getInstance()->getExchangeTracker();
  for (unsigned i = 0; i < state_.upper_id_list_.size(); i++) {
    NodeID id = state_.upper_id_list_[i];
    /* Repeatedly ask node with ip for port information until the received port
     * is other than 0, which means
     * that the exchangeId on noede ip is registered to the exchangeTracker*/
    int wait_time_in_millisecond = 1;
    NodeAddress node_addr;
    while (!et->AskForSocketConnectionInfo(ExchangeID(state_.exchange_id_, i),
                                           id, node_addr)) {
      usleep(wait_time_in_millisecond);
      wait_time_in_millisecond =
          wait_time_in_millisecond < 200 ? wait_time_in_millisecond + 20 : 200;
    }
  }
}
bool ExchangeMerger::IsMaster() {
  if (0 == partition_offset_) {
    LOG(INFO) << "the master role of exchange_merger_id = "
              << state_.exchange_id_
              << " partition_offset = " << partition_offset_ << endl;
    return true;
  }
  return false;
}
/**
 * 1. for every next segment of plan, one ExchangeSenderPipeline or
 * ExchangeSenderMaterialized will be add at top of it;
 * 2. serialize it and send it to every lower senders.
 */
bool ExchangeMerger::SerializeAndSendPlan() {
  IteratorExecutorMaster* IEM = IteratorExecutorMaster::getInstance();
  // GETCURRENTTIME(start);
  if (Config::pipelined_exchange) {
    for (unsigned i = 0; i < state_.lower_id_list_.size(); i++) {
      ExchangeSenderPipeline::State EIELstate(
          state_.schema_->duplicateSchema(), state_.child_,
          state_.upper_id_list_, state_.block_size_, state_.exchange_id_,
          state_.partition_schema_);

      /* set the partition offset*/
      EIELstate.partition_offset_ = i;
      PhysicalOperatorBase* EIEL = new ExchangeSenderPipeline(EIELstate);

      LOG(INFO) << "exchange_merger_id = " << state_.exchange_id_
                << "send the serialized segment of plan";
      if (IEM->ExecuteBlockStreamIteratorsOnSite(
              EIEL, state_.lower_id_list_[i]) == false) {
        LOG(INFO) << " failed!" << endl;
        return false;
      }
      LOG(INFO) << " successfully!" << endl;
      (reinterpret_cast<ExchangeSenderPipeline*>(EIEL))->state_.child_ = NULL;
      delete EIEL;
    }
  } else {
    for (unsigned i = 0; i < state_.lower_id_list_.size(); i++) {
      ExchangeSenderMaterialized::State EIELstate(
          state_.schema_->duplicateSchema(), state_.child_,
          state_.upper_id_list_, state_.block_size_, state_.exchange_id_,
          state_.partition_schema_);
      /* set the partition offset*/
      EIELstate.partition_offset_ = i;
      PhysicalOperatorBase* EIEL = new ExchangeSenderMaterialized(EIELstate);
      LOG(INFO) << "exchange_merger_id = " << state_.exchange_id_
                << "send the serialized segment of plan";
      if (IEM->ExecuteBlockStreamIteratorsOnSite(
              EIEL, state_.lower_id_list_[i]) == false) {
        LOG(INFO) << " failed!" << endl;
        return false;
      }
      LOG(INFO) << " successfully!" << endl;
      (reinterpret_cast<ExchangeSenderMaterialized*>(EIEL))->state_.child_ =
          NULL;
      delete EIEL;
    }
  }
  // GetElapsedTime(start));
  return true;
}

bool ExchangeMerger::CreateReceiverThread() {
  int error = 0;
  error = pthread_create(&receiver_thread_id_, NULL, Receiver, this);
  if (0 != error) {
    PLOG(ERROR) << " exchange_id = " << state_.exchange_id_
                << " partition_offset = " << partition_offset_
                << " merger Failed to create receiver thread." << endl;
    return false;
  }
  return true;
}
void ExchangeMerger::CancelReceiverThread() {
  assert(receiver_thread_id_ != 0);
  pthread_cancel(receiver_thread_id_);
  void* res = 0;
  pthread_join(receiver_thread_id_, &res);
}

/**
 * first, block_for_socket_ for receive data from senders, then if one block is
 * enough, next serialize it and put it into all_merged_block_buffer.
 *  epoll is good at listening every coming block for different socket.
 *
 */
void* ExchangeMerger::Receiver(void* arg) {
  ExchangeMerger* Pthis = reinterpret_cast<ExchangeMerger*>(arg);
  struct epoll_event event;
  struct epoll_event* events;
  int status;
  stringstream ss;
  ss << "EXCHID" << Pthis->state_.exchange_id_;
  string lower_passwd = ss.str();
  // create epoll
  Pthis->epoll_fd_ = epoll_create1(0);
  if (Pthis->epoll_fd_ == -1) {
    LOG(ERROR) << " exchange_id = " << Pthis->state_.exchange_id_
               << " partition_offset = " << Pthis->partition_offset_
               << " merger fail to create epoll!" << endl;
    return NULL;
  }

  event.data.fd = Pthis->sock_fd_;
  event.events = EPOLLIN | EPOLLET;
  status = epoll_ctl(Pthis->epoll_fd_, EPOLL_CTL_ADD, Pthis->sock_fd_, &event);
  if (-1 == status) {
    LOG(ERROR) << " exchange_id = " << Pthis->state_.exchange_id_
               << " partition_offset = " << Pthis->partition_offset_
               << " merger fail to create epoll_ctl!" << endl;
    return NULL;
  }

  events = reinterpret_cast<epoll_event*>(
      calloc(Pthis->lower_num_, sizeof(epoll_event)));
  int fd_cur = 0;
  ticks start = curtick();
  std::vector<int> finish_times;  // in ms
  while (true) {
    usleep(1);
    pthread_testcancel();
    const int event_count =
        epoll_wait(Pthis->epoll_fd_, events, Pthis->lower_num_, -1);
    for (int i = 0; i < event_count; i++) {
      if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) ||
          (!(events[i].events & EPOLLIN))) {
        if ( errno == EINTR ) {
          continue;
        }
        LOG(WARNING) << " exchange_id = " << Pthis->state_.exchange_id_
                     << " partition_offset = " << Pthis->partition_offset_
                     << " epoll error=" << errno
                     << " reason: " << strerror(errno)
                     << " close fd = " << events[i].data.fd << endl;
        FileClose(events[i].data.fd);
        continue;
      } else if (Pthis->sock_fd_ == events[i].data.fd) {
        /* We have a notification on the listening socket, which means one or
         * more incoming connections.
         */
        while (true) {
          pthread_testcancel();

          sockaddr in_addr;
          socklen_t in_len;
          int infd;
          char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

          in_len = sizeof in_addr;
          infd = accept(Pthis->sock_fd_, &in_addr, &in_len);
          LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
                  << " partition_offset = " << Pthis->partition_offset_
				  << "After accept fd:" << infd;
          if (infd == -1) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
              /* all the incoming connections are processed.*/
              break;
            } else {
              LOG(WARNING) << " exchange_id = " << Pthis->state_.exchange_id_
                           << " partition_offset = " << Pthis->partition_offset_
                           << " epoll accept error, try again!" << endl;
              break;
            }
          }

// for debug useless now
#if 1
          status = getnameinfo(&in_addr, in_len, hbuf, sizeof(hbuf), sbuf,
                               sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
          if (0 == status) {

            LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
                      << " partition_offset = " << Pthis->partition_offset_
                      << " Accepted connection on descriptor " << infd
                      << " host= " << hbuf << " port= " << sbuf << endl;

            Pthis->lower_ip_list_.push_back(hbuf);
     	    Pthis->lower_fd_to_ip_[infd] = hbuf;
       //     Pthis->lower_sock_fd_to_id_[infd] =
       //        Pthis->lower_ip_list_.size() - 1;
     	    /*
            for (auto &it : Pthis->lower_fd_to_ip_) {
		  LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
            << " partition_offset = " << Pthis->partition_offset_
		    << it.second << " fd=" << it.first;
	       }
	
            for_each(Pthis->state_.lower_id_list_.begin(),
                     Pthis->state_.lower_id_list_.end(),
                     [=](const int s) { 

					 LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
							   << " partition_offset = " << Pthis->partition_offset_
			                   << " lower_id:"  << s << " "; });
     	     */

            assert(Pthis->lower_ip_list_.size() <=
                   Pthis->state_.lower_id_list_.size());
          }
#endif
          /*Make the incoming socket non-blocking and add it to the list of fds
           * to monitor.*/
          if (!Pthis->SetSocketNonBlocking(infd)) {
            return 0;
          }
          event.data.fd = infd;
          event.events = EPOLLIN | EPOLLET;
          status = epoll_ctl(Pthis->epoll_fd_, EPOLL_CTL_ADD, infd, &event);
          if (-1 == status) {
            LOG(ERROR) << " exchange_id = " << Pthis->state_.exchange_id_
                       << " partition_offset = " << Pthis->partition_offset_
                       << " epoll_ctl error2" << endl;
            return NULL;
          }
          Pthis->lower_fd_to_passwd_[infd] = "";
          LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
        		  << " partition_offset = " << Pthis->partition_offset_
				  << " After epoll ctl a connect fd:" << infd;
          /*
          for (auto &it : Pthis->lower_fd_to_passwd_) {
        	  LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
				  << " partition_offset = " << Pthis->partition_offset_
				  << " fd = " << it.first;
	       }*/
        }
        continue;
      } else {
        /* We have data on the fd waiting to be read.*/
        int done = 0;
        int byte_received = 0;
        char lower_passwd_buf[64];

        while (true) {
          pthread_testcancel();
#if 1 // verify connection passwd // old version code here
        ticks startconfirm = curtick();

  	  if ( Pthis->lower_sock_fd_list_.find(events[i].data.fd) ==  Pthis->lower_sock_fd_list_.end() ) {

  		  memset(lower_passwd_buf, 0, sizeof(lower_passwd_buf));

  		  int lower_passwd_size = lower_passwd.length();
  		  int rest_passwd_size = lower_passwd_size - Pthis->lower_fd_to_passwd_[events[i].data.fd].length();

          LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
              << " partition_offset = " << Pthis->partition_offset_
  			<< " DO CONFIRM THIS CONNECTION fd:[" << events[i].data.fd
              << "] lower_passwd_size:["<< lower_passwd_size
              << "] passwd:[" << Pthis->lower_fd_to_passwd_[events[i].data.fd]
              << "] passwd length:["<< Pthis->lower_fd_to_passwd_[events[i].data.fd].length()
              << "] rest_passwd_size:[" << rest_passwd_size << "]";

  	  	byte_received = read(events[i].data.fd, lower_passwd_buf, rest_passwd_size);

  		if (byte_received == -1 || byte_received == 0) {
  			LOG(WARNING) << "byte_received:" << byte_received << " error.";
  			break;
  		}

  		LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
              << " partition_offset = " << Pthis->partition_offset_
  			<< " fd:" << events[i].data.fd
  			<< " byte_received: [" << byte_received
  			<< "] lower_passwd_buf:[" << lower_passwd_buf << "]";

  		rest_passwd_size -= byte_received;
  		Pthis->lower_fd_to_passwd_[events[i].data.fd] += lower_passwd_buf;
  		if (rest_passwd_size > 0) {
      		  	continue;
  		}

  		if( lower_passwd.compare(Pthis->lower_fd_to_passwd_[events[i].data.fd]) == 0 ) {
              LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
              << " partition_offset = " << Pthis->partition_offset_
  				<< "this exchange pwd:[" << lower_passwd << "] and fd:[" << events[i].data.fd << "]'s passwd:[" << Pthis->lower_fd_to_passwd_[events[i].data.fd] << "] add this fd into lower_sock_fd_list_";

  			Pthis->lower_sock_fd_list_.insert(events[i].data.fd);
  			Pthis->lower_sock_fd_to_id_[events[i].data.fd] = Pthis->lower_sock_fd_list_.size() - 1;
  			Pthis->ReplyAllBlocksConsumed(events[i].data.fd);
  			/*
  			for ( auto & fd : Pthis->lower_sock_fd_list_ ) {
  				LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
              << " partition_offset = " << Pthis->partition_offset_
  					<< "Pthis->lower_sock_fd_list: " << fd;
  			}*/
  			assert(Pthis->lower_sock_fd_list_.size() <= Pthis->state_.lower_id_list_.size());
  	        } else {
           	  	LOG(WARNING) << " exchange_id = " << Pthis->state_.exchange_id_
           	  			<< " partition_offset = " << Pthis->partition_offset_
  						<< "this exchange pwd:[" << lower_passwd
  						<< "], Illegal connection passwd: [" << Pthis->lower_fd_to_passwd_[events[i].data.fd]
  						<< "], epoll del and close this connection";
           	  		epoll_ctl(Pthis->epoll_fd_, EPOLL_CTL_DEL, events[i].data.fd, &event);
          	        FileClose(events[i].data.fd);
  	               // break;
            	}

                }

          double endconfirm = getMilliSecond(startconfirm);
		  /*
		  LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
						  << " partition_offset = " << Pthis->partition_offset_
							<< " fd:" << events[i].data.fd
							<< " find fd time:" << endconfirm << "ms";
		  */
  		 Pthis->confirm_sender_time += endconfirm;
  		 Pthis->frequence++;

#endif
          int socket_fd_index = Pthis->lower_sock_fd_to_id_[events[i].data.fd];
          byte_received = read(
              events[i].data.fd,
              (reinterpret_cast<char*>(
                  Pthis->block_for_socket_[socket_fd_index]->getBlock())) +
                  Pthis->block_for_socket_[socket_fd_index]->GetCurSize(),
              Pthis->block_for_socket_[socket_fd_index]->GetRestSizeToHandle());
          if (byte_received == -1) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
              /*We have read all the data,so go back to the loop.*/
              break;
            }

            LOG(WARNING) << " exchange_id = " << Pthis->state_.exchange_id_
                         << " partition_offset = " << Pthis->partition_offset_
                         << " merger read error!" << endl;
            done = 1;
          } else if (byte_received == 0) {
            /* End of file. The remote has closed the connection.*/
            done = 1;
            break;
          }

          /* The data is successfully read.*/
		 // LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
         //   << " partition_offset = " << Pthis->partition_offset_ << events[i].data.fd << " receive DATA byte_received: [" << byte_received << "]";

          Pthis->block_for_socket_[socket_fd_index]->IncreaseActualSize(
              byte_received);
          if (Pthis->block_for_socket_[socket_fd_index]->GetRestSizeToHandle() >
              0) {
            /** the current block is not read entirely from the Sender, so
             * continue the loop to read.**/
            continue;
          }

          /** a block is completely read. **/

          /** deserialize the data block from Sender to the blockstreambase
           * (block_for_deserialization) **/
          Pthis->block_for_deserialization->deserialize(
              reinterpret_cast<Block*>(
                  Pthis->block_for_socket_[socket_fd_index]));

          /** mark block_for_socket_[socket_fd_index] to be empty so that it can
           * accommodate the subsequent data **/
          Pthis->block_for_socket_[socket_fd_index]->reset();

          /**
           * In the current implementation, a empty block stream means
           * End-Of-File
           */
          const bool eof = Pthis->block_for_deserialization->Empty();
          if (!eof) {
            /** the newly obtained data block is validate, so we insert it into
             * the all_merged_block_buffer_ and post sem_new_block_or_eof_ so
             * that all the threads waiting for the semaphore continue.
             */
            Pthis->all_merged_block_buffer_->insertBlock(
                Pthis->block_for_deserialization);

            //??? why is all ,not 1
            // multiple threads will still compete with lock
            Pthis->sem_new_block_or_eof_.post(
                Pthis->number_of_registered_expanded_threads_);
          } else {
/** The newly obtained data block is the end-of-file.  **/
            LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
                      << " partition_offset = " << Pthis->partition_offset_
                      << " This block is the last one." << endl;
            finish_times.push_back(static_cast<int>(getMilliSecond(start)));

            /** update the exhausted senders count and post
             *sem_new_block_or_eof_ so that all the
             * threads waiting for the semaphore continue.
             **/
            Pthis->exhausted_lowers++;
            Pthis->sem_new_block_or_eof_.post(
                Pthis->number_of_registered_expanded_threads_);

            if (Pthis->exhausted_lowers == Pthis->lower_num_) {
              /*
               * When all the exchange lowers are exhausted, notify the
               * all_merged_block_buffer_
               * that the input data is completely received.
               */
              Pthis->all_merged_block_buffer_->setInputComplete();

              /* print the finish times */
               for (unsigned i = 0; i < finish_times.size(); i++)
               {
                  //printf("%d\t", finish_times[i]);
            	   LOG(INFO) << "FINISH TIMES:" << finish_times[i];
               }
               //printf("\t Var:%5.4f\n", get_stddev(finish_times));
               LOG(INFO) << "Var:" << get_stddev(finish_times);
            }

            LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
                      << " partition_offset = " << Pthis->partition_offset_
                      << " exhausted lowers = " << Pthis->exhausted_lowers
                      << " senders have exhausted" << endl;

            /** tell the Sender that all the block are consumed so that the
             * Sender can close the socket**/
            pthread_testcancel();

            Pthis->ReplyAllBlocksConsumed(events[i].data.fd);
#ifdef GLOG_STATUS
            LOG(INFO)
                << " exchange_id = " << Pthis->state_.exchange_id_
                << " partition_offset = " << Pthis->partition_offset_
				<< " fd = " << events[i].data.fd
                << " This notification (all the blocks in the socket buffer "
                   "are consumed) is replied to the lower ";
               // << Pthis->lower_ip_list_[socket_fd_index] << endl;
#endif
        }
      }
        if (done) {

#ifdef GLOG_STATUS
          LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
                    << " partition_offset = " << Pthis->partition_offset_
                    << " Closed connection on descriptor " << events[i].data.fd
                    << " "
                    << Pthis->lower_ip_list_
                           [Pthis->lower_sock_fd_to_id_[events[i].data.fd]];

          /* Closing the descriptor will make epoll remove it
           from the set of descriptors which are monitored. */
          LOG(INFO) << " exchange_id = " << Pthis->state_.exchange_id_
                    << " partition_offset = " << Pthis->partition_offset_
                    << " Closed connection on descriptor " << events[i].data.fd;
#endif

          //Pthis->lower_sock_fd_list_.erase(events[i].data.fd);
          epoll_ctl(Pthis->epoll_fd_, EPOLL_CTL_DEL, events[i].data.fd, &event);
          //FileClose(events[i].data.fd);

        }
      }
    }
  }
}

void ExchangeMerger::SendBlockBufferedNotification(int target_socket_fd) {
  char content = 'c';
  if (send(target_socket_fd, &content, sizeof(char), 0) == -1) {
    LOG(ERROR) << " exchange_id = " << state_.exchange_id_
               << " partition_offset = " << partition_offset_
               << " send block buffer notifi error!" << endl;
    return;
  }
}
void ExchangeMerger::ReplyAllBlocksConsumed(int target_socket_fd) {
  char content = 'e';
  if (send(target_socket_fd, &content, sizeof(char), MSG_WAITALL) == -1) {
    PLOG(ERROR) << " exchange_id = " << state_.exchange_id_
               << " partition_offset = " << partition_offset_
               << " merger reply all blocks consumed error! fd:" << target_socket_fd
			   << endl;
    return;
  }
}

bool ExchangeMerger::SetSocketNonBlocking(int socket_fd) {
  int flags, s;

  flags = fcntl(socket_fd, F_GETFL, 0);
  if (flags == -1) {
    LOG(ERROR) << " exchange_id = " << state_.exchange_id_
               << " partition_offset = " << partition_offset_
               << " merger fcntl error!" << endl;
    return false;
  }

  flags |= O_NONBLOCK;
  s = fcntl(socket_fd, F_SETFL, flags);
  if (s == -1) {
    LOG(ERROR) << " exchange_id = " << state_.exchange_id_
               << " partition_offset = " << partition_offset_
               << " merger fcntl error!" << endl;
    return false;
  }
  return true;
}

void ExchangeMerger::CreatePerformanceInfo() {
  perf_info_ =
      ExpanderTracker::getInstance()->getPerformanceInfo(pthread_self());
  perf_info_->initialize();
}

void ExchangeMerger::ResetStatus() {
  /* reset the expanded status such that this iterator instance will act
   * correctly,
   * if open() and next() are called again. */
  InitExpandedStatus();

  lower_sock_fd_to_id_.clear();
  lower_ip_list_.clear();
  lower_sock_fd_list_.clear();
}
RetCode ExchangeMerger::GetAllSegments(stack<Segment*>* all_segments) {
  RetCode ret = rSuccess;
  PhysicalOperatorBase* ret_plan = NULL;
  if (NULL != state_.child_) {
    state_.child_->GetAllSegments(all_segments);
    if (Config::pipelined_exchange) {
      ExchangeSenderPipeline::State EIELstate(
          state_.schema_->duplicateSchema(), state_.child_,
          state_.upper_id_list_, state_.block_size_, state_.exchange_id_,
          state_.partition_schema_);
      ret_plan = new ExchangeSenderPipeline(EIELstate);
    } else {
      ExchangeSenderMaterialized::State EIELstate(
          state_.schema_->duplicateSchema(), state_.child_,
          state_.upper_id_list_, state_.block_size_, state_.exchange_id_,
          state_.partition_schema_);
      ret_plan = new ExchangeSenderMaterialized(EIELstate);
    }
    all_segments->push(new Segment(ret_plan, state_.lower_id_list_,
                                   state_.upper_id_list_, state_.exchange_id_));
    state_.child_ = NULL;
  }
  return ret;
}
}  // namespace physical_operator
}  // namespace claims
