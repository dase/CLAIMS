/*
3.6日
目前主要完成对表名/属性名判断是否唯一存在以及根据单个属性获得表的别名等
尚需完善：
1.函数用的格式
2.表达式是否符合正确格式
3.groupby子句运用规则
4.aggrection(没有groupby)
*/

#include <cstdio>
#include <map>
#include <new>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include "../Catalog/Attribute.h"
#include "../Catalog/Catalog.h"
#include "../Catalog/table.h"
#include "../common/Comparator.h"
#include "sql_node_struct.h"
#include "../Environment.h"
#include "../common/Logging.h"
#include <ostream>

#include "../logical_operator/logical_aggregation.h"
#include "../logical_operator/logical_equal_join.h"
#include "../logical_operator/logical_filter.h"
#include "../logical_operator/logical_operator.h"
#include "../logical_operator/logical_scan.h"
#include "../physical_operator/physical_aggregation.h"
using namespace std;
bool semantic_analysis(Node *parsetree, bool issubquery);
bool wherecondition_analysis(Query_stmt *qstmt, Node *cur,
                             vector<Node *> rtable);

int subquery_has_column(char *colname, Node *subquery) {
  int result = 0;
  Query_stmt *querynode = (Query_stmt *)subquery;

  for (Node *p = querynode->select_list; p != NULL;) {
    Select_list *slist = (Select_list *)p;
    Select_expr *sexpr = (Select_expr *)slist->args;
    if (sexpr->ascolname != NULL) {
      if (strchr(colname, '.') != NULL &&
          strchr(sexpr->ascolname, '.') != NULL) {
        if (strcmp(strchr(colname, '.') + 1,
                   strchr(sexpr->ascolname, '.') + 1) ==
            0)  // in the form like a.b
        {
          result++;
        }
      } else if (strchr(colname, '.') == NULL &&
                 strchr(sexpr->ascolname, '.') != NULL) {
        if (strcmp(colname, strchr(sexpr->ascolname, '.') + 1) ==
            0)  // in the form like a.b
        {
          result++;
        }
      } else {
        if (strcmp(strchr(colname, '.') + 1, sexpr->ascolname) ==
            0)  // in the form like a.b
        {
          result++;
        }
      }
    } else  // TODO
    {
      if (sexpr->colname->type == t_name) {
        Columns *col = (Columns *)sexpr->colname;
        if (strcmp(col->parameter2, sexpr->ascolname) == 0) {
          result++;
        }
      } else if (sexpr->colname->type == t_name_name) {
        Columns *col = (Columns *)sexpr->colname;
        char temp_name[100];
        int i;
        for (i = 0; i < strlen(col->parameter2) && col->parameter2[i] != '.';
             i++)
          ;
        if (i < strlen(col->parameter2)) {
          strcpy(temp_name, col->parameter2 + i + 1);
        } else {
          strcpy(temp_name, col->parameter2);
        }
        if (strcmp(temp_name, colname) == 0) {
          result++;
        }
      } else {
        //				SQLParse_elog("the column in subquery should
        //be aliased");
      }
    }
    p = slist->next;
  }
  return result;
}
int table_has_column(char *colname, vector<Node *> rtable,
                     char *&tablename)  // judge the table has column?
{
  int result = 0;
  for (int i = 0; i < rtable.size(); i++)
  //	for(Node *p=node;p!=NULL;)
  {
    //	From_list *fromlist=(From_list *)rtable[i];
    Table *table = (Table *)rtable[i];
    if (table->issubquery == 0) {
      //	char *str="";
      stringstream ss;
      ss << table->tablename << "." << colname;
      //	str=(char *)ss.str().c_str();

      if (Environment::getInstance()->getCatalog()->isAttributeExist(
              table->tablename, ss.str()) == true) {
        tablename = table->astablename;
        result++;
      }
    } else if (table->issubquery ==
               1)  // judge  the attribute exists in subquery not in basetable
    {
      int fg = subquery_has_column(colname, table->subquery);
      if (fg > 0) {
        tablename = table->astablename;
        result += fg;
      }
    }
    //	p=fromlist->next;
  }
  return result;
}
int fromlist_has_astablename(char *astablename, vector<Node *> rtable,
                             char *&tablename,
                             Node *&subnode)  // according to
                                              // astablename,getting tablename
                                              // from fromlist,if subquery then
                                              // 2，basetabel then 1
{
  for (int i = 0; i < rtable.size(); i++) {
    Table *table0 = (Table *)rtable[i];
    if (strcmp(table0->astablename, astablename) == 0) {
      if (table0->issubquery == 0) {
        tablename = table0->tablename;
        return 1;
      } else {
        subnode = table0->subquery;
        return 2;
      }
    }
  }
  return 0;
}
Node *get_copy_selectlist_from_subquery(
    Node *subnode, Node *&tailnode)  // return the first and the last pointer
{
  Select_list *node, *temp;
  node = temp =
      (Select_list *)newSelectList(t_select_list, 0, NULL, NULL, NULL);
  Query_stmt *subquery = (Query_stmt *)subnode;
  for (Node *p = subquery->select_list;
       p != NULL;)  // not copy all，just copy select_list
  {
    Select_list *slist = (Select_list *)p;
    Node *q =
        newSelectList(t_select_list, slist->isall, slist->args, NULL, NULL);
    temp->next = q;
    temp = (Select_list *)q;
    p = slist->next;
  }
  tailnode = (Node *)temp;
  return node->next;
}
void add_all_table_column(Select_list *selectlist, vector<Node *> rtable,
                          Node *next) {
  for (int i = 0; i < rtable.size(); i++)
  //	for(Node *p=flist;p!=NULL;)
  {
    //	output(rtable[i],0);
    //	From_list *fromlist=(From_list *)rtable[i];
    Table *table = (Table *)rtable[i];
    if (table->issubquery == 0) {
      vector<Attribute> columns = Environment::getInstance()
                                      ->getCatalog()
                                      ->getTable(table->tablename)
                                      ->getAttributes();
      for (vector<Attribute>::iterator it = columns.begin();
           it != columns.end(); it++) {
        Node *q = newColumn(t_name_name, table->astablename,
                            (char *)(it->attrName).c_str(), NULL, NULL);
        q = newSelectExpr(t_select_expr, (char *)(it->attrName).c_str(), q,
                          NULL);
        q = newSelectList(t_select_list, 0, q, NULL, NULL);
        selectlist->next = q;
        selectlist = (Select_list *)q;
      }
    } else {
      Node *tailnode;
      Node *q = get_copy_selectlist_from_subquery(table->subquery, tailnode);
      selectlist->next = q;
      selectlist = (Select_list *)tailnode;
    }
    // p=fromlist->next;
  }
  selectlist->next = next;
}
void add_table_column(Select_list *selectlist, Node *next,
                      vector<Node *> rtable)  // table.*
{
  Select_expr *sexpr = (Select_expr *)selectlist->args;
  Columns *col = (Columns *)sexpr->colname;
  char *tablename;
  Node *subnode;
  int fg =
      fromlist_has_astablename(col->parameter1, rtable, tablename, subnode);
  if (fg == 1) {
    vector<Attribute> columns = Environment::getInstance()
                                    ->getCatalog()
                                    ->getTable(tablename)
                                    ->getAttributes();
    for (vector<Attribute>::iterator it = columns.begin(); it != columns.end();
         it++) {
      Node *q = newColumn(t_name_name, col->parameter1,
                          (char *)(it->attrName).c_str(), NULL, NULL);
      q = newSelectExpr(t_select_expr, (char *)(it->attrName).c_str(), q, NULL);
      q = newSelectList(t_select_list, 0, q, NULL, NULL);
      selectlist->next = q;
      selectlist = (Select_list *)q;
    }
  } else if (fg == 2) {
    Node *tailnode;
    Node *q = get_copy_selectlist_from_subquery(subnode, tailnode);
    selectlist->next = q;
    selectlist = (Select_list *)tailnode;
  }
  selectlist->next = next;
}
int selectlist_expr_analysis(Node *slnode, Query_stmt *qstmt, Node *node,
                             vector<Node *> rtable) {
  Select_list *selectlist = (Select_list *)slnode;
  Select_expr *sexpr = (Select_expr *)selectlist->args;

  switch (node->type) {
    case t_expr_func:  // judge using the function is ok ? including the
                       // parameter type and number
    {
      Expr_func *funcnode = (Expr_func *)node;
      if (strcmp(funcnode->funname, "FCOUNTALL") == 0) {
        return 3;
      } else {
        int result = 0;
        if (funcnode->args != NULL) {
          result =
              selectlist_expr_analysis(slnode, qstmt, funcnode->args, rtable);
          if (result == 3 || result == 0) return result;
        }
        if (funcnode->parameter1 != NULL) {
          result = selectlist_expr_analysis(slnode, qstmt, funcnode->parameter1,
                                            rtable);
          if (result == 3 || result == 0) return result;
        }
        if (funcnode->parameter2 != NULL) {
          result = selectlist_expr_analysis(slnode, qstmt, funcnode->parameter2,
                                            rtable);
          if (result == 3 || result == 0) return result;
        }
        if (funcnode->next != NULL)  // there are next in case_list
        {
          result =
              selectlist_expr_analysis(slnode, qstmt, funcnode->next, rtable);
          if (result == 3 || result == 0) return result;
        }
      }
    } break;
    case t_expr_cal:  // judge using the expr ok? like the parameter type of the
                      // expression ,the divided !=0 etc.
    {
      Expr_cal *calnode = (Expr_cal *)node;
      int result = 0;
      if (calnode->lnext == 0) {
        result = 1;
      } else {
        result =
            selectlist_expr_analysis(slnode, qstmt, calnode->lnext, rtable);
      }
      if (result == 0) {
        return false;
      } else {
        return selectlist_expr_analysis(slnode, qstmt, calnode->rnext, rtable);
      }
    } break;
    case t_name: {
      Columns *col = (Columns *)node;
      char *astablename = "";
      int result = table_has_column(col->parameter2, rtable, astablename);
      if (result == 1) {
        col->parameter1 = astablename;
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());

        //				char * strtmp=(char
        //*)malloc(sizeof(col->parameter1)+sizeof(col->parameter2)+10);
        //				strcpy(strtmp,col->parameter1);
        //				strcat(strtmp,".");
        //				strcat(strtmp,col->parameter2);
        //				col->parameter2=strtmp;
        col->type = t_name_name;
        //				if(sexpr->ascolname==NULL)
        //				{
        //					sexpr->ascolname=col->parameter2;
        //				}
      } else if (result == 0) {
        SQLParse_elog("selectlist %s can't find in tables", col->parameter2);
        return 0;
      } else {
        SQLParse_elog("selectlist %s is ambiguous", col->parameter2);
        return 0;
      }
    } break;
    case t_name_name: {
      Columns *col = (Columns *)node;
      char *tablename = "";
      Node *subnode;
      int fg =
          fromlist_has_astablename(col->parameter1, rtable, tablename, subnode);
      if (fg == 0) {
        SQLParse_elog("selectlist %s can't find ", col->parameter2);
        return 0;
      }
      if (strcmp(col->parameter2, "*") == 0) {
        selectlist->isall = 1;
        return 1;
      }
      if (fg == 1)  // base_table, if the base_table has alias, how?
      {
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());

        if (Environment::getInstance()->getCatalog()->isAttributeExist(
                tablename, col->parameter2) == 0) {
          char *str = col->parameter2;
          // strcat(str,"  can't find ");
          SQLParse_elog("selectlist %s %s ", col->parameter1, str);
          return 0;
        } else {
          //					if(sexpr->ascolname==NULL)
          //					{
          //						sexpr->ascolname=col->parameter2;
          //					}
          return 1;
        }
      }
      if (fg == 2)  // subquery
      {
        int fg = subquery_has_column(col->parameter2, subnode);
        if (fg > 1) return 0;
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());
        return fg;
      }
    } break;
    default: {}
  }
  return 1;
}
/*


*/
bool selectlist_analysis(Query_stmt *qstmt, vector<Node *> rtable) {
  Node *sltree = (Node *)qstmt->select_list;
  if (sltree == NULL) {
    SQLParse_elog("selectlist query_stmt select_list is null");
  } else {
    for (Node *p = sltree; p != NULL;) {
      Select_list *selectlist = (Select_list *)p;
      if (selectlist->isall == 0) {
        Select_expr *sexpr = (Select_expr *)selectlist->args;
        Node *node =
            (Node *)sexpr->colname;  // selectexpr including
                                     // expr_func,t_name,t_name_name,expr_cal
        int result = selectlist_expr_analysis(p, qstmt, node, rtable);
        if (result == 3)  // special case:fcountall
        {
          p = selectlist->next;
          continue;
        } else if (result == 0) {
          return false;
        }
      }
      p = selectlist->next;
      if (selectlist->isall == 1)  // solve *
      {
        if (selectlist->args == NULL)  //*
        {
          add_all_table_column(selectlist, rtable, p);
        } else  // table.*
        {
          add_table_column(selectlist, p, rtable);
        }
      }
    }
    /*delete the node of isall*/
    for (Node *p = sltree; p != NULL;)  // solve the first node of isall
    {
      Select_list *selectlist = (Select_list *)p;
      if (selectlist->isall == 0) {
        sltree = p;
        break;
      }
      p = selectlist->next;
      //	free(selectlist);////is ok?
    }
    qstmt->select_list = sltree;
    Node *frontnode = sltree;
    Node *rearnode;
    for (Node *p = ((Select_list *)sltree)->next; p != NULL;) {
      Select_list *selectlist = (Select_list *)p;
      if (selectlist->isall == 1) {
        ((Select_list *)frontnode)->next = selectlist->next;
        //				free(selectlist);
      } else {
        frontnode = p;
      }
      p = selectlist->next;
    }
  }
  return true;
}
int oncondition_check(Node *&cur, vector<Node *> rtable) {
  switch (cur->type) {
    case t_name: {
      Columns *col = (Columns *)cur;
      char *astablename;
      int result = table_has_column(col->parameter2, rtable, astablename);
      if (result == 1) {
        col->parameter1 = astablename;
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());
        col->type = t_name_name;
      } else if (result == 0) {
        SQLParse_elog("oncondition %s can't find in tables and selectlist",
                      col->parameter2);
        return false;
      } else {
        SQLParse_elog("oncondition %s in fromlist is ambiguous",
                      col->parameter2);
        return false;
      }
    } break;
    case t_name_name: {
      Columns *col = (Columns *)cur;
      char *tablename;
      Node *subnode;
      int fg =
          fromlist_has_astablename(col->parameter1, rtable, tablename, subnode);
      if (fg == 0) {
        SQLParse_elog("oncondition %s  can't find", col->parameter2);
        return false;
      } else if (fg == 1) {
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());
        if (Environment::getInstance()->getCatalog()->isAttributeExist(
                tablename, col->parameter2) == 0) {
          SQLParse_elog("oncondition %s  can't find ", col->parameter2);
          return false;
        }
      } else if (fg == 2) {
        int fg = subquery_has_column(col->parameter2, subnode);
        if (fg > 1 || fg == 0) return false;
      }

    } break;
    case t_query_stmt: {
      return semantic_analysis(cur, true);
    } break;
    case t_expr_cal: {
      Expr_cal *node = (Expr_cal *)cur;
      if (node->lnext == NULL || node->rnext == NULL) return false;
      bool flag = true;
      flag = oncondition_check(node->lnext, rtable);
      if (flag == false) return false;
      flag = oncondition_check(node->rnext, rtable);
      if (flag == false) return false;
    } break;
    case t_expr_func: {
      Expr_func *node = (Expr_func *)cur;
      if (node->args == NULL) return false;
      return oncondition_check(node->args, rtable);
    } break;
    case t_expr_list: {
      Expr_list *node = (Expr_list *)cur;
      bool flag = true;
      if (node->data != NULL) {
        flag = oncondition_check(node->data, rtable);
        if (flag == false) return false;
      } else {
        return false;
      }
      if (node->next != NULL) {
        return oncondition_check(node->next, rtable);
      }
    } break;
    default: {}
  }
  return true;
}
bool oncondition_analysis(Node *&cur, vector<Node *> jointable) {
  if (cur == NULL) {
    return true;
  }
  switch (cur->type) {
    case t_expr_cal: {
      struct Expr_cal *exprnode = (struct Expr_cal *)cur;
      if (strcmp((exprnode->sign), "ANDOP") == 0) {
        bool flag = true;
        flag = oncondition_analysis(exprnode->lnext, jointable);
        if (flag == false) return false;
        flag = oncondition_analysis(exprnode->rnext, jointable);
        if (flag == false) return false;
      } else {
        oncondition_check(cur, jointable);
      }
    } break;
    default: { oncondition_check(cur, jointable); }
  }
  return true;
}
bool fromlist_analysis(Query_stmt *&querynode, Node *qnode,
                       vector<Node *> &rtable) {
  if (qnode == NULL) {
    SQLParse_elog("fromlist query_stmt from_list is null");
  }
  switch (qnode->type) {
    case t_query_stmt: {
      Query_stmt *qstmt = (Query_stmt *)qnode;
      bool fg = fromlist_analysis(qstmt, qstmt->from_list, rtable);
      if (fg == false) return false;
    } break;
    case t_from_list: {
      From_list *flnode = (From_list *)qnode;
      bool fg = false;
      fg = fromlist_analysis(querynode, flnode->args, rtable);
      if (fg == false) return false;
      if (flnode->next != NULL) {
        fg = fromlist_analysis(querynode, flnode->next, rtable);
        if (fg == false) return false;
      }
    } break;
    case t_table: {
      Table *table = (Table *)qnode;
      rtable.push_back(qnode);
      if (table->issubquery == 0)  // not subquery
      {
        if (Environment::getInstance()->getCatalog()->getTable(
                table->tablename) == NULL) {
          SQLParse_elog("fromlist %s can't find ", table->tablename);
          return false;
        } else {
          if (table->astablename == NULL) {
            table->astablename = table->tablename;
            //	strcpy(table->astablename,(const char *)table->tablename);
          }
        }
      } else  // subquery
      {
        bool fg = semantic_analysis(table->subquery, true);
        if (fg == false) return false;
        // preprocess subquery
        Node *node = table->subquery;
        preprocess(node);
        Query_stmt *querynode = (Query_stmt *)node;
        if (querynode->from_list != NULL)
          int fg = solve_join_condition(querynode->from_list);
        if (querynode->where_list != NULL) {
          struct Where_list *curt =
              (struct Where_list *)(querynode->where_list);
          struct Node *cur = (struct Node *)(curt->next);
          SQLParse_log("wc2tb");
          departwc(cur, querynode->from_list);
        }
      }
    } break;
    case t_join: {
      Join *jnode = (Join *)qnode;
      bool fg = false;
      fg = fromlist_analysis(querynode, jnode->lnext, rtable);
      if (fg == false) return false;
      fg = fromlist_analysis(querynode, jnode->rnext, rtable);
      if (fg == false) return false;
      vector<Node *> jointable;
      jointable.push_back(jnode->lnext);
      jointable.push_back(jnode->rnext);
      if (jnode->condition != NULL) /*on 1 using 2*/
      {
        Condition *cnode = (Condition *)jnode->condition;
        if (cnode->conditiontype == 1) {
          fg = oncondition_analysis(
              cnode->args, rtable);  // should restrict the scope of table
          if (fg == false) return false;
        } else  // using join :if two column is the same ,just keep one TODO
        {
          Node *tp = newExprList(t_expr_list, NULL, NULL, NULL);
          Node *temp = tp;
          for (Node *p = cnode->args; p != NULL;) {
            Columns *col = (Columns *)p;
            //						cout<<((Table
            //*)(jnode->lnext))->astablename<<"dddddddddddddd
            //"<<col->parameter1<<endl;
            Node *cal = newExprCal(
                t_expr_cal, "CMP", NULL, 4,
                newColumn(t_name_name, ((Table *)(jnode->lnext))->astablename,
                          col->parameter1, NULL, NULL),
                newColumn(t_name_name, ((Table *)(jnode->rnext))->astablename,
                          col->parameter1, NULL, NULL),
                NULL);
            Node *q = newExprList(t_expr_list, cal, NULL, NULL);
            ((Expr_list *)tp)->next = q;
            tp = q;
            p = col->next;
          }
          cnode->args = ((Expr_list *)temp)->next;
        }
      }
    } break;
    default: {}
  }
  return true;
}
bool fromlist_table_is_unique(vector<Node *> rtable) {
  for (int i = 0; i < rtable.size(); i++) {
    Table *table = (Table *)rtable[i];
    for (int j = i + 1; j < rtable.size(); j++) {
      Table *table1 = (Table *)rtable[j];
      if (strcmp(table->astablename, table1->astablename) == 0) {
        SQLParse_elog("fromlist %s is ambiguous", table->astablename);
        return false;
      }
    }
  }
  return true;
}
//
int selectlist_has_column(char *&ascolname, Node *sltree,
                          char *&astablename)  // selectlist has ascolumn?
{
  int result = 0;
  for (Node *p = sltree; p != NULL;) {
    Select_list *selectlist = (Select_list *)p;
    Select_expr *sexpr = (Select_expr *)selectlist->args;
    Node *node = (Node *)sexpr->colname;  // not column is possible
    switch (node->type) {
      case t_name_name: {
        Columns *col = (Columns *)node;

        if (sexpr->ascolname != NULL &&
            strcmp(sexpr->ascolname, ascolname) == 0) {
          //					strcpy(ascolname,col->parameter2);
          astablename = col->parameter1;
          result++;
        } else {
          if (strcmp(col->parameter2, ascolname) == 0) {
            astablename = col->parameter1;
            result++;
          }
        }

      } break;
      default: {
        if (sexpr->ascolname != NULL &&
            strcmp(sexpr->ascolname, ascolname) == 0) {
          //					strcpy(ascolname,col->parameter2);
          //					astablename=col->parameter1;
          astablename = ascolname;
          result++;
        }
      }
    }
    p = selectlist->next;
  }
  return result;
}
bool wherecondition_check(Query_stmt *qstmt, Node *cur, vector<Node *> rtable) {
  switch (cur->type) {
    case t_name: {
      Columns *col = (Columns *)cur;
      char *astablename;
      int result = table_has_column(col->parameter2, rtable, astablename);
      if (result == 1) {
        col->parameter1 = astablename;
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());

        col->type = t_name_name;
      } else if (result ==
                 0)  // if not in fromlist,then to search in selectlisth
      {
        int result = selectlist_has_column(col->parameter2, qstmt->select_list,
                                           astablename);
        if (result == 0) {
          SQLParse_elog(
              "wherecondition %s can't find in tables and selectlist ",
              col->parameter2);
          return false;
        } else if (result == 1) {
          col->parameter1 = astablename;
          col->type = t_name_name;
        } else {
          SQLParse_elog("wherecondition %s in selectlist is ambiguous",
                        col->parameter2);
          return false;
        }
      } else {
        SQLParse_elog("wherecondition %s in fromlist is ambiguous",
                      col->parameter2);
        return false;
      }
    } break;
    case t_name_name: {
      Columns *col = (Columns *)cur;
      char *tablename;
      Node *subnode;
      int fg =
          fromlist_has_astablename(col->parameter1, rtable, tablename, subnode);
      if (fg == 0) {
        SQLParse_elog("wherecondition %s.%s can't find", col->parameter1,
                      col->parameter2);
        return false;
      } else if (fg == 1) {
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());
        if (Environment::getInstance()->getCatalog()->isAttributeExist(
                tablename, col->parameter2) == 0) {
          SQLParse_elog("wherecondition %s  can't find", col->parameter2);
          return false;
        }
      } else if (fg == 2) {
        int fg = subquery_has_column(col->parameter2, subnode);
        if (fg > 1 || fg == 0) {
          return false;
        } else {
          stringstream ss;
          ss << string(col->parameter1).c_str() << "."
             << string(col->parameter2).c_str();
          col->parameter2 = (char *)malloc(ss.str().length() + 1);
          strcpy(col->parameter2, ss.str().c_str());
          return true;
        }
      }

    } break;
    case t_query_stmt: {
      return semantic_analysis(cur, true);
    } break;
    case t_expr_cal: {
      Expr_cal *node = (Expr_cal *)cur;
      if (node->lnext == NULL && node->rnext == NULL) return false;
      bool flag = true;
      if (node->lnext != NULL)
        flag = wherecondition_check(qstmt, node->lnext, rtable);
      if (node->rnext != NULL)
        flag *= wherecondition_check(qstmt, node->rnext, rtable);
      return flag;
    } break;
    case t_expr_func: {
      Expr_func *node = (Expr_func *)cur;
      bool flag = true;
      // aggregation function shouldn't occur in where
      if (strcmp(node->funname, "FCOUNTALL") == 0 ||
          strcmp(node->funname, "FCOUNT") == 0 ||
          strcmp(node->funname, "FSUM") == 0 ||
          strcmp(node->funname, "FMIN") == 0 ||
          strcmp(node->funname, "FMAX") == 0 ||
          strcmp(node->funname, "FAVG") == 0) {
        return false;
      }
      if (node->args != NULL)
        flag = wherecondition_check(qstmt, node->args, rtable);
      if (node->parameter1 != NULL && flag)
        flag *= wherecondition_check(qstmt, node->parameter1, rtable);
      if (node->parameter2 != NULL && flag)
        flag *= wherecondition_check(qstmt, node->parameter2, rtable);
      return flag;
    } break;
    case t_expr_list: {
      Expr_list *node = (Expr_list *)cur;
      bool flag = true;
      if (node->data != NULL) {
        flag = wherecondition_check(qstmt, node->data, rtable);
        if (flag == false) return false;
      } else {
        return false;
      }
      if (node->next != NULL) {
        return wherecondition_check(qstmt, node->next, rtable);
      }
    } break;
    default: {}
  }
  return true;
}
bool wherecondition_analysis(Query_stmt *qstmt, Node *cur,
                             vector<Node *> rtable) {
  if (cur == NULL) {
    return true;
  }
  switch (cur->type) {
    case t_expr_cal: {
      struct Expr_cal *exprnode = (struct Expr_cal *)cur;
      if (strcmp((exprnode->sign), "ANDOP") == 0) {
        bool flag = true;
        flag = wherecondition_analysis(qstmt, exprnode->lnext, rtable);
        if (flag == false) return false;
        flag = wherecondition_analysis(qstmt, exprnode->rnext, rtable);
        if (flag == false) return false;
      } else {
        return wherecondition_check(qstmt, cur, rtable);
      }
    } break;
    default: { return wherecondition_check(qstmt, cur, rtable); }
  }
  return true;
}
bool groupby_analysis(Query_stmt *qstmt, Node *cur, vector<Node *> rtable) {
  switch (cur->type) {
    case t_name: {
      Columns *col = (Columns *)cur;
      char *astablename;
      int result = table_has_column(col->parameter2, rtable, astablename);

      if (result == 1) {
        col->parameter1 = astablename;
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());

        col->type = t_name_name;
      } else if (result ==
                 0)  // if not in fromlist,then to search in selectlisth
      {
        int result = selectlist_has_column(col->parameter2, qstmt->select_list,
                                           astablename);
        if (result == 0) {
          SQLParse_elog(
              "groupby_analysis %s can't find in tables and selectlist ",
              col->parameter2);
          return false;
        } else if (result == 1) {
          col->parameter1 = astablename;
          col->type = t_name_name;
        } else {
          SQLParse_elog("groupby_analysis %s in selectlist is ambiguous",
                        col->parameter2);
          return false;
        }
      } else {
        SQLParse_elog("groupby_analysis %s in fromlist is ambiguous",
                      col->parameter2);
        return false;
      }
    } break;
    case t_name_name: {
      Columns *col = (Columns *)cur;
      char *tablename;
      Node *subnode;
      int fg =
          fromlist_has_astablename(col->parameter1, rtable, tablename, subnode);
      if (fg == 0) {
        SQLParse_elog("groupby_analysis %s.%s can't find", col->parameter1,
                      col->parameter2);
        return false;
      } else if (fg == 1) {
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());
        //				if(Environment::getInstance()->getCatalog()->isAttributeExist(tablename,col->parameter2)==0)
        //				{
        //					SQLParse_elog("groupby_analysis %s
        //can't find",col->parameter2);
        //					return false;
        //				}
      } else if (fg == 2) {
        int fg = subquery_has_column(col->parameter2, subnode);

        if (fg > 1 || fg == 0) return false;
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());
        return true;
      }

    } break;
    case t_query_stmt: {
      return semantic_analysis(cur, true);
    } break;
    case t_expr_cal: {
      Expr_cal *node = (Expr_cal *)cur;
      if (node->lnext == NULL && node->rnext == NULL) return false;
      bool flag = true;
      if (node->lnext != NULL)
        flag = groupby_analysis(qstmt, node->lnext, rtable);
      if (node->rnext != NULL)
        flag *= groupby_analysis(qstmt, node->rnext, rtable);
      return flag;
    } break;
    case t_expr_func: {
      Expr_func *node = (Expr_func *)cur;
      bool flag = true;
      // aggregation function shouldn't occur in where
      if (strcmp(node->funname, "FCOUNTALL") == 0 ||
          strcmp(node->funname, "FCOUNT") == 0 ||
          strcmp(node->funname, "FSUM") == 0 ||
          strcmp(node->funname, "FMIN") == 0 ||
          strcmp(node->funname, "FMAX") == 0 ||
          strcmp(node->funname, "FAVG") == 0) {
        return false;
      }
      if (node->args != NULL)
        flag = groupby_analysis(qstmt, node->args, rtable);
      if (node->parameter1 != NULL && flag)
        flag *= groupby_analysis(qstmt, node->parameter1, rtable);
      if (node->parameter2 != NULL && flag)
        flag *= groupby_analysis(qstmt, node->parameter2, rtable);
      return flag;
    } break;
    case t_expr_list: {
      Expr_list *node = (Expr_list *)cur;
      bool flag = true;
      if (node->data != NULL) {
        flag = groupby_analysis(qstmt, node->data, rtable);
        if (flag == false) return false;
      } else {
        return false;
      }
      if (node->next != NULL) {
        return groupby_analysis(qstmt, node->next, rtable);
      }
    } break;
    case t_groupby_expr: {
      Groupby_expr *gexpr = (Groupby_expr *)cur;
      bool flag = true;
      if (gexpr->args != NULL)
        flag = flag * groupby_analysis(qstmt, gexpr->args, rtable);
      if (gexpr->next != NULL)
        flag = flag * groupby_analysis(qstmt, gexpr->next, rtable);
      return flag;
    } break;
    default: {}
  }
  return true;
}

