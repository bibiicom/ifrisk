#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
ulimit -c unlimited
ulimit  unlimited
umask 0027

workdir=$(cd $(dirname $0); pwd)
cd $workdir

#in case of reading while trade is writing,ignore the max snap

maxsnap=`ls -d snap/snap.* 2>/dev/null | awk -F . '{print $2}' | sort -r -n | head -n 1`
if [ "$maxsnap" == "0" -o "$maxsnap" == "" ]
then
	echo only snap.0 or no snap no need to do!
	exit
fi
echo maxsnap is $maxsnap

minsnap=`ls -d snap/check.snap.* 2>/dev/null | awk -F . '{print $3}' | sort -r -n | head -n 1`
if [ "$minsnap" == "" ]
then
	minsnap=0
fi
echo minsnap is $minsnap

if [ -e $0.lock ]
then
	echo $0 is running,exit!
	exit
fi
touch $0.lock

for snap in `ls -d snap/snap.*0 2>/dev/null`
do
	snapname=${snap#snap/}
	if [ -f snap/check.$snapname ]
	then
		echo skip $snapnum: check.$snapname exist
		continue
	fi
	snapnum1=${snap#snap/snap.}
	snapnum=${snapnum1%%.*}
	if [ $snapnum -eq $maxsnap ]
	then
		echo skip $snapnum: maxsnap $maxsnap
		continue
	fi
	if [ $snapnum -le $minsnap ]
	then
		echo skip $snapnum: smaller than $minsnap
		continue
	fi
	chmod u+x snapcheck
	./snapcheck $snap snap/check.$snapname
done

rm -rf $0.lock

