#!/bin/bash

##leave last $keepnum inputall.*.flow
keepnum=20
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

echo backup_remote until $backnum ================

thispathall=`pwd`
thispath=${thispathall/$HOME/.}

ssh -n $1 "mkdir $thispath 2>/dev/null"
ssh -n $1 "mkdir $thispath/flow 2>/dev/null"
ssh -n $1 "mkdir $thispath/snap 2>/dev/null"
ssh -n $1 "mkdir $thispath/inc 2>/dev/null"
scp flow/flowviewer  $1:$thispath/flow/
scp flow/libdataswap.so  $1:$thispath/flow/
scp flow/libdataswap.so  $1:$thispath/
scp backup.sh $1:$thispath/
scp *.ini $1:$thispath/
scp trade $1:$thispath/
scp snapgen.sh $1:$thispath/
scp snapcheck.sh $1:$thispath/
scp snapdiff.sh $1:$thispath/
scp snapcheck $1:$thispath/
scp *.so $1:$thispath/
ssh -n $1 "chmod u+x $thispath/*.sh"

for each in `ls flow/inputall.*.flow 2>/dev/null`
do
	thisnum1=${each#flow/inputall.}
	thisnum=${thisnum1%%.*}

	if [ $thisnum -lt $backnum ]
	then
		echo mv $each to $1:$2/flow
		scp -rp $each  $1:$thispath/flow/
		if [ $? -eq 0 ]
		then
			echo del $each
			rm -rf $each
			if [ -d snap/snap.$thisnum ]
			then
				echo scp snap/snap.$thisnum $1:$thispath/snap/
				scp -rp snap/snap.$thisnum $1:$thispath/snap/
				if [ $? -eq 0 ]
				then	
					echo del snap/snap.$thisnum
					rm -rf snap/snap.$thisnum
				else
					echo ERROR: scp snap/snap.$thisnum failed!
				fi
			else
				echo scp snap/snap.$thisnum.* $1:$thispath/snap/
				scp -rp snap/snap.$thisnum.* $1:$thispath/snap/
				if [ $? -eq 0 ]
				then	
					echo del snap/snap.$thisnum.*
					rm -rf snap/snap.$thisnum.*
				else
					echo ERROR: scp snap/snap.$thisnum.* failed!
				fi
			fi				
		else
			echo ERROR: scp $each failed!
		fi
	fi
done

for each in `ls -d snap/snap.*.Stop 2>/dev/null`
do
	thisnum1=${each#snap/snap.}
	thisnum=${thisnum1%%.*}
	if [ $thisnum -lt $backnum ]
	then
		echo scp $each $1:$thispath/snap/
		scp -rp $each $1:$thispath/snap/
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

for each in `ls -d snap/init.*.* 2>/dev/null`
do
	thisnum1=${each#snap/init.*.}
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
		echo rm $each
		rm -rf $each
	fi
done

rm -rf $0.lock
