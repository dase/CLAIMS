/*
 * Comparator.h
 *
 *  Created on: Jun 13, 2013
 *      Author: wangli
 */

#ifndef COMPARATOR_H_
#define COMPARATOR_H_
#ifdef DMALLOC
#include "dmalloc.h"
#endif
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <map>

#include "data_type.h"
#include "../utility/lock.h"

typedef bool (*comFun)(const void*,const void*);



class Comparator {
public:
	enum comparison{L,LEQ,EQ,NEQ,G,GEQ};
	struct Pair
	{
		column_type first;
		column_type second;
	public:
		Pair(column_type first,column_type second):first(first),second(second){};
		Pair(){};
		bool operator==(const struct Comparator::Pair &p) const
		{
			return p.first==first&&p.second==second;
		}
		bool operator<(const struct Comparator::Pair &p) const
		{

//			return this->first<p.first||this->second<p.second; //This is a bug.
			return ((int)this->first.type*DATA_TYPE_NUMBER+(int)this->second.type)<((int)p.first.type*DATA_TYPE_NUMBER+(int)p.second.type);
		}
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & first & second;
		}
	};
	Comparator(column_type x, column_type b, Comparator::comparison c);
	Comparator():compare(0){};
	Comparator(const Comparator & r);
	virtual ~Comparator();
	bool (*compare)(const void*,const void*) ;
	comparison getCompareType()const{
		return compareType;
	}
protected:
	Pair pair;
	comparison compareType;
private:
	static std::map<Pair,comFun> funs_L;
	static void initialize_L();
	static std::map<Pair,comFun> funs_GEQ;
	static void initialize_GEQ();
	static std::map<Pair,comFun> funs_EQ;
	static void initialize_EQ();
	static std::map<Pair,comFun> funs_NEQ;
	static void initialize_NEQ();
	static std::map<Pair,comFun> funs_G;
	static void initialize_G();
	static std::map<Pair,comFun> funs_LEQ;
	static void initialize_LEQ();
	void iniatilize();
	static Lock lock_;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & pair & compareType;
		if(compare==0)
		{
			iniatilize();
		}
	}
};

#endif /* COMPARATOR_H_ */
