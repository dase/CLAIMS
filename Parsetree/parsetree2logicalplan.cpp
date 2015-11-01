#ifndef __PARSERTREE__
#define __PARSERTREE__
#include <cstdio>

#include <map>
#include <new>
#include <set>
#include <string>
#include <vector>
#include <string.h>

#include "sql_node_struct.h"
#include "../Environment.h"

#include "../Catalog/Attribute.h"
#include "../Catalog/Catalog.h"
#include "../Catalog/table.h"

#include "../common/Comparator.h"
#include "../common/ExpressionItem.h"
#include "../common/Logging.h"
#include "../common/AttributeComparator.h"
#include "../common/Logging.h"
#include "../common/AttributeComparator.h"
#include "../common/TypePromotionMap.h"

#include "../common/Expression/initquery.h"
#include "../common/Expression/qnode.h"

#include "../logical_operator/logical_operator.h"

#include "../common/Logging.h"
#include "../common/AttributeComparator.h"
#include <string.h>
#include "../common/TypePromotionMap.h"
#include "../common/Expression/initquery.h"
#include "../common/Expression/qnode.h"
#include <assert.h>

#include "../logical_operator/logical_aggregation.h"
#include "../logical_operator/logical_cross_join.h"
#include "../logical_operator/logical_equal_join.h"
#include "../logical_operator/logical_filter.h"
#include "../logical_operator/logical_project.h"
#include "../logical_operator/logical_scan.h"
#include "../logical_operator/logical_sort.h"
#include "../physical_operator/physical_aggregation.h"

using namespace claims::logical_operator;
using claims::physical_operator::PhysicalAggregation;
static LogicalOperator *parsetree2logicalplan(Node *parsetree);
static void get_a_expression_item(vector<ExpressionItem> &expr, Node *node,
                                  LogicalOperator *input);
