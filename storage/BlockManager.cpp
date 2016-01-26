/*
 * BlockManager.cpp
 *
 *  Created on: 2013-10-11
 *      Author: casa
 */
#include <sstream>
#include "BlockManager.h"
#include "../Environment.h"
#include "../common/rename.h"
#include "../common/Message.h"
#include "../common/Logging.h"
#include "../Config.h"
BlockManager* BlockManager::blockmanager_ = 0;

BlockManager* BlockManager::getInstance() {
  if (blockmanager_ == 0) {
    blockmanager_ = new BlockManager();
  }
  return blockmanager_;
}
BlockManager::BlockManager() {
  framework_ =
      new Theron::Framework(*Environment::getInstance()->getEndPoint());
  std::ostringstream actor_name;
  actor_name << "blockManagerWorkerActor://"
             << Environment::getInstance()->getNodeID();

  actor_ =
      new BlockManagerWorkerActor(framework_, actor_name.str().c_str(), this);
  logging_ = new StorageManagerLogging();
  logging_->log("BlockManagerSlave is initialized. The ActorName=%s",
                actor_name.str().c_str());
  memstore_ = MemoryChunkStore::getInstance();
}
BlockManager::~BlockManager() {
  blockmanager_ = 0;
  delete actor_;
  delete framework_;
  delete logging_;
  delete memstore_;
}
MemoryChunkStore* BlockManager::getMemoryChunkStore() const {
  return memstore_;
}

void BlockManager::initialize() {
  // 读配置文件中的配置，然后根据是否是master注册
  // 1，建两个存储，一个是内存的，一个磁盘的
  diskstore_ = new DiskStore(DISKDIR);
  memstore_ = MemoryChunkStore::getInstance();

  /// the version written by zhanglei/////////////////////////////////
  //	blockManagerId_=new BlockManagerId();
  // 2，注册
  registerToMaster(blockManagerId_);
  // 3，开启心跳监听
  heartBeat();
  ///////////////////////////////////////////////////////////////////

  //////////////the version written by Li////////////////////////////
  // the following values should be read from configure file.
  const int memory =
      BufferManager::getInstance()->getStorageMemoryBudegeInMilibyte();
  const int disk = 1000000;
  const NodeID NodeID = Environment::getInstance()->getNodeID();
  //

  StorageBudgetMessage message(disk, memory, NodeID);

  Environment::getInstance()->getResourceManagerSlave()->ReportStorageBudget(
      message);
}

void BlockManager::registerToMaster(BlockManagerId* blockManagerId) {
  worker_->_reigisterToMaster(blockManagerId);
}

void BlockManager::heartBeat() {
  // 向master发送心跳信息,3秒一次，在此必须开一个新的线程
  //	while(true){
  // 可以在这里有个配置property的指定，然后优化网络
  //		sleep(3);
  worker_->_sendHeartBeat();
  //	}
  reregister();
}

void BlockManager::reregister() {
  // 当storage节点出现失联，在再次注册的时候要上报block的信息,包括blockId和block的storageLevel
  reportAllBlocks();
}

void BlockManager::asyncReregister() {
  // 在put进去本地的存储的时候突然失联，重新注册，和一般失联会有区别
}

void BlockManager::reportAllBlocks() {
  // 在此函数中有个for循环将这个节点中的每个block的信息都上报
  // 与其作对比的还有reportBlockStatus函数，这个函数一般在block的状态有所变化的时候
  // 调用，reportBlockStatus里面还是会调用tryToReportBlockStatus如果不成功
  // 就重新开始注册
}

bool BlockManager::reportBlockStatus(string blockId) {
  if (!tryToReportBlockStatus(blockId)) {
    asyncReregister();
  }
  return true;
}

// 向master发送blocks的信息，当收到master的回应的时候
bool BlockManager::tryToReportBlockStatus(string blockId) {
  worker_->_reportBlockStatus(blockId);
  return true;
}

void BlockManager::get(string blockId) { getLocal(blockId); }

