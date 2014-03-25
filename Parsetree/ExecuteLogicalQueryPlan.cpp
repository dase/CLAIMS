/*
 * ExecuteLogicalQueryPlan.cpp
 *
 *  Created on: 2014-3-3
 *      Author: imdb
 */

#ifndef __EXECLOGICALQUERYPLAN__
#define __EXECLOGICALQUERYPLAN__

#include <iostream>
#include "../Environment.h"
#include "../Catalog/stat/Analyzer.h"
#include "../Catalog/ProjectionBinding.h"
#include "../Parsetree/sql_node_struct.h"
#include "../Parsetree/parsetree2logicalplan.cpp"
#include "../Parsetree/runparsetree.cpp"
#include "../Parsetree/ExecuteLogicalQueryPlan.h"
#include "../LogicalQueryPlan/Scan.h"
#include "../LogicalQueryPlan/LogicalQueryPlanRoot.h"
#include "../LogicalQueryPlan/EqualJoin.h"
#include "../LogicalQueryPlan/Filter.h"
#include "../LogicalQueryPlan/Aggregation.h"
#include "../LogicalQueryPlan/Buffer.h"
#include "../utility/rdtsc.h"

using namespace std;

void ExecuteLogicalQueryPlan()	// 2014-3-4---因为根结点的结构已经改变，相关代码进行修改---by余楷
{

	Environment::getInstance(true);

	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();

	int count=1;
	while(count)
	{
		cout<<"SQL start:\n";
		puts("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

		string tablename;
		Node* oldnode=getparsetreeroot();

		if(oldnode == NULL)	// 2014-2-24---增加node为空的判断---by余楷
		{
			printf("there are some wrong!\n");
			FreeAllNode();	//释放SQL解析过程忠所有申请的内存		// 2014-3-6---增加解析错误后的处理---by余楷
			printf("Continue(1) or not (others number)?\n");
			scanf("%d",&count);
			getchar();	// 2014-3-4---屏蔽换行符对后面的影响---by余楷
			//setbuf(stdin, NULL);	//关闭缓冲
			continue;
		}

		Stmt *stmtList = (Stmt *)oldnode;

		while (stmtList != NULL)
		{
			Node *node = (Node *)stmtList->data;

			// 语义分析

			switch(node->type)
			{
			case t_create_table_stmt:	// 创建表的语句
			{
				/* nodetype type, int create_type, int check, char * name1, char * name2, Node * list, Node * select_stmt */

				cout<<"this is create table stmt "<<endl;
				Create_table_stmt * ctnode = (Create_table_stmt *)node;
				//获取新建表的表名
				if(ctnode->name2 != NULL)
				{
					tablename = ctnode->name2;
				}
				else if(ctnode->name1 != NULL)
				{
					tablename = ctnode->name1;
				}
				else
				{
					puts("No table name!");
					exit(0);
				}

				// 2014-3-25---检查表是否已存在---by Yu
				TableDescriptor *new_table = Environment::getInstance()->getCatalog()->getTable(tablename);
				if (new_table != NULL)
				{
					cout<<"[ERROR]: The table "<<tablename<<" has existed!"<<endl;
					break;
				}

				new_table = new TableDescriptor(tablename,Environment::getInstance()->getCatalog()->allocate_unique_table_id());
				Create_col_list *list = (Create_col_list*)ctnode->list;
				string primaryname;
				int colNum = 0;
				while (list)
				{
					Create_def *data = (Create_def*) list->data;
					if (data->deftype == 1)	//若为这个语法：NAME data_type column_atts
					{
						++colNum;	// 2014-2-24---移动位置到if语句内
						string colname = data->name;	// 2014-2-24---移动位置到if语句内
						primaryname = colname;
						Datatype * datatype = (Datatype *)data->datatype;
						switch (datatype->datatype)
						{
						case 5:
						case 6:
						{
							new_table->addAttribute(colname, data_type(t_int), 0, true);
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 7:
						{
							if (datatype->opt_uz & 01 != 0)
							{
								new_table->addAttribute(colname, data_type(t_u_long), 0, true);
								cout<<colname<<" is created"<<endl;
							}
							break;
						}
						case 10:
						{
							new_table->addAttribute(colname, data_type(t_float), 0, true);
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 9:
						{
							new_table->addAttribute(colname, data_type(t_double), 0, true);
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 17:
						case 18:
						{
							if (datatype->length)	//已指定长度
							{
								Length * l = (Length*)datatype->length;
								new_table->addAttribute(colname, data_type(t_string), l->data1, true);
							}
							else	//未指定长度
							{
								new_table->addAttribute(colname, data_type(t_string), 1, true);
							}
							cout<<colname<<" is created"<<endl;
							break;
						}
						}
					}
					list = (Create_col_list* )list->next;
				}
				catalog->add_table(new_table);

				break;
			}

			case t_create_projection_stmt:	// 创建projection的语句
			{
				cout<<"this is create projection "<<endl;
				Create_projection_stmt *newnode = (Create_projection_stmt *)node;
				int partition_num = newnode->partition_num;
				string tablename = newnode->tablename;
				TableID table_id=catalog->getTable(tablename)->get_table_id();
				string partition_attribute_name = newnode->partition_attribute_name;

				std::vector<ColumnOffset> index;
				Columns *col_list = (Columns *)newnode->column_list;
				while(col_list)
				{
					string colname;
					if (col_list->parameter2 != NULL)
					{
						colname = col_list->parameter2;
					}
					else if (col_list->parameter1 != NULL)
					{
						colname = col_list->parameter1;
					}
					else
					{
						cout<<"NO column name!"<<endl;
						exit(0);
					}
					cout<<colname<<endl;
					index.push_back(catalog->getTable(table_id)->getAttribute(colname).index);
					col_list = (Columns *)col_list->next;
				}

				catalog->getTable(table_id)->createHashPartitionedProjection(index,partition_attribute_name,partition_num);

				for(unsigned i=0;i<catalog->getTable(table_id)->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

					catalog->getTable(table_id)->getProjectoin(catalog->getTable(table_id)->getNumberOfProjection()-1)->getPartitioner()->RegisterPartition(i,2);
					//				catalog->getTable(table_id)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
				}
				break;
			}
			case t_query_stmt: // 2014-3-4---修改为t_query_stmt,添加对查询语句的处理---by余楷
			{
				cout<<"this is query stmt"<<endl;

				if (!semantic_analysis(node))
					cout<<"semantic_analysis error"<<endl;
				Query_stmt *querynode=(Query_stmt *)node;
				puts("select_stmt2>>>>>>>>");
				if(querynode->where_list!=NULL)
				{
					struct Where_list * curt=(struct Where_list *)(querynode->where_list);
					struct Node *cur=(struct Node *)(curt->next);
					puts("wc2tb");
					departwc(cur,querynode->from_list);
					puts("partree complete!!!");
				}
				output(node,0);
				puts("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

				LogicalOperator* plan=parsetree2logicalplan(node);//现在由于没有投影，所以只把from_list传输进去。因此在完善之后，需要在parsetree2logicalplan()中
				//进行判断，对于不同的语句，比如select,update等选择不同的操作。

				//const NodeID collector_node_id=0;
				LogicalOperator* root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::PERFORMANCE);
				unsigned long long int timer_start=curtick();

				BlockStreamIteratorBase* please=root->getIteratorTree(64*1024-sizeof(unsigned));
				root->print();


				please->print();

				IteratorExecutorMaster::getInstance()->ExecuteBlockStreamIteratorsOnSite(please,"127.0.0.1");//

				break;
			}
			default:
			{
				cout<<node->type<<endl;
				puts("nothing matched!\n");
			}
			}

			stmtList = (Stmt *)stmtList->next;
			//			sleep(1);	//留出时间，等待内部输出	bug已修复，不再需要
			//			getchar();	//输入任意字符，继续执行 bug已修复，不再需要
		}


//		FreeAllNode();	//---完成对节点的释放 ！！！！！！！！！！！！！！

		printf("SQL Complete! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("Continue(1) or not (0)?\n");
		scanf("%d",&count);
		getchar();	// 2014-3-4---屏蔽换行符对后面的影响---by余楷
	}
}

#endif
