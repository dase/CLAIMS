/*
 * test_iterator_serialization.cpp
 *
 *  Created on: Jun 21, 2013
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

#include "../Schema/Schema.h"
#include "../Schema/SchemaFix.h"
#include "../Serialization/Serialization_column_type.h"
#include "../iterator.hpp"
#include "../iterator/SingleColumnScanIterator.h"
#include "../iterator/ExchangeIteratorWithWideDependency.h"
#include "../iterator/PrintIterator.h"
#include "../Serialization/RegisterDerivedClass.h"
#include "../Message.h"
int maincdcdajij(int argc,char* argv[])
{





	///////////////////////////////////////////Scan////////////////////////////////////////|
	std::vector<column_type> column_list;
	column_list.push_back(column_type(t_int));

	Schema* input=new SchemaFix(column_list);
	Schema* output=new SchemaFix(column_list);
	SingleColumnScanIterator::State SCstate1("/home/imdb/temp/1_0.column",input, output);
	Iterator* scs1=new SingleColumnScanIterator(SCstate1);


	///////////////////////////////////////////Exchange////////////////////////////////////|
	printf("argc: %d\n", argc);
	int block_size;

	if(argc==2)
	{
		block_size=atoi(argv[1]);

	}
	else
	{
		block_size=64;
	}
	std::vector<std::string> uppernode;
//	std::vector<std::pair<std::string,std::string> > lowernode;

	std::string p1("10.11.1.204");
	std::string p2("10.11.1.205");
//	pair<std::string,std::string> p3("10.11.1.206","4242");
//	pair<std::string,std::string> p4("10.11.1.207","4242");
//	pair<std::string,std::string> p5("10.11.1.208","4242");

	uppernode.push_back(p1);
	uppernode.push_back(p2);

//	lowernode.push_back(p3);
//	lowernode.push_back(p4);
//	lowernode.push_back(p5);

//	ExchangeIteratorWithWideDependency::State EIstate(input, scs1,block_size,lowernode,uppernode);
	ExchangeIteratorLowerWithWideDependency::State EIstate(input, scs1,uppernode,block_size);
	Iterator* ei=new ExchangeIteratorLowerWithWideDependency(EIstate);





	///////////////////////////////////////Print///////////////////////////////////////////|


	PrintIterator::State PIstate(input,ei);
	Iterator *pi=new PrintIterator(PIstate);
	//-------------------------------------------------------------------------------------|

	std::ostringstream ostr;
	boost::archive::text_oarchive oa(ostr);
//	//	oa.register_type(static_cast<SingleColumnScanIterator *>(NULL));
//		Register_Schemas<boost::archive::text_oarchive>(oa);
//		Register_Iterators(oa);
//		oa<<pi;
//		std::cout<<"Serialization Result:"<<ostr.str()<<std::endl;
	IteratorMessage IM(pi);
	oa<<IM;
	cout<<"serialized: "<<ostr.str()<<endl;
	return 1;
}
