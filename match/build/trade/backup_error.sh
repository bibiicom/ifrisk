#!/bin/bash

./backup.sh $*

backupdir=$1/trade

for each in `ls $backupdir/snap/diff.snap.* 2>/dev/null`
do
	echo $backupdir/snap/diff.snap.* exsit,exit!
	exit
done

for each in `ls $backupdir/snap/check.snap.* 2>/dev/null`
do
	echo $backupdir/snap/check.snap.* exsit,exit!
	exit
done

for each in `ls $backupdir/snap/init.*.* 2>/dev/null`
do
	echo $backupdir/snap/init.snap.* exsit,exit!
	exit
done

echo rm $backupdir/	
rm -rf $backupdir
