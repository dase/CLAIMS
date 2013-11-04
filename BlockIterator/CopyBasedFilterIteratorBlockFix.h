/*
 * FilterIteratorBlockFix.h
 *
 * This class is the implementation of copy-based block iterator.
 * The word 'copy' means that the tuples that pass the filter are
 * copied to a buffer, namely BlockFix.
 * In contrast, a potential more efficient way for the filter is
 * bitmap-based filter, where a bitmap is added as a member
 * variable of the block which indicates whether a tuple passes
 * the filter. This implementation is expected to be better than
 * the copy-based filter as the data copy is saved.
 *
 *  Created on: Aug 23, 2013
 *      Author: wangli
 */

#ifndef COPYBASEDFILTERITERATORBLOCKFIX_H_
#define COPYBASEDFILTERITERATORBLOCKFIX_H_
#include <malloc.h>
#include "../configure.h"
#include "BlockIteratorFix.h"
#include "../Schema/SchemaFix.h"
#include "../Comparator.h"
#include "../Block/BlockFix.h"
#include "../iterator/FilterIterator.h"
class CopyBasedFilterIteratorBlockFix:public BlockIteratorFix {
public:

	class State{
	public:
		SchemaFix* schema;
		BlockIteratorFix* child;
		std::vector<FilterIterator::AttributeComparator> ComparatorList;
		State(SchemaFix* schema, BlockIteratorFix* child, std::vector<FilterIterator::AttributeComparator> ComparatorList)
		:schema(schema),child(child),ComparatorList(ComparatorList){};

	};
	CopyBasedFilterIteratorBlockFix(State State);
	virtual ~CopyBasedFilterIteratorBlockFix();
	bool open();
	inline bool next(BlockFix & block){

//		if(state.child->next(*block_for_asking)){
//			block_for_asking->cur_reset();
//			void* tuple;
//			while((tuple=block_for_asking->next())>0){
////				printf("Tuple=%d",*(int*)tuple);
//				tuples++;
//			}
////			for(unsigned i=0;i<block_for_asking->getTupleCount();i++){
////				tuples++;
////			}
//			block_for_asking->setEmpty();
//			return true;
//		}
//		return false;



		void* tuple;
		bool pass_filter;
		while((tuple=block_for_asking->cur_get())>0){
//			printf("new tuple comes...\n");
			pass_filter=true;
			for(unsigned i=0;i<state.ComparatorList.size();i++){
				if(!state.ComparatorList[i].filter(tuple)){
					pass_filter=false;
//					printf("filtered...\n");
					break;
				}
			}
//			pass_filter=*(int*)tuple<100;
			if(pass_filter){
//				printf("passes...\n");
//				void* tuple_to_insert;
//				if((tuple_to_insert=block.allocateTuple())>0){
//					state.schema->columns[0].operate->assign(tuple,tuple_to_insert);
//					block_for_asking->next();
//				}
//
				if(block.insert(tuple)){
//					printf("new tuple inserted!\n");
					block_for_asking->next();
				}
				else{
//					printf("The block is full!\n");
					return true;
				}
			}
			else{
				block_for_asking->next();
			}

		}
//		printf("block_for_asking is exhausted!\n");
		if(received_last_block){
			return false;
		}
		block_for_asking->setEmpty();
//		printf("asking the child for the new block...!\n");
		received_last_block=!state.child->next(*block_for_asking);
		block_for_asking->cur_reset();
		if(received_last_block){
//			printf("Last block received!\n");
		}
		return next(block);

//
//		return true;
	}

	/* bitmap-based filter*/
//	inline bool next(BlockFix &blockfix){
//		void* tuple;
//		bool pass_filter;
//		blockfix.setEmpty();
//		unsigned filtered;
//		unsigned passed;
//		if(state.child->next(blockfix)){
//			blockfix.cur_reset();
//			while(tuple=blockfix.next()){
//			//			for(unsigned i=0;i<blockfix.getTupleCount();i++){
//				pass_filter=true;
//				for(unsigned j=0;j<state.ComparatorList.size();j++){
//					if(!state.ComparatorList[j].filter(tuple)){
//						pass_filter=false;
//
//					}
//					if(pass_filter){
//						filtered++;
//					}
//					else{
//						passed++;
//					}
//				}
//
//			}
//			return true;
//		}
//		return false;
//	}
	bool close();
	private:
	State state;
	BlockFix* block_for_asking;
	bool received_last_block;
	//debug
	long tuples;

};

#endif /* FILTERITERATORBLOCKFIX_H_ */
