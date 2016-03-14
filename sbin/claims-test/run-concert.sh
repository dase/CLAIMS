#!/bin/sh

cd $CLAIMS_HOME/sbin/2-claims-conf
source ./load-config.sh

while [ 1 ]
do
  cd $CLAIMS_HOME/sbin/claims-test/
  read -p "Hit the ENTER |__>" tempuseless
  echo $tempuseless
  starttime=$(date '+%Y-%m-%d %H:%M:%S')
#  ./claimstest.sh 1 1 concertdroptable
#  ./claimstest.sh 1 1 concertcreatetable
#  ./claimstest.sh 1 1 concertload50wdata
  ./claimstest.sh 1 100 concert
  echo "start time:$starttime"
  echo "end time:$(date '+%Y-%m-%d %H:%M:%S')"
done
