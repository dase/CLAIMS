/*
 * LocalDiskConnector.cpp
 *
 *  Created on: Sep 19, 2014
 *      Author: scdong
 */

#include "LocalDiskConnector.h"
#include "../common/rename.h"
#include <string.h>
#include <iostream>
using namespace std;
LocalDiskConnector::LocalDiskConnector(vector<vector <string> > hdfs_writepath):Connector(hdfs_writepath) {

}

LocalDiskConnector::~LocalDiskConnector() {
}

bool LocalDiskConnector::openFiles(open_flag open_flag_) {
	vector<vector<string> >::iterator prj_writepath;
		vector<string>::iterator par_writepath;

		for (prj_writepath = writepath.begin(); prj_writepath != writepath.end(); prj_writepath++)
		{
			vector<int> partitions_file_handles;
			for (par_writepath = (*prj_writepath).begin(); par_writepath != (*prj_writepath).end(); par_writepath++)
			{
				switch (open_flag_)
				{
				case CREATEE:
				{
					int fd=FileOpen( (*par_writepath).c_str(),O_RDWR|O_TRUNC|O_CREAT,S_IWUSR|S_IRUSR);
					if(fd==-1){
						printf("Cannot create disk file! Reason: %s",strerror(errno));
					}
					partitions_file_handles.push_back(fd);
					break;
				}
				case APPENDD:
				{
					int fd=FileOpen( (*par_writepath).c_str(),O_RDWR|O_CREAT|O_APPEND,S_IWUSR|S_IRUSR);
					if(fd==-1){
						printf("Cannot create disk file! Reason: %s",strerror(errno));
					}
					partitions_file_handles.push_back(fd);
					break;
				}
				default:
				{
					cout << "[ERROR: Hdfsconnector.cpp->assgin_open_file()]: Illegal file open flag for data loading!\n";
					return false;
				}
				}
			}
			file_handles_.push_back(partitions_file_handles);
		}
		return true;
}

bool LocalDiskConnector::closeFiles() {
	for(int i = 0; i < file_handles_.size(); i++)
	{
		for (int j = 0; j < file_handles_[i].size(); j++)
		{
			FileClose(file_handles_[i][j]);
		}
	}
	return true;
}

int LocalDiskConnector::flush(unsigned projection_offset,
		unsigned partition_offset, void* source, unsigned length) {
	return write(file_handles_[projection_offset][partition_offset], source, length);
}

