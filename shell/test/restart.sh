#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir
./stop.sh $*

if [ "$1" == "trade" -o "$1" == "" ]
then
	if [ -d "trade" ]
	then
		cd trade
		dumpNo1=`cat dumpPhase.log`
		./trade -s
		dumpNo2=`cat dumpPhase.log`
		while ((dumpNo2<=dumpNo1))
		do
			sleep 2
			dumpNo2=`cat dumpPhase.log`
			echo $dumpNo2 ...
		done
		echo trade snap[$dumpNo2] has been writen to disk,start others...
		cd ..
	fi
fi
./start.sh $*