void* BlockManager::getLocal(string blockId) {
  void* rt = 0;
  bool exists = false;
  map<string, BlockInfo*>::iterator it_;
  it_ = blockInfoPool_.find(blockId);
  if (it_ != blockInfoPool_.end()) {
    exists = true;
  }
  if (exists) {
    // 如果存在就代表文件从hdfs上已经被拉到local了
    if ((*it_).second->level_ == BlockManager::memory) {
      // spark中的storageLevel是磁盘和内存中都有的，在storageLevel.scala中
      // 我们有那样的应用吗？todo:
      // 在此预留的序列化和反序列化接口，序列化与否也是
      // 在storageLevel中的，是否备份也是在storageLevel中
      rt = memstore_->getChunk(blockId);
      return rt;
      // 这里不需要再判断是否在内存中了
    }
    if ((*it_).second->level_ == BlockManager::disk) {
      // todo: 将磁盘上的数据load到内存中返回，将指针再次存储在内存中
    }
  } else {
    // 还不在local上面，此时就要调用函数从hdfs上取
    cout << "the chunkId is not registered locally, it's on the hdfs!" << endl;
    ChunkInfo ci = loadFromHdfs(blockId);
    put(blockId, BlockManager::memory, ci.hook);
    rt = ci.hook;
  }
  return rt;
}

bool BlockManager::put(string blockId, storageLevel level, void* value) {
  // 由于stroageLevel的单一，所以put也比较简单，put里面直接加上blockId的value
  // replication和serialization都没有考虑进去 todo: 数据在内存中的备份
  BlockInfo* bi = new BlockInfo(level);
  blockInfoPool_[blockId.c_str()] = bi;
  if (level == BlockManager::memory) {
    memstore_->putValue(blockId, value);
    // 在这一层中，storageLevel就没有了，但是在spark中是有的，因在此比较单一
  }
  if (level == BlockManager::disk) {
    diskstore_->putValue(blockId, value);
  }
  reportBlockStatus(blockId);
  return true;
}

//// 这个函数返回一个构造好的chunkid和每个chunk的指针
//// 这个里面的chunkId肯定是要在blockManager注册然后汇报信息的
//// put的话也是会这样的，可以将这个函数中调用put然后统一汇报信息的接口
// vector<ChunkInfo> BlockManager::loadFromHdfs(string file_name){
//	// 由此函数得到的<blockId,指针>
//	vector<ChunkInfo> vci;
//	map<string, void *>::iterator it_;
//	it_=bufferpool_.find(file_name);
//	if(it_!=bufferpool_.end()){
////		rt=bufferpool_[file_name];
//	}
//	else{
//		hdfsFS fs=hdfsConnect(HDFS_N,9000);
//		hdfsFile
//readFile=hdfsOpenFile(fs,file_name.c_str(),O_RDONLY,0,0,0);
//		hdfsFileInfo *hdfsfile=hdfsGetPathInfo(fs,file_name.c_str());
////		char
///***path=hdfsGetHosts(fs,"/home/hayue/input/3_64m",0,201326592+12);
//		cout<<"file size: "<<hdfsfile->mSize<<endl;
//		if(!readFile){
//			cout<<"open file error"<<endl;
//		}
//		unsigned length=0;
//		int offset=0;
//		while(length<hdfsfile->mSize){
//			ChunkInfo ci;
//			void *rt=malloc(CHUNK_SIZE);		//newmalloc
//			tSize
//bytes_num=hdfsPread(fs,readFile,length,rt,CHUNK_SIZE);
//			cout<<"split interface: "<<bytes_num<<endl;
//			ostringstream chunkid;
//			chunkid<<file_name.c_str()<<"_"<<offset++;
//			ci.chunkId=chunkid.str().c_str();
//			ci.hook=rt;
//			vci.push_back(ci);
//			cout<<ci.chunkId.c_str()<<"---"<<length<<endl;
//			length=length+CHUNK_SIZE;
//		}
////		hdfsSeek(fs,readFile,CHUNK_SIZE);
//		hdfsCloseFile(fs,readFile);
////		cout<<bytes_num<<endl;
////		bufferpool_[file_name]=rt;
//	}
//	return vci;
//}