bool judge_expr_equalto_expr(Node *lnode, Node *rnode) {
  if (lnode == NULL) {
    if (rnode != NULL) return false;
    return true;
  }
  switch (lnode->type) {
    case t_name: {
      if (rnode->type != t_name) return false;
      Columns *lname = (Columns *)lnode;
      Columns *rname = (Columns *)rnode;
      if (strcmp(lname->parameter1, rname->parameter1) != 0) return false;
      if (strcmp(lname->parameter2, rname->parameter2) != 0) return false;
      return true;
    } break;
    case t_name_name: {
      if (rnode->type != t_name_name) return false;
      Columns *lname = (Columns *)lnode;
      Columns *rname = (Columns *)rnode;
      if (strcmp(lname->parameter1, rname->parameter1) != 0) return false;
      if (strcmp(lname->parameter2, rname->parameter2) != 0) return false;
      return true;
    } break;
    case t_expr_cal: {
      if (rnode->type != t_expr_cal) return false;
      Expr_cal *rcal = (Expr_cal *)rnode;
      Expr_cal *lcal = (Expr_cal *)lnode;
      if (rcal->str == NULL) {
        if (lcal->str != NULL) return false;
      } else {
        if (lcal->str == NULL) return false;
        if (strcmp(lcal->str, rcal->str) != 0) return false;
      }
      if (rcal->sign == NULL) {
        if (lcal->sign != NULL) return false;
      } else {
        if (lcal->sign == NULL) return false;
        if (strcmp(rcal->sign, lcal->sign) != 0) return false;
      }
      if (rcal->parameter == NULL) {
        if (lcal->parameter != NULL) return false;
      } else {
        if (lcal->parameter == NULL) return false;
        if (strcmp(rcal->parameter, lcal->parameter) != 0) return false;
      }
      if (rcal->cmp == NULL) {
        if (lcal->cmp != NULL) return false;
      } else {
        if (lcal->cmp == NULL) return false;
        if (lcal->cmp != rcal->cmp) return false;
      }
      if (rcal->lnext == NULL) {
        if (lcal->lnext != NULL) return false;
      } else {
        if (lcal->lnext == NULL) return false;
        bool fg = judge_expr_equalto_expr(lcal->lnext, rcal->lnext);
        if (fg == false) return false;
      }
      if (rcal->rnext == NULL) {
        if (lcal->rnext != NULL) return false;
      } else {
        if (lcal->rnext == NULL) return false;
        bool fg = judge_expr_equalto_expr(lcal->rnext, rcal->rnext);
        if (fg == false) return false;
      }
      return true;
    } break;
    case t_expr_func: {
      if (rnode->type != t_expr_func) return false;
      Expr_func *frnode = (Expr_func *)rnode;
      Expr_func *flnode = (Expr_func *)lnode;
      bool flag = true;
      if (frnode->args == NULL) {
        if (flnode->args != NULL) return false;
      } else {
        if (flnode->args == NULL) return false;
        flag = judge_expr_equalto_expr(frnode->args, flnode->args);
        if (flag == false) return false;
      }
      if (frnode->funname == NULL) {
        if (flnode->funname != NULL) return false;
      } else {
        if (flnode->funname == NULL) return false;
        if (strcmp(flnode->funname, frnode->funname) != 0) return false;
      }
      if (frnode->next == NULL) {
        if (flnode->next != NULL) return false;
      } else {
        if (flnode->next == NULL) return false;
        flag = judge_expr_equalto_expr(flnode->next, frnode->next);
        if (flag == false) return false;
      }
      if (frnode->parameter1 == NULL) {
        if (flnode->parameter1 != NULL) return false;
      } else {
        if (flnode->parameter1 == NULL) return false;
        flag = judge_expr_equalto_expr(flnode->parameter1, frnode->parameter1);
        if (flag == false) return false;
      }
      if (frnode->parameter2 == NULL) {
        if (flnode->parameter2 != NULL) return false;
      } else {
        if (flnode->parameter2 == NULL) return false;
        flag = judge_expr_equalto_expr(flnode->parameter2, frnode->parameter2);
        if (flag == false) return false;
      }
      if (frnode->str == NULL) {
        if (flnode->str != NULL) return false;
      } else {
        if (flnode->str == NULL) return false;
        if (strcmp(frnode->str, flnode->str) != 0) return false;
      }
      return true;
    } break;
    default: {}
  }
  return true;
}
bool selectlist_has_agg(Node *sltree,
                        Node *fnode)  // now just support single item
{
  int result = 0;
  for (Node *p = sltree; p != NULL;) {
    Select_list *selectlist = (Select_list *)p;
    Select_expr *sexpr = (Select_expr *)selectlist->args;
    Node *node = (Node *)sexpr->colname;  // not only column is possible
    result = judge_expr_equalto_expr(node, fnode);
    if (result) return true;
    p = selectlist->next;
  }
  return result;
}
/*
 * TODO
 * the basis expr should in dataflow
 */
