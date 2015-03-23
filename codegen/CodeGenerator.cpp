/*
 * CodeGenerator.cpp
 *
 *  Created on: Mar 21, 2015
 *      Author: wangli
 */

#include "CodeGenerator.h"

#include <assert.h>

using llvm::IRBuilder;
using llvm::legacy::FunctionPassManager;
using llvm::createCFGSimplificationPass;
using llvm::createDeadCodeEliminationPass;
using llvm::InitializeNativeTarget;
using llvm::EngineBuilder;

llvm::IRBuilder<>* CodeGenerator::builder_=0;
Module* CodeGenerator::module_=0;
CodeGenerator* CodeGenerator::instance_=0;
FunctionPassManager* CodeGenerator::fpm_=0;
ExecutionEngine* CodeGenerator::engine_=0;

CodeGenerator* CodeGenerator::getInstance() {
	if(instance_==0){
		instance_ = new CodeGenerator();
	}
	return instance_;
}

Module* CodeGenerator::getModule() const {
	return module_;
}

llvm::IRBuilder<>* CodeGenerator::getBuilder() const {
	return builder_;
}

FunctionPassManager* CodeGenerator::getFunctionPassManager() const {
	return fpm_;
}

ExecutionEngine* CodeGenerator::getExecutionEngine() const {
	return engine_;
}

void CodeGenerator::lock() {
	lock_.acquire();
}

void CodeGenerator::release() {
	lock_.release();
}

CodeGenerator::CodeGenerator() {
	 InitializeNativeTarget();
	builder_=new IRBuilder<>(llvm::getGlobalContext());
	module_=new Module("my cool jit", llvm::getGlobalContext());
    fpm_=new FunctionPassManager (module_);
	fpm_->add(createCFGSimplificationPass());
	fpm_->add(createDeadCodeEliminationPass());
	engine_ = EngineBuilder(module_).create();
	assert(engine_);

}

CodeGenerator::~CodeGenerator() {
	delete builder_;
	delete module_;
	delete fpm_;
}

