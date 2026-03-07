#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

path=`pwd`
echo $path

number=${path##*triggerorder}
echo $number
if [ "$number" == "" ]
then
	number=0
fi

#httpfront_triggerorder_0=manager|http://127.0.0.1:10100/action#1#swapCFD:2
port=`grep httpfront_triggerorder_$number network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`
echo httprisk_triggerorder_$number 
echo curl http://127.0.0.1:$port/DumpMDB
curl http://127.0.0.1:$port/DumpMDB
