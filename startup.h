/*
 * startup.h
 *
 *  Created on: Jul 7, 2014
 *      Author: wangli
 */

#ifndef STARTUP_H_
#define STARTUP_H_
#include <stdio.h>
#include "Environment.h"
#include <unistd.h>
#include "Config.h"

void print_welcome(){


	printf("\033[;31m");
	printf("      ______  __           ___       __  .___  ___.      _______.\n");
	printf("     /      ||  |         /   \\     |  | |   \\/   |     /       |\n");
	printf("    |  ,----'|  |        /  ^  \\    |  | |  \\  /  |    |   (----`\n");
	printf("    |  |     |  |       /  /_\\  \\   |  | |  |\\/|  |     \\   \\   \n");
	printf("    |  `----.|  `----. /  _____  \\  |  | |  |  |  | .----)   |   \n");
	printf("     \\______||_______|/__/     \\__\\ |__| |__|  |__| |_______/    \n");
	printf("\033[0m\n");

}


#endif /* STARTUP_H_ */
