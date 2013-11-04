/*
 * BlockWritableFix.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: wangli
 */

#include "BlockWritableFix.h"
#include "../rename.h"
#include <iostream>
using namespace std;

BlockWritableFix::BlockWritableFix(unsigned BlockSize, Schema* schema)
:BlockWritable(BlockSize),schema(schema),free((char*)start),tuplecount(0)
{
	// TODO Auto-generated constructor stub

}

BlockWritableFix::~BlockWritableFix() {
}




bool BlockWritableFix::hasSpace()
{
	return (char*)free+schema->getTupleMaxSize()<=(char*)start+BlockSize-2*sizeof(tuplecount);
}
void BlockWritableFix::foldLastBlock(){
	*(unsigned*)((char*)start+BlockSize-2*sizeof(tuplecount))=tuplecount;
	*(unsigned*)((char*)start+BlockSize-sizeof(tuplecount))=0;
}
bool BlockWritableFix::isLastBlock(){
	return *(unsigned*)((char*)start+BlockSize-sizeof(tuplecount))==0;
}
void BlockWritableFix::fold()
{
//	cout<<"I am in the fold func"<<endl;
//	if(s==1){
////		cout<<"hi, ?"<<endl;
		*(unsigned*)((char*)start+BlockSize-2*sizeof(tuplecount))=tuplecount;
//		cout<<"cao"<<endl;
		*(unsigned*)((char*)start+BlockSize-sizeof(tuplecount))=1;
//	}
//	else{
//////		cout<<"hi"<<endl;
//		*(unsigned*)((char*)start+BlockSize-2*sizeof(tuplecount))=tuplecount;
//		*(unsigned*)((char*)start+BlockSize-sizeof(tuplecount))=0;
//	}
}

void BlockWritableFix::reset()
{
	free=(char*)start;
	tuplecount=0;
}

unsigned BlockWritableFix::getTupleCount()
{
	return tuplecount;
}
void BlockWritableFix::setEmpty(){
	reset();
	*(unsigned*)((char*)start+BlockSize-2*sizeof(tuplecount))=0;
}
