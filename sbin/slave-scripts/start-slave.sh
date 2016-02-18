#!/bin/sh

cd $CLAIMS_HOME
timestr=`date '+%Y-%m-%d'`
logpath=$CLAIMS_HOME/claimslogs

if [ ! -d "$logpath" ]; then
 echo " path not exist, mkdir: "$logpath
 mkdir -p "$logpath"
fi

thisip=`ifconfig | grep "10.11.1.*"`
thisip=${thisip:20:11}

logfile=$logpath/claimsserver-$thisip-$timestr.log

if [ -z $1 ]; then
 echo "please start with config file!" >> $logfile
 echo "please start with config file!"
 exit
fi

claimspid=`ps x | grep -w ./claimsserver | grep -v grep | awk '{print $1}'`
if [ "$claimspid" != "" ]; then
 echo "claimsserver pid $claimspid"
 kill -9 $claimspid
fi

ulimit -c unlimited
echo "========run claimsserver on slave:[$thisip] time:$(date '+%Y-%m-%d %H:%M:%S')========" >> $logfile
./claimsserver -c $1 >> $logfile &
