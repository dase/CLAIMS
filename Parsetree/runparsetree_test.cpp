/*
 * runparsetree_test.cpp
 *
 *  Created on: 2015-2-9
 *      Author: imdb
 */

/*
 *  test example:
 * "[ \t]*\n"
 *	"[ \t]* select *\n"
 *	"[ \t]* select * from PART\n"
 *	"[ \t]* select *\n from PART; select *\n"
 *	"[ \t]* select *\n from PART; [ \t]* select * from PART;\n"
 *	"[ \t]* select *\n from PART; [ \t]* select * from PART; select * from PART;\n"
 */
