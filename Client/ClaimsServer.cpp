/*
 * ClaimsServer.cpp
 *
 *  Created on: Feb 20, 2014
 *      Author: volt
 */

#include "ClaimsServer.h"

#include <malloc.h>
#include <arpa/inet.h>
#include <glog/logging.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include "../catalog/catalog.h"
#include "../Daemon/Daemon.h"
#include "../common/Logging.h"

// ClientListener::standard_input = dup(STD)
int ClientListener::standard_input = dup(STDIN_FILENO);
int ClientListener::standard_output = dup(STDOUT_FILENO);
int ClientListener::standard_err = dup(STDERR_FILENO);

ClientListener::ClientListener(int port) {
  m_num = 0;
  m_clientFds = new int[MAXCONN];  // new
  for (int i = 0; i < MAXCONN; ++i) {
    m_clientFds[i] = -1;
  }
  m_fd = -1;
  m_port = port;
}

ClientListener::~ClientListener() { delete[] m_clientFds; }

int ClientListener::addClient(const int fd) {
  if (m_num == MAXCONN) {
    perror("Server has the maximum connection");
    return -1;
  }

  for (int i = 0; i < MAXCONN; ++i) {
    if (m_clientFds[i] < 0) {
      m_clientFds[i] = fd;
      ++m_num;
      break;
    }
  }
  return 0;
}

int ClientListener::removeClient(const int fd) {
  int ret = -1;
  for (int i = 0; i < MAXCONN; ++i) {
    if (m_clientFds[i] == fd) {
      m_clientFds[i] = -1;
      --m_num;
      ret = 0;
      break;
    }
  }
  return ret;
}

int ClientListener::receiveRequest(const int fd, const char *cmd) {
  remote_command rcmd;
  rcmd.socket_fd = fd;
  //	rcmd.cmd.append(cmd);
  cout << cmd << endl;
  rcmd.cmd = std::string(cmd);

  Daemon::getInstance()->addRemoteCommand(rcmd);
  return 0;
}

// void ClientListener::backupStd() {
//	// backup the input and output--Yu
//	dup2(STDIN_FILENO, standard_input);
//	dup2(STDOUT_FILENO, standard_output);
//	dup2(STDERR_FILENO, standard_err);
//}

void ClientListener::restoreStd() {
  // backup the input and output--Yu
  dup2(standard_input, STDIN_FILENO);
  dup2(standard_output, STDOUT_FILENO);
  dup2(standard_err, STDERR_FILENO);
}

void ClientListener::checkFdValid(int fd) {
  // check whether the file descriptor is closed
  int status = fcntl(fd, F_GETFL);
  if (-1 == status) {
    ClientLogging::elog("fd %d is invalid.%s", fd, strerror(errno));
  } else {
    ClientLogging::log("fd %d is valid.", fd);
  }
}
/**
 * configure the socket parameter, bind to the specific ip/port
 * startup the send/receive threads
 */
