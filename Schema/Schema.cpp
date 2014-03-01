/*
 * Schema.cpp
 *
 *  Created on: Jun 7, 2013
 *      Author: wangli
 */
#include <iostream>
#include "Schema.h"
using namespace std;
Schema::Schema(std::vector<column_type> columns):columns(columns) {
	// TODO Auto-generated constructor stub

}
Schema::Schema(const Schema& r){
	this->columns=r.columns;
}
Schema::~Schema() {
	// TODO Auto-generated destructor stub
}

unsigned Schema::getncolumns()
{
	return columns.size();
}

void Schema::displayTuple(const void* tuple_start_address,const char* spliter)const{
	for(unsigned i=0;i<columns.size();i++){
		cout<<columns[i].operate->toString(getColumnAddess(i,tuple_start_address))<<spliter;
	}
	cout<<endl;
//	cout<<"____________________________________"<<endl;
//	cout<<endl;
//	sleep(1);
}
