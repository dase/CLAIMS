#!/bin/sh

CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR

cd 2-claims-conf/
source ./load-config.sh 
cd ../../
# now in CLAIMS_HOME

for node in $slaves $master
do
 ssh -f -n -l $user $node "$claimshome/sbin/stop-node.sh $1>/dev/null 2>&1" &
 echo -e "$node claimsserver stop [\033[32mOK\033[0m]"
done
