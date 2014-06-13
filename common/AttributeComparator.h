/*
 * AttributeComparator.h
 *
 *  Created on: May 24, 2014
 *      Author: wangli
 */

#ifndef ATTRIBUTECOMPARATOR_H_
#define ATTRIBUTECOMPARATOR_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include "../common/Comparator.h"
#include <malloc.h>
#include "../configure.h"


class AttributeComparator:public Comparator
	{
		friend class FilterIterator;
	public:
		AttributeComparator(column_type x,column_type y,Comparator::comparison c,unsigned index, void* value);
		AttributeComparator(column_type x,Comparator::comparison c,unsigned index, void* value);
		AttributeComparator(const  AttributeComparator & c_a);
		AttributeComparator():value_(0),Comparator(),index(0){};
		~AttributeComparator();
		inline bool filter(void* x) const __attribute__((always_inline))
		{
		   return compare(x,(void*)value_);
		}
		inline unsigned get_index()const{
			return index;
		}
		void* get_value()const{
			return value_;
		}
	protected:
		void* value_;
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
					valuebytes.push_back(*((char*)value_+i));
				}
			}

			ar & boost::serialization::base_object<Comparator>(*this) & index & valuebytes;

			if(value_==0)
			{
				value_=memalign(cacheline_size,pair.second.get_length());
				for(unsigned i=0;i<pair.second.get_length();i++)
				{
					*((char*)value_+i)=valuebytes[i];

				}
			}
		}
	};

#endif /* ATTRIBUTECOMPARATOR_H_ */
