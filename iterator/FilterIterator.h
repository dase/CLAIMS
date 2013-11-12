/*
 * FilterIterator.h
 *
 *  Created on: Jun 9, 2013
 *      Author: wangli
 */

#ifndef FILTERITERATOR_H_
#define FILTERITERATOR_H_

#include <vector>
#include <malloc.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include "../iterator.hpp"
#include "../Schema/Schema.h"
#include "../Comparator.h"
#include "../configure.h"

class FilterIterator:public Iterator {
public:
	class AttributeComparator:public Comparator
	{
		friend class FilterIterator;
	public:
		AttributeComparator(column_type x,column_type y,Comparator::comparison c,unsigned index, void* value);
		AttributeComparator(column_type x,Comparator::comparison c,unsigned index, void* value);
		AttributeComparator(const  AttributeComparator & c_a);
		AttributeComparator():value(0){};
		~AttributeComparator();
		inline bool filter(void* x) const __attribute__((always_inline))
		{
		   return compare(x,(void*)value);
		}
		inline unsigned get_index()const{
			return index;
		}
		void* get_value()const{
			return value;
		}
	protected:
		void* value;
		unsigned index;

	private:
		/**
		 * a kind of ugly here. Because of the length of the value is unknown,
		 * we first store the value into the vector and then serialize the vector.
		 * TODO:
		 */
		std::vector<char> valuebytes;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
		//	Register_Iterators_Except_For_PrintIterator(ar);
			if(valuebytes.empty()&&compare!=0)
			{
				for(unsigned i=0;i<pair.second.get_length();i++)
				{
					valuebytes.push_back(*((char*)value+i));
				}
			}

			ar & boost::serialization::base_object<Comparator>(*this) & index & valuebytes;

			if(value==0)
			{
				value=memalign(cacheline_size,pair.second.get_length());
				for(unsigned i=0;i<pair.second.get_length();i++)
				{
					*((char*)value+i)=valuebytes[i];

				}
			}
		}
	};


public:
	struct State
	{
	public:
		Schema* input;
		Schema* output;
		Iterator* child;
		std::vector<AttributeComparator> ComparatorList;
		State(){};
		State(Schema* input,Schema* output,Iterator* child,std::vector<AttributeComparator> ComparatorList )
		:input(input),output(output),child(child),ComparatorList(ComparatorList){};
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & input & output & child & ComparatorList;
		}
	};
public:
	FilterIterator(FilterIterator::State state);
	FilterIterator(){};
	virtual ~FilterIterator();
	bool open();
	inline bool next(void* desc) __attribute__((always_inline))
	{
//		return state.child->next(desc);

		while(state.child->next(desc))
		{
			bool satisfied=true;
			const unsigned comparator_count=state.ComparatorList.size();
			for(unsigned i=0;i<comparator_count;i++)
			{
//				if(!(*(int*)desc<100))
				if(!state.ComparatorList[i].filter(state.input->getColumnAddess(state.ComparatorList[i].index,desc)))
//				if(!state.ComparatorList[i].filter(desc))
//				if(!compare(desc,value))
//				if(!(*(int*)desc<*(int*)value))
				{
					satisfied=false;
					break;
				}
			}
			if(satisfied)
			{
				return true;
			}


		}
		return false;
	};
	bool close();
public:
	FilterIterator::State state;//should be serialized.
	bool (*compare)(const void*,const void*);
	const void* value;

	void try_it();
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Iterator>(*this) & state;
	}

};

#endif /* FILTERITERATOR_H_ */
