/*
 * rename.h
 *
 *  Created on: Jun 25, 2013
 *      Author: wangli
 */

#ifndef RENAME_H_
#define RENAME_H_



#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>
#include <stdio.h>

typedef int (f)(int);

static f * FileClose=close;

typedef void (f1)(void*);
static f1 * memory_free=free;


typedef int (f3)(const char*,int,...);
static f3 *FileOpen=open;



#endif /* RENAME_H_ */

