/*
 * Connector.h
 *
 *  Created on: Sep 19, 2014
 *      Author: scdong
 */

#ifndef CONNECTOR_H_
#define CONNECTOR_H_
#include <vector>
#include <string>

using std::vector;
using std::string;
enum open_flag { CREATEE, APPENDD };
class Connector {
public:
	Connector(vector<vector<string> > writepath):writepath(writepath){
	};
	virtual ~Connector(){};

	virtual bool openFiles(open_flag open_flag_)=0;
	virtual bool closeFiles()=0;

	virtual int flush(unsigned projection_offset, unsigned partition_offset, void* source, unsigned length)=0;
private:
protected:
	vector<vector<string> > writepath;
};


#endif /* CONNECTOR_H_ */
