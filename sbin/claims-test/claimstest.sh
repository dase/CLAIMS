#!/bin/sh

if [ ! -f "${0##*/}" ]; then
 echo "please run script in sbin/claims-test/ directory!"
 exit 1
fi

set -e
CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR
cd ../2-claims-conf
source ./load-config.sh
cd ../../
# now in CLAIMS_HOME

# for debug begin #####
cd install
ulimit -c unlimited
cd ../
# for debug end #######

if [ "$1" = "" ]; then
  runloops=1
else
  runloops=$1
fi

if [ "$2" = "" ]; then
  concurrency_count=1
else
  concurrency_count=$2
fi

if [ "$3" = "" ]; then
 echo "please input test case as the third param."
 exit 1
fi
echo "loops=[$runloops];""concurrency=[$concurrency_count]"

thislog=$logpath/client.$(date +%Y-%m-%d).log
cd sbin/claims-test
if [ ! -d "testresult" ]; then
 mkdir -p "testresult"
fi
cd ../../

for((loop=1;loop<=runloops;loop++))
do
{
 for((cur=1;cur<=concurrency_count;cur++))  
 do
 {
  datestr=`date '+%Y-%m-%d %H:%M:%S'`
  thisstartstr="========run test:[$3] [$loop-$cur] time:[$datestr]========"
  echo -e "\033[33m$thisstartstr\033[0m"
  ./install/client $master $client_listener_port < ./sbin/claims-test/testcase/$3.test > ./sbin/claims-test/testresult/$3-$loop-$cur-$(date '+%Y-%m-%d-%H%M%S').result
  sleep 1
 }&
 done
 wait
 echo -e "\033[36mtest loop:[$loop] end time:[`date '+%Y-%m-%d %H:%M:%S'`]\033[0m"
}
done
