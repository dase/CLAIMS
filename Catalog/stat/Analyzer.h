/*
 * Analyzer.h
 *
 *  Created on: 2014年1月20日
 *      Author: volt
 */

#ifndef ANALYZER_H_
#define ANALYZER_H_

#include "../../ids.h"
#include "../../Catalog/Attribute.h"
#include "Statistic.h"
class Analyzer {
public:
	Analyzer();

	static void analyse(const AttributeID &attrID);
	static void analyses( TableID tableID);
//	static void print();

	virtual ~Analyzer();
private:
	static void mcvAnalyse(void **list, const unsigned long size, const Attribute &attr, Histogram *stat);
	static void equiDepthAnalyse(void **list, const unsigned long size, const Attribute &attr, Histogram *stat);
};

#endif /* ANALYZER_H_ */
