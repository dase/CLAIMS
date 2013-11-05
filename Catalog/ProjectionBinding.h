/*
 * ProjectionBinding.h
 *
 *  Created on: Nov 2, 2013
 *      Author: wangli
 */

#ifndef PROJECTIONBINDING_H_
#define PROJECTIONBINDING_H_
#include "Partitioner.h"

class ProjectionBinding {
public:
	ProjectionBinding();
	virtual ~ProjectionBinding();

	/*binding an entire projection independently*/
	bool BindingEntireProjection(Partitioner*);
};

#endif /* PROJECTIONBINDING_H_ */
