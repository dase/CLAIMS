/*
 * Message.h
 *
 *  Created on: May 12, 2013
 *      Author: wangli
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

/*
 * Message.h
 *
 *  Created on: May 11, 2013
 *      Author: wangli
 *
 *      This header defines all kinds of messages that are transformed by Theron.
 */

#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <sstream>
#include <assert.h>
#include "TablePartition.h"
#include <Theron/Defines.h>
#include <Theron/Theron.h>
#include "iterator.hpp"
#include "Serialization/RegisterDerivedClass.h"
#include "BlockStreamIterator/BlockStreamIteratorBase.h"
#include "Debug.h"

struct RegisterStorageMessage{
	explicit RegisterStorageMessage(const char * const text){
		mText[0]='\0';
		memcpy(mText,text,REGISTER_MESSAGE_LEN);
	}
	char mText[REGISTER_MESSAGE_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(RegisterStorageMessage)

struct RegisterStorageRespond{
	explicit RegisterStorageRespond(const char * const text){
		mText[0]='\0';
		memcpy(mText,text,REGISTER_MESSAGE_LEN);
	}
	char mText[REGISTER_MESSAGE_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(RegisterStorageRespond)

struct HeartBeatMessage{
	explicit HeartBeatMessage(const char * const text){
		mText[0]='\0';
		memcpy(mText,text,HEARTBEAT_MESSAGE_LEN);
	}
	char mText[HEARTBEAT_MESSAGE_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(HeartBeatMessage)

struct HeartBeatRespond{
	explicit HeartBeatRespond(const char * const text){
		mText[0]='\0';
		memcpy(mText,text,HEARTBEAT_MESSAGE_LEN);
	}
	char mText[HEARTBEAT_MESSAGE_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(HeartBeatRespond)

struct BlockStatusMessage{
	explicit BlockStatusMessage(const char * const text){
		mText[0]='\0';
		memcpy(mText,text,BLOCK_STATUS_MESSAGE_LEN);
	}
	char mText[BLOCK_STATUS_MESSAGE_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(BlockStatusMessage)

struct BlockStatusRespond{
	explicit BlockStatusRespond(const char * const text){
		mText[0]='\0';
		memcpy(mText,text,BLOCK_STATUS_MESSAGE_LEN);
	}
	char mText[BLOCK_STATUS_MESSAGE_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(BlockStatusRespond)

struct MatcherMessage{
	explicit MatcherMessage(const char * const filename,const char *const bmi){
		filenameText[0]='\0';
		memcpy(filenameText,filename,MATCHER_MESSAGE_FILENAME_LEN);
		bmiText[0]='\0';
		memcpy(bmiText,bmi,MATCHER_MESSAGE_BMI_LEN);
	}
	char filenameText[MATCHER_MESSAGE_FILENAME_LEN];
	char bmiText[MATCHER_MESSAGE_BMI_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(MatcherMessage)

struct MatcherRespond{
	explicit MatcherRespond(const char * const text){
		mText[0]='\0';
		memcpy(mText,text,MATCHER_MESSAGE_PROJECT_LEN);
	}
	char mText[MATCHER_MESSAGE_PROJECT_LEN];
};
THERON_DECLARE_REGISTERED_MESSAGE(MatcherRespond)

struct Message256
{
	unsigned length;
	char message[256-sizeof(unsigned)];
	static unsigned Capacity()
	{
		return 256-sizeof(unsigned);
	}
};
THERON_DECLARE_REGISTERED_MESSAGE(Message256)
struct Message4K
{
	unsigned length;
	char message[4096-sizeof(unsigned)];
	static unsigned Capacity()
	{
		return 4096-sizeof(unsigned);
	}
};
THERON_DECLARE_REGISTERED_MESSAGE(Message4K)
THERON_DECLARE_REGISTERED_MESSAGE(int)
THERON_DECLARE_REGISTERED_MESSAGE(unsigned long long int)
template<typename T>
static T Deserialize(Message256 input)
{
	ostringstream ostr;
	ostr.write(input.message,input.length);

	std::string tmp=ostr.str();
	istringstream istr(tmp);

	T ret;
	boost::archive::text_iarchive ia(istr);
	ia>>ret;
	return ret;
}
template<typename T>
static Message256 Serialize(T object)
{
	ostringstream ostr;
	boost::archive::text_oarchive oa(ostr);
	oa<<object;

	//get the string
	std::string str=ostr.str();
	//in=str;
	assert(Message4K::Capacity()>=str.length());


	//copy the string into the message and set the length.
	Message256 ret;
	ret.length=str.length();
	//memcpy(&ret.message,&str,ret.length);
	str.copy(ret.message,ret.length);

	return ret;
}
template<typename T>
static T Deserialize(Message4K input)
{
	ostringstream ostr;
	ostr.write(input.message,input.length);

	std::string tmp=ostr.str();
	istringstream istr(tmp);

	T ret;
	boost::archive::text_iarchive ia(istr);
	ia>>ret;
	return ret;
}
template<typename T>
static Message4K Serialize4K(T &object)
{
//	std::cout<<"Serialize 1"<<std::endl;
	ostringstream ostr;
//	std::cout<<"Serialize 2"<<std::endl;
	boost::archive::text_oarchive oa(ostr);
//	std::cout<<"Serialize 3"<<std::endl;
	oa<<object;
//	std::cout<<"wrong?"<<endl;
	//get the string
	std::string str=ostr.str();
	//in=str;
	assert(Message4K::Capacity()>=str.length());
//	std::cout<<"Serialize 4"<<std::endl;
	//copy the string into the message and set the length.
	Message4K ret;
//	std::cout<<"Serialize 5"<<std::endl;
	ret.length=str.length();
//	std::cout<<"Serialize 6"<<std::endl;
	//memcpy(&ret.message,&str,ret.length);
	str.copy(ret.message,ret.length);

	return ret;
}




class CreateTableRespond
{
public:
	CreateTableRespond(){};
	CreateTableRespond(bool result):result(result){};
	static CreateTableRespond deserialize(Message256 message)
	{
		return Deserialize<CreateTableRespond>(message);
	}
	static Message256 serialize(CreateTableRespond input)
	{
		return Serialize<CreateTableRespond>(input);
	}
	bool result;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
	    ar & result;

	}

};


class CreateTableMessage
{
public:
	CreateTableMessage(){};
	CreateTableMessage(unsigned table_id,std::vector<data_type> type_list):table_id(table_id),type_list(type_list){};

	unsigned table_id;
	std::vector<data_type> type_list;

	static CreateTableMessage deserializae(Message256 message)
	{

		return Deserialize<CreateTableMessage>(message);

	}

	static Message256 serializae(CreateTableMessage input)
	{

		return Serialize<CreateTableMessage>(input);

	}
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
	    ar & table_id;
	    ar & type_list;

	}
};

class RegisterSlaveMessage
{
public:
	RegisterSlaveMessage(){};
	RegisterSlaveMessage(std::string ip,std::string port):ip(ip),port(port){};
	static RegisterSlaveMessage deserialize(Message256 message)
	{
		return Deserialize<RegisterSlaveMessage>(message);
	}
	static Message256 serialize(RegisterSlaveMessage input)
	{
		return Serialize<RegisterSlaveMessage>(input);
	}

	std::string ip;
	std::string port;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
	    ar & ip;
	    ar & port;

	}

};

/*
 * The early version of this class is only for the iterator serials,
 * the block stream iterator is also added here to save our labor.
 * TODO: In the release version, this class should be divided into two
 * classes to make the codes more readable.
 */
class IteratorMessage
{
public:
	IteratorMessage(Iterator* it)
	:tuple_stream_iterator_root_(it),block_stream_iterator_root_(0)
	{};
	IteratorMessage(BlockStreamIteratorBase* it)
		:tuple_stream_iterator_root_(0),block_stream_iterator_root_(it)
		{};

	IteratorMessage():tuple_stream_iterator_root_(0),block_stream_iterator_root_(0){};
	~IteratorMessage(){
		if(tuple_stream_iterator_root_>0)
			tuple_stream_iterator_root_->~Iterator();
		if(block_stream_iterator_root_>0)
			block_stream_iterator_root_->~BlockStreamIteratorBase();
	};
	void run()
	{
		if(tuple_stream_iterator_root_>0){
			tuple_stream_iterator_root_->open();
			while(tuple_stream_iterator_root_->next(0));
			tuple_stream_iterator_root_->close();
		}
		if(block_stream_iterator_root_>0){
			block_stream_iterator_root_->open();
			while(block_stream_iterator_root_->next(0));
			block_stream_iterator_root_->close();
		}


	}
	static IteratorMessage deserialize(Message256 message)
	{
		return Deserialize<IteratorMessage>(message);
	}

	static Message256 serialize(IteratorMessage input)
	{
		return Serialize<IteratorMessage>(input);
	}

	static IteratorMessage deserialize4K(Message4K message)
	{
		return Deserialize<IteratorMessage>(message);
	}

	static Message4K serialize4K(IteratorMessage& input)
	{
		std::cout<<"in the serialize4K func!"<<std::endl;
		return Serialize4K<IteratorMessage>(input);
	}
private:
	Iterator* tuple_stream_iterator_root_;
	BlockStreamIteratorBase* block_stream_iterator_root_;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		Register_Schemas(ar);
		Register_Tuple_Stream_Iterators(ar);
		Register_Block_Stream_Iterator(ar);
		ar & tuple_stream_iterator_root_ & block_stream_iterator_root_;
//		ar & block_stream_iterator_root_;

	}
};

class NodeConnectionMessage{
public:
	NodeConnectionMessage(){};
	NodeConnectionMessage(std::string ip,std::string port):ip(ip),port(port){};
	std::string ip;
	std::string port;
private:

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & ip & port;
	}
public:
	static NodeConnectionMessage deserialize(Message256 message){
		return Deserialize<NodeConnectionMessage>(message);
	}

	static Message256 serialize(NodeConnectionMessage input){
		return Serialize<NodeConnectionMessage>(input);
	}

	static NodeConnectionMessage deserialize4K(Message4K message){
		return Deserialize<NodeConnectionMessage>(message);
	}

	static Message4K serialize4K(NodeConnectionMessage input){
		return Serialize4K<NodeConnectionMessage>(input);
	}
};



#endif /* MESSAGE_H_ */
