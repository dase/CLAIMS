/*
 * Logging.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: wangli
 */
#include "./Logging.h"
#include <iostream>
#include "log/logging.h"

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

//#define CLAIMS_QUEIT
//#ifndef CLAIMS_QUEIT  // If defined, all the output information is binded.
#define DEBUG_Config
#define DEBUG_ExpanderTracker
#define DEBUG_BlockStreamExpander
#define DEBUG_BlockStreamJoin
#define DEBUG_PerformanceTop
#define DEBUG_Coordinator
#define DEBUG_Environment
#define DEBUG_AdaptiveEndPoint
#define DEBUG_ExchangeIteratorLowerWithWideDependency
#define DEBUG_ExchangeIteratorWithWideDependency
#define DEBUG_ExchangeIteratorEager
#define DEBUG_ExchangeIteratorEagerLower

#define DEBUG_ExchangeRegister
#define DEBUG_ExchangeTracker

#define DEBUG_PortManager
#define DEBUG_IteratorExecutorSlave
#define DEBUG_IteratorExecutorMaster

#define DEBUG_Client
#define DEBUG_ClientLinsener

#define DEBUG_ExchangeIteratorEager
#define DEBUG_ExchangeIteratorSenderMaterialized
#define DEBUG_ExpandableBlockStreamExchangeMaterialized
#define DEBUG_BlockStreamExchangeLowerBase
#define DEBUG_ExpandableBlockStreamExchangeLM

#define DEBUG_ResourceManagerMaster
#define DEBUG_ResourceManagerSlave
#define DEBUG_Catalog
#define DEBUG_BufferManager

#define DEBUG_ASTParser

#define DEBUG_ThreadPool
#define SQL_Parser

//#endif  //CLAIMS_QUEIT

void RawLog(const char* where, const char* format, va_list args) {
  const int message_max_length = 1000;  // set initial message length
  char p[message_max_length];
  memset(p, 0, message_max_length * sizeof(char));

  int real_length = vsnprintf(p, message_max_length, format, args);

  if (unlikely(real_length < 0)) {  // check error code and output
    LOG(ERROR) << "vsnprintf error. " << strerror(errno) << std::endl;
  } else if (likely(real_length < message_max_length)) {
    // if it worked, output the message
    //    std::cout<<where<<p<<std::endl;
    LOG(INFO) << where << p << std::endl;
  } else if (real_length < 0) {  // check error code and output
    std::cerr << "vsnprintf error. " << strerror(errno) << std::endl;
  } else {  // try again with more space
    int new_message_length = real_length + 1;
    char* temp = new char[new_message_length];
    if (temp == NULL) {
      LOG(ERROR) << "new " << new_message_length << " bytes failed."
                 << strerror(errno) << std::endl;
      return;
    }
    // if enough space got, do it again
    real_length = vsnprintf(temp, new_message_length, format, args);
    LOG(INFO) << where << temp << std::endl;
    delete[] temp;
  }
}

void RawElog(const char* where, const char* format, va_list args) {
  const int message_max_length = 1000;  // set initial message length
  static char p[message_max_length];

  int real_length = vsnprintf(p, message_max_length, format, args);
  // if it worked, output the message
  if (likely(real_length < message_max_length)) {
    LOG(ERROR) << where << p << std::endl;
  } else if (real_length < 0) {  // check error code and output
    std::cerr << "vsnprintf error. " << strerror(errno) << std::endl;
  } else {  // try again with more space
    int new_message_length = real_length + 1;
    char* temp = new char[new_message_length];
    if (temp == NULL) {
      LOG(ERROR) << "new " << new_message_length << " bytes failed."
                 << strerror(errno) << std::endl;
      return;
    }
    // if enough space got, do it again
    real_length = vsnprintf(temp, new_message_length, format, args);
    LOG(ERROR) << where << temp << std::endl;
    delete[] temp;
  }
}

void IteratorExecutorMasterLogging::log(const char* format, ...) {
#ifdef DEBUG_IteratorExecutorMaster
  va_list arg;
  va_start(arg, format);
  RawLog("[IteratorExecutorMaster]: ", format, arg);
  va_end(arg);
#endif
}
void IteratorExecutorMasterLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[IteratorExecutorMaster]: ", format, arg);
  va_end(arg);
}

