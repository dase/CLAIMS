#!/bin/sh

if [ "$1" = "" ]; then
export CLAIMS_HOME=$CLAIMS_HOME
else
export CLAIMS_HOME=$1
fi

echo "CLAIMS_HOME:[$CLAIMS_HOME]"
sed -i 's:^export CLAIMS_HOME=.*$:export CLAIMS_HOME='$CLAIMS_HOME':g' ~/.bashrc


cd $CLAIMS_HOME

./build.sh clean
./build.sh init
mkdir install
cd install
../configure --prefix=$CLAIMS_HOME/install

gcc --version
make -j 3
make install
 
