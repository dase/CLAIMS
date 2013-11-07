/*
 * Logging.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: wangli
 */
#include "Logging.h"





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
	printf("AdaptiveEndPoint: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void CoordinatorLogging::elog(const char* format,...){
	fprintf(stderr,"Error[AdaptiveEndPoint]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

void ExchangeTrackerLogging::log(const char* format,...){
#ifdef DEBUG_ExchangeTracker
//	printf("ExchangeTracker: ");
//	va_list arg;
//	va_start (arg, format);
//	vprintf(format,arg);
//	printf("\n");
//	va_end (arg);
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
	printf("Catalog: ");
	va_list arg;
	va_start (arg, format);
	vprintf(format,arg);
	printf("\n");
	va_end (arg);
#endif
}
void ResourceManagerSlaveLogging::elog(const char* format,...){
	fprintf(stderr,"Error[Catalog]: ");
	va_list arg;
	va_start (arg, format);
	vfprintf(stderr,format,arg);
	printf("\n");
	va_end (arg);
}

