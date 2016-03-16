#!/bin/sh  

if [ ! -f "${0##*/}" ]; then
 echo "please run script in sbin/ directory!"
 exit 1
fi

cd $CLAIMS_HOME/sbin/2-claims-conf/
source ./load-config.sh
source ./generate-config.sh
  
# start cluster
for node in $master $slaves
do
{
  ssh -f -n -l $user $node "$claimshome/sbin/start-node.sh $claimshome/sbin/2-claims-conf/config-$node>/dev/null 2>&1"
  echo -e "$node claimsserver start [\033[32mOK\033[0m]"
}&
done
wait
