#!/bin/sh
if [ ! -f "${0##*/}" ]; then
 echo "please run script in sbin/ directory!"
 exit 1
fi

cd $CLAIMS_HOME/sbin/2-claims-conf/
source ./load-config.sh
source ./generate-config.sh
cd ../

for node in $slaves $master
do
  echo "<-$node->"
  ssh -f -n -l $user $node "cd $claimshome; if [ ! -d 'install' ]; then mkdir install; fi; ./sbin/stop-node.sh"
  
  scp $CLAIMS_HOME/install/claimsserver $user@$node:$claimshome/install
  scp $CLAIMS_HOME/install/client $user@$node:$claimshome/install
  scp $CLAIMS_HOME/install/test $user@$node:$claimshome/install
  scp -r $CLAIMS_HOME/sbin $user@$node:$claimshome
done
