/*
 * SingleColumnScanBlockIterator.h
 *
 *  Created on: Aug 23, 2013
 *      Author: wangli
 */

#ifndef SINGLECOLUMNSCANBLOCKITERATORFIX_H_
#define SINGLECOLUMNSCANBLOCKITERATORFIX_H_
#include <string>

#include "../Block/Block.h"
#include "../Block/BlockFix.h"
#include "../Block/BlockWritableFix.h"
#include "../Schema/SchemaFix.h"
#include "../BlockIterator/BlockIteratorFix.h"

class SingleColumnScanBlockIteratorFix:public BlockIteratorFix {
public:
	class State{
		friend class SingleColumnScanBlockIteratorFix;
		SchemaFix* schema;
		std::string file_name;
	public:
		State(std::string file_name,SchemaFix* schema)
		:file_name(file_name),schema(schema){}
	};
	SingleColumnScanBlockIteratorFix(State state);
	virtual ~SingleColumnScanBlockIteratorFix();
	bool open();
	inline bool next(BlockFix &blockfix){
//		sleep(1);
		const unsigned empty_tuples_in_block=blockfix.getFreeTuples();

		const unsigned copy_bytes=empty_tuples_in_block*tuple_length;
		if(cursor+copy_bytes<=(char*)data+length){
			void* addr=blockfix.allocateTuples(empty_tuples_in_block);
//			memcpy(addr,cursor,copy_bytes);
			blockfix.setBlock(cursor);
			cursor+=copy_bytes;

//			printf("Block %d, copy %d bytes\n",block_count++,copy_bytes);
			return true;
		}
//		printf("Copy last!\n");
		const unsigned remaining_tuples=((char*)data+length-cursor)/tuple_length;
		void* addr=blockfix.allocateTuples(remaining_tuples);
//		memcpy(addr,cursor,remaining_tuples*tuple_length);
		blockfix.setBlock(cursor);
		cursor+=remaining_tuples*tuple_length;

		return false;
//		void* tuple;
//		while(cursor<(char*)data+length){
//			if((tuple=blockfix.allocateTuple())>0){
//				state.schema->columns[0].operate->assign(cursor,tuple);
////				state.schema->copyTuple(cursor,tuple);
//				cursor+=tuple_length;
//			}
//			else{
//				blockfix.fold();
//				return true;
//			}
//			if(blockfix.insert(cursor)){
//				cursor+=tuple_length;
//			}
//			else{
//				blockfix.fold();
//				return true;
//			}
//		}
//		blockfix.foldLastBlock();
//		return false;
	}
	bool close();
private:
	State state;
	void* base;
	void* data;
	char* cursor;
	unsigned length;
	int fd;
	int tuple_length;
	//debug
	int block_count;
};

#endif /* SINGLECOLUMNSCANBLOCKITERATOR_H_ */
