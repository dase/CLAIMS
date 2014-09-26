/*
 * Logging.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: wangli
 */
#include "Logging.h"
#ifdef CLAIMS_QUEIT //If defined, all the output information is binded.
//#define DEBUG_Config
//#define DEBUG_ExpanderTracker
//#define DEBUG_BlockStreamExpander
//#define DEBUG_BlockStreamJoin
//#define DEBUG_PerformanceTop
//#define DEBUG_Coordinator
//#define DEBUG_Environment
//#define DEBUG_AdaptiveEndPoint
//#define DEBUG_ExchangeIteratorLowerWithWideDependency
//#define DEBUG_ExchangeIteratorWithWideDependency
//#define DEBUG_ExchangeIteratorEager
//#define DEBUG_ExchangeIteratorEagerLower

//#define DEBUG_BlockStreamExpander

//#define DEBUG_ExchangeRegister
//#define DEBUG_ExchangeTracker
//
//#define DEBUG_PortManager
//#define DEBUG_IteratorExecutorSlave
//#define DEBUG_IteratorExecutorMaster
//
#define DEBUG_Client
#define DEBUG_ClientLinster

//#define DEBUG_ExchangeIteratorEager
//#define DEBUG_ExchangeIteratorSenderMaterialized
//#define DEBUG_ExpandableBlockStreamExchangeMaterialized
//#define DEBUG_BlockStreamExchangeLowerBase
//#define DEBUG_ExpandableBlockStreamExchangeLM

//#define DEBUG_ResourceManagerMaster
//#define DEBUG_ResourceManagerSlave
//#define DEBUG_Catalog
//#define DEBUG_BufferManager

#define DEBUG_ASTParser
#define SQL_Parser

