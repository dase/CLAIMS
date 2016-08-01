#!/bin/sh

source /home/claims/.bashrc

CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR
cd ../2-claims-conf
source ./load-config.sh
cd ../../
# now in CLAIMS_HOME

cd sbin/claims-test/
  rm testresult/* -rf
  starttime=$(date '+%Y-%m-%d %H:%M:%S')
  ./claimstestnr.sh 1 1 gtestfor >> auto-gtestfor.log
  echo "start time:$starttime"
  echo "end time:$(date '+%Y-%m-%d %H:%M:%S')"
cd ../../
