#!/bin/sh

CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR
cd ../2-claims-conf
source ./load-config.sh
cd ../../
# now in CLAIMS_HOME

cd sbin/claims-test/
while [ 1 ]
do
  read -p "Hit the ENTER |__>" tempuseless
  echo $tempuseless
  starttime=$(date '+%Y-%m-%d %H:%M:%S')
#  ./claimstest.sh 1 1 concertdroptable
#  ./claimstest.sh 1 1 concertcreatetable
#  ./claimstest.sh 1 1 concertload50wdata
  ./claimstest.sh 1000 20 AggregationLargeGroups
  echo "start time:$starttime"
  echo "end time:$(date '+%Y-%m-%d %H:%M:%S')"
done
cd ../../
