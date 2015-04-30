/*
 * CodeGenerator.h
 *
 *  Created on: Mar 21, 2015
 *      Author: wangli
 */

#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/TargetSelect.h"
#include <llvm/ExecutionEngine/JIT.h>
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "../utility/lock.h"

using llvm::Module;
using llvm::legacy::FunctionPassManager;
using llvm::ExecutionEngine;
class CodeGenerator {
public:
	static CodeGenerator* getInstance();
	virtual ~CodeGenerator();
	Module * getModule()const;
	llvm::IRBuilder<>* getBuilder()const;
	FunctionPassManager* getFunctionPassManager()const;
	ExecutionEngine* getExecutionEngine()const;
	void lock();
	void release();
private:
	CodeGenerator();
	static llvm::IRBuilder<>* builder_;
	static Module* module_;
	static FunctionPassManager* fpm_;
	static CodeGenerator* instance_;
	static ExecutionEngine* engine_;
private:
	Lock lock_;

};

#endif /* CODEGENERATOR_H_ */