// because Filter::Condition no longer exists
/*
static void getfiltercondition(Node * wcexpr,Filter::Condition
&filter_condition,char * tablename,bool &hasin,LogicalOperator* loperator){
        SQLParse_log("getfiltercondition   ");
        //filter_condition.add(catalog->getTable(node->tablename)->getAttribute(4),AttributeComparator::EQ,&order_type_);
//	cout<<"wcexpr->type  "<<wcexpr->type<<endl;
        switch(wcexpr->type)
        {
                case t_expr_cal:
                {
                        Expr_cal * node=(Expr_cal *)wcexpr;
                        if(strcmp(node->sign,"+")==0)
                        {

                        }
                        else if(strcmp(node->sign,"-")==0)
                        {

                        }
                        else if(strcmp(node->sign,"*")==0)
                        {

                        }
                        else if(strcmp(node->sign,"/")==0)
                        {

                        }
                        else if(strcmp(node->sign,"MOD")==0)
                        {

                        }
                        else if(strcmp(node->sign,"INS")==0)
                        {
                                hasin=true;
                        }
                        else if(strcmp(node->sign,"ANDOP")==0)
                        {

                        }
                        else if(strcmp(node->sign,"OR")==0)
                        {

                        }
                        else if(strcmp(node->sign,"XOR")==0)
                        {

                        }
                        else if(strcmp(node->sign,"CMP")==0)
                        {
                                char * attribute;
                                switch((node->lnext)->type)//获得左边的属性名
                                {
                                        case t_name:
                                        {
                                                Expr *expr=(Expr
*)(node->lnext);
                                                attribute=expr->data;
                                        }break;
                                        case t_name_name:
                                        {
                                                Columns *col=(Columns
*)(node->lnext);
                                                attribute=col->parameter2;
                                        }break;
                                        default:
                                        {

                                        }
                                };

                                switch(node->cmp)
                                {
                                        case 1://"<"
                                        {
                                                Expr *expr=(Expr
*)(node->rnext);
                                                char * temp=expr->data;
                                                filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::L,string(temp));

                                        }break;
                                        case 2://">"
                                        {
                                                Expr *expr=(Expr
*)(node->rnext);
                                                char * temp=expr->data;
                                                filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::G,string(temp));
                                        }break;
                                        case 3://"<>"
                                        {
                                                Expr *expr=(Expr
*)(node->rnext);
                                                char * temp=expr->data;
                                                filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::NEQ,string(temp));
                                        }break;
                                        case 4://"="
                                        {
                                                Expr *expr=(Expr
*)(node->rnext);
                                                char * temp=expr->data;
                                                filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::EQ,string(temp));
                                        }break;
                                        case 5://"<="
                                        {
                                                Expr *expr=(Expr
*)(node->rnext);
                                                char * temp=expr->data;
                                                filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::LEQ,string(temp));
                                        }break;
                                        case 6://">="
                                        {
                                                Expr *expr=(Expr
*)(node->rnext);
                                                char * temp=expr->data;
                                                filter_condition.add(loperator->getDataflow().getAttribute(attribute),AttributeComparator::GEQ,string(temp));
                                        }break;

                                }
                        }
                }break;
                default:
                {
                        SQLParse_elog("getfiltercondition type error");
                }
        }
}*/
static int getjoinpairlist(Node *wcexpr,
                           vector<LogicalEqualJoin::JoinPair> &join_pair_list,
                           LogicalOperator *filter_1,
                           LogicalOperator *filter_2) {
  switch (wcexpr->type) {
    case t_expr_cal:  //目前只支持equaljoin
    {
      Expr_cal *node = (Expr_cal *)wcexpr;
      if (node->lnext->type != t_name_name ||
          node->rnext->type != t_name_name) {
        return 0;
      }
      if (strcmp(node->sign, "CMP") == 0) {
        switch (node->cmp) {
          case 1:  //"<"
          {
          } break;
          case 2:  //">"
          {
          } break;
          case 3:  //"<>"
          {
          } break;
          case 4:  //"="
          {
            Columns *lnode = (Columns *)(node->lnext);
            Columns *rnode = (Columns *)(node->rnext);
            //						join_pair_list.push_back(EqualJoin::JoinPair(filter_1->getDataflow().getAttribute(lnode->parameter2),filter_2->getDataflow().getAttribute(rnode->parameter2)));
            //	printf("left %s.%s   right
            //%s.%s\n",lnode->parameter1,lnode->parameter2,rnode->parameter1,rnode->parameter2);
            // TODO just select PART.row_id,LINEITEM.row_id from LINEITEM,PART
            // where LINEITEM.row_id=PART.row_id and PART.row_id<20;

            Attribute la =
                filter_1->GetPlanContext().GetAttribute(lnode->parameter2);
            Attribute ra =
                filter_2->GetPlanContext().GetAttribute(rnode->parameter2);

            if (strcmp(la.attrName.c_str(), "NULL") != 0 &&
                strcmp(ra.attrName.c_str(), "NULL") != 0) {
              join_pair_list.push_back(LogicalEqualJoin::JoinPair(la, ra));
              return 1;
            }
            la = filter_1->GetPlanContext().GetAttribute(rnode->parameter2);
            ra = filter_2->GetPlanContext().GetAttribute(lnode->parameter2);
            if (strcmp(la.attrName.c_str(), "NULL") != 0 &&
                strcmp(ra.attrName.c_str(), "NULL") != 0) {
              join_pair_list.push_back(LogicalEqualJoin::JoinPair(la, ra));
              return 1;
            } else {
              assert(false);
              SQLParse_elog("can't find join attrbutes");
              return 0;
            }
          } break;
          case 5:  //"<="
          {
          } break;
          case 6:  //">="
          {
          } break;
          default: {}
        }
      } else {
      }

    } break;
    default: { return 0; }
  }
  return 0;
}
static LogicalOperator *solve_insubquery(Node *exprnode,
                                         LogicalOperator *input) {
  switch (exprnode->type) {
    case t_expr_cal: {
      Expr_cal *node = (Expr_cal *)exprnode;
      if (strcmp(node->sign, "INS") == 0) {
        if (node->rnext->type == t_query_stmt) {
          LogicalOperator *sublogicalplan = parsetree2logicalplan(
              node->rnext);  // 1.获得原子查询的logicalplan
          Query_stmt *subquery = (Query_stmt *)(node->rnext);
          vector<Attribute> group_by_attributes;
          vector<Attribute> aggregation_attributes;
          for (Node *p = subquery->select_list;
               p != NULL;)  // 2.1获得groupby的属性
          {
            Select_list *selectlist = (Select_list *)p;
            Select_expr *sexpr = (Select_expr *)selectlist->args;
            group_by_attributes.push_back(
                sublogicalplan->GetPlanContext().GetAttribute(
                    sexpr->ascolname));  ///????
            p = selectlist->next;
          }  // 2.2在1中的logicalplan上做groupby
          LogicalOperator *aggrection_sublogicalplan = new LogicalAggregation(
              group_by_attributes, std::vector<Attribute>(),
              std::vector<PhysicalAggregation::State::Aggregation>(),
              sublogicalplan);
          vector<LogicalEqualJoin::JoinPair> join_pair_list;
          Node *lp, *sp;
          for (lp = node->lnext, sp = ((Query_stmt *)node->rnext)->select_list;
               lp != NULL;)  // 3.1获得equaljoin的左右属性
          {
            Expr_list *lpexpr = (Expr_list *)lp;
            Columns *lcol = (Columns *)lpexpr->data;
            Select_list *spexpr = (Select_list *)sp;
            Columns *rcol = (Columns *)spexpr->args;
            join_pair_list.push_back(LogicalEqualJoin::JoinPair(
                input->GetPlanContext().GetAttribute(lcol->parameter2),
                sublogicalplan->GetPlanContext().GetAttribute(
                    rcol->parameter2)));
            lp = lpexpr->next;
            sp = spexpr->next;
          }
          LogicalOperator *join_logicalplan = new LogicalEqualJoin(
              join_pair_list, input, aggrection_sublogicalplan);
          return join_logicalplan;
        }
      }
    } break;
    default: {}
  }

  return NULL;
}
static LogicalOperator *where_from2logicalplan(
    Node *
        parsetree)  //实现where_from_parsetree(即将where转换到from_list后的)到logicalplan的转换
{
  if (parsetree == NULL) {
    return NULL;
  }
  switch (parsetree->type) {
    case t_table:  // table节点获得scan 和在该节点上condition的filter
    {
      Table *node = (Table *)parsetree;
      LogicalOperator *tablescan;
      if (node->issubquery == 0) {
        tablescan = new LogicalScan(Environment::getInstance()
                                        ->getCatalog()
                                        ->getTable(std::string(node->tablename))
                                        ->getProjectoin(0));  // todo

        //				// change for selecting best projection
        //				tablescan=new
        //LogicalScan(Environment::getInstance()->getCatalog()->getTable(std::string(node->tablename))->get_table_id());//
      } else  // need to modify the output_schema_attrname from the subquery to
              // the form of subquery's alias.attrname
      {
        tablescan = parsetree2logicalplan(node->subquery);
        vector<Attribute> output_attribute =
            tablescan->GetPlanContext().attribute_list_;
        vector<QNode *> exprTree;
        string subquery_alias = string(node->astablename);
        for (int i = 0; i < output_attribute.size(); i++) {
          string attrname = output_attribute[i].attrName;
          int pos;
          for (pos = 0; pos < attrname.size() && attrname[pos] != '.'; pos++)
            ;
          if (pos < attrname.size()) {
            attrname = attrname.substr(pos + 1, attrname.size() - pos - 1);
          }
          exprTree.push_back(
              new QColcumns(subquery_alias.c_str(), attrname.c_str(),
                            output_attribute[i].attrType->type,
                            string(subquery_alias + "." + attrname).c_str()));
          //					cout<<"The "<<i<<"
          //"<<subquery_alias+"."+attrname<<endl;
        }
        tablescan = new LogicalProject(tablescan, exprTree);
      }
      Expr_list_header *whcdn = (Expr_list_header *)node->whcdn;
      if (whcdn->header != NULL) {
        assert(tablescan != NULL);
        Node *p;
        bool hasin = false;
        vector<QNode *> v_qual;
        for (p = whcdn->header; p != NULL; p = ((Expr_list *)p)->next) {
          QNode *qual =
              transformqual((Node *)((Expr_list *)p)->data, tablescan);
          v_qual.push_back(qual);
        }
        LogicalOperator *filter = new LogicalFilter(tablescan, v_qual);
        if (hasin == true) {
          for (p = whcdn->header; p != NULL; p = ((Expr_list *)p)->next) {
            filter = solve_insubquery(((Expr_list *)p)->data, filter);
          }
        }
        return filter;
      } else {
        return tablescan;
      }

    } break;
    case t_from_list:  //由from_list递归进入args/next,并获得在其上的equaljoin
    {
      From_list *node = (From_list *)parsetree;
      LogicalOperator *filter_1 = where_from2logicalplan(node->args);
      LogicalOperator *filter_2 =
          where_from2logicalplan(node->next);  // maybe NULL
      LogicalOperator *lopfrom = NULL;
      if (filter_2 == NULL)  // a join b on c where a.a>0;
      {
        Expr_list_header *whcdn = (Expr_list_header *)node->whcdn;
        if (whcdn->header != NULL) {
          Node *p;
          vector<QNode *> v_qual;
          for (
              p = whcdn->header; p != NULL;
              p = ((Expr_list *)
                   p)->next)  //应该根据getdataflow的信息确定joinpair跟filter1/2是否一致
          {
            QNode *qual =
                transformqual((Node *)((Expr_list *)p)->data, filter_1);
            v_qual.push_back(qual);
          }
          if (v_qual.size() > 0) {
            lopfrom = new LogicalFilter(filter_1, v_qual);
          } else {
            lopfrom = filter_1;
          }
          return lopfrom;
        } else {
          return filter_1;
        }
      }
      Expr_list_header *whcdn = (Expr_list_header *)node->whcdn;
      if (whcdn->header != NULL) {
        vector<LogicalEqualJoin::JoinPair> join_pair_list;
        Node *p;
        vector<QNode *> v_qual;
        vector<Node *> raw_qual;
        for (p = whcdn->header; p != NULL; p = ((Expr_list *)p)->next) {
          int fg = getjoinpairlist((Node *)((Expr_list *)p)->data,
                                   join_pair_list, filter_1, filter_2);
          if (fg == 0)  // get raw qualification from whcdn
          {
            raw_qual.push_back((Node *)((Expr_list *)p)->data);
          }
        }
        if (join_pair_list.size() > 0) {
          lopfrom = new LogicalEqualJoin(join_pair_list, filter_1, filter_2);
        } else  // other join
        {
          lopfrom = new LogicalCrossJoin(filter_1, filter_2);
        }
        for (int i = 0; i < raw_qual.size(); i++) {
          v_qual.push_back(transformqual(raw_qual[i], lopfrom));
        }
        if (v_qual.size() > 0) {
          lopfrom = new LogicalFilter(lopfrom, v_qual);
        }
        return lopfrom;
      } else  // other to crossjoin
      {
        lopfrom = new LogicalCrossJoin(filter_1, filter_2);
        return lopfrom;
      }

    } break;
    case t_join: {
      Join *node = (Join *)parsetree;
      LogicalOperator *filter_1 = where_from2logicalplan(node->lnext);
      LogicalOperator *filter_2 = where_from2logicalplan(node->rnext);
      if (node->condition != NULL) {
        vector<LogicalEqualJoin::JoinPair> join_pair_list;
        Node *p;
        vector<QNode *> v_qual;
        vector<Node *> raw_qual;
        for (p = node->condition; p != NULL; p = ((Expr_list *)p)->next) {
          int fg = getjoinpairlist((Node *)((Expr_list *)p)->data,
                                   join_pair_list, filter_1, filter_2);
          if (fg == 0)  // get raw qualification from whcdn
          {
            raw_qual.push_back((Node *)((Expr_list *)p)->data);
          }
        }

        LogicalOperator *join;
        if (join_pair_list.size() > 0) {
          join = new LogicalEqualJoin(join_pair_list, filter_1, filter_2);
        } else  // other join
        {
          join = new LogicalCrossJoin(filter_1, filter_2);
        }
        for (int i = 0; i < raw_qual.size(); i++) {
          v_qual.push_back(transformqual(raw_qual[i], join));
        }
        if (v_qual.size() > 0) {
          join = new LogicalFilter(join, v_qual);
        }
        return join;
      } else  // other to crossjoin
      {
        LogicalOperator *join = new LogicalCrossJoin(filter_1, filter_2);
        return join;
      }

    } break;
    default: {
      SQLParse_elog("parsetree2logicalplan type error");
      return NULL;
    }
  }
  return NULL;
}

