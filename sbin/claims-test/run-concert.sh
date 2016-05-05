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
<<<<<<< HEAD
#  ./claimstest.sh 1 1 concertdroptable
#  ./claimstest.sh 1 1 concertcreatetable
  ./claimstest.sh 1 1 concertload50wdata
  ./claimstest.sh 100 20 concert
=======
  ./claimstest.sh 1 1 concertdroptable
  ./claimstest.sh 1 1 concertcreatetable
  ./claimstest.sh 1 1 concertload50wdata
  ./claimstest.sh 10 20 concert
>>>>>>> for_auto_test_new
  echo "start time:$starttime"
  echo "end time:$(date '+%Y-%m-%d %H:%M:%S')"
done
cd ../../
