/*
 * Hdfsconnector.cpp
 *
 *  Created on: Dec 23, 2013
 *      Author: root
 */

#include "Hdfsconnector.h"

HdfsConnector::HdfsConnector(vector<vector <string> > hdfs_writepath):writepath(hdfs_writepath) {
	hdfsFS hdfsfs = hdfsConnect("10.11.1.201", 9000);
	fs = hdfsfs;

}

bool HdfsConnector::assgin_open_file(){

	vector<vector<string> >::iterator prj_writepath;
	vector<string>::iterator par_writepath;

	for (prj_writepath = writepath.begin(); prj_writepath != writepath.end(); prj_writepath++)
	{
		vector<hdfsFile> prj_writefile;
		prj_writefile.clear();
		for (par_writepath = (*prj_writepath).begin(); par_writepath != (*prj_writepath).end(); par_writepath++)
		{
			prj_writefile.push_back(hdfsOpenFile(fs, (*par_writepath).c_str(), O_WRONLY|O_CREAT, 0, 0, 0));
		}
		writefile.push_back(prj_writefile);
	}
	return true;
}

bool HdfsConnector::op_connect(){
	if(!fs)
	{
		fprintf(stderr,"Failed to connect to hdfs.\n");
		exit(-1);
	}
	assgin_open_file();
	if(writefile.size()==0)
	{
		cout << "Failed to connect HDFS!" << endl;
		exit(-1);
 	}else
 	{
 		cout << "HDFS connect successfully." << "\t";
 		cout << "writefile.size(): " << writefile.size() << endl;
 	}

	return true;
}

bool HdfsConnector::op_disconnect(){
	int result = -1;
	for(int i = 0; i < writefile.size(); i++)
	{
		for (int j = 0; j < writefile[i].size(); j++)
		{
			result = hdfsCloseFile(fs, writefile[i][j]);
		}
		writefile[i].clear();
	}
	writefile.clear();
	if(result == 0)
		return true;
	return false;
}
hdfsFS HdfsConnector::get_fs(){
	return fs;
}
vector<vector<hdfsFile> > HdfsConnector::get_writefile(){
	return writefile;
}


HdfsConnector::~HdfsConnector() {

}

