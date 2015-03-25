#ifndef __LLVM_MAIN__
#define __LLVM_MAIN__
#include <iostream>
using namespace std;
#include "generating_expr.cpp"
#include "../Test/gtest.h"
#include "codegen_test.h"
#include "ExpressionGenerator.h"
int main(int argc, char** argv){
//	return test::test_llvm();
//	g_test_main(argc, argv);
//	get_one();
//	test_reference();
	myllvm::test();
}
#endif
