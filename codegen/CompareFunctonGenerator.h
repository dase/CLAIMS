/*
 * NValueExpressionGenerator.h
 *
 *  Created on: 2015-4-1
 *      Author: imdb
 */

#ifndef NVALUEEXPRESSIONGENERATOR_H_
#define NVALUEEXPRESSIONGENERATOR_H_

#include "CodeGenerator.h"
#include "../common/types/NValue.hpp"
#include "../common/Expression/qnode.h"

typedef llvm::Value lv;

extern "C"
bool NValueLess(void* l, void* r);

extern "C"
bool NValueGreat(void* l, void* r);

extern "C"
bool NValueEqual(void* l, void* r);

extern "C"
bool NValueLessEqual(void *l, void *r);

extern "C"
bool NValueGreatEqual(void *l, void *r);

extern "C"
bool DateGreat(void* l, void *r);
extern "C"
bool DateLess(void* l, void *r);

llvm::Function* CreateNValueCompareFunc(enum oper_type compare_type);

llvm::Function* CreateDateCompareFunc(enum oper_type compare_type);


#endif /* NVALUEEXPRESSIONGENERATOR_H_ */
