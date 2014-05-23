#!/bin/bash
#eject -r
FILE="./Default/Claims"
#echo 
#CMD $$2$
LAST=`ls -l "$FILE"`
#C=`ls`
#echo "$C"
NULL=
while true; do
  sleep 1
  EXIST='ls -l "$FILE"'
  if [ "$EXIST" != "$NULL" ]; then
  	NEW=`ls -l "$FILE"`
  	if [ "$NEW" != "$LAST" ]; then
     		sleep 15
     		eject -r
    		echo "$NEW"

    		LAST="$NEW"
     		eject -t
  	fi
  fi
done
