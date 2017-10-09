#include "../common/expression/data_type_oper.h"
#include "../common/Schema/Schema.h"

using claims::common::DataTypeOper;

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
 * physical_operator/exchange_merger.h
 *
 *  Created on: Sep 4, 2013
 *      Author: wangli, fangzhuhe
 *       Email: fzhedu@gmail.com
 *
 * Description:
 */
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <netdb.h>
#include <arpa/inet.h>
#include <glog/logging.h>
#include <iostream>
#include "../Environment.h"
#include "../common/Logging.h"
#include "../common/ids.h"
#include "../physical_operator/exchange_sender.h"

#define CONNECTION_VERIFY

namespace claims {
namespace physical_operator {
ExchangeSender::ExchangeSender() {}

ExchangeSender::~ExchangeSender() {}
/**
 * ask the upper merger about the socket port information, and build socket
 * connection with it. the socket port information is maintain by
 * ExchangeTracker at corresponding Node.
 */
bool ExchangeSender::ConnectToUpper(const ExchangeID& exchange_id,
                                    const NodeID& id, int& sock_fd) const {
  //  struct hostent* host;
  ExchangeTracker* et = Environment::getInstance()->getExchangeTracker();
  int upper_port;
  NodeAddress upper_addr;
  if (!(et->AskForSocketConnectionInfo(exchange_id, id, upper_addr))) {
    LOG(ERROR)
        << "Fails to ask Node for socket connection info, the exchange id= "
        << exchange_id.exchange_id << std::endl;
    return false;
  }
#ifdef CONNECTION_VERIFY
  stringstream ss;
  ss << "EXCHID" << exchange_id.exchange_id;
  string upper_passwd = ss.str();
  int upper_passwd_len = upper_passwd.length();
#endif
  if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket creation errors!\n");
    return false;
  }
  struct sockaddr_in serv_add;
  serv_add.sin_family = AF_INET;
  serv_add.sin_port = htons(atoi(upper_addr.port.c_str()));
  serv_add.sin_addr.s_addr = inet_addr(upper_addr.ip.c_str());
  bzero(&(serv_add.sin_zero), 8);

  int returnvalue;

  if ((returnvalue = connect(sock_fd, (struct sockaddr*)&serv_add,
                             sizeof(struct sockaddr))) == -1) {
    LOG(ERROR) << "Fails to connect remote socket: "
               << inet_ntoa(serv_add.sin_addr) << " , port= " << upper_addr.port
               << std::endl;
    return false;
  }
  LOG(INFO) << "exchid=" << exchange_id.exchange_id
		  << "upper_offset=" << exchange_id.partition_offset
		  << " connected to the upper socket.("<< upper_addr.ip.c_str() <<":"  << upper_addr.port.c_str()
		  << " sock_fd=" << sock_fd
		  <<") return value:" << returnvalue << std::endl;
#ifdef CONNECTION_VERIFY
  if ((returnvalue = send(sock_fd, upper_passwd.c_str(), upper_passwd_len, 0))  == -1 ) {
  	LOG(ERROR) << "Failed to send acknowledgement to the upper socket. returnvalue:[" << returnvalue 
		<< "] errno:[" << errno << "]";
	return false;
  }
  LOG(INFO) << "send acknowledgement to the upper socket: ("<< upper_passwd <<")" << std::endl;
  WaitingForNotification(sock_fd);
#endif
  return true;
}

void ExchangeSender::WaitingForNotification(const int& target_socket_fd) const {
  char byte;
  int recvbytes;
  if ((recvbytes = recv(target_socket_fd, &byte, sizeof(char), 0)) == -1) {
    LOG(ERROR) << "recv error!" << std::endl;
  }
  LOG(INFO) << "wait for connection acknowledge notification:" << byte;
}

void ExchangeSender::WaitingForCloseNotification(
    const int& target_socket_fd) const {
  char byte;
  int recvbytes;
  if ((recvbytes = recv(target_socket_fd, &byte, sizeof(char), 0)) == -1) {
    LOG(ERROR) << "sock_fd:" <<  target_socket_fd
    		<< " recv error!";
  } else {
    LOG(INFO) << "sock_fd:" <<  target_socket_fd
    		<< " received close message from one merger" << endl;
  }
  FileClose(target_socket_fd);
}

unsigned ExchangeSender::GetHashPartitionId(void* input_tuple, Schema* schema,
                                            unsigned partition_key_index,
                                            unsigned nuppers) {
  const void* hash_key_address =
      schema->getColumnAddess(partition_key_index, input_tuple);
  return DataTypeOper::partition_value_[schema->getcolumn(partition_key_index)
                                            .type][0](hash_key_address,
                                                      nuppers);
}
}  // namespace physical_operator
}  // namespace claims