// 这个函数返回一个构造好的chunkid和每个chunk的指针
// 这个里面的chunkId肯定是要在blockManager注册然后汇报信息的
// put的话也是会这样的，可以将这个函数中调用put然后统一汇报信息的接口
ChunkInfo BlockManager::loadFromHdfs(string file_name) {
  // 由此函数得到的<blockId,指针>
  ChunkInfo ci;
  string file_name_former, file_name_latter;
  unsigned pos = file_name.rfind("$");
  file_name_former = file_name.substr(0, pos);
  file_name_latter = file_name.substr(pos + 1, file_name.length());
  int offset = atoi(file_name_latter.c_str());
  hdfsFS fs =
      hdfsConnect(Config::hdfs_master_ip.c_str(), Config::hdfs_master_port);
  hdfsFile readFile =
      hdfsOpenFile(fs, file_name_former.c_str(), O_RDONLY, 0, 0, 0);
  hdfsFileInfo* hdfsfile = hdfsGetPathInfo(fs, file_name_former.c_str());
  if (!readFile) {
    cout << "open file error" << endl;
  }
  unsigned length = 0;
  length = length + CHUNK_SIZE * offset;
  if (length < hdfsfile->mSize) {
    void* rt = malloc(CHUNK_SIZE);  // newmalloc
    tSize bytes_num = hdfsPread(fs, readFile, length, rt, CHUNK_SIZE);
    ostringstream chunkid;
    chunkid << file_name.c_str() << "$" << offset;
    //		ci.chunkId=chunkid.gestr().c_str();
    ci.hook = rt;
  } else {
    ostringstream chunkid;
    chunkid << file_name.c_str() << "$" << offset;
    //		ci.chunkId=chunkid.str().c_str();
    ci.hook = 0;
  }
  hdfsCloseFile(fs, readFile);
  hdfsDisconnect(fs);
  return ci;
}
int BlockManager::loadFromHdfs(const ChunkID& chunk_id, void* const& desc,
                               const unsigned& length) {
  lock.acquire();
  int ret;
  uint64_t offset = chunk_id.chunk_off;
  hdfsFS fs =
      hdfsConnect(Config::hdfs_master_ip.c_str(), Config::hdfs_master_port);
  hdfsFile readFile = hdfsOpenFile(
      fs, chunk_id.partition_id.getPathAndName().c_str(), O_RDONLY, 0, 0, 0);
  hdfsFileInfo* hdfsfile = hdfsGetPathInfo(
      fs, chunk_id.partition_id.getPathAndName()
              .c_str());  // to be refined after communicating with Zhang Lei
  if (!readFile) {
    logging_->elog("Fail to open file [%s].Reason:%s",
                   chunk_id.partition_id.getPathAndName().c_str(),
                   strerror(errno));
    hdfsDisconnect(fs);
    lock.release();
    return -1;
  } else {
    logging_->log("file [%s] is opened for offset[%d]\n",
                  chunk_id.partition_id.getPathAndName().c_str(), offset);
  }
  uint64_t start_pos = CHUNK_SIZE * offset;
  if (start_pos < hdfsfile->mSize) {
    ret = hdfsPread(fs, readFile, start_pos, desc, length);
  } else {
    lock.release();
    ret = -1;
  }
  hdfsCloseFile(fs, readFile);
  hdfsDisconnect(fs);
  lock.release();
  return ret;
}
int BlockManager::loadFromDisk(const ChunkID& chunk_id, void* const& desc,
                               const unsigned& length) const {
  int ret;
  unsigned offset = chunk_id.chunk_off;
  int fd = FileOpen(chunk_id.partition_id.getPathAndName().c_str(), O_RDONLY);
  if (fd == -1) {
    logging_->elog("Fail to open file [%s].Reason:%s",
                   chunk_id.partition_id.getPathAndName().c_str(),
                   strerror(errno));
    return -1;
  } else {
    logging_->log("file [%s] is opened for offset[%d]\n",
                  chunk_id.partition_id.getPathAndName().c_str(), offset);
  }
  long int file_length = lseek(fd, 0, SEEK_END);

  long start_pos = CHUNK_SIZE * offset;
  logging_->log("start_pos=%ld**********\n", start_pos);

  lseek(fd, start_pos, SEEK_SET);
  if (start_pos < file_length) {
    ret = read(fd, desc, length);
  } else {
    ret = 0;
  }
  FileClose(fd);
  return ret;
}

