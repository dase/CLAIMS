/*
 * Hdfsconnector.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: root
 */

#include "Hdfsconnector.h"
#include <assert.h>
#include "../Config.h"

HdfsConnector::HdfsConnector(vector<vector <string> > hdfs_writepath):Connector(hdfs_writepath) {
	hdfsFS hdfsfs = hdfsConnect(Config::hdfs_master_ip.c_str(),Config::hdfs_master_port);
	fs = hdfsfs;

}

bool HdfsConnector::assgin_open_file(open_flag open_flag_){

	vector<vector<string> >::iterator prj_writepath;
	vector<string>::iterator par_writepath;

	for (prj_writepath = writepath.begin(); prj_writepath != writepath.end(); prj_writepath++)
	{
		vector<hdfsFile> prj_writefile;
		prj_writefile.clear();
		for (par_writepath = (*prj_writepath).begin(); par_writepath != (*prj_writepath).end(); par_writepath++)
		{
			switch (open_flag_)
			{
			case CREATE:
			{
				if (hdfsExists(fs, (*par_writepath).c_str()) == 0)
					cout << "[WARNINIG: Hdfsconnector.cpp->assgin_open_file()]: The file " << *par_writepath << " is already exits! It will be override!\n";
				prj_writefile.push_back(hdfsOpenFile(fs, (*par_writepath).c_str(), O_WRONLY|O_CREAT, 0, 0, 0));
				break;
			}
			case APPEND:
			{
				if (hdfsExists(fs, (*par_writepath).c_str()) == -1)
				{
					prj_writefile.push_back(hdfsOpenFile(fs, (*par_writepath).c_str(), O_WRONLY|O_CREAT, 0, 0, 0));
					break;
//					cout << "[ERROR: Hdfsconnector.cpp->assgin_open_file()]: The file " << *par_writepath << "is not exits!\n";
//					return false;
				}
				prj_writefile.push_back(hdfsOpenFile(fs, (*par_writepath).c_str(), O_WRONLY|O_APPEND, 0, 0, 0));
				break;
			}
			default:
			{
				cout << "[ERROR: Hdfsconnector.cpp->assgin_open_file()]: Illegal file open flag for data loading!\n";
				return false;
			}
			}
		}
		file_handles_.push_back(prj_writefile);
	}
	return true;
}

bool HdfsConnector::openFiles(open_flag open_flag_){
	if(!fs)
	{
		fprintf(stderr,"[ERROR: Hdfsconnector.cpp->op_connect()]: Failed to connect to hdfs.\n");
		exit(-1);
	}
	assgin_open_file(open_flag_);
	if(file_handles_.size()==0)
	{
		cout << "[ERROR: Hdfsconnector.cpp->op_connect()]: the writefile is empty!" << endl;
		exit(-1);
 	}else
 	{
 		cout << "HDFS connect successfully." << "\t";
 	}

	return true;
}

int HdfsConnector::flush(unsigned projection_offset, unsigned partition_offset, void* source, unsigned length)
{
	return hdfsWrite(fs, file_handles_[projection_offset][partition_offset], source, length);
}

bool HdfsConnector::closeFiles(){
	int result = -1;
	for(int i = 0; i < file_handles_.size(); i++)
	{
		for (int j = 0; j < file_handles_[i].size(); j++)
		{
			result = hdfsCloseFile(fs, file_handles_[i][j]);
		}
		file_handles_[i].clear();
	}
	file_handles_.clear();
	if(result == 0)
		return true;
	return false;
}


HdfsConnector::~HdfsConnector() {

}

