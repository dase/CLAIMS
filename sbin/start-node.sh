#!/bin/sh

CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR
cd 2-claims-conf
source ./load-config.sh
#source ./generate-config.sh
cd ../../
# now in CLAIMS_HOME

timestr=$(date +%Y-%m-%d)

if [ ! -d "$logpath" ]; then
 echo "log path not exist, mkdir:$logpath"
 mkdir -p "$logpath"
fi

if [ -z $1 ]; then
 echo "please start with config file!" >> $logpath/noconfig-$timestr
 echo "please start with config file!"
 exit 1
fi

echo "-----------------------------------"
echo "configfile: 【$1】"
echo "-----------------------------------"

echo -e "\033[31m`pwd`\033[0m"

thisip=${1#*config-}
thislog=$logpath/claimsserver-$thisip-$timestr.log

./sbin/stop-node.sh

# for debug begin ######
cd install
ulimit -c unlimited
cd ../
# for debug end ########

echo "========run claimsserver on:[$thisip] time:[$(date '+%Y-%m-%d %H:%M:%S')]========" >> $thislog
./install/claimsserver -c $1 >> $thislog &
claimsserverpid=$!
echo "claimsserver=$claimsserverpid" > $runclaimsprocid
echo -e "$thisip start claimsserver pid:[$claimsserverpid][\033[32mOK\033[0m]"

echo "start tracker for debug..."
#./sbin/claims-test/statustracker.sh &

