#!/bin/bash

User=`whoami`
proc=`ps -fu $User | grep -w "./trade"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]
then
	echo  trade is running exit!
	exit
fi

if [ -e backup.sh.lock ]
then
	echo backup is running,exit!
	exit
fi

if [ -e backup_remote.sh.lock ]
then
	echo backup_remote is running,exit!
	exit
fi

touch backup_remote.sh.lock
touch backup.sh.lock

User=`whoami`
touch snapdiff.sh.lock
proc=`ps -fu $User | grep snapdiff.sh | grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]
then
	echo stop snapdiff.sh ...
	kill -9 $proc
fi

touch snapcheck.sh.lock
proc=`ps -fu $User | grep snapcheck.sh | grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]
then
	echo stop snapcheck.sh ...
	kill -9 $proc
fi

proc=`ps -fu $User | grep ./snapcheck | grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
if [ ! "$proc" == "" ]
then
	echo stop snapcheck ...
	kill -9 $proc
fi

maxsnap=`ls -d snap/snap.* 2>/dev/null | grep -v ".redo" | awk -F . '{print $2}' | sort -r -n | head -n 1`
if [ ! "$maxsnap" == "" ]
then
	if [ -d snap/snap.$maxsnap ]
	then
		echo cp -r snap/snap.$maxsnap snap.backup
		cp -r snap/snap.$maxsnap snap.backup
	else
		echo cp -r snap/snap.$maxsnap.* snap.backup
		cp -r snap/snap.$maxsnap.* snap.backup
	fi
fi

if [ ! "$1" == "" ]
then
	rm -rf backup.sh.lock
	./backup.sh $1 0
	touch backup.sh.lock
fi

rm -rf flow/*.flow
rm -rf flow/*.txt
rm -rf flow/*.*log
rm -rf snap/*
rm -rf *.log
rm -rf dump/*
rm -rf nohup.out
mv snap.backup snap/snap.0

rm -rf backup.sh.lock
rm -rf backup_remote.sh.lock
rm -rf snapcheck.sh.lock
rm -rf snapdiff.sh.lock
