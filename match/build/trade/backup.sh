#!/bin/bash

if [ "$1" == "" ]
then
	echo no backupdir exit!
	exit
fi

backupdir=$1/trade

##leave last $keepnum inputall.*.flow
keepnum=20
if [ ! "$2" == "" ]
then
	keepnum=$2
fi

backnum=999999999999999999

nCount=0
for each in `ls -d flow/inputall.*.flow 2>/dev/null | awk -F . '{print $2}' | sort -r -n`
do
	if [ $nCount -gt $keepnum ]
	then
		break
	fi
	nCount=`expr $nCount + 1`
	backnum=$each
done

if [ $nCount -le $keepnum ]
then
	echo no need to backup: inputflow Count[$nCount] "<=" KeepNum[$keepnum]
	exit
fi

if [ -e $0.lock ]
then
	echo $0 is running,exit!
	exit
fi
touch $0.lock

echo backup until $backnum ================

mkdir $backupdir
mkdir $backupdir/snap
mkdir $backupdir/flow
cp flow/flowviewer $backupdir/flow/
cp flow/libdataswap.so $backupdir/flow/
cp omq.ini $backupdir/
cp trade.ini $backupdir/
cp trade $backupdir/
cp snapgen.sh $backupdir/
cp snapcheck.sh $backupdir/
cp snapdiff.sh $backupdir/
cp snapcheck $backupdir/
cp *.so $backupdir/
cp backup_error.sh $backupdir/

for each in `ls flow/inputall.*.flow 2>/dev/null`
do
	thisnum1=${each#flow/inputall.}
	thisnum=${thisnum1%%.*}
	if [ $thisnum -eq $backnum ]
	then
		if [ -d snap/snap.$thisnum ]
		then
			echo cp -rp snap/snap.$thisnum $backupdir/snap/
			cp -rp snap/snap.$thisnum $backupdir/snap/
		else
			echo cp -rp snap/snap.$thisnum.* $backupdir/snap/
			cp -rp snap/snap.$thisnum.* $backupdir/snap/
		fi
	else
		if [ $thisnum -lt $backnum ]
		then
			echo mv $each to $backupdir/flow/
			mv $each $backupdir/flow/
			if [ -d snap/snap.$thisnum ]
			then
				echo mv snap/snap.$thisnum $backupdir/snap/
				mv snap/snap.$thisnum $backupdir/snap/
			else
				echo mv snap/snap.$thisnum.* $backupdir/snap/
				mv snap/snap.$thisnum.* $backupdir/snap/
			fi
		fi
	fi
done

for each in `ls -d snap/snap.*.redo 2>/dev/null`
do
	thisnum1=${each#snap/snap.}
	thisnum=${thisnum1%%.*}
	if [ $thisnum -lt $backnum ]
	then
		echo rm $each
		rm -rf $each
	fi
done

for each in `ls -d snap/snap.*.Stop 2>/dev/null`
do
	thisnum1=${each#snap/snap.}
	thisnum=${thisnum1%%.*}
	if [ $thisnum -lt $backnum ]
	then
		echo mv $each to $backupdir/snap/
		mv $each $backupdir/snap/
	fi
done

for each in `ls -d snap/snap.* 2>/dev/null`
do
	thisnum1=${each#snap/snap.}
	thisnum=${thisnum1%%.*}
	if [ $thisnum -lt $backnum ]
	then
		echo rm $each
		rm -rf $each
	fi
done

for each in `ls -d snap/inc.* 2>/dev/null`
do
	thisnum1=${each#snap/inc.}
	thisnum=${thisnum1%%.*}
	if [ $thisnum -lt $backnum ]
	then
		echo rm $each
		rm -rf $each
	fi
done

for each in `ls snap/diff.snap.* 2>/dev/null`
do
	thisnum1=${each#snap/diff.snap.}
	thisnum=${thisnum1%%.*}
	if [ $thisnum -lt $backnum ]
	then
		if [ -s $each ]
		then
			echo mv $each
			mv $each $backupdir/snap/
		else
			echo rm $each
			rm -rf $each			
		fi
	fi
done

for each in `ls snap/check.snap.* 2>/dev/null`
do
	thisnum1=${each#snap/check.snap.}
	thisnum=${thisnum1%%.*}
	if [ $thisnum -lt $backnum ]
	then
		if [ -s $each ]
		then
			echo mv $each
			mv $each $backupdir/snap/
		else
			echo rm $each
			rm -rf $each			
		fi
	fi
done

for each in `ls snap/init.*.* 2>/dev/null`
do
	thisnum1=${each#snap/init.*.}
	thisnum=${thisnum1%%.*}
	if [ $thisnum -lt $backnum ]
	then
		if [ -s $each ]
		then
			echo mv $each
			mv $each $backupdir/snap/
		else
			echo rm $each
			rm -rf $each			
		fi
	fi
done

rm -rf $0.lock
