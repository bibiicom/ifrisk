#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

path=`pwd`
echo $path

number=${path##*private}
echo $number
if [ "$number" == "" ]
then
	number=0
fi

#httpfront_gateway_0=manager|http://127.0.0.1:10100/action#1#precheck:2
port=`grep httpfront_private_$number network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`
echo httpfront_private_$number 
echo curl http://127.0.0.1:$port/DumpMDB
curl http://127.0.0.1:$port/DumpMDB
