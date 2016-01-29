#!/bin/sh  

if [ ! -f "${0##*/}" ]; then
 echo "please run script in jenkins-scripts/ directory!"
 exit 1
fi

cd $CLAIMS_HOME/sbin/2-claims-conf/
source ./load-config.sh

# master config
configfile=$CLAIMS_HOME/sbin/2-claims-conf/config-$master


#######################################
# start cluster                       #
#######################################
  
cd $CLAIMS_HOME
cd install
ulimit -c unlimited

thislog=$logfilepath/claimsserver.$(date +%Y-%m-%d).log

# start master firstly
if [ -f "$runclaimsprocid" ]; then 
claimspids=`sed '/^claimsserver=/!d;s/.*=//' $runclaimsprocid`
if [ "$claimspids" != "" ]; then
  echo -e "\033[31m  claimsserver is already running with process pid:[$claimspids]
  please run script: ./3-stopclaimscluster.sh to stop it firstly!\033[0m" 
  exit
fi
fi
echo "-------------------------------"
echo "configfile: [$configfile]"
echo "-------------------------------"
datestr=`date '+%Y-%m-%d %H:%M:%S'`
thisstartstr="========run claimsserver time: $datestr========"
echo $thisstartstr
echo $thisstartstr >>  $thislog
echo -e "\033[31m`pwd`\033[0m"
./claimsserver -c $configfile >> $thislog &
echo "claimsserver=$!" > $runclaimsprocid
echo -e "master start claimsserver [\033[32mOK\033[0m]"

# start slaves
for slave in $slaves
do
{
  ssh $user@$slave "$deploypath/start-slave.sh config-$slave>/dev/null 2>&1" & 
  echo -e "$slave claimsserver start [\033[32mOK\033[0m]"
}
done