static char *get_expr_str(Node *node) {
  char *str = "";
  switch (node->type) {
    case t_expr_cal: {
      Expr_cal *calnode = (Expr_cal *)node;
      str = calnode->str;
    } break;
    case t_expr_func: {
      Expr_func *funcnode = (Expr_func *)node;
      str = funcnode->str;
    } break;
    case t_name:
    case t_name_name: {
      Columns *col = (Columns *)node;
      str = col->parameter2;
    } break;
    case t_stringval: {
      Expr *exprval = (Expr *)node;
    } break;
    case t_intnum: {
      Expr *exprval = (Expr *)node;
    } break;
    case t_approxnum: {
      Expr *exprval = (Expr *)node;

    } break;
    case t_bool: {
    } break;
    default: {}
  }
  return str;
}

static void get_aggregation_args(
    Node *selectlist, vector<Attribute> &aggregation_attributes,
    vector<PhysicalAggregation::State::Aggregation> &aggregation_function,
    LogicalOperator *input) {
  if (selectlist == NULL) {
    //	puts("get_aggregation_args is null");
    return;
  }
  switch (selectlist->type) {
    case t_select_list: {
      Select_list *selectnode = (Select_list *)selectlist;
      get_aggregation_args(selectnode->args, aggregation_attributes,
                           aggregation_function, input);
      get_aggregation_args(selectnode->next, aggregation_attributes,
                           aggregation_function, input);
    } break;
    case t_select_expr: {
      Select_expr *selectnode = (Select_expr *)selectlist;
      get_aggregation_args(selectnode->colname, aggregation_attributes,
                           aggregation_function, input);

    } break;
    case t_expr_func: {
      Expr_func *funcnode = (Expr_func *)selectlist;
      if (strcmp(funcnode->funname, "FCOUNTALL") == 0) {
        aggregation_function.push_back(PhysicalAggregation::State::kCount);
        aggregation_attributes.push_back(Attribute(ATTRIBUTE_ANY));
      } else if (strcmp(funcnode->funname, "FCOUNT") == 0) {
        aggregation_function.push_back(PhysicalAggregation::State::kCount);
        if (input == NULL) {
          Columns *funccol = (Columns *)funcnode->parameter1;
          aggregation_attributes.push_back(
              Environment::getInstance()
                  ->getCatalog()
                  ->getTable(funccol->parameter1)
                  ->getAttribute(funccol->parameter2));
        } else {
          aggregation_attributes.push_back(input->GetPlanContext().GetAttribute(
              get_expr_str(funcnode->parameter1)));
        }
      } else if (strcmp(funcnode->funname, "FSUM") == 0) {
        aggregation_function.push_back(PhysicalAggregation::State::kSum);
        if (input == NULL) {
          Columns *funccol = (Columns *)funcnode->parameter1;
          aggregation_attributes.push_back(
              Environment::getInstance()
                  ->getCatalog()
                  ->getTable(funccol->parameter1)
                  ->getAttribute(funccol->parameter2));
        } else {
          aggregation_attributes.push_back(input->GetPlanContext().GetAttribute(
              get_expr_str(funcnode->parameter1)));
        }
      } else if (strcmp(funcnode->funname, "FMIN") == 0) {
        aggregation_function.push_back(PhysicalAggregation::State::kMin);
        if (input == NULL) {
          Columns *funccol = (Columns *)funcnode->parameter1;
          aggregation_attributes.push_back(
              Environment::getInstance()
                  ->getCatalog()
                  ->getTable(funccol->parameter1)
                  ->getAttribute(funccol->parameter2));
        } else {
          aggregation_attributes.push_back(input->GetPlanContext().GetAttribute(
              get_expr_str(funcnode->parameter1)));
        }
      } else if (strcmp(funcnode->funname, "FMAX") == 0) {
        aggregation_function.push_back(PhysicalAggregation::State::kMax);
        if (input == NULL) {
          Columns *funccol = (Columns *)funcnode->parameter1;
          aggregation_attributes.push_back(
              Environment::getInstance()
                  ->getCatalog()
                  ->getTable(funccol->parameter1)
                  ->getAttribute(funccol->parameter2));
        } else {
          aggregation_attributes.push_back(input->GetPlanContext().GetAttribute(
              get_expr_str(funcnode->parameter1)));
        }
      } else if (strcmp(funcnode->funname, "FAVG") == 0) {
        aggregation_function.push_back(PhysicalAggregation::State::kAvg);
        if (input == NULL) {
          Columns *funccol = (Columns *)funcnode->parameter1;
          aggregation_attributes.push_back(
              Environment::getInstance()
                  ->getCatalog()
                  ->getTable(funccol->parameter1)
                  ->getAttribute(funccol->parameter2));
        } else {
          aggregation_attributes.push_back(input->GetPlanContext().GetAttribute(
              get_expr_str(funcnode->parameter1)));
        }
      } else {
        if (strcmp(funcnode->funname, "FSUBSTRING0") == 0 ||
            strcmp(funcnode->funname, "FSUBSTRING1") == 0) {
          get_aggregation_args(funcnode->args, aggregation_attributes,
                               aggregation_function, input);
        } else {
          get_aggregation_args(funcnode->parameter1, aggregation_attributes,
                               aggregation_function, input);
        }
      }

    } break;
    case t_expr_cal: {
      Expr_cal *calnode = (Expr_cal *)selectlist;
      get_aggregation_args(calnode->lnext, aggregation_attributes,
                           aggregation_function, input);
      get_aggregation_args(calnode->rnext, aggregation_attributes,
                           aggregation_function, input);

    } break;
    case t_name:
    case t_name_name: {
      //			Columns *colnode=(Columns *)selectlist;
      //			if(input==NULL)
      //			{
      //				aggregation_attributes.push_back(Environment::getInstance()->getCatalog()->getTable(colnode->parameter1)->getAttribute(colnode->parameter2));
      //			}
      //			else
      //			{
      //				aggregation_attributes.push_back(input->getDataflow().getAttribute(get_expr_str(selectlist)));
      //
      //			}
    } break;
    case t_stringval: {
    } break;
    case t_intnum: {
    } break;
    case t_approxnum: {
    } break;
    case t_bool: {
    } break;
    default: {}
  }
}

