#!/bin/sh

testname=concert
num=$2
vimdiff $testname-$1-$num.result $testname-$1-$[num+1].result $testname-$1-$[num+2].result $testname-$1-$[num+3].result
