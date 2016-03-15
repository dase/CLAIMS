#!/bin/sh
if [ ! -f "${0##*/}" ]; then
 echo "please run script in sbin/ directory!"
 exit 1
fi

cd $CLAIMS_HOME/sbin/2-claims-conf/
source ./load-config.sh
source ./generate-config.sh
cd ../

for slave in $slaves
do
  ssh -f -n -l $user $slave "cd $claimshome; if [ ! -d 'install' ]; then mkdir install; fi; ./stop-node.sh"
  
  scp $CLAIMS_HOME/install/claimsserver $user@$slave:$claimshome/install
  scp $CLAIMS_HOME/install/client $user@$slave:$claimshome/install
  scp $CLAIMS_HOME/install/test $user@$slave:$claimshome/install
  scp -r $CLAIMS_HOME/sbin $user@$slave:$claimshome
done
