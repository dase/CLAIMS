/*
 * test_tool.h
 *
 *  Created on: Feb 27, 2014
 *      Author: wangli
 */

#ifndef TEST_TOOL_H_
#define TEST_TOOL_H_

#include <string>
inline void print_pass(){
	printf("\033[;32mPASS!\033[0m\n");
}
inline void print_fail(){
	printf("\033[;31mFAILED!\033[0m\n");
}
inline void print_test_result(bool result){
	if(result)
		print_pass();
	else
		print_fail();
}
inline void print_test_name_result(bool result,std::string test_name=std::string()){
	if(test_name.size()>0){
		printf("Test %5.15s\t",test_name.c_str());
	}
	if(result)
		print_pass();
	else
		print_fail();
}
inline bool is_equal(float x,float b){
	return fabs(x-b)<0.000001;
}

#endif /* TEST_TOOL_H_ */
