#!/bin/sh

cd $CLAIMS_HOME/sbin/2-claims-conf
source ./load-config.sh

while [ 1 ]
do
  cd $CLAIMS_HOME/sbin/claims-test/
  read -p "Hit the ENTER |__>" tempuseless
  echo $tempuseless
  ./claimstest.sh 1 1 concertdroptable
  ./claimstest.sh 1 1 concertcreatetable
  ./claimstest.sh 1 1 concertload50wdata
  ./claimstest.sh 1 100 concert
done
