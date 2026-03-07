#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

if [ "$1" == "" ]
then
	echo "usage: rsync.sh user@ip"
	exit
fi

thispath=`pwd`
ssh -n $1 "mkdir $thispath"
rsync -avz --log-file=rsync.log backup.sh $1:$thispath

for module in `ls -F | grep "/$" | grep -w "$1" | grep "$2" |  awk -F / '{print $1}'`
do
	cd $module
	echo backup $module ...
	if [ -f rsync.sh ]
	then
		chmod u+x rsync.sh
		./rsync.sh $1
	fi
	cd ..
done
