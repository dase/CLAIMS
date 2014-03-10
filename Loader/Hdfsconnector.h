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

using namespace std;

class HdfsConnector {
public:
	HdfsConnector(vector<vector<string> > hdfs_writepath);

	bool assgin_open_file();
	bool op_connect();
	bool op_disconnect();

	hdfsFS get_fs();
	vector<vector<hdfsFile> > get_writefile();

	virtual ~HdfsConnector();

private:
	hdfsFS fs;
	vector<vector<hdfsFile> > writefile;
	vector<vector<string> > writepath;
};

#endif /* HDFSCONNECTOR_H_ */
