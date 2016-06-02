#!/bin/sh

CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR
cd ../2-claims-conf
source ./load-config.sh
cd ../../
# now in CLAIMS_HOME

while [ 1 ]
do
  echo "claimsserver is running..."
  echo -e "\033[33m========time:[`date '+%Y-%m-%d %H:%M:%S'`]========\033[0m"
  ./install/test --ip $master --port $client_listener_port
  if [ $? -ne 0 ]; then
    echo "test error."
    exit 0
  fi
done