static void get_group_by_attributes(Node *groupby_node,
                                    vector<Attribute> &group_by_attributes,
                                    LogicalOperator *input) {
  if (groupby_node == NULL) {
    SQLParse_log("groupby_node is null");
    return;
  }
  switch (groupby_node->type) {
    case t_groupby_list: {
      Groupby_list *node = (Groupby_list *)groupby_node;
      get_group_by_attributes(node->next, group_by_attributes, input);
    } break;
    case t_groupby_expr: {
      Groupby_expr *node = (Groupby_expr *)groupby_node;
      get_group_by_attributes(node->args, group_by_attributes, input);
      get_group_by_attributes(node->next, group_by_attributes, input);
    } break;
    case t_name:
    case t_name_name:
    case t_column: {
      group_by_attributes.push_back(
          input->GetPlanContext().GetAttribute(get_expr_str(groupby_node)));
    } break;
    case t_expr_cal: {
      group_by_attributes.push_back(
          input->GetPlanContext().GetAttribute(get_expr_str(groupby_node)));
    } break;
    case t_expr_func: {
      group_by_attributes.push_back(
          input->GetPlanContext().GetAttribute(get_expr_str(groupby_node)));
    } break;
    default: { SQLParse_elog("get_group_by_attributes case is null"); }
  }
}

