#!/bin/sh
if [ ! -f "${0##*/}" ]; then
 echo "please run script in sbin/ directory!"
 exit 1
fi

cd $CLAIMS_HOME/sbin/2-claims-conf/
source ./load-config.sh
source ./generate-config.sh
cd ../

if [ "$1" = "all" ]; then 
for slave in $slaves
do
  ssh $user@$slave "$deploypath/stop-slave.sh>/dev/null 2>&1 &"
  
  scp $CLAIMS_HOME/install/claimsserver $user@$slave:$deploypath
  scp $CLAIMS_HOME/sbin/2-claims-conf/config-$slave $user@$slave:$deploypath
  scp $CLAIMS_HOME/sbin/slave-scripts/start-slave.sh $user@$slave:$deploypath
  scp $CLAIMS_HOME/sbin/slave-scripts/stop-slave.sh $user@$slave:$deploypath
done
else
for slave in $slaves
do
  scp $CLAIMS_HOME/sbin/2-claims-conf/config-$slave $user@$slave:$deploypath
done
fi

