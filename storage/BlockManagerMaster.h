/*
 * BlockManagerMaster.h
 *
 *  Created on: 2013-10-15
 *      Author: casa
 */
/* consider to be merged with Catalog.binding*/
#ifndef BLOCKMANAGERMASTER_H_
#define BLOCKMANAGERMASTER_H_
/*
 * 和blockmanager一样尚未完成的带有actor的类
 * */
#include <Theron/Theron.h>
#include <Theron/Defines.h>

#include "../Message.h"
#include "BlockManagerId.h"
#include "AllBlockInfo.h"
#include "BlanceMatcher.h"

#include <string>
#include <iostream>
using namespace std;

class BlockManagerMaster {
public:
	enum storageLevel{memory,disk};
	struct BlockInfo{
		string blockId_;
		storageLevel level_;
	};

	class BlockManagerMasterActor:public Theron::Actor{
		friend class BlockManagerMaster;
	public:
		BlockManagerMasterActor(Theron::EndPoint *endpoint,Theron::Framework *framework,const char *name);
		virtual ~BlockManagerMasterActor();

		void workerRegister(const RegisterStorageMessage &message,const Theron::Address from);
		void heartbeatReceiver(const HeartBeatMessage &message,const Theron::Address from);
		void blockStatusReceiver(const BlockStatusMessage &message,const Theron::Address from);
		void matcherReceiver(const MatcherMessage &message,const Theron::Address from);
	private:
		Theron::Framework *framework_;
		Theron::EndPoint *endpoint_;
	};

	static BlockManagerMaster* getInstance(BlockManagerMasterActor *actor){
		if(master_==0){
			master_=new BlockManagerMaster(actor);
		}
		return master_;
	}
	virtual ~BlockManagerMaster();

	void initialize();

	/* 共测试使用 */
	void testForPoc();

	// 这个函数返回的是blockmanagerId
//	BlockManagerId getLocations(string blockId){};

private:
	BlockManagerMaster(BlockManagerMasterActor *driverActor);
private:
	static BlockManagerMaster *master_;
	// 将blockMessage收到之后，首先看他是什么消息，然后传给BlockManagerMasterActor处理
	BlockManagerMasterActor *driverActor_;
	AllBlockInfo *abi_;
	BlanceMatcher *bm_;
};


#endif /* BLOCKMANAGERMASTER_H_ */