/*
 * 只需要获得agg函数参数的expr并加入到allexpr
 */

static void recurse_get_item_in_expr(Node *node, vector<QNode *> &exprTree,
                                     LogicalOperator *input) {
  switch (node->type) {
    case t_expr_func: {
      Expr_func *funcnode = (Expr_func *)node;
      if (strcmp(funcnode->funname, "FCOUNT") == 0 ||
          strcmp(funcnode->funname, "FSUM") == 0 ||
          strcmp(funcnode->funname, "FAVG") == 0 ||
          strcmp(funcnode->funname, "FMIN") == 0 ||
          strcmp(funcnode->funname, "FMAX") == 0) {
        exprTree.push_back(transformqual(funcnode->parameter1, input));
      } else if (strcmp(funcnode->funname, "FCOUNTALL") == 0) {
      } else {
        if (strcmp(funcnode->funname, "FSUBSTRING0") == 0 ||
            strcmp(funcnode->funname, "FSUBSTRING1") == 0) {
          recurse_get_item_in_expr(funcnode->args, exprTree, input);
        } else {
          recurse_get_item_in_expr(funcnode->parameter1, exprTree, input);
        }
      }
    } break;
    case t_expr_cal: {
      Expr_cal *calnode = (Expr_cal *)node;
      if (calnode->lnext != 0)
        recurse_get_item_in_expr(calnode->lnext, exprTree, input);
      recurse_get_item_in_expr(calnode->rnext, exprTree, input);

    } break;
    case t_name:
    case t_name_name: {
    } break;
    case t_stringval: {
    } break;
    case t_intnum: {
    } break;
    case t_approxnum: {
    } break;
    case t_bool: {
    } break;
    default: {}
  }
}
static void ExprCanCancelProject1(Node *node, int flag, bool &ans) {
  switch (node->type) {
    case t_expr_func: {
      Expr_func *funcnode = (Expr_func *)node;
      if (strcmp(funcnode->funname, "FSUM") == 0 ||
          strcmp(funcnode->funname, "FAVG") == 0 ||
          strcmp(funcnode->funname, "FMIN") == 0 ||
          strcmp(funcnode->funname, "FMAX") == 0) {
        if (flag) {
          ans = 0;
          return;
        }
        ExprCanCancelProject1(funcnode->parameter1, 1, ans);
      } else if (strcmp(funcnode->funname, "FCOUNT") == 0 ||
                 strcmp(funcnode->funname, "FCOUNTALL") == 0) {
        return;
      } else {
        if (flag) {
          ans = 0;
          return;
        }
        if (strcmp(funcnode->funname, "FSUBSTRING0") == 0 ||
            strcmp(funcnode->funname, "FSUBSTRING1") == 0) {
          ExprCanCancelProject1(funcnode->args, flag, ans);
        } else {
          ExprCanCancelProject1(funcnode->parameter1, flag, ans);
        }
      }
    } break;
    case t_expr_cal: {
      if (flag) {
        ans = 0;
        return;
      }
      Expr_cal *calnode = (Expr_cal *)node;
      if (calnode->lnext != 0) ExprCanCancelProject1(calnode->lnext, flag, ans);
      if (calnode->rnext != 0) ExprCanCancelProject1(calnode->rnext, flag, ans);

    } break;
    case t_name:
    case t_name_name: {
    } break;
    case t_stringval: {
    } break;
    case t_intnum: {
    } break;
    case t_approxnum: {
    } break;
    case t_bool: {
    } break;
    default: {}
  }
}
static void ExprCanCancelProject2(Node *node, int flag, bool &ans) {
  switch (node->type) {
    case t_expr_func: {
      Expr_func *funcnode = (Expr_func *)node;
      if (strcmp(funcnode->funname, "FSUM") == 0 ||
          strcmp(funcnode->funname, "FAVG") == 0 ||
          strcmp(funcnode->funname, "FMIN") == 0 ||
          strcmp(funcnode->funname, "FMAX") == 0) {
      } else if (strcmp(funcnode->funname, "FCOUNT") == 0 ||
                 strcmp(funcnode->funname, "FCOUNTALL") == 0) {
      } else {
        ans = 0;
      }
    } break;
    case t_expr_cal: {
      ans = 0;
    } break;
    case t_name:
    case t_name_name: {
      ans = 0;
    } break;
    case t_stringval: {
    } break;
    case t_intnum: {
    } break;
    case t_approxnum: {
    } break;
    case t_bool: {
    } break;
    default: {}
  }
  return;
}
static bool CanCancelProject1(Node *node) {
  bool result = 1;
  for (Node *p = node; p != NULL && result;) {
    Select_list *selectlist = (Select_list *)p;
    Select_expr *sexpr = (Select_expr *)selectlist->args;
    if (selectlist->isall == -1) {
      ExprCanCancelProject1(sexpr->colname, 0, result);
    } else if (selectlist->isall == -2)  // count(*) 不能参与运算
    {
      SQLParse_log("this sql has count(*)");
    }
    p = selectlist->next;
  }
  return result;
}
static bool CanCancelProject2(Node *node) {
  bool result = 1;
  for (Node *p = node; p != NULL && result;) {
    Select_list *selectlist = (Select_list *)p;
    Select_expr *sexpr = (Select_expr *)selectlist->args;

    ExprCanCancelProject2(sexpr->colname, 0, result);

    p = selectlist->next;
  }
  return result;
}
static void get_all_selectlist_expression_item(Node *node,
                                               LogicalOperator *input,
                                               int proj_type,
                                               vector<QNode *> &exprTree) {
  for (Node *p = node; p != NULL;) {
    Select_list *selectlist = (Select_list *)p;
    Select_expr *sexpr = (Select_expr *)selectlist->args;
    if (proj_type == 0) {
      QNode *qnode = transformqual(sexpr->colname, input);
      if (sexpr->ascolname != NULL) {
        qnode->alias = string(sexpr->ascolname);
      }
      exprTree.push_back(qnode);
    } else if (proj_type == 1) {
      if (selectlist->isall == -1) {
        recurse_get_item_in_expr(sexpr->colname, exprTree, input);
      } else if (selectlist->isall == -2)  // count(*) 不能参与运算
      {
        SQLParse_log("this sql has count(*)");
      }
    } else if (proj_type == 2)  // the expression in select clause which dosen't
                                // contain aggregation function should be in
                                // group by clause as the same form.
    {
      QNode *qnode = transformqual(sexpr->colname, input);
      if (sexpr->ascolname != NULL) {
        qnode->alias = string(sexpr->ascolname);
      }
      exprTree.push_back(qnode);
    }
    p = selectlist->next;
  }
}
static void get_all_groupby_expression_item(Node *node, LogicalOperator *input,
                                            vector<QNode *> &exprTree) {
  for (Node *p = (Node *)(((Groupby_list *)node)->next); p != NULL;) {
    Groupby_expr *gbexpr = (Groupby_expr *)p;
    exprTree.push_back(transformqual(gbexpr->args, input));
    p = gbexpr->next;
  }
}