void IteratorExecutorSlaveLogging::log(const char* format, ...) {
#ifdef DEBUG_IteratorExecutorSlave
  va_list arg;
  va_start(arg, format);
  RawLog("[IteratorExecutorSlave]: ", format, arg);
  va_end(arg);
#endif
}
void IteratorExecutorSlaveLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[IteratorExecutorSlave]: ", format, arg);
  va_end(arg);
}

void EnvironmentLogging::log(const char* format, ...) {
#ifdef DEBUG_Environment
  va_list arg;
  va_start(arg, format);
  RawLog("[Environment]: ", format, arg);
  va_end(arg);
#endif
}
void EnvironmentLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[Environment]: ", format, arg);
  va_end(arg);
}

void AdaptiveEndPointLogging::log(const char* format, ...) {
#ifdef DEBUG_AdaptiveEndPoint
  va_list arg;
  va_start(arg, format);
  RawLog("[AdaptiveEndPoint]: ", format, arg);
  va_end(arg);
#endif
}
void AdaptiveEndPointLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[AdaptiveEndPoint]: ", format, arg);
  va_end(arg);
}

void CoordinatorLogging::log(const char* format, ...) {
#ifdef DEBUG_Coordinator
  va_list arg;
  va_start(arg, format);
  RawLog("[Coordinator]: ", format, arg);
  va_end(arg);
#endif
}
void CoordinatorLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[Coordinator]: ", format, arg);
  va_end(arg);
}

void ExchangeTrackerLogging::log(const char* format, ...) {
#ifdef DEBUG_ExchangeTracker
  va_list arg;
  va_start(arg, format);
  RawLog("[ExchangeTracker]: ", format, arg);
  va_end(arg);
#endif
}
void ExchangeTrackerLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[ExchangeTracker]: ", format, arg);
  va_end(arg);
}

void ExpanderTrackerLogging::log(const char* format, ...) {
#ifdef DEBUG_ExpanderTracker
  va_list arg;
  va_start(arg, format);
  RawLog("[ExpanderTracker]: ", format, arg);
  va_end(arg);
#endif
}
void ExpanderTrackerLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[ExpanderTracker]: ", format, arg);
  va_end(arg);
}

void ExchangeIteratorEagerLogging::log(const char* format, ...) {
#ifdef DEBUG_ExchangeIteratorEager
  va_list arg;
  va_start(arg, format);
  RawLog("[ExchangeEagerUpper]: ", format, arg);
  va_end(arg);
#endif
}
void ExchangeIteratorEagerLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[ExchangeEagerUpper]: ", format, arg);
  va_end(arg);
}

void ExchangeIteratorEagerLowerLogging::log(const char* format, ...) {
#ifdef DEBUG_ExchangeIteratorEager
  va_list arg;
  va_start(arg, format);
  RawLog("[ExchangeEagerLower]: ", format, arg);
  va_end(arg);
#endif
}
void ExchangeIteratorEagerLowerLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[ExchangeEagerLower]: ", format, arg);
  va_end(arg);
}

void ExchangeIteratorSenderMaterialized::log(const char* format, ...) {
#ifdef DEBUG_ExchangeIteratorSenderMaterialized
  va_list arg;
  va_start(arg, format);
  RawLog("[ExchangeEagerLowerMaterialized]: ", format, arg);
  va_end(arg);
#endif
}
void ExchangeIteratorSenderMaterialized::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[ExchangeEagerLowerMaterialized]: ", format, arg);
  va_end(arg);
}

void CatalogLogging::log(const char* format, ...) {
#ifdef DEBUG_Catalog
  va_list arg;
  va_start(arg, format);
  RawLog("[Catalog]: ", format, arg);
  va_end(arg);
#endif
}
void CatalogLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[Catalog]: ", format, arg);
  va_end(arg);
}

void ResourceManagerMasterLogging::log(const char* format, ...) {
#ifdef DEBUG_ResourceManagerMaster
  va_list arg;
  va_start(arg, format);
  RawLog("[ResourceManagerMaster]: ", format, arg);
  va_end(arg);
#endif
}
void ResourceManagerMasterLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[ResourceManagerMaster]: ", format, arg);
  va_end(arg);
}

void ResourceManagerSlaveLogging::log(const char* format, ...) {
#ifdef DEBUG_ResourceManagerSlave
  va_list arg;
  va_start(arg, format);
  RawLog("[ResourceManagerSlave]: ", format, arg);
  va_end(arg);
#endif
}
void ResourceManagerSlaveLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[ResourceManagerSlave]: ", format, arg);
  va_end(arg);
}

