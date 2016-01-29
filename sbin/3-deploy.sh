#!/bin/sh
if [ ! -f "${0##*/}" ]; then
 echo "please run script in jenkins-scripts/ directory!"
 exit 1
fi

cd $CLAIMS_HOME/sbin/2-claims-conf/
source ./load-config.sh
cd ../

if [ "$1" = "all" ]; then 
for slave in $slaves
do
  ssh $user@$slave "$deploypath/start-slave.sh>/dev/null 2>&1 &"
  echo -e "$slave claimsserver stop [\033[32mOK\033[0m]"
  
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

