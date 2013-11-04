#include <iostream>
#include <list>
#include <pair>
using namespace std;

#include "../Scheduler.h"
#include "../../Catalog/table.h"

int mainasdfasdf(){

		/*
		                0
		              /   \
		             1     2
		            / \   / \
		           3   4 5   6
		                                       */
	std::vector<column_type> columns;
	columns.push_back(column_type(t_int));
	columns.push_back(column_type(t_int));
	Schema *s=new SchemaFix(columns);

	//scan 3
	pair<string,string> pair1;
	pair1=make_pair("10.11.1.201","/home/data");
	vector<pair<string,string> > vp1;
	vp1.push_back(pair1);
	OperateNode *on3=new ScanNode(0,0,s,vp1);

	//scan 4
	pair<string,string> pair2;
	pair2=make_pair("10.11.1.202","/home/data");
	vector<pair<string,string> > vp2;
	vp2.push_back(pair2);
	OperateNode *on4=new ScanNode(0,0,s,vp2);

	//scan 5
	pair<string,string> pair3;
	pair3=make_pair("10.11.1.203","/home/data");
	vector<pair<string,string> > vp3;
	vp3.push_back(pair3);
	OperateNode *on5=new ScanNode(0,0,s,vp3);

	//scan 6
	pair<string,string> pair4;
	pair4=make_pair("10.11.1.204","/home/data");
	vector<pair<string,string> > vp4;
	vp4.push_back(pair4);
	OperateNode *on6=new ScanNode(0,0,s,vp4);

	//join 1
	vector<unsigned> left_index_1;
	vector<unsigned> right_index_1;
	left_index_1.push_back(1);
	right_index_1.push_back(2);
	vector<unsigned> left_payload_1;
	vector<unsigned> right_payload_1;
	left_payload_1.push_back(2);
	right_payload_1.push_back(1);
	OperateNode *on1=new BroadcastJoinNode(on3,on4,left_index_1,right_index_1,left_payload_1,right_payload_1);

	//join 2
	vector<unsigned> left_index_2;
	vector<unsigned> right_index_2;
	left_index_2.push_back(1);
	right_index_2.push_back(2);
	vector<unsigned> left_payload_2;
	vector<unsigned> right_payload_2;
	left_payload_2.push_back(2);
	right_payload_2.push_back(1);
	OperateNode *on2=new BroadcastJoinNode(on5,on6,left_index_2,right_index_2,left_payload_2,right_payload_2);

	//join 0
	vector<unsigned> left_index_0;
	vector<unsigned> right_index_0;
	left_index_0.push_back(1);
	right_index_0.push_back(2);
	vector<unsigned> left_payload_0;
	vector<unsigned> right_payload_0;
	left_payload_0.push_back(2);
	right_payload_0.push_back(1);
	OperateNode *on0=new BroadcastJoinNode(on1,on2,left_index_0,right_index_0,left_payload_0,right_payload_0);

//	Catalog *cl=new Catalog();
//	cout<<"in the main"<<endl;
//	cl->init();
	cout<<"in the main"<<endl;
	Scheduler *sd=new FIFOScheduler();
	cout<<"in the main"<<endl;
	sd->init();
	cout<<"in the main"<<endl;
	sd->RewriteTree(on0);
	cout<<"in the main"<<endl;
	cout<<endl<<endl;
	sd->ConstructParent(on0);
	sd->displayTree(on0);
	cout<<"----------------"<<endl<<endl<<endl;
	sd->GenerateTree(on0);
	return 0;
}