void StorageManagerLogging::log(const char* format, ...) {
#ifdef DEBUG_StorageManager
  va_list arg;
  va_start(arg, format);
  RawLog("[StorageManager]: ", format, arg);
  va_end(arg);
#endif
}
void StorageManagerLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[StorageManager]: ", format, arg);
  va_end(arg);
}
void StorageManagerMasterLogging::log(const char* format, ...) {
#ifdef DEBUG_StorageManager
  va_list arg;
  va_start(arg, format);
  RawLog("[StorageManagerMaster]: ", format, arg);
  va_end(arg);
#endif
}
void StorageManagerMasterLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[StorageManagerMaster]: ", format, arg);
  va_end(arg);
}

void BufferManagerLogging::log(const char* format, ...) {
#ifdef DEBUG_BufferManager
  va_list arg;
  va_start(arg, format);
  RawLog("[BufferManager]: ", format, arg);
  va_end(arg);
#endif
}
void BufferManagerLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[BufferManager]: ", format, arg);
  va_end(arg);
}

void ASTParserLogging::log(const char* format, ...) {
#ifdef DEBUG_ASTParser
  va_list arg;
  va_start(arg, format);
  RawLog("[ASTParser]: ", format, arg);
  va_end(arg);
#endif
}
void ASTParserLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[ASTParser]: ", format, arg);
  va_end(arg);
}

void ClientListenerLogging::log(const char* format, ...) {
#ifdef DEBUG_ClientLinsener
  va_list arg;
  va_start(arg, format);
  RawLog("[ClientListener]: ", format, arg);
  va_end(arg);
#endif
}
void ClientListenerLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[ClientListener]: ", format, arg);
  va_end(arg);
}

void BlockStreamExpanderLogging::log(const char* format, ...) {
#ifdef DEBUG_BlockStreamExpander
  va_list arg;
  va_start(arg, format);
  RawLog("[BlockStreamExpander]: ", format, arg);
  va_end(arg);
#endif
}
void BlockStreamExpanderLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[BlockStreamExpander]: ", format, arg);
  va_end(arg);
}
void PerformanceTopLogging::log(const char* format, ...) {
#ifdef DEBUG_PerformanceTop
  va_list arg;
  va_start(arg, format);
  RawLog("[PerformanceTop]: ", format, arg);
  va_end(arg);
#endif
}
void PerformanceTopLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[PerformanceTop]: ", format, arg);
  va_end(arg);
}
void QueryOptimizationLogging::log(const char* format, ...) {
#ifdef DEBUG_QueryOptimization
  va_list arg;
  va_start(arg, format);
  RawLog("[QueryOptimization]: ", format, arg);
  va_end(arg);
#endif
}
void QueryOptimizationLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[QueryOptimization]: ", format, arg);
  va_end(arg);
}

void BlockStreamJoinLogging::log(const char* format, ...) {
#ifdef DEBUG_BlockStreamJoin
  va_list arg;
  va_start(arg, format);
  RawLog("[BlockStreamJoin]: ", format, arg);
  va_end(arg);
#endif
}

void BlockStreamJoinLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[BlockStreamJoin]:", format, arg);
  va_end(arg);
}

void SQLParse_log(const char* format, ...) {
#ifdef SQL_Parser
  va_list arg;
  va_start(arg, format);
  RawLog("[SQLParse]:", format, arg);
  va_end(arg);
#endif
}

void SQLParse_elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawLog("[SQLParse]: ", format, arg);
  va_end(arg);
}

void ClientLogging::log(const char* format, ...) {
#ifdef DEBUG_Client
  va_list arg;
  va_start(arg, format);
  RawLog("[Client]: ", format, arg);
  va_end(arg);
#endif
}

void ClientLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[Client]: ", format, arg);
  va_end(arg);
}

void ThreadPoolLogging::log(const char* format, ...) {
#ifdef DEBUG_ThreadPool
  va_list arg;
  va_start(arg, format);
  RawLog("[ThreadPool]: ", format, arg);
  va_end(arg);
#endif
}

void ThreadPoolLogging::elog(const char* format, ...) {
  va_list arg;
  va_start(arg, format);
  RawElog("[ThreadPool]: ", format, arg);
  va_end(arg);
}
