/*
 * Message.h
 *
 *  Created on: May 12, 2013
 *      Author: wangli
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_
#include "../common/Message.h"

#include <arpa/inet.h>
#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <sstream>
#include <assert.h>
#include "Theron/Defines.h"
#include "Theron/Theron.h"
#include "serialization/RegisterDerivedClass.h"
#include "../physical_operator/physical_operator_base.h"
#include "../Debug.h"
#include "../storage/StorageLevel.h"
#include "ids.h"

using claims::physical_operator::PhysicalOperatorBase;

// It's better to use fixed length information for implementation concern.
THERON_DECLARE_REGISTERED_MESSAGE(ExchangeID)
struct StorageBudgetMessage {
  explicit StorageBudgetMessage(const int& disk_budget,
                                const int& memory_budget, const int& nodeid)
      : disk_budget(disk_budget),
        memory_budget(memory_budget),
        nodeid(nodeid) {}
  int disk_budget;
  int memory_budget;
  int nodeid;
};
THERON_DECLARE_REGISTERED_MESSAGE(StorageBudgetMessage)

struct PartitionBindingMessage {
  PartitionBindingMessage(const PartitionID& pid, const unsigned& num,
                          const StorageLevel& level)
      : partition_id(pid), number_of_chunks(num), storage_level(level){};
  PartitionID partition_id;
  unsigned number_of_chunks;
  StorageLevel storage_level;
};
THERON_DECLARE_REGISTERED_MESSAGE(PartitionBindingMessage)

struct PartitionUnbindingMessage {
  PartitionUnbindingMessage(const PartitionID& pid) : partition_id(pid){};
  PartitionID partition_id;
};
THERON_DECLARE_REGISTERED_MESSAGE(PartitionUnbindingMessage)

struct RegisterStorageRespond {
  explicit RegisterStorageRespond(const char* const text) {
    mText[0] = '\0';
    memcpy(mText, text, REGISTER_MESSAGE_LEN);
  }
  char mText[REGISTER_MESSAGE_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(RegisterStorageRespond)

struct HeartBeatMessage {
  explicit HeartBeatMessage(const char* const text) {
    mText[0] = '\0';
    memcpy(mText, text, HEARTBEAT_MESSAGE_LEN);
  }
  char mText[HEARTBEAT_MESSAGE_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(HeartBeatMessage)

struct HeartBeatRespond {
  explicit HeartBeatRespond(const char* const text) {
    mText[0] = '\0';
    memcpy(mText, text, HEARTBEAT_MESSAGE_LEN);
  }
  char mText[HEARTBEAT_MESSAGE_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(HeartBeatRespond)

struct BlockStatusMessage {
  explicit BlockStatusMessage(const char* const text) {
    mText[0] = '\0';
    memcpy(mText, text, BLOCK_STATUS_MESSAGE_LEN);
  }
  char mText[BLOCK_STATUS_MESSAGE_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(BlockStatusMessage)

struct BlockStatusRespond {
  explicit BlockStatusRespond(const char* const text) {
    mText[0] = '\0';
    memcpy(mText, text, BLOCK_STATUS_MESSAGE_LEN);
  }
  char mText[BLOCK_STATUS_MESSAGE_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(BlockStatusRespond)

struct MatcherMessage {
  explicit MatcherMessage(const char* const filename, const char* const bmi) {
    filenameText[0] = '\0';
    memcpy(filenameText, filename, MATCHER_MESSAGE_FILENAME_LEN);
    bmiText[0] = '\0';
    memcpy(bmiText, bmi, MATCHER_MESSAGE_BMI_LEN);
  }
  char filenameText[MATCHER_MESSAGE_FILENAME_LEN];
  char bmiText[MATCHER_MESSAGE_BMI_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(MatcherMessage)

struct MatcherRespond {
  explicit MatcherRespond(const char* const text) {
    mText[0] = '\0';
    memcpy(mText, text, MATCHER_MESSAGE_PROJECT_LEN);
  }
  char mText[MATCHER_MESSAGE_PROJECT_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(MatcherRespond)

/* NodeRegisterMessage has the same function compared with NodeConnectionMessage
 * except for that
 * NodeRegisterMessage is fixed-length.*/
