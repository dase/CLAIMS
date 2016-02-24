#!/bin/sh

num=$2
vimdiff concert-$1-$num.result concert-$1-$[num+1].result concert-$1-$[num+2].result concert-$1-$[num+3].result
