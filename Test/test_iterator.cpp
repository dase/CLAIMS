/*
 * test_iterator.cpp
 *
 *  Created on: Jun 7, 2013
 *      Author: wangli
 */
#include <typeinfo>
#include <malloc.h>
#include <stdlib.h>

#include "../TableManager.h"
#include "../rdtsc.h"
#include "../Schema/SchemaFix.h"
#include "../iterator/SingleColumnScanIterator.h"
#include "../iterator/FilterIterator.h"
#include "../iterator/JoinIterator.h"
#include "../iterator/CombinedIterator.h"
#include "../iterator/PrintIterator.h"
#include "../iterator/AggregationIterator.h"
#include "../Comparator.h"
#include "../Message.h"
struct int_float
{
	int i;
	int f;
};
template<unsigned length>
struct fixrecord
{
	char data[length];
};

bool LE(void* a, void* b)
{
	return *(int*)a<*(int*)b;
}
class com
{
public:



	bool (*compare)(void*,void*);
};

int main2()		//generate data
{



	printf("100: %s, 100:%s, 200: %s",typeid(fixrecord<10>).name(),typeid(fixrecord<100>).name(),typeid(fixrecord<200>).name());

	vector<data_type> dt;
	dt.push_back(t_int);
	dt.push_back(t_int);
	TableManager tm;
	tm.create_table_partition(1,dt);


	printf("\n sizof of TableManager is %ld\n",sizeof(Column));


	unsigned long long t1;
	startTimer(&t1);

	int tupleCount=1000;
	int oid=0;
	int_float tmp;
	while(tupleCount>0)
	{

		tmp.i=rand()%80;
		tmp.f=rand()%80;
//		tm.append(1,&tmp,oid++);
		tm.append(1,&tmp,oid++);
		tupleCount--;
	}
	double time=getMilliSecond(t1);
	printf("Total Time: %lf ms, %f M record/s!\n",time, 8*10000/(float)1024/1024/(time/1000));
	return 1;
}
enum ccc{c1,c2};



int main1ds()
{





	///////////////////////////////////////////Scan////////////////////////////////////////|
	std::vector<column_type> column_list;
	column_list.push_back(column_type(t_int));

	Schema* input=new SchemaFix(column_list);
	Schema* output=new SchemaFix(column_list);
	SingleColumnScanIterator::State SCstate1("/home/temp/1_0.column",input, output);
	Iterator* scs1=new SingleColumnScanIterator(SCstate1);

	SingleColumnScanIterator::State SCstate2("/home/temp/1_1.column",input, output);
	Iterator* scs2=new SingleColumnScanIterator(SCstate2);

	SingleColumnScanIterator::State SCstate3("/home/temp/1_1.column",input, output);
	Iterator* scs3=new SingleColumnScanIterator(SCstate3);
	//---------------------------------------------------------------------------------------|


	///////////////////////////////////////////Combined////////////////////////////////////////|
	std::vector<Schema*> inputs;
	inputs.push_back(output);
	inputs.push_back(output);
	inputs.push_back(output);
	column_list.push_back(column_type(t_int));
	std::vector<column_type> column_list1;
	column_list1.push_back(column_type(t_int));
	column_list1.push_back(column_type(t_int));
	column_list1.push_back(column_type(t_int));
	Schema* outputs=new SchemaFix(column_list1);
	std::vector<Iterator*> children;
	children.push_back(scs1);
	children.push_back(scs2);
	children.push_back(scs3);

	CombinedIterator::State CIstate(inputs,outputs,children);
	Iterator* ci=new CombinedIterator(CIstate);

	//---------------------------------------------------------------------------------------|



	///////////////////////////////////////////Filter////////////////////////////////////////|
	FilterIterator::State FIstate;
	FIstate.child=ci;
	FIstate.input=outputs;
	FIstate.output=outputs;
	int f=0;

	FilterIterator::AttributeComparator filter1(column_type(t_int),Comparator::GEQ,0,&f);
	FIstate.ComparatorList.push_back(filter1);



	int f2=10000;
	FilterIterator::AttributeComparator filter2(column_type(t_int),Comparator::L,1,&f2);
	FIstate.ComparatorList.push_back(filter2);
	Iterator* fi=new FilterIterator(FIstate);

	//----------------------------------------------------------------------------------------|

	///////////////////////////////////////Aggregation////////////////////////////////////|

	std::vector<unsigned> GroIndex;
	GroIndex.push_back(0);
//	GroIndex.push_back(1);
	std::vector<unsigned> AggIndex;
	AggIndex.push_back(1);
	AggIndex.push_back(2);
	std::vector<AggregationIterator::State::aggregation> aggtype;
	aggtype.push_back(AggregationIterator::State::sum);
	aggtype.push_back(AggregationIterator::State::count);
	AggregationIterator::State AIstate(outputs,outputs,fi,GroIndex,AggIndex,aggtype,1024,48);
	Iterator* ai=new AggregationIterator(AIstate);


	//------------------------------------------------------------------------------------|

	///////////////////////////////////////Print///////////////////////////////////////////|


	PrintIterator::State PIstate(outputs,ai);
	Iterator *pi=new PrintIterator(PIstate);
	//-------------------------------------------------------------------------------------|

	pi->open();

	void* tuple=malloc(100);

	while(pi->next(tuple))
	{
//		printf("%d\t%d\n",*(int*)tuple,*(int*)((char*)tuple +4));
	}
	pi->close();

	return 1;
}

