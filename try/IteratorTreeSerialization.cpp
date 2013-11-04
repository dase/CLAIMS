/*
 * IteratorTreeSerialization.cpp
 *
 *  Created on: Jun 20, 2013
 *      Author: wangli
 */


#include <cstddef> // NULL
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <boost/archive/tmpdir.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>
enum type{t_int,t_float};
class Base
{
public:
	 int a;
	 type t;
public:
	 Base(int _a)
	 {
		 a=_a;
		 t=t_int;
	 }
	 Base()
	 {
		 a=0;
	 }
	 virtual void print()
	 {
		printf("%d",a);
	 }
};

class Double:public Base
{
	 friend class boost::serialization::access;
	 int b;
	 Base* child;
	 template<class Archive>
	 void serialize(Archive & ar, const unsigned int /* file_version */);
public:
	 Double(){}
	 Double(int _a,int _b, Base* base):Base(_a)
	 {
		child=base;
		b=_b;
	 }
	 void print()
	 {
		 printf("(%d,%d)",a,b);
		 printf("|->");
		 if(child)
			 child->print();
	 }
};

class Triple:public Base
{
	friend class boost::serialization::access;
	int b;
	int c;
	Base *child;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int)
	{
		ar.register_type(static_cast<Double *>(NULL));
		ar & boost::serialization::base_object<Base>(*this) & b & c& child;
//	    if(child!=0)
//	    {
//		    ar & child;
//	    }
	}
public:
	Triple(){};
	Triple(int a, int b, int c,Base* base):Base(a),b(b),c(c),child(base)
	{

	}
	void print()
	{
		printf("(%d,%d,%d)",a,b,c);
		printf("|->");
		if(child)
			child->print();
	}
};
template<class Archive>
void Double::serialize(Archive & ar, const unsigned int /* file_version */)
{
	 ar.register_type(static_cast<Triple *>(NULL));
	 ar & boost::serialization::base_object<Base>(*this) & b & child;
//	 if(child!=0)
//	 {
//		 ar & child;
//	 }

}
namespace boost{
namespace serialization
{
	template<class Archive>
	void serialize(Archive & ar,Base & g, const unsigned int /* file_version */)
	{

		  ar & g.a & g.t;
	}
}
}

int mainasfasfasdfasfasfhws5ehdgsf()
{
	Base* d =new Double(3,4,0);
	Base* t=new Triple(5,6,7,d);

	t->print();

	std::ostringstream ostr;
	boost::archive::text_oarchive oa(ostr);
	oa.register_type(static_cast<Double *>(NULL));
	oa.register_type(static_cast<Triple *>(NULL));
	oa<<t;
	std::cout<<"Serialization Result:"<<ostr.str()<<std::endl;
	std::istringstream istr(ostr.str());
	boost::archive::text_iarchive ia(istr);
	ia.register_type(static_cast<Double *>(NULL));
	ia.register_type(static_cast<Triple *>(NULL));
	Base *n;
	ia>>n;
	n->print();
	return 1;
}


