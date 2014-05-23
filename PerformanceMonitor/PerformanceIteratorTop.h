/*
 * PerformanceIteratorTop.h
 *
 *  Created on: Aug 14, 2013
 *      Author: wangli
 */

#ifndef PERFORMANCEITERATORTOP_H_
#define PERFORMANCEITERATORTOP_H_
#include "../common/Schema/Schema.h"
#include "../iterator.hpp"
#include <pthread.h>
class PerformanceIteratorTop:public Iterator {
public:
	struct State{
		State(Schema* schema,Iterator* child, unsigned report_cycles)
		:schema(schema),child(child),report_cycles(report_cycles){};
		Schema* schema;
		Iterator* child;
		unsigned report_cycles;//in milliseconds.

	};
	PerformanceIteratorTop(PerformanceIteratorTop::State state);
	virtual ~PerformanceIteratorTop();
	bool open();
	inline bool next(void*){

		if(state.child->next(tuple)){
			tuplecount++;
			return true;
		}
		return false;
	}
	bool close();
private:
	static void* report(void* arg);
private:
	State state;
	void* tuple;
	unsigned long int tuplecount;
	unsigned long long int start;
	pthread_t report_tid;
};

#endif /* PERFORMANCEITERATORTOP_H_ */