bool having_analysis(Query_stmt *qstmt, Node *cur, vector<Node *> &rtable,
                     bool allflag) {
  if (cur == NULL) return true;
  switch (cur->type) {
    case t_name: {
      Columns *col = (Columns *)cur;
      char *astablename;
      /* first to find the table
       * next to find the talbe.column is in selectlist?
       */
      int result = table_has_column(col->parameter2, rtable, astablename);
      if (result == 1) {
        col->parameter1 = astablename;
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());
        col->type = t_name_name;
        if (allflag) return true;
        int tres = selectlist_has_column(col->parameter2, qstmt->select_list,
                                         astablename);
        if (tres == 0) {
          SQLParse_elog(
              "havingcondition %s can't find in table.column and selectlist ",
              col->parameter2);
          return false;
        } else if (tres == 1) {
          return true;
        } else {
          SQLParse_elog("havingcondition %s in selectlist is ambiguous",
                        col->parameter2);
          return false;
        }
      } else  // maybe alais,so in selectlist
      {
        int tres = selectlist_has_column(col->parameter2, qstmt->select_list,
                                         astablename);
        if (tres == 0) {
          SQLParse_elog(
              "havingcondition %s can't find in tables and selectlist ",
              col->parameter2);
          return false;
        } else if (tres == 1) {
          return true;
        } else {
          SQLParse_elog("havingcondition %s in selectlist is ambiguous",
                        col->parameter2);
          return false;
        }
      }
    } break;
    case t_name_name: {
      Columns *col = (Columns *)cur;
      char *astablename;
      stringstream ss;
      ss << string(col->parameter1).c_str() << "."
         << string(col->parameter2).c_str();
      col->parameter2 = (char *)malloc(ss.str().length() + 1);
      strcpy(col->parameter2, ss.str().c_str());
      if (allflag) return true;
      int tres = selectlist_has_column(col->parameter2, qstmt->select_list,
                                       astablename);
      if (tres == 0) {
        SQLParse_elog(
            "havingcondition %s can't find in table.column and selectlist ",
            col->parameter2);
        return false;
      } else if (tres == 1) {
        return true;
      } else {
        SQLParse_elog("havingcondition %s in selectlist is ambiguous",
                      col->parameter2);
        return false;
      }
    } break;
    case t_expr_cal: {
      Expr_cal *node = (Expr_cal *)cur;
      if (node->lnext == NULL && node->rnext == NULL) return false;
      bool flag = true;
      if (node->lnext != NULL)
        flag = having_analysis(qstmt, node->lnext, rtable, allflag);
      if (node->rnext != NULL)
        flag = flag && having_analysis(qstmt, node->rnext, rtable, allflag);
      return flag;

    } break;
    case t_expr_func: {
      Expr_func *node = (Expr_func *)cur;
      bool flag = true;
      // aggregation function shouldn't occur in where
      if ((strcmp(node->funname, "FCOUNTALL") == 0 ||
           strcmp(node->funname, "FCOUNT") == 0 ||
           strcmp(node->funname, "FSUM") == 0 ||
           strcmp(node->funname, "FMIN") == 0 ||
           strcmp(node->funname, "FMAX") == 0 ||
           strcmp(node->funname, "FAVG") == 0)) {
        allflag = 1;
        if (node->args != NULL)
          flag = flag && having_analysis(qstmt, node->args, rtable, allflag);
        if (node->parameter1 != NULL && flag)
          flag =
              flag && having_analysis(qstmt, node->parameter1, rtable, allflag);
        if (node->parameter2 != NULL && flag)
          flag =
              flag && having_analysis(qstmt, node->parameter2, rtable, allflag);
        flag = flag && selectlist_has_agg(qstmt->select_list, cur);
        if (flag == 0) {
          SQLParse_elog(
              "the aggregation function %s of having doesn't exist in "
              "selectlist!\n",
              node->funname);
        }
      } else {
        if (node->args != NULL)
          flag = flag && having_analysis(qstmt, node->args, rtable, allflag);
        if (node->parameter1 != NULL && flag)
          flag =
              flag && having_analysis(qstmt, node->parameter1, rtable, allflag);
        if (node->parameter2 != NULL && flag)
          flag =
              flag && having_analysis(qstmt, node->parameter2, rtable, allflag);
      }
      return flag;
    }
    case t_expr_list: {
      Expr_list *node = (Expr_list *)cur;
      bool flag = true;
      if (node->data != NULL) {
        flag = having_analysis(qstmt, node->data, rtable, allflag);
        if (flag == false) return false;
      } else {
        return false;
      }
      if (node->next != NULL) {
        return having_analysis(qstmt, node->next, rtable, allflag);
      }
    } break;
    default: {}
  }
  return true;
}
bool orderby_analysis(Node *node, vector<Node *> rtable,
                      Query_stmt *qstmt)  // like the solving of groupby
{
  if (node == NULL) return true;
  switch (node->type) {
    case t_name: {
      Columns *col = (Columns *)(node);
      char *astablename;
      int result = table_has_column(col->parameter2, rtable, astablename);
      if (result == 1) {
        col->parameter1 = astablename;
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());
        //					char * strtmp=(char
        //*)malloc(strlen(col->parameter1)+strlen(col->parameter2)+10);
        //					strcpy(strtmp,col->parameter1);
        //					strcat(strtmp,".");
        //					strcat(strtmp,col->parameter2);
        //					col->parameter2=strtmp;
        col->type = t_name_name;
      } else if (result ==
                 0)  // if not in fromlist,then to search in select_list
      {
        int result = selectlist_has_column(col->parameter2, qstmt->select_list,
                                           astablename);
        if (result == 0) {
          SQLParse_elog("orderbylist", col->parameter2,
                        "can't find in tables and selectlist ");
          return false;
        } else if (result == 1) {
          if (astablename == NULL) astablename = col->parameter2;
          col->parameter1 = astablename;
          col->type = t_name_name;
        } else {
          SQLParse_elog("orderbylist", col->parameter2,
                        "in selectlist is ambiguous");
          return false;
        }
      } else {
        SQLParse_elog("orderbylist", col->parameter2,
                      "in fromlist is ambiguous");
        return false;
      }
    } break;
    case t_name_name: {
      Columns *col = (Columns *)(node);
      char *tablename;
      Node *subnode;
      int fg =
          fromlist_has_astablename(col->parameter1, rtable, tablename, subnode);
      if (fg == 0) {
        SQLParse_elog("orderbylist %s.%s  can't find", col->parameter1,
                      col->parameter2);
        return false;
      } else if (fg == 1) {
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());

        if (Environment::getInstance()->getCatalog()->isAttributeExist(
                tablename, col->parameter2) == 0) {
          SQLParse_elog("orderbylist %s %s can't find ", col->parameter1,
                        col->parameter2);
          return false;
        }
      } else if (fg == 2) {
        int fg = subquery_has_column(col->parameter2, subnode);
        if (fg > 1 || fg == 0) return false;
        stringstream ss;
        ss << string(col->parameter1).c_str() << "."
           << string(col->parameter2).c_str();
        col->parameter2 = (char *)malloc(ss.str().length() + 1);
        strcpy(col->parameter2, ss.str().c_str());
        return true;
      }
    } break;
    case t_expr_func: {
      Expr_func *func = (Expr_func *)(node);
      if (func->parameter1 != NULL) {
        return orderby_analysis(func->parameter1, rtable, qstmt);
      } else if (func->args != NULL) {
        return orderby_analysis(func->args, rtable, qstmt);
      } else {
        SQLParse_elog("orderbylist func has errors\n");
      }
    } break;
    case t_expr_cal: {
      Expr_cal *ecal = (Expr_cal *)node;
      bool flag = orderby_analysis(ecal->rnext, rtable, qstmt);
      if (flag == false) return false;
      return orderby_analysis(ecal->lnext, rtable, qstmt);
    } break;
    case t_orderby_list: {
      Orderby_list *olist = (Orderby_list *)node;
      return orderby_analysis(olist->next, rtable, qstmt);
    } break;
    case t_groupby_expr: {
      Groupby_expr *gexpr = (Groupby_expr *)node;
      bool flag = orderby_analysis(gexpr->args, rtable, qstmt);
      if (flag == false) return false;
      return orderby_analysis(gexpr->next, rtable, qstmt);
    } break;
    default: {
      SQLParse_elog(
          "orderbylist there is other type in orderby list not the colname ");
      return false;
    }
  }
  return true;
}