int mainndnadsdsdcdcdcs(){

	///////////////////////////////////////////Scan////////////////////////////////////////|
	std::vector<column_type> column_list;
	column_list.push_back(column_type(t_int));

	Schema* input=new SchemaFix(column_list);
	Schema* output=new SchemaFix(column_list);
	SingleColumnScanIterator::State SCstate1("/home/imdb/temp/1_0.column",input, output);
	Iterator* scs1=new SingleColumnScanIterator(SCstate1);

	SingleColumnScanIterator::State SCstate2("/home/imdb/temp/1_1.column",input, output);
	Iterator* scs2=new SingleColumnScanIterator(SCstate2);

	SingleColumnScanIterator::State SCstate3("/home/imdb/temp/1_0.column",input, output);
	Iterator* scs3=new SingleColumnScanIterator(SCstate3);

	SingleColumnScanIterator::State SCstate4("/home/imdb/temp/1_1.column",input, output);
	Iterator* scs4=new SingleColumnScanIterator(SCstate4);
	//---------------------------------------------------------------------------------------|

	///////////////////////////////////////////Combined////////////////////////////////////////|
	std::vector<Schema*> inputsl;
	inputsl.push_back(output);
	inputsl.push_back(output);
	std::vector<column_type> column_listl;
	column_listl.push_back(column_type(t_int));
	column_listl.push_back(column_type(t_int));
	Schema* outputsl=new SchemaFix(column_listl);
	std::vector<Iterator*> childrenl;
	childrenl.push_back(scs1);
	childrenl.push_back(scs2);

	CombinedIterator::State CIstatel(inputsl,outputsl,childrenl);
	Iterator* cil=new CombinedIterator(CIstatel);

		//---------------------------------------------------------------------------------------|

	///////////////////////////////////////////Combined////////////////////////////////////////|
	std::vector<Schema*> inputsr;
	inputsr.push_back(output);
	inputsr.push_back(output);
	std::vector<column_type> column_listr;
	column_listr.push_back(column_type(t_int));
	column_listr.push_back(column_type(t_int));
	Schema* outputsr=new SchemaFix(column_listr);
	std::vector<Iterator*> childrenr;
	childrenr.push_back(scs3);
	childrenr.push_back(scs4);

	CombinedIterator::State CIstater(inputsr,outputsr,childrenr);
	Iterator* cir=new CombinedIterator(CIstater);

		//---------------------------------------------------------------------------------------|


	///////////////////////////////////////////Joined////////////////////////////////////////|
	std::vector<column_type> column_listjoin;
	column_listjoin.push_back(column_type(t_int));
	column_listjoin.push_back(column_type(t_int));
	column_listjoin.push_back(column_type(t_int));
	Schema* outputjoin=new SchemaFix(column_listjoin);

	std::vector<unsigned> joinIndexl;
	joinIndexl.push_back(0);
	std::vector<unsigned> linkIndexl;
	linkIndexl.push_back(1);
	std::vector<unsigned> joinIndexr;
	joinIndexr.push_back(1);
	std::vector<unsigned> linkIndexr;
	linkIndexr.push_back(0);
	JoinIterator::State JOstater(outputsl,outputsr,outputjoin,cil,cir,joinIndexl,joinIndexr,linkIndexl,linkIndexr,1024,1024);
	Iterator* ji=new JoinIterator(JOstater);

	/////////////////////////////////////Print///////////////////////////////////////////|


	PrintIterator::State PIstate(outputjoin,ji);
	Iterator *pi=new PrintIterator(PIstate);
		//-------------------------------------------------------------------------------------|

	cout<<"hello/"<<endl;
	IteratorMessage IM(pi);
	cout<<"hello/"<<endl;
//	IM.run();
//	return 0;
	Message4K M4K=IteratorMessage::serialize4K(IM);
	cout<<"hello/"<<endl;
	IteratorMessage tmp=IteratorMessage::deserialize4K(M4K);
	cout<<"hello/"<<endl;
	tmp.run();
	return 1;

	pi->open();

	void* tuple=malloc(100);

	while(pi->next(tuple))
	{
	//		printf("%d\t%d\n",*(int*)tuple,*(int*)((char*)tuple +4));
	}

	pi->close();

	return 1;

}
