/*
 * Client.cpp
 *
 *  Created on: Sep 25, 2014
 *      Author: wangli
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include "./startup.h"
#include "Client/ClientResponse.h"
#include "Client/Client.h"
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "common/log/logging.h"
#include "common/Block/ResultSet.h"
#include "utility/command_line.h"
#include "utility/rdtsc.h"

void readStrigFromTerminal(string& input) {
  while (true) {
    std::cin.clear();
    std::cin.sync();
    std::string str;
    if (getline(std::cin, str)) {
      bool finish = false;
      for (unsigned i = 0; i < str.length(); i++) {
        if (str[i] == ';') {
          input += str.substr(0, i + 1);
          finish = true;
          break;
        }
      }
      if (finish) break;
      input += str + " ";
    }
  }
}

void submit_command(Client& client, std::string& command) {
  ResultSet rs;
  std::string message;
  switch (client.submit(command, message, rs)) {
    case Client::result:
      rs.print();
      //				if(i!=0)
      //					total_time+=rs.query_time_;
      break;
    case Client::message:
      printf("%s", message.c_str());
      break;
    case Client::error:
      printf(
          "\e[0;31m"
          "%s\033[0m\n",
          message.c_str());
      break;
    default:
      assert(false);
      break;
  }
}

void submit_command_repeated(Client& client, std::string& command,
                             int repeated) {
  double total_time = 0;
  for (int i = 0; i < repeated; i++) {
    ResultSet rs;
    std::string message;
    switch (client.submit(command, message, rs)) {
      case Client::result:
        if (i != 0) total_time += rs.query_time_;
        break;
      case Client::message:
        printf("%s", message.c_str());
        break;
      case Client::error:
        printf(
            "\e[0;31m"
            "%s\033[0m\n",
            message.c_str());
        break;
      default:
        assert(false);
        break;
    }
  }
}
void PrintUsage() {
  cout << "Welcome to use CLAIMS. " << endl;
  cout << "Type: " << endl;
  cout << "\t help; for usage of CLAIMS." << endl;
  cout << "\t copyright; for distribution terms." << endl;
  cout << "\t exit; or shutdown; for exiting CLAIMS." << endl;
}

void PrintCopyright() {
  cout << "Copyright [2012-2015] DaSE@ECNU " << endl
       << " Licensed to the Apache Software Foundation (ASF) under one or more"
       << " contributor license agreements.  See the NOTICE file distributed "
          "with"
       << " this work for additional information regarding copyright ownership."
       << " The ASF licenses this file to You under the Apache License, "
          "Version 2.0"
       << " (the \" License\"); you may not use this file except in compliance "
          "with"
       << " the License.  You may obtain a copy of the License at" << endl
       << "     http://www.apache.org/licenses/LICENSE-2.0" << endl
       << " Unless required by applicable law or agreed to in writing, software"
       << " distributed under the License is distributed on an \" AS IS \" "
          "BASIS,"
       << " WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or "
          "implied."
       << " See the License for the specific language governing permissions and"
       << " limitations under the License." << endl;
}
int main(int argc, char** argv) {
  /* Client */

  if (argc != 3) {
    printf(
        "argc=%d, Illegal input. \nPlease use client master_ip "
        "client_listener_port.\n",
        argc);
    printf(
        "HINT: the master ip and the client_listener_port can be found in the "
        "configure file.\n");
    return 0;
  }

  claims::common::Logging claims_logging(argv[0]);
  print_welcome();
  PrintUsage();

  Client client;
  client.connection(argv[1], atoi(argv[2]));
  std::cout << std::endl;
  init_command_line();

  while (1) {
    std::string command, message;

    get_one_command(command);

    command = trimSpecialCharactor(command);

    if (command == "exit;" || command == "shutdown;") {
      break;
    } else if (command.empty()) {
      continue;
    } else if (command == "help;") {
      PrintUsage();
      continue;
    } else if (command == "copyright;") {
      PrintCopyright();
      continue;
    }

    submit_command(client, command);

    /*
     * the following command execute the query for a given time and p
     * rint the averaged query response time.*/
    //		submit_command_repeated(client,command,50);
  }
  client.shutdown();
}
