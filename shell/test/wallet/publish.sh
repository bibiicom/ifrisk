#!/bin/bash
workdir=$(cd $(dirname $0); pwd)
cd $workdir

proc=`ps -ux | grep "wallet.sh" | grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
if [ ! "$proc" == "" ]
then
    printf "%-14s is running\n" nginx
    exit
fi

for project in arb; do
	mkdir $1/arb 2>/dev/null
	echo cp arb/* $1/arb/
	cp arb/* $1/arb/
done
