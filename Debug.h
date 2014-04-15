/*
 * Debug.h
 *
 *  Created on: Aug 9, 2013
 *      Author: wangli
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define COOR "/home/imdb/config/dsc/coor.conf"
#define CONFIG "/home/imdb/config/dsc/config"

//#define CONFIG_SAME "/home/imdb/config/config"

/*
 * needed by blockstreamjoiniterator
 * */
#define BARRIER 1
/*
 * block in hdfs and sublock in block
 * */
#define BLOCK_SIZE 64*1024
#define SUCHUNK_SIZE 64*1024
#define CHUNK_SIZE 64*1024*1024
#define CHUNK_SIZE_IN_MB 64
#define HEARTBEAT_MESSAGE_LEN 64
#define REGISTER_MESSAGE_LEN 64
#define BLOCK_STATUS_MESSAGE_LEN 256
#define MATCHER_MESSAGE_FILENAME_LEN 256
#define MATCHER_MESSAGE_BMI_LEN 256
#define MATCHER_MESSAGE_PROJECT_LEN 256
// 分布式文件系统的主节点
#define HDFS_N "10.11.1.190"
// 磁盘目录
#define DISKDIR "/home/imdb/diskdata"


#endif /* DEBUG_H_ */
