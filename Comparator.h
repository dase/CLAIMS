/*
 * Comparator.h
 *
 *  Created on: Jun 13, 2013
 *      Author: wangli
 */

#ifndef COMPARATOR_H_
#define COMPARATOR_H_
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <map>

#include "data_type.h"


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

			return this->first<p.first||this->second<p.second;
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
	virtual ~Comparator();
	bool (*compare)(const void*,const void*) ;
protected:
	Pair pair;
	comparison compareType;
private:
	static std::map<Pair,comFun> funs_L;
	static void initialize_L();
	static std::map<Pair,comFun> funs_GEQ;
	static void initialize_GEQ();
	void iniatilize();
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