static void dfs_select_args(int flag, int &ans, Node *node) {
  switch (node->type) {
    case t_expr_func: {
      Expr_func *funcnode = (Expr_func *)node;
      if (strcmp(funcnode->funname, "FCOUNT") == 0 ||
          strcmp(funcnode->funname, "FSUM") == 0 ||
          strcmp(funcnode->funname, "FAVG") == 0 ||
          strcmp(funcnode->funname, "FMIN") == 0 ||
          strcmp(funcnode->funname, "FMAX") == 0) {
        if (flag == -2) {
          SQLParse_elog("dfs_select_args error!!!!");
        }
        if (flag == -1 && (ans & 4) == 0) {
          ans += 4;  // expr has agg
        }
        if ((ans & 1) == 0) {
          ans += 1;
        }
        dfs_select_args(-2, ans, funcnode->parameter1);
      } else if (strcmp(funcnode->funname, "FCOUNTALL") == 0) {
        if (flag == -2) {
          SQLParse_elog("dfs_select_args error!!!!");
        }
        if (flag == -1 && (ans & 4) == 0) {
          ans += 4;  // expr has agg
        }
        if ((ans & 8) == 0) {
          ans += 8;
        }
      } else if (strcmp(funcnode->funname, "CASE3") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->parameter1);
      } else if (strcmp(funcnode->funname, "CASE4") ==
                 0)  //目前只支持case when [expr then expr]* [else expr] end
      {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->parameter1);
      } else if (strcmp(funcnode->funname, "WHEN1") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->parameter1);
      } else if (strcmp(funcnode->funname, "WHEN2") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->parameter1);
      } else if (strcmp(funcnode->funname, "FSUBSTRING0") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->args);
      } else if (strcmp(funcnode->funname, "FSUBSTRING1") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->args);
      } else if (strcmp(funcnode->funname, "FTRIM0") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->parameter1);
      } else if (strcmp(funcnode->funname, "FTRIM1") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->parameter1);
      } else if (strcmp(funcnode->funname, "FTRIM2") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->parameter1);
      } else if (strcmp(funcnode->funname, "FTRIM3") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->parameter1);
      } else if (strcmp(funcnode->funname, "FUPPER") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->parameter1);
      } else if (strcmp(funcnode->funname, "FCAST") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->parameter1);
      } else if (strcmp(funcnode->funname, "FCOALESCE") == 0) {
        if (flag == -2 && (ans & 2 == 0)) {
          ans += 2;  // agg has expr
        }
        dfs_select_args(-1, ans, funcnode->parameter1);
      } else {
      }
    } break;
    case t_expr_cal: {
      Expr_cal *calnode = (Expr_cal *)node;
      if (flag == -2 && (ans & 2 == 0)) {
        ans += 2;  // agg has expr
      }
      if (calnode->lnext != 0) dfs_select_args(-1, ans, calnode->lnext);
      dfs_select_args(-1, ans, calnode->rnext);

    } break;
    case t_name:
    case t_name_name: {
    } break;
    case t_stringval: {
    } break;
    case t_intnum: {
    } break;
    case t_approxnum: {
    } break;
    case t_bool: {
    } break;
    default: {}
  }
  return;
}
static LogicalOperator *select_where_from2logicalplan(
    Node *parsetree, LogicalOperator *last_logicalplan, int proj_type) {
  vector<QNode *> exprTree;
  if (parsetree == NULL) {
    return NULL;
  } else {
    Query_stmt *node = (Query_stmt *)parsetree;
    if (proj_type == 0) {
      get_all_selectlist_expression_item(node->select_list, last_logicalplan, 0,
                                         exprTree);
    } else if (proj_type == 1) {
      if (node->groupby_list != NULL) {
        get_all_groupby_expression_item(node->groupby_list, last_logicalplan,
                                        exprTree);
      }
      get_all_selectlist_expression_item(node->select_list, last_logicalplan, 1,
                                         exprTree);
    } else if (proj_type == 2)  // the expression in select clause which dosen't
                                // contain aggregation function should be in
                                // group by clause as the same form.
    {
      for (Node *p = node->select_list; p != NULL;) {
        int ans = 0;
        Node *expr_node;
        Select_list *selectlist = (Select_list *)p;
        Select_expr *sexpr = (Select_expr *)selectlist->args;
        dfs_select_args(0, ans, sexpr->colname);
        if ((ans & 1) == 1 ||
            (ans & 8) ==
                8)  // if the expression has aggregation function,then skip it
        {
          expr_node = sexpr->colname;
        } else  // if the expression doesn't have aggregation function,the
                // expression must be in group_by_clause as the same form
        {
          int flag = 0;
          char *expr_name =
              get_expr_str(sexpr->colname);  // TODO need to judge whether the
                                             // expr_name is in group_by_list
          expr_node = newColumn(t_name_name, "", expr_name, NULL,
                                NULL);  // construct temporary node
        }
        QNode *qnode = transformqual(expr_node, last_logicalplan);
        if (sexpr->ascolname != NULL) {
          qnode->alias = string(sexpr->ascolname);
        }
        exprTree.push_back(qnode);
        p = selectlist->next;
      }
      //			get_all_selectlist_expression_item(node->select_list,last_logicalplan,2,exprTree);
    }
    LogicalOperator *proj = NULL;
    if (exprTree.size() > 0) {
      proj = new LogicalProject(last_logicalplan, exprTree);
    } else {
      SQLParse_log("allexpr.size=0");
      proj = last_logicalplan;
    }
    return proj;
  }
}
/*判断selectlist的expression中是否存在
 * 聚集函数agg中的参数为表达式expr eg: min(a+b)
 * 或者聚集函数agg包含在表达式中 eg:min(a)+max(b)
 */