BlockManagerId* BlockManager::getId() { return blockManagerId_; }

string BlockManager::askForMatch(string filename, BlockManagerId bmi) {
  if (!file_proj_.count(filename.c_str())) {
    string rt = worker_->_askformatch(filename, bmi);
    file_proj_[filename.c_str()] = rt;
  }
  return file_proj_[filename.c_str()];
}
bool BlockManager::containsPartition(const PartitionID& part) const {
  boost::unordered_map<PartitionID, PartitionStorage*>::const_iterator it =
      partition_id_to_storage_.find(part);
  return !(it == partition_id_to_storage_.cend());
}
bool BlockManager::addPartition(const PartitionID& partition_id,
                                const unsigned& number_of_chunks,
                                const StorageLevel& desirable_storage_level) {
  boost::unordered_map<PartitionID, PartitionStorage*>::const_iterator it =
      partition_id_to_storage_.find(partition_id);
  if (it != partition_id_to_storage_.cend()) {
    partition_id_to_storage_[partition_id]->updateChunksWithInsertOrAppend(
        partition_id, number_of_chunks, desirable_storage_level);
    logging_->log(
        "Successfully updated partition[%s](desriable_storage_level = %d)!",
        partition_id.getName().c_str(), desirable_storage_level);
    return true;
  }
  partition_id_to_storage_[partition_id] = new PartitionStorage(
      partition_id, number_of_chunks, desirable_storage_level);
  logging_->log("Successfully added partition[%s](desriable_storage_level=%d)!",
                partition_id.getName().c_str(), desirable_storage_level);
  return true;
}

bool BlockManager::removePartition(const PartitionID& partition_id) {
  boost::unordered_map<PartitionID, PartitionStorage*>::iterator it =
      partition_id_to_storage_.find(partition_id);
  if (it == partition_id_to_storage_.cend()) {
    logging_->elog("Fail to unbinding partition [%s].",
                   partition_id.getName().c_str());
    return false;
  }
  it->second->removeAllChunks(it->first);
  partition_id_to_storage_.erase(it);
  return true;
}

PartitionStorage* BlockManager::getPartitionHandle(
    const PartitionID& partition_id) const {
  boost::unordered_map<PartitionID, PartitionStorage*>::const_iterator it =
      partition_id_to_storage_.find(partition_id);
  if (it == partition_id_to_storage_.cend()) {
    return 0;
  }
  return it->second;
}
BlockManager::BlockManagerWorkerActor::BlockManagerWorkerActor(
    Theron::Framework* framework, const char* name, BlockManager* bm)
    : Actor(*framework, name), bm_(bm) {
  RegisterHandler(this, &BlockManagerWorkerActor::getBlock);
  RegisterHandler(this, &BlockManagerWorkerActor::putBlock);
  RegisterHandler(this, &BlockManagerWorkerActor::BindingPartition);
  RegisterHandler(this, &BlockManagerWorkerActor::UnbindingPartition);
}

BlockManager::BlockManagerWorkerActor::~BlockManagerWorkerActor() {}

