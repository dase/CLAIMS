/*
 * BlockStreamIteratorBase.h
 *
 *  Created on: Aug 26, 2013
 *      Author: wangli
 */

#ifndef BLOCKSTREAMITERATORBASE_H_
#define BLOCKSTREAMITERATORBASE_H_
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <string>
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../common/Block/ResultSet.h"
#include "../common/Block/BlockStream.h"
#include "../common/ids.h"



using namespace std;

/**
 * This is the base class for the block stream iterators.
 */
class BlockStreamIteratorBase {
public:
	BlockStreamIteratorBase();
	virtual ~BlockStreamIteratorBase();

	static BlockStreamIteratorBase * createIterator(const string &IteratorName);

	virtual bool Open(const PartitionOffset& part_off=0)=0;
	virtual bool Next(BlockStreamBase*)=0;
	virtual bool Close()=0;
	virtual void Print(){printf("??\n");};

	/*
	 * Suggested by scdong, this method can be removed.
	 */
	virtual ResultSet* getResultSet();

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		/*This is the base class, nothing needs to be serialized.*/
	}
};

#endif /* BLOCKSTREAMITERATORBASE_H_ */