static void judge_selectlist_agg_has_or_in_expr(Node *node, int &has_agg,
                                                int &agg_has_expr,
                                                int &agg_in_expr) {
  int ans = 0;
  for (Node *p = node; p != NULL;) {
    Select_list *selectlist = (Select_list *)p;
    Select_expr *sexpr = (Select_expr *)selectlist->args;
    ans = 0;
    dfs_select_args(0, ans, sexpr->colname);
    //		if((ans&1)==1) has_agg=1;
    //		if((ans&2)==1) agg_has_expr=1;
    //		if((ans&4)==1)
    //		{
    //			agg_in_expr=1;
    //			selectlist->isall=-1;
    //		}
    //		SQLParse_log("judge_selectlist_agg_has_or_in_expr ans=%d",ans);
    if ((ans & 1) == 1) {
      has_agg = 1;
      selectlist->isall = -1;
    }
    //		SQLParse_log("ans&8= %d ",ans&8);
    if ((ans & 8) == 8) {
      has_agg = 1;
      selectlist->isall = -2;
    }
    p = selectlist->next;
  }
}
/*
 *a||g==0 selectlist中所有表达式放入，记为proj0
 *a||g==1 groupbylist+agg函数中的参数，记为proj1
 *对于proj2
 *含有agg的参数用整体表示，并完成该表达式的计算；如果没有，则直接用整体
 *eg:max(a+b),c+d => a+b,c+d
 *
 *目前的groupby中可以含有表达式，但是不支持该表达式在selectexpr中参与运算
 *
 */
static LogicalOperator *groupby_select_where_from2logicalplan(
    Node *parsetree)  // proj,agg处理合并在一起,形成p-a-p模型
{
  Query_stmt *node = (Query_stmt *)parsetree;
  LogicalOperator *where_from_logicalplan =
      where_from2logicalplan(node->from_list);
  vector<Attribute> group_by_attributes;
  vector<Attribute> aggregation_attributes;
  vector<PhysicalAggregation::State::Aggregation> aggregation_function;
  int agg_has_expr, agg_in_expr, has_agg, sid = 0;
  agg_has_expr = agg_in_expr = has_agg = 0;
  judge_selectlist_agg_has_or_in_expr(node->select_list, has_agg, agg_has_expr,
                                      agg_in_expr);
  LogicalOperator *select_logicalplan = NULL;
  //	SQLParse_log("has_agg=
  //%d,agg_has_expr=%d,agg_in_expr=%d",has_agg,agg_has_expr,agg_in_expr);
  if (has_agg == 0 && node->groupby_list == NULL) {
    if (!CanCancelProject2(((Query_stmt *)parsetree)->select_list)) {
      select_logicalplan =
          select_where_from2logicalplan(parsetree, where_from_logicalplan, 0);
    } else {
      select_logicalplan = where_from_logicalplan;
    }
  } else {
    if (CanCancelProject1(((Query_stmt *)parsetree)->select_list)) {
      select_logicalplan = where_from_logicalplan;
    } else {
      select_logicalplan =
          select_where_from2logicalplan(parsetree, where_from_logicalplan, 1);
    }
    if (node->groupby_list != NULL)  //获取group_by_attributes
    {
      get_group_by_attributes(node->groupby_list, group_by_attributes,
                              select_logicalplan);
      //#ifdef SQL_Parser
      //			cout<<"groupby size=
      //"<<group_by_attributes.size()<<endl;
      //			for(int i=0;i<group_by_attributes.size();i++)
      //			{
      //				cout<<"groupby_att
      //"<<group_by_attributes[i].attrName<<endl;
      //			}
      //#endif
    }
    if (has_agg == 1) {
      get_aggregation_args(node->select_list, aggregation_attributes,
                           aggregation_function, select_logicalplan);
      //#ifdef SQL_Parser
      //			cout<<"agg_att size=
      //"<<aggregation_attributes.size()<<endl;
      //			for(int i=0;i<aggregation_attributes.size();i++)
      //			{
      //				cout<<"agg_att
      //"<<aggregation_attributes[i].attrName<<endl;
      //			}
      //#endif
    }
    LogicalOperator *projection_or_aggregation =
        new LogicalAggregation(group_by_attributes, aggregation_attributes,
                               aggregation_function, select_logicalplan);
    if (!CanCancelProject2(((Query_stmt *)parsetree)->select_list)) {
      select_logicalplan = select_where_from2logicalplan(
          parsetree, projection_or_aggregation, 2);
    } else {
      select_logicalplan = projection_or_aggregation;
    }
  }
  return select_logicalplan;
}

