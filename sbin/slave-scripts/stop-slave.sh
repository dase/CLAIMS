#!/bin/sh

#claimsps=`ps ux | grep claimsserver | grep -v grep`
claimspids=`ps x | grep -w ./claimsserver | grep -v grep | awk '{print $1}'`
if [ "$claimspids" != "" ]; then
for claimspid in $claimspids
do
echo "kill claimsserver pid : $claimspid"
kill -9 $claimspid
done
fi
