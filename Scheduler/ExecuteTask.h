/*
 * ExecuteTask.h
 *
 *  Created on: 2013-9-26
 *      Author: casa
 */

#ifndef EXECUTETASK_H_
#define EXECUTETASK_H_

#include <map>
#include <string>
using namespace std;

#include "../BlockStreamIterator/BlockStreamIteratorBase.h"

//TODO: support the "extends" Serialize class
class ExecuteTask {
public:
	ExecuteTask();
	ExecuteTask(BlockStreamIteratorBase *operate_tree)
	:operate_tree_(operate_tree){}
	virtual ~ExecuteTask();

    string Serialize(BlockStreamIteratorBase *operate_tree);

private:
	BlockStreamIteratorBase *operate_tree_;
};

//class SereiralizedExecuteTask:public ExecuteTask,public serialization{
//
//};

#endif /* EXECUTETASK_H_ */
