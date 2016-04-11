#!/bin/sh

CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR
cd ../
# now in CLAIMS_HOME
./build.sh clean
./build.sh init
mkdir install
cd install
../configure 

gcc --version
make -j 3
