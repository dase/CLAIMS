/*
 * BlockStreamExchangeBase.h
 *
 *  Created on: Sep 4, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMEXCHANGEBASE_H_
#define BLOCKSTREAMEXCHANGEBASE_H_
#include <vector>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include "BlockStreamIteratorBase.h"

class BlockStreamExchangeBase:public BlockStreamIteratorBase {
public:
	BlockStreamExchangeBase(const std::vector<std::string> &upper_ip_list,const unsigned &nlowers);
	BlockStreamExchangeBase(){};
	virtual ~BlockStreamExchangeBase();
	virtual bool open(const PartitionOffset& part_off=0)=0;
	virtual bool next(BlockStreamBase*)=0;
	virtual bool close()=0;
protected:
	bool PrepareTheSocket();
    bool RegisterExchange(ExchangeID exchange_id);
    bool isMaster();
    bool WaitForConnectionFromLowerExchanges();
    void CloseTheSocket();
	void SendBlockBufferedNotification(int target_socket_fd);
	void SendBlockAllConsumedNotification(int target_socket_fd);
protected:
	unsigned nlowers_;
	int* socket_fd_lower_list_;
	std::string * lower_ip_array_;
private:
	int sock_fd_;
	unsigned socket_port_;

	std::vector<std::string> upper_ip_list_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this);
	}

};

#endif /* BLOCKSTREAMEXCHANGEBASE_H_ */
