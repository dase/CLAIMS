//#include <iostream>
//#include <map>
//using namespace std;
//
//#include "../Scheduler.h"
//
//int main(){
//
//	//--------------------------------------
//	cout<<"catalog!"<<endl;
//	map<string, string> table_path_map;
//	table_path_map["A"]="10.11.1.201:/home/casa/a.txt";
//	table_path_map["B"]="10.11.1.202:/home/casa/b.txt";
//	table_path_map["C"]="10.11.1.203:/home/casa/b.txt";
//	table_path_map["D"]="10.11.1.204:/home/casa/b.txt";
//
//	QueryNode *qn_1=new QueryNode("scan",0,0);
//
//	QueryNode *qn_2=new QueryNode("scan",0,0);
//	QueryNode *qn_3=new QueryNode("filter",qn_2,0);
//
//	QueryNode *qn_0=new QueryNode("hashjoin_broadcast",qn_1,qn_3);
//
//	list<QueryDag*> lqn;
//
//	/*
//	                0
//	              /   \
//	             1     2
//	            / \   / \
//	           3   4 5   6
//	          /
//	         7                             */
//	QueryNode *n7=new QueryNode("scan",0,0);
//	//TODO:	virtual QueryNode
////	hashmap<string, QueryNode>
//	QueryNode *n4=new QueryNode("scan",0,0);
//	QueryNode *n5=new QueryNode("scan",0,0);
//	QueryNode *n6=new QueryNode("scan",0,0);
//	QueryNode *n3=new QueryNode("filter",n7,0);
//	QueryNode *n1=new QueryNode("hashjoin_broadcast",n3,n4);
//	QueryNode *n2=new QueryNode("hashjoin_broadcast",n5,n6);
//	QueryNode *n0=new QueryNode("hashjoin_broadcast",n1,n2);
//	//----------------------------------------------
//	QueryDag *qd=new QueryDag(n0,table_path_map);
//	qd->preorder_visit(n0);
//	cout<<"++++++++++++++++++++++++"<<endl;
//	qd->postorder_visit(n0,table_path_map);
//	cout<<"++++++++++++++++++++++++"<<endl;
//	cout<<"---------------------------------"<<endl;
//	qd->display(n0);
//	cout<<"---------------------------------"<<endl;
//
//	//input the table_path_map
//	Scheduler *s=new FIFOScheduler(lqn);
//	s->PushtoQueryDagList(qd);
//	QueryDag *qd_todo=s->PullfromQueryDagList();
//	s->LocateTree(qd_todo);
//
//
//
//	return 0;
//}