static void get_orderby_column_from_selectlist(
    Node *olnode, Node *slnode, vector<LogicalSort::OrderByAttr *> &obcol) {
  Orderby_list *gblist = (Orderby_list *)(olnode);
  for (Node *p = (Node *)(gblist->next); p != NULL;) {
    Groupby_expr *gbexpr = (Groupby_expr *)p;
    switch (gbexpr->args->type) {
      case t_name:
      case t_column:
      case t_name_name: {
        Columns *col = (Columns *)(gbexpr->args);
        //				obcol.push_back(col->parameter2);
        // obcol.push_back(col->parameter2);
        obcol.push_back(
            new LogicalSort::OrderByAttr(col->parameter2, gbexpr->sorttype));
      } break;
      case t_intnum: {
        Expr *exprval = (Expr *)(gbexpr->args);
        int num = 0;
        Node *q = slnode;
        for (; q != NULL;) {
          Select_list *selectlist = (Select_list *)q;
          if (num == atof(exprval->data)) break;
          num++;
          q = selectlist->next;
        }
        if (q == NULL) {
          // string str=(exprval->data.int_val);
          SQLParse_elog(
              "get_orderby_column_from_selectlist order by attribute num ", "",
              "is beyond");
        } else {
          Select_list *selectlist = (Select_list *)q;
          Select_expr *sexpr = (Select_expr *)selectlist->args;
          switch (sexpr->colname->type) {
            case t_name:
            case t_name_name:
            case t_column: {
              Columns *col = (Columns *)sexpr->colname;
              obcol.push_back(new LogicalSort::OrderByAttr(col->parameter2,
                                                           gbexpr->sorttype));
            } break;
            default: {
              SQLParse_elog("get_orderby_column_from_selectlist",
                            "gbexpr->colname->type ", "not exist");
            }
          }
        }
      } break;
      case t_expr_func: {
        Expr_func *func = (Expr_func *)(gbexpr->args);
        assert(func->str != NULL);
        obcol.push_back(
            new LogicalSort::OrderByAttr(func->str, gbexpr->sorttype));
      } break;
      case t_expr_cal: {
        Expr_cal *ecal = (Expr_cal *)(gbexpr->args);
        assert(ecal->str != NULL);
        obcol.push_back(
            new LogicalSort::OrderByAttr(ecal->str, gbexpr->sorttype));
      } break;
      default: {
        SQLParse_elog("get_orderby_column_from_selectlist",
                      "gbexpr->args->type ", "not exist");
      }
    }
    p = gbexpr->next;
  }
}
static LogicalOperator *having_select_groupby_where_from2logicalplan(
    Node *&parsetree) {
  LogicalOperator *having_logicalplan = NULL;
  LogicalOperator *select_logicalplan =
      groupby_select_where_from2logicalplan(parsetree);
  Query_stmt *node = (Query_stmt *)parsetree;
  if (node->having_list == NULL) {
    having_logicalplan = select_logicalplan;
  } else {
    vector<QNode *> h_qual;
    h_qual.push_back(transformqual(((Having_list *)node->having_list)->next,
                                   select_logicalplan));
    having_logicalplan = new LogicalFilter(select_logicalplan, h_qual);
  }
  return having_logicalplan;
}
static LogicalOperator *orderby_having_select_groupby_where_from2logicalplan(
    Node *&parsetree) {
  LogicalOperator *orderby_logicalplan = NULL;
  LogicalOperator *having_logicalplan =
      having_select_groupby_where_from2logicalplan(parsetree);
  Query_stmt *node = (Query_stmt *)parsetree;
  if (node->orderby_list == NULL) {
    orderby_logicalplan = having_logicalplan;
  } else {
    //		vector<string>obstr;
    vector<LogicalSort::OrderByAttr *> obcol;
    get_orderby_column_from_selectlist(node->orderby_list, node->select_list,
                                       obcol);
    //		for(int i=obstr.size()-1;i>=0;i--)
    //		{
    //			obcol.push_back(new
    //LogicalSort::OrderByAttr(obstr[i].c_str()));
    //		}
    //	obcol.push_back(&LogicalSort::OrderByAttr("LINEITEM.L_LINESTATUS"));
    reverse(obcol.begin(), obcol.end());
    //#ifdef SQL_Parser
    //		cout<<"orderby size=  "<<obcol.size()<<endl;
    //		for(int i=0;i<obcol.size();i++)
    //		{
    //			cout<<"orderby att= "<<obcol[i]->ta_ <<endl;
    //		}
    //#endif
    orderby_logicalplan = new LogicalSort(having_logicalplan, obcol);
  }
  return orderby_logicalplan;
}
static LogicalOperator *parsetree2logicalplan(
    Node *parsetree)  //实现parsetree 到logicalplan的转换，
{
  //	initialize_arithmetic_type_promotion_matrix();
  LogicalOperator *alllogicalplan = NULL;
  switch (parsetree->type) {
    case t_query_stmt: {
      alllogicalplan =
          orderby_having_select_groupby_where_from2logicalplan(parsetree);
    } break;
    default: {
      SQLParse_log("parsetree type is null");
      alllogicalplan = NULL;
    }
  }
  SQLParse_log("parsetree2Logicalplan is completed!!!!!!!!!!!!!!!!!!!!!");
  return alllogicalplan;
}

#endif