bool BlockManager::BlockManagerWorkerActor::_reigisterToMaster(
    BlockManagerId* bMId) {
  //	cout<<"in the worker actor to register"<<endl;
  //	//
  //在注册的时候，就有receiverId的构造，也就是用这个string来作为所有的stroage
  //node的标志，
  //	// 不会出现冲突，是因为传输的message是不一样的，
  //	receiverId_=bMId->blockManagerId;
  //	StorageBudgetMessage rsm(receiverId_.c_str());
  //
  //	tor_=new TimeOutReceiver(endpoint_,receiverId_.c_str());
  //	Theron::Catcher<RegisterStorageRespond> resultCatcher;
  //	tor_->RegisterHandler(&resultCatcher,
  //&Theron::Catcher<RegisterStorageRespond>::Push);
  //	framework_->Send(rsm,tor_->GetAddress(),Theron::Address("blockManagerMasterActor"));
  //	// TimeOutWait(count,time_out),如果返回的数值小于count,那就是超时了
  //	if(tor_->TimeOutWait(1,1000)==1){
  //		cout<<"register respond"<<endl;
  //		return true;
  //	}else{
  //		cout<<"not receive the register respond"<<endl;
  //		return false;
  //	}
  return true;
}

bool BlockManager::BlockManagerWorkerActor::_sendHeartBeat() {
  //	string alive="I am ok";
  //	HeartBeatMessage hbm(alive.c_str());
  //	Theron::Catcher<HeartBeatRespond> resultCatcher;
  //	tor_->RegisterHandler(&resultCatcher,
  //&Theron::Catcher<HeartBeatRespond>::Push);
  //	framework_->Send(hbm,tor_->GetAddress(),Theron::Address("blockManagerMasterActor"));
  //	// TimeOutWait(count,time_out),如果返回的数值小于count,那就是超时了
  //	if(tor_->TimeOutWait(1,1000)==1){
  //		cout<<"heartbeat respond"<<endl;
  //		return true;
  //	}else{
  //		cout<<"not receive heartbeat respond"<<endl;
  //		return false;
  //	}
  return true;
}

bool BlockManager::BlockManagerWorkerActor::_reportBlockStatus(string blockId) {
  //	BlockStatusMessage bsm(blockId.c_str());
  //	Theron::Catcher<BlockStatusRespond> resultCatcher;
  //	tor_->RegisterHandler(&resultCatcher,
  //&Theron::Catcher<BlockStatusRespond>::Push);
  //	framework_->Send(bsm,tor_->GetAddress(),Theron::Address("blockManagerMasterActor"));
  //	// TimeOutWait(count,time_out),如果返回的数值小于count,那就是超时了
  //	if(tor_->TimeOutWait(1,1000)==1){
  //		cout<<"block status respond"<<endl;
  //		return true;
  //	}else{
  //		cout<<"not receive block status respond"<<endl;
  //		return false;
  //	}
  return true;
}

string BlockManager::BlockManagerWorkerActor::_askformatch(string filename,
                                                           BlockManagerId bmi) {
  //	MatcherMessage mm(filename.c_str(),bmi.blockManagerId.c_str());
  //	Theron::Catcher<MatcherRespond> resultCatcher;
  //	tor_->RegisterHandler(&resultCatcher,
  //&Theron::Catcher<MatcherRespond>::Push);
  //	framework_->Send(mm,tor_->GetAddress(),Theron::Address("blockManagerMasterActor"));
  //	// TimeOutWait(count,time_out),如果返回的数值小于count,那就是超时了
  //	cout<<"already send the message of matcher out"<<endl;
  //	if(tor_->TimeOutWait(1,1000)==1){
  //		cout<<"matcher respond"<<endl;
  //		MatcherRespond mr("");
  //		Theron::Address addr("blockManagerMasterActor");
  //		resultCatcher.Pop(mr,addr);
  //		cout<<"the receiver string is: "<<mr.mText<<endl;
  //		return mr.mText;
  //	}else{
  //		cout<<"not receive matcher respond"<<endl;
  //	}
  return string("Hello~");
}
void BlockManager::BlockManagerWorkerActor::BindingPartition(
    const PartitionBindingMessage& message, const Theron::Address from) {
  bm_->addPartition(message.partition_id, message.number_of_chunks,
                    message.storage_level);
  Send(int(0), from);
}

void BlockManager::BlockManagerWorkerActor::UnbindingPartition(
    const PartitionUnbindingMessage& message, const Theron::Address from) {
  bm_->removePartition(message.partition_id);
  Send(int(0), from);
}
