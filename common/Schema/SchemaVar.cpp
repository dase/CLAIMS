/*
 * SchemaVar.cpp
 *
 *  Created on: 2014-2-17
 *      Author: casa
 */

#include "SchemaVar.h"

SchemaVar::SchemaVar() {
	// TODO 自动生成的构造函数存根

}

SchemaVar::~SchemaVar() {
	// TODO 自动生成的析构函数存根
}

SchemaVar::SchemaVar(std::vector<column_type> columns):Schema(columns) {
	attributes_=columns.size();
	for(unsigned i=0;i<columns.size();i++){
		if(columns[i].type==t_string)
			var_attributes_++;
	}
}

unsigned SchemaVar::getTupleActualSize(void* tuple) const{
	unsigned ofs=0;
	unsigned column_off=0;
	for(unsigned i=0;i<columns.size();i++){
		if(columns[i].type!=t_string){
			ofs+=columns[i].get_length();
		}
		else{
			ofs+=*(int *)(tuple+column_off*4);
			column_off++;
		}
	}
	ofs+=var_attributes_*4;
	return ofs;
};

unsigned SchemaVar::copyTuple(void* src, void* desc) const{
	memcpy(desc,src,getTupleActualSize(src));
};

Schema::schema_type SchemaVar::getSchemaType()const{
	return Schema::varaible;
};
Schema* SchemaVar::duplicateSchema()const{
	return new SchemaVar(columns);
}
