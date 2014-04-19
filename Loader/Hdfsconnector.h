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

enum open_flag { CREATE, APPEND };

class HdfsConnector {
public:
	HdfsConnector(vector<vector<string> > hdfs_writepath);
	virtual ~HdfsConnector();

	bool op_connect(open_flag open_flag_);
	bool op_disconnect();

	hdfsFS get_fs();
	vector<vector<hdfsFile> > get_writefile();

private:
	bool assgin_open_file(open_flag open_flag_);

private:
	hdfsFS fs;
	vector<vector<hdfsFile> > writefile;
	vector<vector<string> > writepath;
};

#endif /* HDFSCONNECTOR_H_ */
