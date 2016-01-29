#!/bin/sh


while [ 1 ]
do
 procid=`pgrep claimsserver`
 if [ "$procid" = "" ]; then
  echo "claimsserver is core dumped. Try to restart claimsserver..."
  cd $CLAIMS_HOME/sbin
  ./4-stop-all.sh
  ./5-start-all.sh
  sleep 1 
 else
  echo "claimsserver is running..."
  cd $CLAIMS_HOME/sbin/claims-test/
  ./claimstest.sh 1 3 decimal 
 fi
done
