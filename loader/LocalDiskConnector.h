/*
 * LocalDiskConnector.h
 *
 *  Created on: Sep 19, 2014
 *      Author: scdong
 */

#ifndef LOCALDISKCONNECTOR_H_
#define LOCALDISKCONNECTOR_H_

#include "Connector.h"

class LocalDiskConnector :public Connector{
public:
	LocalDiskConnector(vector<vector <string> > hdfs_writepath);
	virtual ~LocalDiskConnector();

	bool OpenFiles(open_flag open_flag_);
	bool CloseFiles();

	int Flush(unsigned projection_offset, unsigned partition_offset, const void* source, unsigned length);
private:
protected:
	std::vector<std::vector<int> > file_handles_;
};

#endif /* LOCALDISKCONNECTOR_H_ */
