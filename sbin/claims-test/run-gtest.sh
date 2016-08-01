#!/bin/sh

CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR
cd ../2-claims-conf
source ./load-config.sh
cd ../../
# now in CLAIMS_HOME

starttime=$(date '+%Y-%m-%d %H:%M:%S')
loops=0
while [ 1 ]
do
  echo "claimsserver is running..."
  ((loop++))
  echo -e "\033[33m=======[$loop]=time:[`date '+%Y-%m-%d %H:%M:%S'`]========\033[0m"
  ./install/test --ip $master --port $client_listener_port

  if [ $? -ne 0 ]; then
    echo "test error."
    break
  fi
done

echo "loops:$loop"
echo "start time:$starttime"
echo "end time:$(date '+%Y-%m-%d %H:%M:%S')"
