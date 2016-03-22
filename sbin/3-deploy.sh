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
  echo -e "\033[36m<-$node->\033[0m"
  ssh -f -n -l $user $node "if [ ! -d '$claimshome' ]; then mkdir -p $claimshome; fi; $claimshome/sbin/stop-node.sh; exit"
  
  ssh -f -n -l $user $node "if [ ! -d '$claimshome/install' ]; then mkdir -p $claimshome/install; fi; exit"
  scp $CLAIMS_HOME/install/claimsserver $user@$node:$claimshome/install
  scp $CLAIMS_HOME/install/client $user@$node:$claimshome/install
  scp $CLAIMS_HOME/install/test $user@$node:$claimshome/install
  ssh -f -n -l $user $node "if [ ! -d '$claimshome/sbin' ]; then mkdir -p $claimshome/sbin; fi; exit"
  scp $CLAIMS_HOME/sbin/*.sh $user@$node:$claimshome/sbin
  ssh -f -n -l $user $node "if [ ! -d '$claimshome/sbin/2-claims-conf' ]; then mkdir -p $claimshome/sbin/2-claims-conf; fi; exit"
  scp -r $CLAIMS_HOME/sbin/2-claims-conf/cluster.config $user@$node:$claimshome/sbin/2-claims-conf
  scp -r $CLAIMS_HOME/sbin/2-claims-conf/*.sh $user@$node:$claimshome/sbin/2-claims-conf
  scp -r $CLAIMS_HOME/sbin/2-claims-conf/config-$node $user@$node:$claimshome/sbin/2-claims-conf
  ssh -f -n -l $user $node "if [ ! -d '$claimshome/sbin/claims-test' ]; then mkdir -p $claimshome/sbin/claims-test; fi; exit"
  scp -r $CLAIMS_HOME/sbin/claims-test/*.sh $user@$node:$claimshome/sbin/claims-test
  ssh -f -n -l $user $node "if [ ! -d '$claimshome/sbin/claims-test/testcase' ]; then mkdir -p $claimshome/sbin/claims-test/testcase; fi; exit"
  scp -r $CLAIMS_HOME/sbin/claims-test/testcase/* $user@$node:$claimshome/sbin/claims-test/testcase
  ssh -f -n -l $user $node "if [ ! -d '$claimshome/sbin/claims-test/monitor' ]; then mkdir -p $claimshome/sbin/claims-test/monitor; fi; exit"
  scp -r $CLAIMS_HOME/sbin/claims-test/monitor/* $user@$node:$claimshome/sbin/claims-test/monitor

done