bool semantic_analysis(Node *parsetree, bool issubquery) {
  if (parsetree == NULL) {
    SQLParse_elog("parsetree is null");
  }
  switch (parsetree->type) {
    case t_query_stmt: {
      Query_stmt *qstmt = (Query_stmt *)parsetree;
      vector<Node *> rtable;
      bool flag = true;
      if (qstmt->from_list != NULL) {
        flag = fromlist_analysis(qstmt, parsetree, rtable);
        //		assert(flag);
        if (flag == false) return false;
        if (issubquery == true && rtable.size() > 1) {
          flag = fromlist_table_is_unique(rtable);
          assert(flag);
          if (flag == false) return false;
        }
      }
      if (qstmt->select_list != NULL) {
        flag = selectlist_analysis(qstmt, rtable);
        if (flag == false) return false;
      }
      if (qstmt->where_list != NULL) {
        flag = wherecondition_analysis(
            qstmt, ((Where_list *)qstmt->where_list)->next, rtable);
        if (flag == false) return false;
      }
      if (qstmt->groupby_list != NULL) {
        flag = groupby_analysis(
            qstmt, ((Groupby_list *)qstmt->groupby_list)->next, rtable);
        if (flag == false) return false;
      }
      if (qstmt->having_list !=
          NULL)  // the item in having_list must be in select_list also
      {
        flag = having_analysis(qstmt, ((Having_list *)qstmt->having_list)->next,
                               rtable, 0);
        if (flag == false) return false;
      }
      if (qstmt->orderby_list != NULL) {
        flag = orderby_analysis(((Orderby_list *)qstmt->orderby_list)->next,
                                rtable, qstmt);
        if (flag == false) return false;
      }
    } break;
    default: {};
  }
  return true;
}
