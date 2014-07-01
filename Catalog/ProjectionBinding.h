/*
 * ProjectionBinding.h
 *
 *  Created on: Nov 2, 2013
 *      Author: wangli
 */

#ifndef PROJECTIONBINDING_H_
#define PROJECTIONBINDING_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "Partitioner.h"
#include "../storage/StorageLevel.h"
class ProjectionBinding {
public:
	ProjectionBinding();
	virtual ~ProjectionBinding();

	/*binding an entire projection independently*/
	bool BindingEntireProjection(Partitioner*,const StorageLevel& desriable_storage_level=MEMORY);
};

#endif /* PROJECTIONBINDING_H_ */
