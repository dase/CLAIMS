#!/bin/sh

CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR
cd 2-claims-conf/
source ./load-config.sh
source ./generate-config.sh
cd ../../
# now in CLAIMS_HOME

./sbin/stop-all.sh>/dev/null 2>&1

echo -e "\033[36m<$claimshome>\033[0m"

for node in $master $slaves
do

  echo -e "\033[36m<-$node->\033[0m"

  if [ "$1" = "" ] || [ "$1" = "exec" ]; then
      ssh -f -n -l $user $node "if [ ! -d '$claimshome/sbin' ]; then mkdir -p '$claimshome/sbin'; fi; exit"
      ssh -f -n -l $user $node "if [ ! -d '$claimshome/install' ]; then mkdir -p '$claimshome/install'; fi; exit"
      scp install/claimsserver $user@$node:$claimshome/install
      scp install/client $user@$node:$claimshome/install
      scp install/test $user@$node:$claimshome/install
      scp sbin/*.sh $user@$node:$claimshome/sbin
  fi

  if [ "$1" = "" ] || [ "$1" = "config" ]; then
      ssh -f -n -l $user $node "if [ ! -d '$claimshome/sbin/2-claims-conf' ]; then mkdir -p '$claimshome/sbin/2-claims-conf'; fi; exit"
      scp -r sbin/2-claims-conf/cluster.config $user@$node:$claimshome/sbin/2-claims-conf
      scp -r sbin/2-claims-conf/*.sh $user@$node:$claimshome/sbin/2-claims-conf
      scp -r sbin/2-claims-conf/config-$node $user@$node:$claimshome/sbin/2-claims-conf
  fi

  if [ "$1" = "" ] || [ "$1" = "test" ]; then
      ssh -f -n -l $user $node "if [ ! -d '$claimshome/sbin/claims-test' ]; then mkdir -p '$claimshome/sbin/claims-test'; fi; exit"
      scp -r sbin/claims-test/*.sh $user@$node:$claimshome/sbin/claims-test
      ssh -f -n -l $user $node "if [ ! -d '$claimshome/sbin/claims-test/testcase' ]; then mkdir -p '$claimshome/sbin/claims-test/testcase'; fi; exit"
      scp -r sbin/claims-test/testcase/* $user@$node:$claimshome/sbin/claims-test/testcase
      ssh -f -n -l $user $node "if [ ! -d '$claimshome/sbin/claims-test/monitor' ]; then mkdir -p '$claimshome/sbin/claims-test/monitor'; fi; exit"
      scp -r sbin/claims-test/monitor/* $user@$node:$claimshome/sbin/claims-test/monitor
  fi

done
