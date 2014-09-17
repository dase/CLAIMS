/*
 * BlockStreamExchangeLowerBase.h
 *
 *  Created on: Sep 4, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMEXCHANGELOWERBASE_H_
#define BLOCKSTREAMEXCHANGELOWERBASE_H_
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include <boost/serialization/utility.hpp>
#include "BlockStreamIteratorBase.h"
#include "../common/ids.h"
class BlockStreamExchangeLowerBase: public BlockStreamIteratorBase {
public:
	BlockStreamExchangeLowerBase();
	virtual ~BlockStreamExchangeLowerBase();
	virtual bool open(const PartitionOffset& part_off=0)=0;
	virtual bool next(BlockStreamBase* )=0;
	virtual bool close()=0;
protected:
	bool ConnectToUpper(const ExchangeID &exchange_id,const std::string &ip,int &sock_fd) const;
	void WaitingForNotification(const int &target_socket_fd) const;
	void WaitingForCloseNotification(const int & target_socket_fd) const;
	unsigned hash(void* input_tuple, Schema* schema, unsigned partition_key_index, unsigned nuppers);
private:

private:

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & boost::serialization::base_object<BlockStreamIteratorBase>(*this);

	}
};

#endif /* BLOCKSTREAMEXCHANGELOWERBASE_H_ */
