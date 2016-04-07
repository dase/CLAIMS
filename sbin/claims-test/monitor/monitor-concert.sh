#!/bin/sh

cd $CLAIMS_HOME/sbin/2-claims-conf
source ./load-config.sh

while [ 1 ]
do
 procid=`pgrep claimsserver`
 if [ "$procid" = "" ]; then
  echo "claimsserver is aborted. Try to restart..."
  cd $CLAIMS_HOME/sbin
  ./4-stop-all.sh
  if [ "$local_disk_mode" = "1" ]; then
  rm $data*
  fi
  if [ -d "$CLAIMS_HOME/install" ]; then
    cd $CLAIMS_HOME/install
    if [ ! -f "claimsserver" ]; then
     cd $CLAIMS_HOME/sbin
     ./1-compile.sh
    fi
  else
    cd $CLAIMS_HOME/sbin
    ./1-compile.sh
  fi 
  cd $CLAIMS_HOME/sbin
  ./5-start-all.sh
  sleep 3 
 else
  echo "claimsserver is running..."
  cd $CLAIMS_HOME/sbin/claims-test/
  read -p "Hit the ENTER |__>" tempuseless
  echo $tempuseless
  #./claimstest.sh 1 1 concertdroptable
  #./claimstest.sh 1 1 concertcreatetable
  #./claimstest.sh 1 1 concertload50wdata
  #./claimstest.sh 1 100 concert
   ./claimstest.sh 100 1 cancel
 fi
done
