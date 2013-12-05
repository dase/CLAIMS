/*
 * A.h
 *
 *  Created on: May 11, 2013
 *      Author: wangli
 */

#ifndef A_H_
#define A_H_
#include <stdio.h>
#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>


#include <fstream>
#include <boost/serialization/serialization.hpp>
struct A
{
public:
	A(){;}
        A(int a)
        {
        	for(int i=0;i<a;i++)
        	{
        		n.push_back(i);
        	}
        }
        void p()
        {
        	printf("asdfasf!\n");

            printf("%d\n",n.size());
        }
public:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize (Archive & at, const unsigned int version)
        {

        	at & n;
        }
        std::vector<int> n;
};
//BOOST_CLASS_IMPLEMENTATION(A,boost:serialization::object_serializable)




#endif /* A_H_ */