struct NodeRegisterMessage {
  explicit NodeRegisterMessage(std::string node_ip, unsigned node_port) {
    port = node_port;
    ip = inet_addr(node_ip.c_str());
  }
  void set_ip(std::string _ip) { ip = inet_addr(_ip.c_str()); }
  std::string get_ip() const {
    in_addr add;
    add.s_addr = ip;
    ostringstream str;
    str << inet_ntoa(add);
    std::string ret;
    ret = str.str();
    return ret;
  }
  unsigned ip;
  unsigned port;
};
THERON_DECLARE_REGISTERED_MESSAGE(NodeRegisterMessage)
struct Message256 {
  unsigned length;
  char message[256 - sizeof(unsigned)];
  static unsigned Capacity() { return 256 - sizeof(unsigned); }
};
THERON_DECLARE_REGISTERED_MESSAGE(Message256)
struct Message4K  // temporary ways to expand the the serialization capacity
    {
  unsigned length;
  char message[4096 * 50 - sizeof(unsigned)];
  static unsigned Capacity() { return 4096 * 50 - sizeof(unsigned); }
};
THERON_DECLARE_REGISTERED_MESSAGE(Message4K)
THERON_DECLARE_REGISTERED_MESSAGE(int)
THERON_DECLARE_REGISTERED_MESSAGE(unsigned long long int)
template <typename T>
static T Deserialize(Message256 input) {
  std::string received(input.message, input.length);

  boost::iostreams::basic_array_source<char> device(received.data(),
                                                    received.size());
  boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(
      device);
  boost::archive::binary_iarchive ia(s);

  T ret;
  ia >> ret;
  return ret;

  //	ostringstream ostr;
  //	ostr.write(input.message,input.length);
  //
  //	std::string tmp=ostr.str();
  //	istringstream istr(tmp);
  //
  //	T ret;
  //	boost::archive::text_iarchive ia(istr);
  //	ia>>ret;
  //	return ret;
}
template <typename T>
static Message256 Serialize(T object) {
  std::string serial_str;

  boost::iostreams::back_insert_device<std::string> inserter(serial_str);
  boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
      ostr(inserter);
  boost::archive::binary_oarchive oa(ostr);
  oa << object;
  ostr.flush();
  Message256 ret;
  assert(serial_str.size() <= Message256::Capacity());
  memcpy(ret.message, serial_str.data(), serial_str.size());
  ret.length = serial_str.size();

  //	ostringstream ostr;
  //	boost::archive::text_oarchive oa(ostr);
  //	oa<<object;
  //
  //	//get the string
  //	std::string str=ostr.str();
  //	//in=str;
  //	assert(Message4K::Capacity()>=str.length());
  //
  //
  //	//copy the string into the message and set the length.
  //	Message256 ret;
  //	ret.length=str.length();
  //	//memcpy(&ret.message,&str,ret.length);
  //	str.copy(ret.message,ret.length);

  return ret;
}
template <typename T>
static T Deserialize(Message4K input) {
  std::string received(input.message, input.length);

  boost::iostreams::basic_array_source<char> device(received.data(),
                                                    received.size());
  boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(
      device);
  boost::archive::binary_iarchive ia(s);

  T ret;
  ia >> ret;
  return ret;

  //	ostringstream ostr;
  //	ostr.write(input.message,input.length);
  //
  //	std::string tmp=ostr.str();
  //	istringstream istr(tmp);
  //
  //	T ret;
  //	boost::archive::text_iarchive ia(istr);
  //	ia>>ret;
  //	return ret;
}
template <typename T>
static Message4K Serialize4K(T& object) {
  std::string serial_str;

  boost::iostreams::back_insert_device<std::string> inserter(serial_str);
  boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> >
      ostr(inserter);
  boost::archive::binary_oarchive oa(ostr);
  oa << object;
  ostr.flush();

  Message4K ret;
  assert(serial_str.size() <= Message4K::Capacity() && serial_str.size() != 0);
  memcpy(ret.message, serial_str.data(), serial_str.size());
  ret.length = serial_str.size();

  return ret;

  ////	std::cout<<"Serialize 1"<<std::endl;
  //	ostringstream ostr;
  ////	std::cout<<"Serialize 2"<<std::endl;
  //	boost::archive::text_oarchive oa(ostr);
  ////	std::cout<<"Serialize 3"<<std::endl;
  //	oa<<object;
  ////	std::cout<<"wrong?"<<endl;
  //	//get the string
  //	std::string str=ostr.str();
  //	//in=str;
  //	assert(Message4K::Capacity()>=str.length());
  ////	std::cout<<"Serialize 4"<<std::endl;
  //	//copy the string into the message and set the length.
  //	Message4K ret;
  ////	std::cout<<"Serialize 5"<<std::endl;
  //	ret.length=str.length();
  ////	std::cout<<"Serialize 6"<<std::endl;
  //	//memcpy(&ret.message,&str,ret.length);
  //	str.copy(ret.message,ret.length);
  //
  //	return ret;
}

