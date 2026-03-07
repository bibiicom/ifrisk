#!/bin/bash

if [ "$1" != "" ]
then
	exit	
fi

workdir=$(cd $(dirname $0); pwd)
cd $workdir

if [ -d trade ]
then
	cd trade
	httpfront_trade='httpfront_trade_0'
else
	if [ -d trade1 ]
	then
		cd trade1
		httpfront_trade='httpfront_trade_1'
	else
		if [ -d trade2 ]
		then
			cd trade2
			httpfront_trade='httpfront_trade_2'
		else
			exit
		fi
	fi
fi

#httpfront_trade_0=manager|http://127.0.0.1:10100/action#1#swapCFD:2
port=`grep $httpfront_trade network.ini | head -n 1 | awk -F : '{print $3}' | awk -F / '{print $1}'`

##Stop And Check System Status Sync Over
curl http://127.0.0.1:$port/Stop

if [ $? -ne 0 ]
then
	echo "curl failed exit"
	exit
else
	echo ""
fi
sleep 1
stopDumpNo=`ls -d snap/snap.*.Stop 2>/dev/null | awk -F . '{print $2}' | sort -r -n | head -n 1`
maxDumpNo=`ls -d snap/snap.* 2>/dev/null | grep -v Stop | awk -F . '{print $2}' | sort -r -n | head -n 1`
while ((maxDumpNo>=stopDumpNo))
do
	echo stopDumpNo[$stopDumpNo] "<" maxDumpNo[$maxDumpNo], waiting ...
	stopDumpNo=`ls -d snap/snap.*.Stop 2>/dev/null | awk -F . '{print $2}' | sort -r -n | head -n 1`
done

dumpNo=`cat dumpPhase.log`
while [ ! "$stopDumpNo" == "$dumpNo" ]
do
	echo get DumpNo: $dumpNo != $stopDumpNo, waiting ...
	sleep 1
	dumpNo=`cat dumpPhase.log`
done
echo dumpNo: $dumpNo
dumpNo=$[dumpNo-1]
cd $workdir

#for sink in sinkdb* sinkredis sinkkafka
for sink in sinkdb* sinkredis
do
	if [ -d $sink ]
	then
		cd $sink
		sinkDumpNo=`tail -n 1000 sink*.*.log | grep nCompleteTaskNo | tail -n 1 | awk -F [ '{print $3}' | awk -F ] '{print $1}'`
		echo $sink: $sinkDumpNo 
		while ((sinkDumpNo<dumpNo))
		do
			echo $sink: $sinkDumpNo "<" $dumpNo not touched waiting ...
			sleep 2
			sinkDumpNo=`tail -n 1000 sink*.*.log | grep nCompleteTaskNo | tail -n 1 | awk -F [ '{print $3}' | awk -F ] '{print $1}'`
			echo $sink: $sinkDumpNo
		done
		echo $sink: $sinkDumpNo '>=' $dumpNo continue to stop trade
		cd ..
	fi
done
