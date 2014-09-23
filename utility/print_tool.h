/*
 * print_tool.h
 *
 *  Created on: Sep 20, 2014
 *      Author: wangli
 */

#ifndef PRINT_TOOL_H_
#define PRINT_TOOL_H_
#include <stdarg.h>

/**  NOTE:
 * The following function only be used for temporarily print debugging
 * informations, and all the calling instances should be removed before
 * commit to git.
 */
inline void p_red(const char* format,...){
	va_list arg;
	va_start (arg, format);
	printf("\033[;31m");
	vprintf(format,arg);
	printf("\033[0m");
	va_end (arg);
}

inline void p_green(const char* format,...){
	va_list arg;
	va_start (arg, format);
	printf("\033[;32m");
	vprintf(format,arg);
	printf("\033[0m");
	va_end (arg);
}




#endif /* PRINT_TOOL_H_ */
