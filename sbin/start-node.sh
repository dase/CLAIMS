#!/bin/sh

cd $CLAIMS_HOME/sbin/2-claims-conf
source ./load-config.sh
source ./generate-config.sh
cd ../

timestr=$(date +%Y-%m-%d)

if [ ! -d "$CLAIMS_HOME/$logpath" ]; then
 echo "log path not exist, mkdir:$CLAIMS_HOME/$logpath"
 mkdir -p "$CLAIMS_HOME/$logpath"
fi

if [ -z $1 ]; then
 echo "please start with config file!" >> $CLAIMS_HOME/$logpath/noconfig-$timestr
 echo "please start with config file!"
 exit 1
fi

echo "-----------------------------------"
echo "configfile: 【$1】"
echo "-----------------------------------"

echo -e "\033[31m`pwd`\033[0m"

thisip=${1#*config-}
thislog=$CLAIMS_HOME/$logpath/claimsserver-$thisip-$timestr.log

./stop-node.sh

# for debug begin ######
cd $CLAIMS_HOME/install
ulimit -c unlimited
# for debug end ########

echo "========run claimsserver on:[$thisip] time:[$(date '+%Y-%m-%d %H:%M:%S')]========" >> $thislog
$CLAIMS_HOME/install/claimsserver -c $1 >> $thislog &
claimsserverpid=$!
echo "claimsserver=$claimsserverpid" > $CLAIMS_HOME/$runclaimsprocid
echo -e "$thisip start claimsserver pid:[$claimsserverpid][\033[32mOK\033[0m]"
