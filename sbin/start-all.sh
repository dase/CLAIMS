#!/bin/sh  

CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR
cd 2-claims-conf/
source ./load-config.sh
source ./generate-config.sh
cd ../../
# now in CLAIMS_HOME

# start cluster
for node in $master $slaves
do
{
  ssh -f -n -l $user $node "$claimshome/sbin/start-node.sh $claimshome/sbin/2-claims-conf/config-$node>/dev/null 2>&1"
  echo -e "$node claimsserver start [\033[32mOK\033[0m]"
}&
done
wait
