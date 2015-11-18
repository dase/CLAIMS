/*
 * gtest.cpp
 *
 *  Created on: May 22, 2014
 *      Author: wangli
 */
#ifndef __G_TEST_MAIN__
#define __G_TEST_MAIN__
#include <gtest/gtest.h>


//#include "utility/generator_test.h"
//#include "common/insert_optimized_hash_table_test.h"
//#include "TestSuit/sql_query_test.h"


static int g_test_main(int argc, char **argv){
	printf("argc %d\n",argc);
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}


#endif