class CreateTableRespond {
 public:
  CreateTableRespond(){};
  CreateTableRespond(bool result) : result(result){};
  static CreateTableRespond deserialize(Message256 message) {
    return Deserialize<CreateTableRespond>(message);
  }
  static Message256 serialize(CreateTableRespond input) {
    return Serialize<CreateTableRespond>(input);
  }
  bool result;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& result;
  }
};

class CreateTableMessage {
 public:
  CreateTableMessage(){};
  CreateTableMessage(unsigned table_id, std::vector<data_type> type_list)
      : table_id(table_id), type_list(type_list){};

  unsigned table_id;
  std::vector<data_type> type_list;

  static CreateTableMessage deserializae(Message256 message) {
    return Deserialize<CreateTableMessage>(message);
  }

  static Message256 serializae(CreateTableMessage input) {
    return Serialize<CreateTableMessage>(input);
  }

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& table_id;
    ar& type_list;
  }
};

class RegisterSlaveMessage {
 public:
  RegisterSlaveMessage(){};
  RegisterSlaveMessage(std::string ip, std::string port) : ip(ip), port(port){};
  static RegisterSlaveMessage deserialize(Message256 message) {
    return Deserialize<RegisterSlaveMessage>(message);
  }
  static Message256 serialize(RegisterSlaveMessage input) {
    return Serialize<RegisterSlaveMessage>(input);
  }

  std::string ip;
  std::string port;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& ip;
    ar& port;
  }
};

/*
 * The early version of this class is only for the iterator serials,
 * the block stream iterator is also added here to save our labor.
 * TODO: In the release version, this class should be divided into two
 * classes to make the codes more readable.
 */
class PhysicalQueryPlan {
 public:
  PhysicalQueryPlan(PhysicalOperatorBase* it)
      : block_stream_iterator_root_(it){};
  PhysicalQueryPlan(const PhysicalQueryPlan& r) {
    block_stream_iterator_root_ = r.block_stream_iterator_root_;
  }

  PhysicalQueryPlan() : block_stream_iterator_root_(0){};
  ~PhysicalQueryPlan(){
      //		if(tuple_stream_iterator_root_>0)
      //			tuple_stream_iterator_root_->~Iterator();
      //		if(block_stream_iterator_root_>0)
      //			block_stream_iterator_root_->~BlockStreamIteratorBase();
  };

  /* the member varaibles' destrution code is implemented in this function to
   * avoid
   * the undesirable destruction of iterator caused by the default destructor of
   * IteratorMessage
   */
  void destory() { block_stream_iterator_root_->~PhysicalOperatorBase(); }
  void run();
  static PhysicalQueryPlan deserialize(Message256 message) {
    return Deserialize<PhysicalQueryPlan>(message);
  }

  static Message256 serialize(PhysicalQueryPlan input) {
    return Serialize<PhysicalQueryPlan>(input);
  }

  static PhysicalQueryPlan deserialize4K(Message4K message) {
    return Deserialize<PhysicalQueryPlan>(message);
  }

  static Message4K serialize4K(PhysicalQueryPlan& input) {
    //		std::cout<<"in the serialize4K func!"<<std::endl;
    return Serialize4K<PhysicalQueryPlan>(input);
  }

 private:
  PhysicalOperatorBase* block_stream_iterator_root_;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    Register_Schemas(ar);
    Register_Block_Stream_Iterator(ar);
    ar& block_stream_iterator_root_;
    //		ar & block_stream_iterator_root_;
  }
};

class NodeConnectionMessage {
 public:
  NodeConnectionMessage(){};
  NodeConnectionMessage(std::string ip, std::string port)
      : ip(ip), port(port){};
  std::string ip;
  std::string port;

 private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& ip& port;
  }

 public:
  static NodeConnectionMessage deserialize(Message256 message) {
    return Deserialize<NodeConnectionMessage>(message);
  }

  static Message256 serialize(NodeConnectionMessage input) {
    return Serialize<NodeConnectionMessage>(input);
  }

  static NodeConnectionMessage deserialize4K(Message4K message) {
    return Deserialize<NodeConnectionMessage>(message);
  }

  static Message4K serialize4K(NodeConnectionMessage input) {
    return Serialize4K<NodeConnectionMessage>(input);
  }
};

#endif /* MESSAGE_H_ */
