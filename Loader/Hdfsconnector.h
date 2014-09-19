/*
 * Hdfsconnector.h
 *
 *  Created on: Dec 23, 2013
 *      Author: root
 */

#ifndef HDFSCONNECTOR_H_
#define HDFSCONNECTOR_H_

#include<iostream>
#include<vector>
#include<string>
#include <hdfs.h>
#include "Connector.h"

using namespace std;



class HdfsConnector:public Connector {
public:
	HdfsConnector(vector<vector<string> > hdfs_writepath);
	virtual ~HdfsConnector();

	bool openFiles(open_flag open_flag_);
	bool closeFiles();

	int flush(unsigned projection_offset, unsigned partition_offset, void* source, unsigned length);
private:
	bool assgin_open_file(open_flag open_flag_);

private:
	hdfsFS fs;
	vector<vector<hdfsFile> > file_handles_;

};

#endif /* HDFSCONNECTOR_H_ */