#endif  //CLAIMS_QUEIT
void IteratorExecutorMasterLogging::log(const char* format,...){
#ifdef DEBUG_IteratorExecutorMaster
	printf("IteratorExecutorMaster: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void IteratorExecutorMasterLogging::elog(const char* format,...){
	fprintf(stderr,"Error[IteratorExecutorMaster]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}


void IteratorExecutorSlaveLogging::log(const char* format,...){
#ifdef DEBUG_IteratorExecutorSlave
	printf("IteratorExecutorSlave: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void IteratorExecutorSlaveLogging::elog(const char* format,...){
	fprintf(stderr,"Error[IteratorExecutorSlave]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void EnvironmentLogging::log(const char* format,...){
#ifdef DEBUG_Environment
	printf("Environment: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void EnvironmentLogging::elog(const char* format,...){
	fprintf(stderr,"Error[Environment]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void AdaptiveEndPointLogging::log(const char* format,...){
#ifdef DEBUG_AdaptiveEndPoint
	printf("AdaptiveEndPoint: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void AdaptiveEndPointLogging::elog(const char* format,...){
	fprintf(stderr,"Error[AdaptiveEndPoint]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void CoordinatorLogging::log(const char* format,...){
#ifdef DEBUG_Coordinator
	printf("Coordinator: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void CoordinatorLogging::elog(const char* format,...){
	fprintf(stderr,"Error[Coordinator]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void ExchangeTrackerLogging::log(const char* format,...){
#ifdef DEBUG_ExchangeTracker
	printf("ExchangeTracker: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void ExchangeTrackerLogging::elog(const char* format,...){
	fprintf(stderr,"Error[ExchangeTracker]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void ExpanderTrackerLogging::log(const char* format,...){
#ifdef DEBUG_ExpanderTracker
	printf("ExpanderTracker: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void ExpanderTrackerLogging::elog(const char* format,...){
	fprintf(stderr,"Error[ExpanderTracker]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void ExchangeIteratorEagerLogging::log(const char* format,...){
#ifdef DEBUG_ExchangeIteratorEager
	printf("ExchangeEagerUpper: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void ExchangeIteratorEagerLogging::elog(const char* format,...){
	fprintf(stderr,"Error[ExchangeEagerUpper]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void ExchangeIteratorEagerLowerLogging::log(const char* format,...){
#ifdef DEBUG_ExchangeIteratorEager
	printf("ExchangeEagerLower: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void ExchangeIteratorEagerLowerLogging::elog(const char* format,...){
	fprintf(stderr,"Error[ExchangeEagerLower]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void ExchangeIteratorSenderMaterialized::log(const char* format,...){
#ifdef DEBUG_ExchangeIteratorSenderMaterialized
	printf("ExchangeEagerLowerMaterialized: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void ExchangeIteratorSenderMaterialized::elog(const char* format,...){
	fprintf(stderr,"Error[ExchangeEagerLowerMaterialized]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void CatalogLogging::log(const char* format,...){
#ifdef DEBUG_Catalog
	printf("Catalog: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void CatalogLogging::elog(const char* format,...){
	fprintf(stderr,"Error[Catalog]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void ResourceManagerMasterLogging::log(const char* format,...){
#ifdef DEBUG_ResourceManagerMaster
	printf("ResourceManagerMaster: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void ResourceManagerMasterLogging::elog(const char* format,...){
	fprintf(stderr,"Error[ResourceManagerMaster]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void ResourceManagerSlaveLogging::log(const char* format,...){
#ifdef DEBUG_ResourceManagerSlave
	printf("ResourceManagerSlave: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void ResourceManagerSlaveLogging::elog(const char* format,...){
	fprintf(stderr,"Error[ResourceManagerSlave]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void StorageManagerLogging::log(const char* format,...){
#ifdef DEBUG_StorageManager
	printf("StorageManager: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void StorageManagerLogging::elog(const char* format,...){
	fprintf(stderr,"Error[StorageManager]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}
void StorageManagerMasterLogging::log(const char* format,...){
#ifdef DEBUG_StorageManager
	printf("StorageManagerMaster: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void StorageManagerMasterLogging::elog(const char* format,...){
	fprintf(stderr,"Error[StorageManagerMaster]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void BufferManagerLogging::log(const char* format,...){
#ifdef DEBUG_BufferManager
	printf("BufferManager: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void BufferManagerLogging::elog(const char* format,...){
	fprintf(stderr,"Error[BufferManager]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void ASTParserLogging::log(const char* format,...){
#ifdef DEBUG_ASTParser
	printf("ASTParser: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void ASTParserLogging::elog(const char* format,...){
	fprintf(stderr,"Error[ASTParser]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}


void ClientListenerLogging::log(const char* format,...){
#ifdef DEBUG_ClientLinsener
	printf("[ClientListener]: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void ClientListenerLogging::elog(const char* format,...){
	fprintf(stderr,"Error[ClientLinster]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void BlockStreamExpanderLogging::log(const char* format,...){
#ifdef DEBUG_BlockStreamExpander
	printf("BlockStreamExpander: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void BlockStreamExpanderLogging::elog(const char* format,...){
	fprintf(stderr,"Error[BlockStreamExpander]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}
void PerformanceTopLogging::log(const char* format,...){
#ifdef DEBUG_PerformanceTop
	printf("PerformanceTop: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void PerformanceTopLogging::elog(const char* format,...){
	fprintf(stderr,"Error[PerformanceTop]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}
void QueryOptimizationLogging::log(const char* format,...){
#ifdef DEBUG_QueryOptimization
	printf("QueryOptimization: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void QueryOptimizationLogging::elog(const char* format,...){
	fprintf(stderr,"Error[PerformanceTop]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void BlockStreamJoinLogging::log(const char* format,...) {
#ifdef DEBUG_BlockStreamJoin
	printf("BlockStreamJoin: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}

void BlockStreamJoinLogging::elog(const char* format,...) {
	fprintf(stderr,"Error[BlockStreamJoin]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void SQLParse_log(const char* format,...) {
#ifdef SQL_Parser
	printf("[SQLParse_log]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
#endif
}

void SQLParse_elog(const char* format,...) {
	printf("[SQLParse_elog]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
}

void ClientLogging::log(const char* format,...) {
#ifdef DEBUG_Client
	printf("Client: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}

void ClientLogging::elog(const char* format,...) {
	fprintf(stderr,"Error[Client]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}
