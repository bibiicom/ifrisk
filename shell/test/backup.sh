#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

if [ -e $0.lock ]
then
	echo $0 is running,exit!
	exit
fi
touch $0.lock

backup=$workdir/../backup

if [ "$1" == "" ]
then
	keepnum=20
else
	keepnum=$1
fi
mkdir $backup 2>/dev/null

echo Beginto BackUp...
date +%Y%m%d%H%M
thistime=`date +%Y%m%d%H%M`
backupdir=$backup/$thistime
mkdir $backup/$thistime
cp $workdir/../test/backup/* $backup/ 2>/dev/null
chmod u+x $backup/*.sh

for module in `ls -F | grep "/$" | awk -F / '{print $1}'`
do
	cd $module
	echo backup $module ...
	if [ -f backup.sh ]
	then
		chmod u+x backup.sh
		./backup.sh $backupdir $keepnum
	fi
	cd ..
done

cd $workdir
rm -rf $0.lock
