#!/bin/bash

path=`pwd`
echo $path

number=${path##*front}
if [ "$number" == "" ]
then
	number=0
fi

port=`grep httpfront_front_$number network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`
echo get httpfront_front_$number from network.ini
ports=${port:0:4}
while [ true ]
do
	echo 'netstat -an | grep' $ports '| grep -v LISTEN | wc -l'
	netstat -an | grep $ports | grep -v LISTEN | wc -l
	sleep 1
done
