#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

path=`pwd`
echo $path

dumpNo1=`cat dumpPhase.log`
echo snap num before snap: $dumpNo1

number=${path##*trade}
if [ "$number" == "" ]
then
	number=0
fi

#httpfront_marketdata_0=manager|http://127.0.0.1:10100/action#1#swapCFD:2
port=`grep httpfront_trade_$number network.ini | head -n 1 | awk -F : '{print $3}' | awk -F / '{print $1}'`
echo get httpfront_trade_$number from network.ini
echo curl http://127.0.0.1:$port/DumpMDB
curl http://127.0.0.1:$port/DumpMDB
echo ""

dumpNo2=`cat dumpPhase.log`

while ((dumpNo2<=dumpNo1))
do
	sleep 2
	dumpNo2=`cat dumpPhase.log`
	echo $dumpNo2 ...
done

echo trade snap[$dumpNo2] has been writen to disk