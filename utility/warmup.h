/*
 * warmup.h
 *
 *  Created on: May 29, 2014
 *      Author: wangli
 */

#ifndef WARMUP_H_
#define WARMUP_H_
#include <xmmintrin.h>
typedef __m128 Unit;
/*
 * This function read the data block specified by start address and length to warm up the cache
 */
inline Unit warmup(void* start_addr,unsigned long length){
	unsigned  num=length/sizeof(Unit);
	unsigned i=0;
	Unit result;
	volatile Unit cheat;
	for(Unit* pointer=(Unit*)start_addr;i<num;i++,pointer++){
		result=_mm_and_ps(result,*pointer);
//		result+=*pointer;
	}
	cheat=result;
	return cheat;
}

#endif /* WARMUP_H_ */
