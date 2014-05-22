/*
 * gtest.cpp
 *
 *  Created on: May 22, 2014
 *      Author: wangli
 */
#ifndef __G_TEST_MAIN__
#define __G_TEST_MAIN__
#include <gtest/gtest.h>


static int g_test_main(int argc, char **argv){
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}


#endif
