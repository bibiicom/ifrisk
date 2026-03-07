#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

if [ -e backup.sh.lock ]
then
	echo backup.sh is running,exit!
	exit
fi
touch backup.sh.lock

thistime=`date +%Y%m%d%H%M%S`

backup=$workdir/../backup
mkdir $backup 2>/dev/null

echo Beginto Clean...
date +%Y%m%d%H%M
thistime=`date +%Y%m%d%H%M`
backupdir=$backup/$thistime
mkdir $backup/$thistime

if [ "$2" == "" ]
then
	grepname=$1
else
	if [ "$2" -ge 0 ] 2>/dev/null 
	then
		grepname=$1$2
	else
		grepname=`echo ${1}_$2`
	fi
fi

for module in `ls -F | grep "/$" | grep "$grepname" | awk -F / '{print $1}'`
do
	cd $module 
	appnum=${module##*[a-zA-Z]}
	appname=${module/%$appnum/}
	echo clean $appname $appnum
	
	User=`whoami`
	if [ "$appnum" == "" ]
	then
			proc=`ps -fu $User | grep -w "./$appname"| grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
	else
			proc=`ps -fu $User | grep -w "./$appname $appnum"| grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
	fi
	if [ ! "$proc" == "" ]
	then
			printf "%-14s is running\n" $module
			rm -rf backup.sh.lock
			exit
	fi
	if [ -f clean.sh ]
	then
		chmod u+x clean.sh
		./clean.sh $backupdir
	fi
	cd ..
done

cd $workdir
rm -rf backup.sh.lock