/*
 * Debug.h
 *
 *  Created on: Aug 9, 2013
 *      Author: wangli
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define COOR "/home/imdb/config/wangli/coor.conf"
#define CONFIG "/home/imdb/config/wangli/config"
//#define CONFIG_SAME "/home/imdb/config/config"

/*
 * needed by blockstreamjoiniterator
 * */
#define BARRIER 3
/*
 * block in hdfs and sublock in block
 * */
#define SUCHUNK_SIZE 64*1024
#define CHUNK_SIZE 64*1024*1024

#define HEARTBEAT_MESSAGE_LEN 64
#define REGISTER_MESSAGE_LEN 64
#define BLOCK_STATUS_MESSAGE_LEN 256
#define MATCHER_MESSAGE_FILENAME_LEN 256
#define MATCHER_MESSAGE_BMI_LEN 256
#define MATCHER_MESSAGE_PROJECT_LEN 256
// 分布式文件系统的主节点
#define HDFS_N "10.11.1.201"
// 磁盘目录
#define DISKDIR "/home/imdb/diskdata"
//#define DEBUG_Coordinator~/
//#define DEBUG_Environment

//#define DEBUG_ExchangeIteratorLowerWithWideDependency
//#define DEBUG_ExchangeIteratorWithWideDependency
//#define DEBUG_ExchangeIteratorEager
//#define DEBUG_ExchangeIteratorEagerLower
//
//#define DEBUG_ExchangeRegister
//#define DEBUG_ExchangeTracker
//
//#define DEBUG_PortManager
//#define DEBUG_IteratorExecutorSlave
//#define DEBUG_IteratorExecutorMaster
//

//#define DEBUG_ExchangeIteratorEager
//#define DEBUG_ExpandableBlockStreamExchangeMaterialized
//#define DEBUG_BlockStreamExchangeLowerBase
//#define DEBUG_ExpandableBlockStreamExchangeLM

#define DEBUG_ResourceManagerMaster

#define DEBUG_Catalog
#endif /* DEBUG_H_ */
