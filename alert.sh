#!/bin/bash
FILE="./Default/Claims"
LAST=`ls -l "$FILE"`
NULL=
CHANGE=0
while true; do
  sleep 2
  EXIST=`ls -l "$FILE"`
#  echo "$EXIST"
  if [ "$EXIST" != "$NULL" ]; then
  	NEW=`ls -l "$FILE"`
  	if [ "$NEW" != "$LAST" ]; then
     	        CHANGE=1

    		LAST="$NEW"
  	else 
		if [ $CHANGE == 1 ]; then
                	eject -r
                        eject -t
                        CHANGE=0
                fi
           
         fi
	
  fi
done
