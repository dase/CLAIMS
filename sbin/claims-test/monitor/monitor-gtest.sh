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
  cd $CLAIMS_HOME/sbin/claims-test
  ./claimstest.sh 1 1 load_tpch_sf1_1p
 else
  echo "claimsserver is running..."
  $CLAIMS_HOME/install/test --ip $master --port $client_listener_port
 fi
done
