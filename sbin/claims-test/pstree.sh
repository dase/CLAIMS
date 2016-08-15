#!/bin/sh

outexe=`pidof outexe`
echo $outexe
while [ 1 ]
do
    pstree -p $outexe
    sleep 1
done
