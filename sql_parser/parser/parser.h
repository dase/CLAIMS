/*
 * Parser.h
 *
 *  Created on: May 8, 2015 10:10:18 AM
 *      Author: fzh
 */

#ifndef PARSER_H_
#define PARSER_H_
#include <string>
#include <vector>

#include "../../sql_parser/ast_node/ast_node.h"

using std::string;
using std::vector;


class Parser
{
public:
	Parser();
	Parser(string SQL_statement);
	virtual ~Parser();
	AstNode* GetRawAST();
private:
	AstNode* CreateRawAST(string SQL_statement);
	string SQL_statement_;
	AstNode* AST_root_;
};

#endif /* PARSER_H_ */
