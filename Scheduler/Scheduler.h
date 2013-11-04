/*
 * Scheduler.h
 *
 *  Created on: 2013-9-26
 *      Author: casa
 * Scheduler提供的功能：
 * 1，翻译成iterator树
 * 2，序列化iterator树并发送
 * 3，杀死某个job让另一个job得以执行，这点要和具体的scheduler策略有关系
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "ExecuteTask.h"
#include "OperateNode.h"
#include "../Catalog/table.h"

#include "../BlockStreamIterator/BlockStreamIteratorBase.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamSingleColumnScan.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamFilter.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamJoinIterator.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/ExpandableBlockStreamBroadcastExchange.h"
#include "../BlockStreamIterator/ParallelBlockStreamIterator/BlockStreamExpander.h"

//TODO: the following four value must be referenced to resource module or scheduler module
#define BLOCK_SIZE 4096
#define THREADS_COUNT 3
#define EXPAND_BUFFER 3
#define N_BUCKETS 10
#define BUCKET_SIZE 1024000
static int exchange_id=0;

struct BlockStreamIteratorNode{
public:
	char *name;
	BlockStreamIteratorBase *bsib;
	struct BlockStreamIteratorNode *next;

	explicit BlockStreamIteratorNode(){};
};

class Scheduler {
public:
	Scheduler(){};

	virtual ~Scheduler(){};

	//TODO: 也许以下函数都不会在scheduler模块中，所以我们可能以后将其移走
	virtual void init()=0;

	virtual void RewriteTree(OperateNode *subtree_root)=0;

	virtual void displayTree(OperateNode *subtree_root)=0;

	virtual void GenerateTree(OperateNode *tree_root)=0;

	virtual void ConstructParent(OperateNode *subtree_root)=0;

protected:

private:
	virtual OperateNode *findLastNode(OperateNode *subtree_root)=0;

};

class FIFOScheduler:public Scheduler{
public:
	FIFOScheduler():Scheduler(){
		cout<<"in the main"<<endl;
		current_=new BlockStreamIteratorNode();
	};

	virtual ~FIFOScheduler(){}

	void init();

	void RewriteTree(OperateNode *subtree_root);

	void displayTree(OperateNode *subtree_root);

	void GenerateTree(OperateNode *tree_root);

	void ConstructParent(OperateNode *subtree_root);

private:
	OperateNode *findLastNode(OperateNode *subtree_root);

protected:
	BlockStreamIteratorNode *current_;

};

#endif /* SCHEDULER_H_ */
