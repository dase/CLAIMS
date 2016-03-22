#!/bin/sh
##############
# $1 = folder#
##############
folder=$1
cd ./testresult/$folder
results=`find . -maxdepth 1 -name "*.result"`

touch compareresult

for result in $results
do
  format=${result%_*}
  realformat=${format##*/}
  diff $result ../../hawq_r/${realformat}_${folder%%_*}.result
  if [ $? -ne 0 ] ; then
     echo "$result" >> ./compareresult
  fi
done

