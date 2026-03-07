#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

User=`whoami`

touch snapgen.sh.lock
proc=`ps -fu $User | grep snapgen.sh | grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]
then
	echo stop snapgen.sh ...
	kill -9 $proc
fi

proc=`ps -fu $User | grep -w "./trade -r"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]
then
	echo stop trade -r...
	kill -9 $proc
fi

if [ "$1" == "" ]
then
	proc=`ps -fu $User | grep -w "./trade"| grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
else
	proc=`ps -fu $User | grep -w "./trade $1"| grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
fi

if [ "$proc" == "" ]
then
	printf "%-14s is not running\n" trade
else
	echo stop trade $1 ...
	kill -9 $proc
fi
