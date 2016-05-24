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
  ./install/test --ip $master --port $client_listener_port
done