void ClientListener::configure() {
  int &serverSockFd = m_fd;
  int clientSockFd;
  unsigned int nread;
  sockaddr_in serverSocket;
  sockaddr_in clientSocket;

  serverSocket.sin_family = AF_INET;
  serverSocket.sin_port = htons(m_port);
  serverSocket.sin_addr.s_addr = INADDR_ANY;

  if ((serverSockFd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    printf("socket error");
    return;
  }

  /* Enable address reuse */
  int on = 1;
  setsockopt(serverSockFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  if (bind(serverSockFd, (sockaddr *)&serverSocket, sizeof(serverSocket)) < 0) {
    perror("Server:: bind error!");
    return;
  }

  int backLog = MAXCONN - 1;
  if (listen(serverSockFd, backLog) == -1) {
    printf("Server::listen error!\n");
    return;
  } else {
    printf("Server::listen ok!\n");
  }

  pthread_t t_Receiver;
  pthread_t t_Sender;

  const int error1 = pthread_create(&t_Receiver, NULL, receiveHandler, this);
  if (error1 != 0) {
    std::cout << "cannot create receive thread!" << strerror(errno)
              << std::endl;
  }

  const int error2 = pthread_create(&t_Sender, NULL, sendHandler, this);
  if (error2 != 0) {
    std::cout << "cannot create send thread!" << strerror(errno) << std::endl;
  }

  ClientListenerLogging::log("sender thread id=%x\n", t_Sender);
}

void *ClientListener::receiveHandler(void *para) {
  ClientListenerLogging::log("-Init receive handler.\n");
  printf("-Init receive handler.\n");

  /**		redirect input/output to console		**/
  //	freopen("/dev/console", "r", stdin);
  //	freopen("/dev/tty", "r", stdin);
  //	freopen("/dev/tty", "a", stdout);
  //	close(stdin);
  //	char c;
  //	c = getchar();
  //	cout<<c<<endl;

  restoreStd();

  ClientListener *server = (ClientListener *)para;
  int &serverSockFd = server->m_fd;
  int clientSockFd;

  const int buffer_size = 102400;

  char *buf = new char[buffer_size];  // new
  memset(buf, 0, sizeof(buf));

  unsigned int sockLen;
  int nread;

  int nfds = serverSockFd + 1;
  fd_set watchFds;
  fd_set readFds;

  FD_ZERO(&watchFds);
  FD_SET(serverSockFd, &watchFds);

  bool stop = false;
  while (!stop) {
    usleep(1);
    readFds = watchFds;

    {  // OutputFdsInfo
      bool isok = true;
      for (int i = 0; i < MAXCONN; ++i) {
        if (server->m_clientFds[i] >= 0 &&
            !FD_ISSET(server->m_clientFds[i], &readFds)) {
          ClientLogging::log("---%s is not in readFds", server->m_clientFds[i]);
          isok = false;
        }
      }
      if (isok) {
        ClientLogging::log("---all is in readFds");
      }
    }

    switch (select(nfds, &readFds, NULL, NULL, (timeval *)0)) {
      case -1: {
        ClientLogging::elog("select failed. %s", strerror(errno));
        stop = true;
        break;
      }
      case 0:
        break;
      default:

        if (FD_ISSET(serverSockFd, &readFds)) {
          // ADD Client
          sockaddr_in clientSocket;
          sockLen = sizeof(sockaddr_in);

          restoreStd();

          //				{	// Yu
          //					cout<<endl<<"clientFds:"<<endl;
          //					for (int i = 0; i < MAXCONN ;
          //++i)
          //{
          //						cout<<server->m_clientFds[i]<<"
          //";
          //					}
          //					cout<<endl;
          //				}

          if (!server->isFull()) {
            if ((clientSockFd = accept(serverSockFd, (sockaddr *)&clientSocket,
                                       &sockLen)) >= 0) {
              checkFdValid(clientSockFd);

              printf("open communication with client, %d\n", clientSockFd);

              //						server->m_clientFds[server->m_num++]
              //= clientSockFd;
              server->addClient(clientSockFd);
              FD_SET(clientSockFd, &watchFds);
              nfds = (clientSockFd >= nfds ? clientSockFd + 1 : nfds);
            } else {
              printf("Accept Error!\n");
              perror("");
            }
          }
        }
        for (int i = 0; i < MAXCONN; ++i) {
          if (server->m_clientFds[i] < 0) continue;

          if (FD_ISSET(server->m_clientFds[i], &readFds)) {
            ioctl(server->m_clientFds[i], FIONREAD, &nread);
            if (0 == nread) {
              // TODO does here means a client close the connection with server?
              FD_CLR(server->m_clientFds[i], &watchFds);
              FileClose(server->m_clientFds[i]);
              ClientLogging::log("-----for debug:close fd %d.",
                                 server->m_clientFds[i]);
              server->removeClient(server->m_clientFds[i]);
              continue;
            }
            memset(buf, 0, sizeof(buf));
            int read_count = read(server->m_clientFds[i], buf, nread);
            buf[read_count] = '\0';  // fix a bug

            int sql_type = buf[0] - 48;  // '1' - 48 = 1
            ClientLogging::log("sql_type is %d", sql_type);
            if (sql_type <= 9 && sql_type >= 0) {
              server->client_type_ = client_type::java;
              LOG(INFO) << "this messege is from java client : " << buf;
              generateSqlStmt(sql_type, buf);
            } else if ('#' - 48 == sql_type) {
              buf += 1;  // ignore the number in the front of buf
              server->client_type_ = client_type::c;
              LOG(INFO) << "this messege is from c client : " << buf;
            }

            assert(buffer_size > nread);

            int retCode = server->receiveRequest(server->m_clientFds[i], buf);
            if (0 == retCode) {
              printf("Successfully receive query\n %s \nfrom client %d.\n", buf,
                     server->m_clientFds[i]);
            }
            //					else if (-1 == retCode) {
            //						server->shutdown();
            //						stop = true;
            //						printf("Shut Down!\n");
            //					} else {
            //						printf("Failed query
            // from
            // client
            //%d.\n",
            //								server->m_clientFds[i]);
            //					}
          }
        }
        break;
    } /*end of switch*/
  }
  FD_ZERO(&watchFds);
  FD_ZERO(&readFds);
  delete buf;

  return NULL;
}

void ClientListener::generateSqlStmt(int type, char *&buf) {
  assert(buf != NULL);
  ClientLogging::log(
      "in generateSqlStmt():type argument is %d, buf argument is %s", type,
      buf);
  // the first byte of buf is type, other are argument
  string arg1 = "";
  string arg2 = "";
  string arg3 = "";

  switch (type) {
    case 0: {
      arg1 = string(buf + 1, 6);
      ClientLogging::log("arg1 is %s", arg1.c_str());
      // select avg(trade_price), trade_date from trade where sec_code =
      // '600036' group by trade_date order by trade_date;
      string sql =
          "select avg(trade_price) as trade_price, trade_date from trade where "
          "sec_code = \'" +
          arg1 + "\' group by trade_date order by trade_date; ";
      //		ClientLogging::log("the whole sql string is:
      //%s",sql.c_str());
      memset(buf, 0, sizeof(buf));
      strcpy(buf, sql.c_str());
      break;
    }
    case 1: {
      arg1 = string(buf + 1, 6);
      arg2 = string(buf + 7, 10);
      arg3 = string(buf + 17, 10);
      cout << "arg is :" << arg1 << "--" << arg2 << "--" << arg3 << endl;
      //		select max(trade_price), min(trade_price) from trade
      // where sec_code = '600036' and trade_date  between '2010-09-10' and
      //'2010-09-10';
      string sql =
          "select max(trade_price), min(trade_price) from trade where sec_code "
          "= \'" +
          arg1 + "\'  and trade_date  between \' " + arg2 + "\' and \'" + arg3 +
          " \' ;";
      //		ClientLogging::log("the whole sql string is:
      //%s",sql.c_str());
      memset(buf, 0, sizeof(buf));
      strcpy(buf, sql.c_str());
      break;
    }
    case 2: {
      arg1 = string(buf + 1, 6);
      cout << "arg is :" << arg1 << "--" << arg2 << "--" << arg3 << endl;
      //		select trade_vol/100, count(*) as frequency from trade
      // where sec_code = '600036' group by trade_vol/100 order by frequency
      // desc
      string sql =
          "select count(*) as frequency, trade_vol/100 from trade where "
          "sec_code = \'" +
          arg1 + "\' group by trade_vol/100 order by frequency desc;";
      //		ClientLogging::log("the whole sql string is:
      //%s",sql.c_str());
      memset(buf, 0, sizeof(buf));
      strcpy(buf, sql.c_str());
      break;
    }
    case 3: {
      arg2 = string(buf + 1, 10);
      arg3 = string(buf + 11, 10);
      cout << "arg is :" << arg1 << "--" << arg2 << "--" << arg3 << endl;
      //
      string sql =
          "select afield/total as proportion, sec_field "
          "from ( "
          "select sum(trade_amt)  total "
          "from trade "
          "where trade_date between '" +
          arg2 + "' and '" + arg3 +
          "' "
          ") as amt_total, "
          " ( "
          "select sum(trade_amt) afield, sec_field sec_field "
          "from trade, field "
          "where trade.sec_code = field.sec_code "
          "and trade_date between '" +
          arg2 + "' and '" + arg3 +
          "' "
          "group by sec_field "
          ") as amt_field "
          "order by proportion desc; ";
      //		ClientLogging::log("the whole sql string is:
      //%s",sql.c_str());
      memset(buf, 0, sizeof(buf));
      strcpy(buf, sql.c_str());
      break;
    }
    case 4: {
      arg2 = string(buf + 1, 10);
      arg3 = string(buf + 11, 10);
      cout << "arg is :" << arg1 << "--" << arg2 << "--" << arg3 << endl;
      //
      string sql =
          "select code_amt/total as proportion, sec_code "
          "from  "
          "     ( "
          "        select sum(trade_amt) as total  "
          "        from trade, field  "
          "        where trade.sec_code = field.sec_code "
          "             and field.sec_field = '房地产'                     "
          "             and trade.trade_date between '" +
          arg2 + "' and '" + arg3 +
          "'      "
          "     ) as field_total, "
          "     ( "
          "         select sum(trade_amt) as code_amt, sec_code as sec_code "
          "         from trade "
          "         where trade_date between '" +
          arg2 + "' and '" + arg3 +
          "'           "
          "         group by sec_code "
          "      ) as field_amt "
          "order by proportion desc; ";

      //		ClientLogging::log("the whole sql string is:
      //%s",sql.c_str());
      memset(buf, 0, sizeof(buf));
      strcpy(buf, sql.c_str());
      break;
    }
    case 5: {
      arg2 = string(buf + 1, 10);
      arg3 = string(buf + 11, 10);
      cout << "arg is :" << arg1 << "--" << arg2 << "--" << arg3 << endl;
      //
      string sql =
          "select sum(flow) as flow_sum, sec_field "
          "from  "
          "     ( "
          "				select TT1.flow_in - "
          "TT2.flow_out as flow, "
          "					   TT1.sec_code sec_code , "
          "					   TT1.sec_field sec_field "
          "				from  "
          "				     ( "
          "						select "
          "sum(trade.trade_vol  * t1.avg_price) as flow_in, "
          "							   "
          "trade.sec_code sec_code, "
          "							   "
          "t1.sec_field sec_field "
          "						from  "
          "							( "
          "								"
          "select min(trade.order_time) as min_time,  "
          "                                    avg(trade.trade_price) as "
          "avg_price, "
          "								"
          "	trade.trade_no trade_no,  "
          "								"
          "	trade.sec_code sec_code, "
          "								"
          "	field.sec_field sec_field "
          "								"
          "from trade, field "
          "								"
          "where trade.sec_code = field.sec_code "
          "								"
          "  and trade_date between '" +
          arg2 + "' and '" + arg3 +
          "'     "
          "								"
          "group by trade.trade_no, trade.sec_code, field.sec_field "
          "							) t1, trade "
          "						where "
          "t1.trade_no = trade.trade_no "
          "						and t1.sec_code "
          "= trade.sec_code "
          "						and t1.min_time "
          "= trade.order_time "
          "						and "
          "trade.trade_dir = 'S' "
          "						and "
          "trade.trade_date between '" +
          arg2 + "' and '" + arg3 +
          "'         "
          "						group by "
          "trade.sec_code, t1.sec_field "
          "					) TT1, "
          "                    ( "
          "						select "
          "sum(trade.trade_vol * t1.avg_price) as flow_out, "
          "							   "
          "trade.sec_code sec_code, "
          "							   "
          "t1.sec_field sec_field "
          "						from  "
          "							( "
          "								"
          "select min(trade.order_time) as min_time,  "
          "                                    avg(trade.trade_price) as "
          "avg_price, "
          "								"
          "	trade.trade_no trade_no,  "
          "								"
          "	trade.sec_code sec_code, "
          "								"
          "	field.sec_field sec_field "
          "								"
          "from trade, field "
          "								"
          "where trade.sec_code = field.sec_code "
          "								"
          "  and trade_date between '" +
          arg2 + "' and '" + arg3 +
          "'     "
          "								"
          "group by trade.trade_no, trade.sec_code, field.sec_field "
          "							) t1, trade "
          "						where "
          "t1.trade_no = trade.trade_no "
          "						and t1.sec_code "
          "= trade.sec_code "
          "						and t1.min_time "
          "= trade.order_time "
          "						and "
          "trade.trade_dir = 'B' "
          "						and "
          "trade.trade_date between '" +
          arg2 + "' and '" + arg3 +
          "'         "
          "						group by "
          "trade.sec_code, t1.sec_field "
          "					) TT2 "
          "				where TT1.sec_code = TT2.sec_code "
          "	              and TT1.sec_field = TT2.sec_field	 "
          "	 ) TT3 "
          "group by sec_field "
          "order by flow_sum desc; ";
      //		ClientLogging::log("the whole sql string is:
      //%s",sql.c_str());
      memset(buf, 0, sizeof(buf));
      strcpy(buf, sql.c_str());
      break;
    }
    case 6: {
      arg2 = string(buf + 1, 10);
      arg3 = string(buf + 11, 10);
      cout << "arg is :" << arg1 << "--" << arg2 << "--" << arg3 << endl;
      //
      string sql =
          "select sum(flow) as flow_sum, sec_area "
          "from  "
          "     ( "
          "				select TT1.flow_in - "
          "TT2.flow_out as flow, "
          "					   TT1.sec_code sec_code , "
          "					   TT1.sec_area sec_area "
          "				from  "
          "				     ( "
          "						select "
          "sum(trade.trade_vol  * t1.avg_price) as flow_in, "
          "							   "
          "trade.sec_code sec_code, "
          "							   "
          "t1.sec_area sec_area "
          "						from  "
          "							( "
          "								"
          "select min(trade.order_time) as min_time,  "
          "                                    avg(trade.trade_price) as "
          "avg_price, "
          "								"
          "	trade.trade_no trade_no,  "
          "								"
          "	trade.sec_code sec_code, "
          "								"
          "	area.sec_area sec_area "
          "								"
          "from trade, area "
          "								"
          "where trade.sec_code = area.sec_code "
          "								"
          "  and trade_date between '" +
          arg2 + "' and '" + arg3 +
          "'     "
          "								"
          "group by trade.trade_no, trade.sec_code, area.sec_area "
          "							) t1, trade "
          "						where "
          "t1.trade_no = trade.trade_no "
          "						and t1.sec_code "
          "= trade.sec_code "
          "						and t1.min_time "
          "= trade.order_time "
          "						and "
          "trade.trade_dir = 'S' "
          "						and "
          "trade.trade_date between '" +
          arg2 + "' and '" + arg3 +
          "'         "
          "						group by "
          "trade.sec_code, t1.sec_area "
          "					) TT1, "
          "                    ( "
          "						select "
          "sum(trade.trade_vol * t1.avg_price) as flow_out, "
          "							   "
          "trade.sec_code sec_code, "
          "							   "
          "t1.sec_area sec_area "
          "						from  "
          "							( "
          "								"
          "select min(trade.order_time) as min_time,  "
          "                                    avg(trade.trade_price) as "
          "avg_price, "
          "								"
          "	trade.trade_no trade_no,  "
          "								"
          "	trade.sec_code sec_code, "
          "								"
          "	area.sec_area sec_area "
          "								"
          "from trade, area "
          "								"
          "where trade.sec_code = area.sec_code "
          "								"
          "  and trade_date between '" +
          arg2 + "' and '" + arg3 +
          "'     "
          "								"
          "group by trade.trade_no, trade.sec_code, area.sec_area "
          "							) t1, trade "
          "						where "
          "t1.trade_no = trade.trade_no "
          "						and t1.sec_code "
          "= trade.sec_code "
          "						and t1.min_time "
          "= trade.order_time "
          "						and "
          "trade.trade_dir = 'B' "
          "						and "
          "trade.trade_date between '" +
          arg2 + "' and '" + arg3 +
          "'         "
          "						group by "
          "trade.sec_code, t1.sec_area "
          "					) TT2 "
          "				where TT1.sec_code = TT2.sec_code "
          "	              and TT1.sec_area = TT2.sec_area	 "
          "	 ) TT3 "
          "group by sec_area "
          "order by flow_sum desc; ";
      //		ClientLogging::log("the whole sql string is:
      //%s",sql.c_str());
      memset(buf, 0, sizeof(buf));
      strcpy(buf, sql.c_str());
      break;
    }
    case 7: {
      arg2 = string(buf + 1, 10);
      arg3 = string(buf + 11, 10);
      cout << "arg is :" << arg1 << "--" << arg2 << "--" << arg3 << endl;

      string sql =
          "select (avg(t2.trade_price) - "
          "avg(t1.trade_price))/avg(t2.trade_price) as prop, t1.sec_code, "
          "t3.sec_name "
          "from tradet1, trade t2, field t3 "
          "where DATE_ADD(t1.trade_date, interval 1 day) = t2.trade_date "
          "and t1.sec_code = t3.sec_code "
          "and t1.trade_time between '14:59:50' and '15:00:00' "
          "and t2.trade_time between '09:00:00' and '09:01:00' "
          "and t1.trade_date between \'" +
          arg2 + "\' and \'" + arg3 +
          "\' "
          "and t2.trade_date between \'" +
          arg2 + "\' and \'" + arg3 +
          "\' "
          "group by t1.sec_code, t3.sec_name "
          "order by prop desc "
          "limit 20;";
      //		ClientLogging::log("the whole sql string is:
      //%s",sql.c_str());
      memset(buf, 0, sizeof(buf));
      strcpy(buf, sql.c_str());
      break;
    }
    case 8: {
      arg2 = string(buf + 1, 10);
      arg3 = string(buf + 11, 10);
      cout << "arg is :" << arg1 << "--" << arg2 << "--" << arg3 << endl;

      string sql =
          "select sum(trade_vol) as vol, sec_code "
          "from trade"
          "where trade.trade_date between '" +
          arg2 + "' and '" + arg3 +
          "' "
          "group by sec_code "
          "order by vol desc "
          "limit 20 ";
      //		ClientLogging::log("the whole sql string is:
      //%s",sql.c_str());
      memset(buf, 0, sizeof(buf));
      strcpy(buf, sql.c_str());
      break;
    }
    case 9: {
      buf += 1;  // ignore the number in the front of buf
      break;
    }
    default: { ClientLogging::elog("No supported"); }
  }

  ClientLogging::log("sql buf is %s\n", buf);
  int temp;
}

void *ClientListener::sendHandler(void *para) {
  ClientListenerLogging::log("-Init send handler!\n");
  ClientListener *server = (ClientListener *)para;

  ClientResponse cliRes;
  while (true) {
    usleep(1);
    ClientListenerLogging::log("-SendHandler: wait for result!\n");
    ExecutedResult result = Daemon::getInstance()->getExecutedResult();
    printf("-SendHandler: get ExecutedResult for %d\n", result.fd_);
    ClientListenerLogging::log("-SendHandler: get ExecutedResult for %d\n",
                               result.fd_);

    checkFdValid(result.fd_);

    if (result.status_ == true) {
      // OK
      if (result.result_ == NULL) {
        // DDL return true
        if ("" == result.warning_)
          cliRes.setChange(result.info_);
        else
          cliRes.setChange(result.info_ + "\n\nWARNINGS:\n" + result.warning_ +
                           "\n");
        LOG(INFO) << "to send change response-- status:" << cliRes.status
                  << "  length:" << cliRes.length
                  << "  content:" << cliRes.content.c_str()
                  << " warnings: " << result.warning_.c_str() << std::endl;
        server->write(result.fd_, cliRes);
      } else {
        if (client_type::java == server->client_type_) {
          sendJsonPacket(cliRes, result);
          server->write(result.fd_, cliRes);
        } else if (client_type::c == server->client_type_) {
          // query return true
          cliRes.setOk("Yes Ok");
          ClientListenerLogging::log(
              "to send data response-- status:%d  length:%d  content:%s  fd:%d",
              cliRes.status, cliRes.length, cliRes.content.c_str(), result.fd_);
          server->write(result.fd_, cliRes);
          printf("send ok response packet ok\n");

          cliRes.setSchema(result.result_->schema_);
          ClientListenerLogging::log(
              "to send data response-- status:%d  length:%d  content:%s  fd:%d",
              cliRes.status, cliRes.length, cliRes.content.c_str(), result.fd_);
          server->write(result.fd_, cliRes);
          printf("send schema response packet ok\n");

          std::vector<std::string> list = result.result_->column_header_list_;
          ColumnHeader header;
          for (int i = 0; i < list.size(); ++i) {
            header.add_header(list[i]);
          }
          cliRes.setAttributeName(header);
          ClientListenerLogging::log(
              "to send data response-- status:%d  length:%d  content:%s  fd:%d",
              cliRes.status, cliRes.length, cliRes.content.c_str(), result.fd_);
          server->write(result.fd_, cliRes);
          printf("send head response packet ok\n");

          //				result.result->print();
          ResultSet::Iterator it = result.result_->createIterator();
          BlockStreamBase *block;

          Block serialzed_block(64 * 1024);

          while (block = (BlockStreamBase *)it.atomicNextBlock()) {
            block->serialize(serialzed_block);
            cliRes.setDataBlock(serialzed_block);
            ClientListenerLogging::log(
                "to send data response-- status:%d  length:%d  content:%s  "
                "fd:%d",
                cliRes.status, cliRes.length, cliRes.content.c_str(),
                result.fd_);
            server->write(result.fd_, cliRes);
          }
          printf("send data packet ok\n");

          cliRes.setEnd(result.result_->query_time_);
          ClientListenerLogging::log(
              "to send end response-- status:%d  length:%d  content:%s",
              cliRes.status, cliRes.length, cliRes.content.c_str());
          server->write(result.fd_, cliRes);
          printf("send end response packet ok\n");
          if (NULL != result.result_) {
            delete result.result_;
            result.result_ = NULL;
            //            cout << "delete result in memory" << endl;
          }
        }
      }
    } else {
      // ERROR
      if ("" == result.warning_)
        cliRes.setError(result.error_info_);
      else
        cliRes.setError(result.error_info_ + "\n\nWARNINGS:" + result.warning_ +
                        "\n");
      ClientListenerLogging::log(
          "to send err response-- status:%d  length:%d  content:%s"
          "warnings: %s",
          cliRes.status, cliRes.length, cliRes.content.c_str(),
          result.warning_.c_str());
      server->write(result.fd_, cliRes);
      printf("send error packet ok\n");
      if (NULL != result.result_) {
        delete result.result_;
        result.result_ = NULL;
        cout << "delete result in memory" << endl;
      }
    }
  }
  return NULL;
}

void ClientListener::sendJsonPacket(ClientResponse &cr, ExecutedResult &res) {
  //	ClientResponse cr;
  if (res.status_) {
    if (res.result_ != NULL) {
      ResultSet *rs = res.result_;
      vector<string> &col_name_list = rs->column_header_list_;
      Json::Value root;
      Json::Value jv_temp;

      DynamicBlockBuffer::Iterator it = rs->createIterator();
      BlockStreamBase *block;
      while (block = it.nextBlock()) {
        BlockStreamBase::BlockStreamTraverseIterator *block_it =
            block->createIterator();
        void *tuple;
        while (tuple = block_it->nextTuple()) {
          //					rs->schema_->displayTuple(tuple,"\t");
          for (int i = 0; i < rs->schema_->getncolumns(); ++i) {
            jv_temp[col_name_list[i]] =
                Json::Value(rs->schema_->getColumnValue(tuple, i));
          }
          root.append(jv_temp);
        }
      }

      //			Json::StyledWriter sw;
      //			cout<<sw.write(root)<<endl;

      Json::FastWriter fw;
      string buf_to_send = fw.write(root);
      buf_to_send += '\n';
      //			if (buf_to_send.length() <= 1000) {
      //				ClientLogging::log("%s",
      // buf_to_send.c_str());
      //			}
      //			else {
      //				ClientLogging::log("%s...",
      // buf_to_send.substr(0, 1000).c_str());
      //			}
      cr.setData(buf_to_send);
    }
  }
}

/**
 * Thread shut down the server when it receives "shut down" command from a
 * client.
 */
// void ClaimsServer::run() {
//
//	int &serverSockFd = m_fd;
//	int clientSockFd;
//	unsigned int nread;
//	char *buf = new char[128];	//new
//	sockaddr_in serverSocket;
//	sockaddr_in clientSocket;
//
//	serverSocket.sin_family = AF_INET;
//	serverSocket.sin_port = htons(8000);
//	serverSocket.sin_addr.s_addr = INADDR_ANY;
//
//	if ((serverSockFd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
//		printf("socket error");
//		return;
//	}
//
//	if (bind(serverSockFd, (sockaddr *) &serverSocket, sizeof(serverSocket))
//			< 0) {
//		perror("Server:: bind error!");
//		return;
//	}
//
//	int backLog = MAXCONN - 1;
//	if (listen(serverSockFd, backLog) == -1) {
//		printf("Server::listen error!\n");
//		return;
//	} else {
//		printf("Server::listen ok!\n");
//	}
//
//	int nfds = serverSockFd + 1;
//	unsigned int sockLen;
//	fd_set watchFds;
//	fd_set readFds;
//
//	FD_ZERO(&watchFds);
//	FD_SET(serverSockFd, &watchFds);
//
//	bool stop = false;
//	while (!stop) {
//
//		readFds = watchFds;
//		switch (select(nfds, &readFds, NULL, NULL, (timeval *) 0)) {
//		case -1:
//			break;
//		case 0:
//			break;
//		default:
//
//			if (FD_ISSET(serverSockFd, &readFds)) {
//				//ADD Client
//				sockLen = sizeof(sockaddr_in);
//
//				if (!isFull()) {
//					if ((clientSockFd = accept(serverSockFd,
//							(sockaddr *)
//&clientSocket,
//&sockLen)) >= 0) {
//
//						printf("open communication with
// client,
//%d\n",
//								clientSockFd);
//						m_clientFds[m_num++] =
// clientSockFd;
//						FD_SET(clientSockFd, &watchFds);
//						nfds = (clientSockFd >= nfds ?
// clientSockFd + 1 : nfds);
//					} else {
//						printf("Accept Error!\n");
//					}
//				}
//			}
//			for (int i = 0; i < MAXCONN; ++i) {
//
//				if (m_clientFds[i] < 0)
//					continue;
//
//				if (FD_ISSET(m_clientFds[i], &readFds)) {
//
//					ioctl(m_clientFds[i], FIONREAD, &nread);
//					if (0 == nread) {
//						//TODO does here means a client
// close
// the
// connection with server?
//						printf("retained!!!\n");
//						FD_CLR(m_clientFds[i],
//&watchFds);
////						::shutdown(m_clientFds[i], 2);
//						::close(m_clientFds[i]);
//
//						m_clientFds[i] = -1;
//						continue;
//					}
//					read(m_clientFds[i], buf, nread);
//
//					int retCode =
// answerRequest(m_clientFds[i],
// buf);
//					if (1 == retCode) {
//						printf("Successfully answer
// query
// from
// client %d.\n",
//								m_clientFds[i]);
//					} else if (-1 == retCode) {
//						shutdown();
//						stop = true;
//						printf("Shut Down!\n");
//					} else {
//						printf("Failed query from client
//%d.\n",
//								m_clientFds[i]);
//					}
//
//				}
//			}
//			break;
//		} /*end of switch*/
//	}
//	FD_ZERO(&watchFds);
//	FD_ZERO(&readFds);
//	delete buf;
//}

/**
 * shut down the server
 */
void ClientListener::shutdown() {
  for (int i = 0; i < MAXCONN; ++i) {
    if (m_clientFds[i] > 0) {
      FileClose(m_clientFds[i]);
      ClientLogging::log("-----for debug:close fd %d.\n", m_clientFds[i]);
    }
  }
  FileClose(standard_err);
  FileClose(standard_input);
  FileClose(standard_output);
  FileClose(m_fd);
  ClientLogging::log("-----for debug:close fd %d", m_fd);
}

int ClientListener::write(const int fd, const ClientResponse &res) const {
  int ret = 0;
  char *buffer;
  int length = res.serialize(buffer);

  checkFdValid(fd);

  //	ret = ::write(fd, buffer, length);
  ret = send(fd, buffer, length, MSG_WAITALL);
  LOG(INFO) << "Server: " << res.status << " bytes:" << ret << "\t"
            << res.length << "\t" << res.content.c_str() << " is send!" << endl;
  //	ret = ::write(fd,buffer,length);
  //	if (ret < 0) {
  //		ClientLogging::elog("when send to fd %d, send buffer failed.%s",
  // fd,
  // strerror(errno));
  //		if (EBADF == errno) {
  //			FileClose(fd);
  //			removeClient(fd);
  //			ClientLogging::log("-----for debug:close fd %d in
  // write()\n",
  // fd);
  //		}
  //	}
  //	else if (length <= 1000) {
  ////		printf("Server: %d bytes:%d\t%d\t%s is send!\n", ret,
  /// res.status,
  /// res.length, res.content.c_str());
  //		printf("Server: %d bytes:%d\t%d\t is send!\n", ret, res.status,
  // res.length);
  //	}
  //	else {
  ////		printf("Server: %d bytes:%d\t%d\t %s... is send!\n", ret,
  /// res.status, res.length, res.content.substr(0,1000).c_str());
  //		printf("Server: %d bytes:%d\t%d\t is send!\n", ret, res.status,
  // res.length);
  //	}
  free(buffer);
  return ret;
}
