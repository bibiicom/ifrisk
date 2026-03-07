#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

proc=`ps -ux | grep -w "./wallet.sh"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]; then
	echo "wallet.sh... $proc is running"
	exit
fi

echo "Starting wallet.sh ... "
nohup ./wallet.sh >>nohup.out 2>&1 &
