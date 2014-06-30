/*
 * Logging.h
 *
 *  Created on: Aug 11, 2013
 *      Author: wangli
 */

#ifndef LOGGING_H_
#define LOGGING_H_
#include <string>
#include <stdarg.h>
#include <stdio.h>
#include "../Debug.h"
#define SQL_Parser


class Logging{
public:
	/*
	 * print the status information to the standard output.
	 */
	virtual void log(const char* format,...)=0;
	/**
	 * print the error information to stderr.
	 */
	virtual void elog(const char* format,...)=0;
	virtual ~Logging(){};
};

class IteratorExecutorMasterLogging:public Logging{
public:
	void log(const char* format,...);
	void elog(const char* format,...);
};

class IteratorExecutorSlaveLogging:public Logging{
public:
	void log(const char* format,...);
	void elog(const char* format,...);
};

class EnvironmentLogging:public Logging{
public:
	void log(const char* format,...);
	void elog(const char* format,...);
};

class AdaptiveEndPointLogging:public Logging{
public:
	void log(const char* format,...);
	void elog(const char* format,...);
};

class CoordinatorLogging:public Logging{
public:
	void log(const char* format,...);
	void elog(const char* format,...);
};

class ExchangeTrackerLogging:public Logging{
public:
	void log(const char* format,...);
	void elog(const char* format,...);
};
class ExpanderTrackerLogging:public Logging{
public:
	void log(const char* format,...);
	void elog(const char* format,...);
};
class ExchangeIteratorEagerLogging:public Logging{
	void log(const char* format,...);
	void elog(const char* format,...);
};
class ExchangeIteratorEagerLowerLogging:public Logging{
	void log(const char* format,...);
	void elog(const char* format,...);
};

class CatalogLogging:public Logging{
	void log(const char* format,...);
	void elog(const char* format,...);
};

class ResourceManagerMasterLogging:public Logging{
	void log(const char* format,...);
	void elog(const char* format,...);
};
class ResourceManagerSlaveLogging:public Logging{
	void log(const char* format,...);
	void elog(const char* format,...);
};
class StorageManagerLogging:public Logging{
	void log(const char* format,...);
	void elog(const char* format,...);
};
class StorageManagerMasterLogging:public Logging{
	void log(const char* format,...);
	void elog(const char* format,...);
};
class BufferManagerLogging:public Logging{
	void log(const char* format,...);
	void elog(const char* format,...);
};
class BlockStreamExpanderLogging:public Logging{
	void log(const char* format,...);
	void elog(const char* format,...);
};
class PerformanceTopLogging:public Logging{
	void log(const char* format,...);
	void elog(const char* format,...);
};

class QueryOptimizationLogging{
public:
	static void log(const char* format,...);
	static void elog(const char* format,...);
};

class BlockStreamJoinLogging{
public:
	static void log(const char* format,...);
	static void elog(const char* format,...);
};

class ASTParserLogging{
public:
	static void log(const char* format,...);
	static void elog(const char* format,...);
};

static void Logging_ExchangeIteratorLowerWithWideDependency(const char* format,...){
#ifdef DEBUG_ExchangeIteratorWithWideDependency
	printf("[EILWWD]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	va_end (arg);
	printf("\n");
#endif
}

static void Logging_ExchangeIteratorWithWideDependency(const char* format,...){
#ifdef DEBUG_ExchangeIteratorWithWideDependency
	printf("[EIWWD]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
#endif
}




//static void Logging_Environment(const char* format,...){
//#ifdef DEBUG_Environment
//	printf("[Environment]: ");
//	va_list arg;
//	va_start (arg, format);
//	vprintf (format, arg);
//	printf("\n");
//	va_end (arg);
//#endif
//}


static void Logging_PortManager(const char* format,...){
#ifdef DEBUG_PortManager
	printf("[PortManager]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
#endif
}


static void Logging_IteratorExecutorSlave(const char* format,...){
#ifdef DEBUG_IteratorExecutorSlave
	printf("[IteratorExecutorSlave]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
#endif
}

static void Logging_ExchangeIteratorEager(const char* format,...){
//#ifdef DEBUG_ExchangeIteratorEager
	printf("[ExchangeIteratorEager]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
//#endif
}

static void Logging_BlockStreamExchangeBase(const char* format,...){
#ifdef BlockStreamExchangeBase
	printf("[BlockStreamExchangeBase]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
#endif
}

static void Logging_ExchangeIteratorEagerLower(const char* format,...){
#ifdef DEBUG_ExchangeIteratorEagerLower
	printf("[ExchangeIteratorEagerLower]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
#endif
}
//
//static void Logging_AdaptiveEndPoint(const char* format,...){
//#ifdef DEBUG_AdaptiveEndPoint
//	printf("[AdaptiveEndPoint]: ");
//	va_list arg;
//	va_start (arg, format);
//	vprintf (format, arg);
//	printf("\n");
//	va_end (arg);
//#endif
//}
static void Logging_ExpandableBlockStreamExchangeMaterialized(const char* format,...){
#ifdef DEBUG_ExpandableBlockStreamExchangeMaterialized
	printf("[ExpandableBlockStreamExchangeMaterialized]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
#endif
}

static void Logging_ExpandableBlockStreamExchangeLM(const char* format,...){
#ifdef DEBUG_ExpandableBlockStreamExchangeLM
	printf("[ExpandableBlockStreamExchangeLM]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
#endif
}

static void Logging_BlockStreamExchangeLowerBase(const char* format,...){
#ifdef DEBUG_BlockStreamExchangeLowerBase
	printf("[BlockStreamExchangeLowerBase]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
#endif
}

static void SQLParse_log(const char* format,...){
#ifdef SQL_Parser
	printf("[SQLParse_log]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
#endif
}
static void SQLParse_elog(const char* format,...){
	printf("[SQLParse_elog]: ");
	va_list arg;
	va_start (arg, format);
	vprintf (format, arg);
	printf("\n");
	va_end (arg);
}

#endif /* LOGGING_H_ */
