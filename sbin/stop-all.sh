#!/bin/sh
if [ ! -f "${0##*/}" ]; then
 echo "please run script in sbin/ directory!"
 exit 1
fi

cd $CLAIMS_HOME/sbin/2-claims-conf/
source ./load-config.sh 
cd ../

for node in $slaves $master
do
#ssh -f -n -l $user $node "$claimshome/sbin/stop-node.sh $1>/dev/null 2>&1" &
ssh -f -n -l $user $node "$claimshome/sbin/stop-node.sh $1 &" &
#echo -e "$node claimsserver stop [\033[32mOK\033[0m]"
done
