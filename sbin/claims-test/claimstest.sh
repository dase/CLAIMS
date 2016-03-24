#!/bin/sh
set -e
cd $CLAIMS_HOME/sbin/2-claims-conf/
source ./load-config.sh

# for debug begin #####
cd $CLAIMS_HOME
cd install
ulimit -c unlimited
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
echo "runloops=[$runloops]"
echo "concurrency_count=[$concurrency_count]"

thislog=$logpath/client.$(date +%Y-%m-%d).log
cd $CLAIMS_HOME/sbin/claims-test
<<<<<<< HEAD
for((loop=1;loop<=runloops;loop++))
do
{
 for((cur=1;cur<=concurrency_count;cur++))  
 do
 {
  datestr=`date '+%Y-%m-%d %H:%M:%S'`
  thisstartstr="========run test:[$3] [$loop-$cur] time: $datestr========"
  echo -e "\033[33m$thisstartstr\033[0m"
#  echo $thisstartstr >> $thislog
  $CLAIMS_HOME/install/client $master $client_listener_port < $CLAIMS_HOME/sbin/claims-test/testcase/$3.test > $CLAIMS_HOME/sbin/claims-test/testresult/$3-$loop-$cur.result
  sleep 1
=======
if [ ! -d "testresult" ]; then
 mkdir -p "testresult"
fi
>>>>>>> for_auto_test_neW

for((loop=1;loop<=runloops;loop++))
do
{
 for((cur=1;cur<=concurrency_count;cur++))  
 do
 {
  datestr=`date '+%Y-%m-%d %H:%M:%S'`
  thisstartstr="========run test:[$3] [$loop-$cur] time:[$datestr]========"
  echo -e "\033[33m$thisstartstr\033[0m"
  $CLAIMS_HOME/install/client $master $client_listener_port < $CLAIMS_HOME/sbin/claims-test/testcase/$3.test > $CLAIMS_HOME/sbin/claims-test/testresult/$3-$loop-$cur.result
  sleep 1
 }&
 done
 wait
}
done
