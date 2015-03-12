/*
 * ClientResponse.h
 *
 *  Created on: Feb 19, 2014
 *      Author: volt
 */

#ifndef CLIENTRESPONSE_H_
#define CLIENTRESPONSE_H_

#include <assert.h>
#include <malloc.h>
//#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/detail/interface_iarchive.hpp>
#include <boost/archive/detail/interface_iarchive.hpp>
//#include <boost/archive/detail/interface_oarchive.hpp>
#include <boost/archive/detail/interface_oarchive.hpp>
//#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
//#include <boost/iostreams/device/back_inserter.hpp>
//#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream.hpp>
#include <malloc.h>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#ifdef DMALLOC
#include "dmalloc.h"
#endif

#include "../common/Block/BlockStream.h"
#include "../common/Schema/SchemaFix.h"
#include "../common/Logging.h"

#define Error 	0
#define OK 		1
#define SCHEMA	2
#define HEADER	3
#define DATA	4
#define END		5
#define CHANGE 6
struct ColumnHeader {
	std::vector<std::string> header_list;
	void add_header(std::string name) {
		header_list.push_back(name);
	}
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & header_list;
	}
};

struct ClientResponse {
	int status;
	int length;
	std::string content;
	unsigned get_length() {
		assert(length == content.length());
		return sizeof(status) + sizeof(length) + content.length();
	}

	std::string getMessage(){
		switch(status){
		case Error:
			return "Error";
			break;
		case OK:
			return "OK";
			break;
		case SCHEMA:
			return "Schema";
			break;
		case HEADER:
			return "Header";
			break;
		case DATA:
			return "Data";
			break;
		case END:
			return "End";
			break;
		}
		return "Unknown";
	}

	void setError(std::string reason) {
		status = Error;
		length = reason.length();
		content = reason;
	}
	std::string getError() const {
		assert(status==Error);
		return content;
	}
	void setEnd(double query_time) {
		status = END;
		std::ostringstream str;
		str<<query_time;
		length = str.str().size();
		content =str.str();
	}
	std::string getEndInfo() const {
		assert(status==END);
		return content;
	}

	void setOk(std::string info) {
		status = OK;
		length = info.length();
		content = info;
	}
	std::string getOK() const {
		assert(status==OK);
		return content;
	}

	void setSchema(Schema* schema) {
		std::string serial_str;

		boost::iostreams::back_insert_device<std::string> inserter(serial_str);
		boost::iostreams::stream<
				boost::iostreams::back_insert_device<std::string> > ostr(
				inserter);
		boost::archive::binary_oarchive oa(ostr);
		oa.register_type(static_cast<SchemaFix *>(NULL));
		oa << schema;
		ostr.flush();
		content = serial_str;
		status = SCHEMA;
		length = serial_str.length();
	}
	Schema* getSchema() const {
		assert(status==SCHEMA);
		boost::iostreams::basic_array_source<char> device(content.data(),
				content.size());
		boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(
				device);
		boost::archive::binary_iarchive ia(s);
		ia.register_type(static_cast<SchemaFix *>(NULL));

		Schema* ret;
		ia >> ret;
		return ret;
	}

	void setAttributeName(ColumnHeader& header) {
		std::string serial_str;

		boost::iostreams::back_insert_device<std::string> inserter(serial_str);
		boost::iostreams::stream<
				boost::iostreams::back_insert_device<std::string> > ostr(
				inserter);
		boost::archive::binary_oarchive oa(ostr);
		oa << header;
		ostr.flush();
		content = serial_str;
		status = HEADER;
		length = content.length();
	}
	ColumnHeader getAttributeName() const {
		assert(status==HEADER);
		boost::iostreams::basic_array_source<char> device(content.data(),
				content.size());
		boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(
				device);
		boost::archive::binary_iarchive ia(s);

		ColumnHeader ret;
		ia >> ret;
		return ret;
	}

	void setDataBlock(Block& block) {

		content = std::string((const char *) block.getBlock(), block.getsize());
		status = DATA;
		length = content.length();
	}
	BlockStreamBase* getDataBlock(Schema* schema) const {
		assert(status==DATA);
		Block block(length, content.data());
//		BlockStreamBase* ret = BlockStreamBase::createBlock(schema, length-sizeof(int));
		BlockStreamBase* ret = BlockStreamBase::createBlock(schema, length);
		ret->constructFromBlock(block);
		return ret;
	}

	void setData(string s) {
		content = s;
		status = DATA;
		length = s.length();
	}

	int serialize(char*& buffer) const {
		int ret = sizeof(int) + sizeof(int) + content.length();
		buffer = (char *) malloc(ret);
		*(int*) buffer = status;		// 小端字节序
		*((int*) buffer + 1) = length;
		void* content_start = buffer + sizeof(int) + sizeof(int);
		memcpy(content_start, content.data(), content.length());

		ClientLogging::log("buffer to send is : %d-%d-%d-%d-%d-%d-%d-%d|	%d=%d",
				(unsigned int)buffer[0],
				(unsigned int)buffer[1],
				(unsigned int)buffer[2],
				(unsigned int)buffer[3],
				(unsigned int)buffer[4],
				(unsigned int)buffer[5],
				(unsigned int)buffer[6],
				(unsigned int)buffer[7],
				status,
				length
				);
		return ret;
	}

	void deserialize(const char* received_buffer, int recieved_bytes_length) {
		const int st = *(int*) received_buffer;
		const int len = *((int*) received_buffer + 1);
		if(!(recieved_bytes_length == len)){
			printf("recieved_bytes_length %d, len %d\n",recieved_bytes_length, len);
			assert(false);
		}
		status = st;
		length = len;
		void* content_start_addr = (char*) received_buffer + sizeof(int) * 2;
		content = std::string((const char *) content_start_addr, len);

	}

	void setChange(std::string info) {
		status = CHANGE;
		length = info.length();
		content = info;
	}

	std::string getChange() const {
		assert(status == CHANGE);
		return content;
	}
};

#endif /* CLIENTRESPONSE_H_ */
