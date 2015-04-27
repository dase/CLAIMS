/*
 * Schema.cpp
 *
 *  Created on: Jun 7, 2013
 *      Author: wangli
 */
#include <iostream>
#include "Schema.h"
using namespace std;
Schema::Schema(const std::vector<column_type>& columns):columns(columns) {
	// TODO Auto-generated constructor stub

}
Schema::Schema(const Schema& r){
	this->columns=r.columns;
}
Schema::~Schema() {
}

unsigned Schema::getncolumns()const
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

std::string Schema::getColumnValue(const void *tuple_start_address, int i) {
	return columns[i].operate->toString(getColumnAddess(i, tuple_start_address));
}

bool Schema::hasSameSchema(Schema* schema) {
	if(this->getSchemaType()!=schema->getSchemaType())
		return false;
	if(this->getncolumns()!=schema->getncolumns())
		return false;
	for(unsigned i=0;i<columns.size();i++){
		if(!(columns[i]==schema->getcolumn(i)))
			return false;
	}
	return true;
}

