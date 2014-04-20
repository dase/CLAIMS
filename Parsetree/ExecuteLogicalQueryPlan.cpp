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

#include "../Loader/Hdfsloader.h"

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

				// 创建新表
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
						Column_atts *column_atts = (Column_atts*)data->col_atts;

						/* TODO: Whether column is unique or not null or has default value is not finished,
						 *  because there are not supports
						 */
						Datatype * datatype = (Datatype *)data->datatype;
						switch (datatype->datatype)	// add more type --- 2014-4-2
						{
						case 3:
						{
							new_table->addAttribute(colname, data_type(t_smallInt), 0, true);
							break;
						}
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
						case 9:
						{
							new_table->addAttribute(colname, data_type(t_double), 0, true);
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 10:
						{
							new_table->addAttribute(colname, data_type(t_float), 0, true);
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 11:
						{
							new_table->addAttribute(colname, data_type(t_decimal), 0, true);
						}
						case 12:	// DATE --- 2014-4-1
						{
							new_table->addAttribute(colname, data_type(t_date), 0, true);
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 13:	// TIME --- 2014-4-1
						{
							new_table->addAttribute(colname, data_type(t_time), 0, true);
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 15:	// DATETIME --- 2014-4-1
						{
							new_table->addAttribute(colname, data_type(t_datetime), 0, true);
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
			}
			break;
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
			}
			break;
			case t_query_stmt: // 2014-3-4---修改为t_query_stmt,添加对查询语句的处理---by余楷
			{
				cout<<"this is query stmt"<<endl;

				if (!semantic_analysis(node))
				{
					cout<<"semantic_analysis error"<<endl;
					break;	// 2014-4-17---add ---by Yu
				}
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
			}
			break;
			case t_load_table_stmt:	//	导入数据的语句
			{
				Loadtable_stmt *new_node = (Loadtable_stmt*)node;

				string table_name(new_node->table_name);
				TableDescriptor *table = Environment::getInstance()->getCatalog()->getTable(table_name);

				// 2014-4-17---check the exist of table---by Yu
				if(table == NULL)
				{
					ASTParserLogging::elog("the table %s does not exist!", table_name);
					break;
				}
				string column_separator(new_node->column_separator);
				string tuple_separator(new_node->tuple_separator);
				printf("wef:%s\n",new_node->tuple_separator);
				Expr_list *path_node = (Expr_list*)new_node->path;

				ASTParserLogging::log("load file\'s name:");
				vector<string> path_names;	// save the name of files which should be loaded
				//for test: the path name is:	/home/imdb/data/tpc-h/part.tbl
				while(path_node)
				{
					Expr *data = (Expr*)path_node->data;
					ASTParserLogging::log("%s",data->data);
					path_names.push_back(string(data->data));
					path_node = (Expr_list*)path_node->next;
				}

				// split sign should be considered carefully, in case of it may be "||" or "###"
				ASTParserLogging::log("The separator are :%c,%c", column_separator[0], tuple_separator[0]);
				HdfsLoader *loader = new HdfsLoader(column_separator[0], tuple_separator[0], path_names, table);

				loader->load();
			}
			break;
			case t_insert_stmt:	// 2014-4-19---add---by Yu
			{
				bool correct = true;
				Insert_stmt *insert_stmt = (Insert_stmt *)node;
				string table_name(insert_stmt->tablename);
				TableDescriptor *table = Environment::getInstance()->getCatalog()->getTable(table_name);
				if(table == NULL)
				{
					ASTParserLogging::elog("the table %s does not exist!", table_name);
					break;
				}

//				vector<string> col_list;
//				Columns *col = (Columns *)insert_stmt->col_list;
//				new HdfsLoader();
//				while(col)
//				{
//					col_list.push_back(col->parameter1);
//					col = (Columns *)col->next;
//				}

				Columns *col = (Columns *)insert_stmt->col_list;
				bool is_all_col = false;
				if(col == NULL)
					is_all_col = true;

				ostringstream ostr;
				Insert_val_list *insert_value_list = (Insert_val_list*)insert_stmt->insert_val_list;
				while(insert_value_list)	// 循环获得 （……），（……），（……）中的每一个（……）
				{
					// init
					Insert_vals *insert_value = (Insert_vals *)insert_value_list->insert_vals;
					col = (Columns *)insert_stmt->col_list;

//					const std::vector<Attribute> attrs = table->getAttributes();
					for(unsigned int position = 0; position < table->getNumberOfAttribute(); position++)
					{
						if(is_all_col || (col && !table->getAttribute(position).attrName.compare(col->parameter1)))	//添加的列与表中的列相匹配 或者 添加的是所有列
						{
							if(insert_value->value_type == 0)	// 指定具体的值
							{
								switch(insert_value->expr->type)	// 2014-4-17---only these type are supported now---by Yu
								{
								case t_stringval: case t_intnum: case t_approxnum: case t_bool:
								{
									Expr *expr = (Expr *)insert_value->expr;
									ostr<<expr->data;
								}
								break;
								default:{}
								}
							}
							if(insert_value->type == 1)	// 设置为default, 暂不支持
							{}

							if(!is_all_col)	// insert part of columns
							{
								col = (Columns *)col->next;	// col point to next column
								insert_value = (Insert_vals *)insert_value->next;	// insert_val point to next value

//								if(col == NULL && insert_value == NULL) // insert column and value both exhaust,which is correct
//								{
//									ostr<<"|";
//									continue;
//								}
//								else
									if((col == NULL) ^ (insert_value == NULL))	// insert columns number don't equal to insert value number
								{
									ASTParserLogging::elog("the number of columns and values are not equal!");
									correct = false;
									break;
								}
							}
							else	// insert all columns
							{
								// make sure value number is equl to the column number
								if((insert_value = (Insert_vals *)insert_value->next)  == NULL)
								{
									if(position <table->getNumberOfAttribute()-1)
									{
										ASTParserLogging::elog("number of values is too few");
										correct = false;
									}
									ostr<<"|";
									break;
								}
							}
						}
						ostr<<"|";

					}
					if(!correct)
						break;

					insert_value_list = (Insert_val_list *)insert_value_list->next;
					if(insert_value_list != NULL)
						ostr<<"\n";
				}
				if(!correct)
					break;
				ASTParserLogging::log("the insert content is \n%s\n",ostr.str().c_str());

				HdfsLoader* Hl = new HdfsLoader(table);
				Hl->append(ostr.str().c_str());
			}
			break;
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
