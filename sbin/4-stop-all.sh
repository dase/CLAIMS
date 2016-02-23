#!/bin/sh
if [ ! -f "${0##*/}" ]; then
 echo "please run script in sbin/ directory!"
 exit 1
fi

cd $CLAIMS_HOME/sbin/2-claims-conf/
source ./load-config.sh 
cd ../

for slave in $slaves
do
ssh $user@$slave "$deploypath/stop-slave.sh>/dev/null 2>&1" &
echo -e "$slave claimsserver stop [\033[32mOK\033[0m]"
done

##############################
# master stop claimsserver   #
##############################
if [ "$1" = "all" ]; then

claimspids=`ps x | grep -w $CLAIMS_HOME/install/claimsserver | grep -v grep | awk '{print $1}'`
if [ "$claimspids" != "" ]; then
for claimspid in $claimspids
do
echo "claimsserver master pid : [$claimspid]"
kill -9 $claimspid
done
fi

if [ -f "$runclaimsprocid" ]; then
rm -f $runclaimsprocid
fi

clientpids=`ps x | grep -w $CLAIMS_HOME/install/client | grep -v grep | awk '{print $1}'`
if [ "$clientpids" != "" ]; then
for clientpid in $clientpids
do
echo "claims client pid : [$clientpid]"
kill -9 $clientpid
done
fi

gtestpids=`ps x | grep -w $CLAIMS_HOME/install/test | grep -v grep | awk '{print $1}'`
if [ "$gtestpids" != "" ]; then
for gtestpid in $gtestpids
do
echo "claims gtest pid : [$gtestpid]"
kill -9 $gtestpid
done
fi

else
if [ -f "$runclaimsprocid" ]; then
claimspids=`sed '/^claimsserver=/!d;s/.*=//' $runclaimsprocid`
if [ "$claimspids" != "" ]; then
echo "claimsserver master pid : [$claimspids]"
kill -9 $claimspids
fi
rm -f $runclaimsprocid
fi
fi
