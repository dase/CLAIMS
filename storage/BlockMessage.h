/*
 * BlockMessage.h
 *
 *  Created on: 2013-10-20
 *      Author: casa
 */

#ifndef BLOCKMESSAGE_H_
#define BLOCKMESSAGE_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
/*
 * 此类将被外层message吸收
 * */
class BlockMessage {
public:
	BlockMessage();
	virtual ~BlockMessage();
};

#endif /* BLOCKMESSAGE_H_ */
