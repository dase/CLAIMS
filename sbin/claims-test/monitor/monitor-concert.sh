#!/bin/sh

CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR
cd ../../2-claims-conf
source ./load-config.sh
cd ../../
# now in CLAIMS_HOME

while [ 1 ]
do
 procid=`pgrep claimsserver`
 if [ "$procid" = "" ]; then
  echo "claimsserver is aborted. Try to restart..."
  ./sbinstop-all.sh
  if [ "$local_disk_mode" = "1" ]; then
  rm $data*
  fi
  if [ -d "install" ]; then
    if [ ! -f "install/claimsserver" ]; then
     ./sbin/1-compile.sh
    fi
  else
    ./sbin/1-compile.sh
  fi 
  ./sbin/3-deploy.sh
  ./sbinstart-all.sh
  sleep 3 
 else
  echo "claimsserver is running..."
  cd sbin/claims-test/
  read -p "Hit the ENTER |__>" tempuseless
  echo $tempuseless
  ./claimstest.sh 1 1 concertdroptable
  ./claimstest.sh 1 1 concertcreatetable
  ./claimstest.sh 1 1 concertload50wdata
  ./claimstest.sh 1 100 concert
  cd ../../
  # now in CLAIMS_HOME
 fi
done
