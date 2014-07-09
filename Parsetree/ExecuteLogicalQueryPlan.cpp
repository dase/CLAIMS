/*
 * ExecuteLogicalQueryPlan.cpp
 *
 *  Created on: 2014-3-3
 *      Author: imdb
 */

#ifndef __EXECLOGICALQUERYPLAN__
#define __EXECLOGICALQUERYPLAN__

#include <iostream>
#include <cstring>
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

const int INT_LENGTH = 10;
const int FLOAT_LENGTH = 10;
const int SMALLINT_LENGTH = 4;

timeval start_time;	//2014-5-4---add---by Yu


void ExecuteLogicalQueryPlan()	// 2014-3-4---因为根结点的结构已经改变，相关代码进行修改---by余楷
{

	Environment::getInstance(true);

	ResourceManagerMaster *rmms=Environment::getInstance()->getResourceManagerMaster();
	Catalog* catalog=Environment::getInstance()->getCatalog();

	int count=1;
	while(1)
	{
		//cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SQL is begginning~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;;
		string tablename;
		Node* oldnode=getparsetreeroot();

		// get parser time	//2014-5-4---add---by Yu
		timeval finish_parser_time;
		gettimeofday(&finish_parser_time, NULL);
		cout<<"parser use "<<(double)(finish_parser_time.tv_usec - start_time.tv_usec)/1000+(finish_parser_time.tv_sec - start_time.tv_sec)<<" ms"<<endl;

		if(oldnode == NULL)	// 2014-2-24---增加node为空的判断---by余楷
		{
			printf("[ERROR]there are some wrong in statement! please try again!!\n");
			FreeAllNode();	//释放SQL解析过程忠所有申请的内存		// 2014-3-6---增加解析错误后的处理---by余楷
//			printf("Continue(1) or not (others number)?\n");
//			scanf("%d",&count);
//			getchar();	// 2014-3-4---屏蔽换行符对后面的影响---by余楷
//			//setbuf(stdin, NULL);	//关闭缓冲
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

				SQLParse_log("this is create table stmt \n");
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
					ASTParserLogging::elog("No table name!");
					break;
				}

				// 2014-3-25---检查表是否已存在---by Yu
				TableDescriptor *new_table = Environment::getInstance()->getCatalog()->getTable(tablename);
				if (new_table != NULL)
				{
//					cout<<"[ERROR]: The table "<<tablename<<" has existed!"<<endl;
					ASTParserLogging::elog("The table %s has existed!", tablename.c_str());
					break;
				}

				// 创建新表
				new_table = new TableDescriptor(tablename,Environment::getInstance()->getCatalog()->allocate_unique_table_id());

				new_table->addAttribute("row_id",data_type(t_u_long),0,true);

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

						/* TODO: Whether column is unique or has default value is not finished,
						 *  because there are no supports
						 */
						Datatype * datatype = (Datatype *)data->datatype;
						switch (datatype->datatype)	// add more type --- 2014-4-2
						{
						case 3:
						{
							if (column_atts && (column_atts->datatype && 01)){
								new_table->addAttribute(colname, data_type(t_smallInt), 0, true, false);
							}
							else if (column_atts && (column_atts->datatype && 02)){
								new_table->addAttribute(colname, data_type(t_smallInt), 0, true, true);
							}
							else{
								new_table->addAttribute(colname, data_type(t_smallInt), 0, true);
							}
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 5: case 6:
						{
							if (column_atts && (column_atts->datatype && 01)){
								new_table->addAttribute(colname, data_type(t_int), 0, true, false);
							}
							else if (column_atts && (column_atts->datatype && 02)){
								new_table->addAttribute(colname, data_type(t_int), 0, true, true);
							}
							else{
								new_table->addAttribute(colname, data_type(t_int), 0, true);
							}
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 7:
						{
							if (datatype->opt_uz & 01 != 0)
							{
								if (column_atts && (column_atts->datatype && 01)){
									new_table->addAttribute(colname, data_type(t_u_long), 0, true, false);
								}
								else if (column_atts && (column_atts->datatype && 02)){
									new_table->addAttribute(colname, data_type(t_u_long), 0, true, true);
								}
								else{
									new_table->addAttribute(colname, data_type(t_u_long), 0, true);
								}
								cout<<colname<<" is created"<<endl;
							}
							else
							{
								//TODO:no supports
								ASTParserLogging::log("This type is not supported!");
							}
							break;
						}
						case 9:
						{
							if (column_atts && (column_atts->datatype && 01)){
								new_table->addAttribute(colname, data_type(t_double), 0, true, false);
							}
							else if (column_atts && (column_atts->datatype && 02)){
								new_table->addAttribute(colname, data_type(t_double), 0, true, true);
							}
							else{
								new_table->addAttribute(colname, data_type(t_double), 0, true);
							}
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 10:
						{
							if (column_atts && (column_atts->datatype && 01)){
								new_table->addAttribute(colname, data_type(t_float), 0, true, false);
							}
							else if (column_atts && (column_atts->datatype && 02)){
								new_table->addAttribute(colname, data_type(t_float), 0, true, true);
							}
							else{
								new_table->addAttribute(colname, data_type(t_float), 0, true);
							}
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 11:
						{
							if (column_atts && (column_atts->datatype && 01)){
								new_table->addAttribute(colname, data_type(t_decimal), 0, true, false);
							}
							else if (column_atts && (column_atts->datatype && 02)){
								new_table->addAttribute(colname, data_type(t_decimal), 0, true, true);
							}
							else{
								new_table->addAttribute(colname, data_type(t_decimal), 0, true);
							}
							cout<<colname<<" is created"<<endl;
						}
						case 12:	// DATE --- 2014-4-1
						{
							if (column_atts && (column_atts->datatype && 01)){
								new_table->addAttribute(colname, data_type(t_date), 0, true, false);
							}
							else if (column_atts && (column_atts->datatype && 02)){
								new_table->addAttribute(colname, data_type(t_date), 0, true, true);
							}
							else{
								new_table->addAttribute(colname, data_type(t_date), 0, true);
							}
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 13:	// TIME --- 2014-4-1
						{
							if (column_atts && (column_atts->datatype && 01)){
								new_table->addAttribute(colname, data_type(t_time), 0, true, false);
							}
							else if (column_atts && (column_atts->datatype && 02)){
								new_table->addAttribute(colname, data_type(t_time), 0, true, true);
							}
							else{
								new_table->addAttribute(colname, data_type(t_time), 0, true);
							}
							cout<<colname<<" is created"<<endl;
							break;
						}
						case 15:	// DATETIME --- 2014-4-1
						{
							if (column_atts && (column_atts->datatype && 01)){
								new_table->addAttribute(colname, data_type(t_datetime), 0, true, false);
							}
							else if (column_atts && (column_atts->datatype && 02)){
								new_table->addAttribute(colname, data_type(t_datetime), 0, true, true);
							}
							else{
								new_table->addAttribute(colname, data_type(t_datetime), 0, true);
							}
							cout<<colname<<" is created"<<endl;
							break;
						}

						case 17: case 18:
						{
							if (datatype->length)	//已指定长度
							{
								Length * l = (Length*)datatype->length;

								if (column_atts && (column_atts->datatype && 01)){
									new_table->addAttribute(colname, data_type(t_string), l->data1, true, false);
								}
								else if (column_atts && (column_atts->datatype && 02)){
									new_table->addAttribute(colname, data_type(t_string), l->data1, true, true);
								}
								else{
									new_table->addAttribute(colname, data_type(t_string), l->data1, true);
								}
							}
							else	//未指定长度
							{
								if (column_atts && (column_atts->datatype && 01)){
									new_table->addAttribute(colname, data_type(t_string), 1, true, false);
								}
								else if (column_atts && (column_atts->datatype && 02)){
									new_table->addAttribute(colname, data_type(t_string), 1, true, true);
								}
								else{
									new_table->addAttribute(colname, data_type(t_string), 1, true);
								}
							}
							cout<<colname<<" is created"<<endl;
							break;
						}
						default:
						{
							ASTParserLogging::log("This type is not supported now!");
						}
						}
					}
					list = (Create_col_list* )list->next;
				}

				// add for  test, create projection default while creating table
				std::vector<ColumnOffset> index;
				index.push_back(1);
				cout<<"Name:"<<new_table->getAttribute(0).getName()<<endl;

				new_table->createHashPartitionedProjectionOnAllAttribute(new_table->getAttribute(1).getName(), 1);

				catalog->add_table(new_table);

				TableID table_id=catalog->getTable(tablename)->get_table_id();

//				for(unsigned i=0;i<catalog->getTable(table_id)->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){
////					catalog->getTable(table_id)->getProjectoin(catalog->getTable(table_id)->getNumberOfProjection()-1)->getPartitioner()->RegisterPartition(i,2);
//					catalog->getTable(table_id)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
//				}

				catalog->saveCatalog();

			}
			break;
			case t_create_projection_stmt:	// 创建projection的语句
			{
				bool is_correct = true;
				cout<<"this is create projection "<<endl;
				Create_projection_stmt *newnode = (Create_projection_stmt *)node;
				int partition_num = newnode->partition_num;
				string tablename = newnode->tablename;
				TableDescriptor *table = NULL;

				if((table = catalog->getTable(tablename)) == NULL)	// 2014-4-30---add check---by Yu
				{
					ASTParserLogging::elog("There is no table named %s", tablename.c_str());
					is_correct = false;
					break;
				}
				TableID table_id=catalog->getTable(tablename)->get_table_id();
				string partition_attribute_name = newnode->partition_attribute_name;

				std::vector<ColumnOffset> index;
				Columns *col_list = (Columns *)newnode->column_list;
				string colname;
				while(col_list)
				{
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
						cout<<"[ERROR]NO column name!"<<endl;
						is_correct = false;
						break;
					}
					cout<<colname<<endl;
					if(table->isExist(colname))	// 2014-4-30---add check---by Yu
						index.push_back(table->getAttribute(colname).index);
					else
					{
						ASTParserLogging::elog("The column %s is not existed!", colname.c_str());
						is_correct = false;
						break;
					}
					col_list = (Columns *)col_list->next;
				}

				if(!is_correct)
					break;

				catalog->getTable(table_id)->createHashPartitionedProjection(index,partition_attribute_name,partition_num);

				for(unsigned i=0;i<catalog->getTable(table_id)->getProjectoin(0)->getPartitioner()->getNumberOfPartitions();i++){

					catalog->getTable(table_id)->getProjectoin(catalog->getTable(table_id)->getNumberOfProjection()-1)->getPartitioner()->RegisterPartition(i,2);
					//				catalog->getTable(table_id)->getProjectoin(0)->getPartitioner()->RegisterPartition(i,2);
				}
			}
			break;
			case t_query_stmt: // 2014-3-4---修改为t_query_stmt,添加对查询语句的处理---by余楷
			{
				SQLParse_log("this is query stmt!!!!!!!!!!!!!!!!!!");
				if (!semantic_analysis(node,false))//---3.22fzh---
					SQLParse_elog("semantic_analysis error");
				preprocess(node);
#ifdef SQL_Parser
				output(node,0);
#endif
					Query_stmt *querynode=(Query_stmt *)node;
					if(querynode->where_list!=NULL)
					{
						struct Where_list * curt=(struct Where_list *)(querynode->where_list);
						struct Node *cur=(struct Node *)(curt->next);
						SQLParse_log("wc2tb");
						departwc(cur,querynode->from_list);
					}
					if(querynode->from_list!=NULL)
					int fg=solve_join_condition(querynode->from_list);
#ifdef SQL_Parser
				output(node,0);
#endif
				LogicalOperator* plan=parsetree2logicalplan(node);//现在由于没有投影，所以只把from_list传输进去。因此在完善之后，需要在parsetree2logicalplan()中
				//进行判断，对于不同的语句，比如select,update等选择不同的操作。

				LogicalOperator* root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::PRINT);
#ifdef SQL_Parser
				root->print(0);
				cout<<"performance is ok!the data will come in,please enter any char to continue!!"<<endl;
				getchar();
				getchar();
#endif
				BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024);
				cout<<"~~~~~~~~~physical plan~~~~~~~~~~~~~~"<<endl;
				physical_iterator_tree->print();
				cout<<"~~~~~~~~~physical plan~~~~~~~~~~~~~~"<<endl;

				physical_iterator_tree->open();
				while(physical_iterator_tree->next(0));
				physical_iterator_tree->close();
			//	printf("Q1: execution time: %4.4f second.\n",getSecond(start));
			}
			break;
			case t_load_table_stmt:	//	导入数据的语句
			{
				Loadtable_stmt *new_node = (Loadtable_stmt*)node;

				string table_name(new_node->table_name);
				TableDescriptor *table = catalog->getTable(table_name);

				// 2014-4-17---check the exist of table---by Yu
				if(table == NULL)
				{
					ASTParserLogging::elog("the table %s does not exist!", table_name.c_str());
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

				catalog->saveCatalog();

			}
			break;
			case t_insert_stmt:	// 2014-4-19---add---by Yu	// 2014-5-1---modify---by Yu
			{
				bool has_warning = false;
				bool is_correct = true;
				bool is_all_col = false;
				Insert_stmt *insert_stmt = (Insert_stmt *)node;

				string table_name(insert_stmt->tablename);
				TableDescriptor *table = Environment::getInstance()->getCatalog()->getTable(table_name);
				if(table == NULL)
				{
					ASTParserLogging::elog("The table %s does not exist!", table_name.c_str());
					break;
				}

				unsigned col_count = 0;
				Columns *col = (Columns *)insert_stmt->col_list;
				if (col == NULL) {	// insert all columns
					is_all_col = true;
				}
				else {	// get insert column count
					++col_count;
					while(col = (Columns *)col->next) ++col_count;
				}

				Insert_val_list *insert_value_list = (Insert_val_list*)insert_stmt->insert_val_list;
				if (insert_value_list == NULL) {
					ASTParserLogging::elog("No value!");
					break;
				}

				ostringstream ostr;
				while(insert_value_list)	// 循环获得 （……），（……），（……）中的每一个（……）
				{
					// make sure: the insert column count = insert value count = used column count = used value count

					// init
					Insert_vals *insert_value = (Insert_vals *)insert_value_list->insert_vals;
					col = (Columns *)insert_stmt->col_list;

					if (is_all_col)	// insert all columns
					{
						// by scdong: Claims adds a default row_id attribute for all tables which is attribute(0), when inserting tuples we should begin to construct the string_tuple from the second attribute.
						for(unsigned int position = 1; position < table->getNumberOfAttribute(); position++)
						{
							// check value count
							if (insert_value == NULL)
							{
								ASTParserLogging::elog("Value count is too few");
								is_correct = false;
								break;
							}

							// insert value to ostringstream and if has warning return 1;	look out the order!
							has_warning = InsertValueToStream(insert_value, table, position, ostr) || has_warning;

							// move back
							insert_value = (Insert_vals*)insert_value->next;
							ostr<<"|";
						}

						if (!is_correct) break;

						// check insert value count
						if (insert_value)
						{
							ASTParserLogging::elog("Value count is too many");
							is_correct = false;
							break;
						}
					}
					else	//insert part of columns
					{
						// get insert value count and check whether it match column count
						unsigned insert_value_count = 0;
						while (insert_value)
						{
							++insert_value_count;
							insert_value = (Insert_vals*)insert_value->next;
						}
						if (insert_value_count != col_count)
						{
							ASTParserLogging::elog("Column count doesn't match value count");
							is_correct = false;
							break;
						}

						unsigned int used_col_count = 0;

						// by scdong: Claims adds a default row_id attribute for all tables which is attribute(0), when inserting tuples we should begin to construct the string_tuple from the second attribute.
						for(unsigned int position = 1; position < table->getNumberOfAttribute(); position++)
						{
							// find the matched column and value by name
							col = (Columns*)insert_stmt->col_list;
							Insert_vals *insert_value = (Insert_vals *)insert_value_list->insert_vals;
							while (col && table->getAttribute(position).attrName.compare(col->parameter1))
							{
								col = (Columns*)col->next;
								insert_value = (Insert_vals*)insert_value->next;
							}

							// if find
							// the column count is proved to match the insert value count, so column exist, then insert_value exist
							if (col && insert_value)
							{
								++used_col_count;

								// insert value to ostringstream and if has warning return 1; look out the order!
								has_warning = InsertValueToStream(insert_value, table, position, ostr) || has_warning;
							}

							ostr<<"|";
						}//end for

						// check if every insert column is existed
						if (used_col_count != col_count)
						{
							ASTParserLogging::elog("Some columns don't exist");
							is_correct = false;
							break;
						}
					}//end else

					if (!is_correct) break;

					insert_value_list = (Insert_val_list*)insert_value_list->next;
					if(insert_value_list != NULL)
						ostr<<"\n";
				}// end while

				if (!is_correct) break;
				if (has_warning) ASTParserLogging::log("[WARNING]: The type is not matched!\n");
				ASTParserLogging::log("the insert content is \n%s\n",ostr.str().c_str());

				HdfsLoader* Hl = new HdfsLoader(table);
				string tmp = ostr.str().c_str();
				Hl->append(ostr.str().c_str());

				catalog->saveCatalog();

			}
			break;
			case t_show_stmt:
			{
				Show_stmt *show_stmt = (Show_stmt *)node;
				switch(show_stmt->show_type)
				{
					case 1:
					{
						cout<<"Tables:"<<endl;
						for (unsigned i = 0; i < catalog->getTableCount(); ++i)
							cout<<catalog->getTable(i)->getTableName()<<endl;
					}
					break;
					default:{
						ASTParserLogging::elog("Sorry, not supported now!");
					}
				}
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

//		SQLParse_log("SQL Complete! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//		printf("Continue(1) or not (0)?\n");
//		scanf("%d",&count);
//		getchar();	// 2014-3-4---屏蔽换行符对后面的影响---by余楷
	}
}

bool InsertValueToStream(Insert_vals *insert_value, TableDescriptor *table, unsigned position, ostringstream &ostr)
{
	bool has_warning = true;
	if (insert_value->value_type == 0)	// 指定具体的值
	{
		// check whether the column type match value type
		has_warning = CheckType(table->getAttribute(position).attrType, (Expr*)insert_value->expr);

		switch (insert_value->expr->type)	// 2014-4-17---only these type are supported now---by Yu
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
	else if(insert_value->type == 1) {}	// 设置为default, 暂不支持

	return has_warning;
}

bool CheckType(const column_type *col_type, Expr *expr)
{
	nodetype insert_value_type = expr->type;
	//TODO
	switch(col_type->type)
	{
	case t_int: return (insert_value_type != t_intnum) || strlen(expr->data) > INT_LENGTH;
	case t_float: return !(insert_value_type == t_approxnum) || strlen(expr->data) > FLOAT_LENGTH;
	case t_double: return (insert_value_type != t_approxnum);
	case t_string: return (insert_value_type != t_stringval) || strlen(expr->data) > col_type->get_length();//---5.27fzh---
	//case t_u_long: return (insert_value_type != t_intnum) || strlen(expr->data) > INT_LENGTH || (expr->data[0] == '-');	// =='-' 实际不可行，‘-’不会被识别进expr中
	case t_date: return false;
	case t_time: return false;
	case t_datetime: return false;
	case t_smallInt: return (insert_value_type != t_intnum) || strlen(expr->data) > SMALLINT_LENGTH;
	case t_decimal: return (insert_value_type != t_intnum);
	//case t_u_smallInt: return (insert_value_type != t_intnum) || (expr->data[0] == '-') || length(expr->data) > INT_LENGTH;
	default: return true;
	}
}

#endif
