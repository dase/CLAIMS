#!/bin/sh
###########################################################################################
#default param                                                                            #
#./1-run-process.sh --concurrency=3 --testsuit=testsuit1 --resultpath=sf1(作为结果文件夹前缀)#
#       --Isprocessresult=1                                                               #
###########################################################################################
concurrency_count=1
testsuit="."
resultpath="sf1"
processresult=0

var=$*
for i in $var
do
 if [[ $i = --concurrency=* ]]
 then
 concurrency_count=${i##*=}
 elif [[ $i = --testsuit=* ]]
 then
 testsuit=${i##*=}
 elif [[ $i = --resultpath=* ]]
 then
 resultpath=${i##*=}
 elif [[ $i = --Isprocessresult=* ]]
 then
 processresult=${i##*=}
 fi
done

set -e
cd $CLAIMS_HOME/sbin/2-claims-conf/
source ./load-config.sh
##################
# start test     #
##################
  cd $CLAIMS_HOME
  cd install
  ulimit -c unlimited

echo "concurrency_count=[$concurrency_count]"

cd $CLAIMS_HOME/sbin/claims-test

filename=${resultpath}_`date '+%Y-%m-%d-%H%M%S'`
mkdir ./testresult/$filename

tests=`find ./testcase/${testsuit} -maxdepth 1 -name "*.test"`
for test in $tests
do
  resultfile=${test##*/}
  result=${resultfile%.*}
  for((cur=1;cur<=concurrency_count;cur++))  
  do
  {
   datestr=`date '+%Y-%m-%d %H:%M:%S'`
   thisstartstr="========run test:[$result]-[$cur] time: $datestr========"
   echo -e "\033[33m$thisstartstr\033[0m"
   $CLAIMS_HOME/install/client $master $client_listener_port < $test  > ./testresult/${filename}/${result}_${cur}.result
   sleep 1
  }&
  done
  wait
done

if [ $processresult = 1 ]
 then 
  echo -e "\033[33m======start process result\033[0m"
  ./process_claims_result.sh ${filename}
  echo -e "\033[33m======end process result\033[0m"
  wait
else
echo "not process result" 
fi
 



