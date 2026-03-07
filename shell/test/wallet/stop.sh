#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

proc=`ps -ux | grep -w "./wallet.sh"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]; then
	echo "Stop wallet.sh... $proc"
	kill -9 $proc
fi

proc=`ps -ux | grep -w "./deposit"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]; then
	echo "Stop deposit... $proc"
	kill -9 $proc
fi

proc=`ps -ux | grep -w "./withdraw"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]; then
	echo "Stop withdraw... $proc"
	kill -9 $proc
fi
