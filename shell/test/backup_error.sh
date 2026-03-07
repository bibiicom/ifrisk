#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

if [ -e $0.lock ]
then
	echo $0 is running,exit!
	exit
fi
touch $0.lock

if [ "$1" == "" ]
then
	backup=$workdir/../backup
else
	backup=$1
fi
mkdir $backup 2>/dev/null

echo Beginto BackUp...
date +%Y%m%d%H%M
thistime=`date +%Y%m%d%H%M`
backupdir=$backup/$thistime
mkdir $backup/$thistime

for module in `ls -F | grep "/$" | awk -F / '{print $1}'`
do
	cd $module
	echo backup $module ...
	if [ -f backup_error.sh ]
	then
		chmod u+x backup_error.sh
		./backup_error.sh $backupdir
	fi
	cd ..
done

cd $workdir
rm -rf $0.lock
