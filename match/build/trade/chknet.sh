#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

path=`pwd`
echo $path

number=${path##*front}
echo $number
if [ "$number" == "" ]
then
	number=0
fi

#httpfront_marketdata_0=manager|http://127.0.0.1:10100/action#1#precheck:2
port=`grep wsfront_front_$number network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`
portNumber=`grep wsfront_front_$number network.ini | awk -F # '{print $2}'`
i=0
while [ $i -lt $portNumber ]
do
	i=`expr $i + 1`
	port=`expr $port + 1`
	echo ---:$port
	netstat -an | grep ":$port"
done
