#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

echo Beginto CleanLog...
date +%Y%m%d%H%M%S

keepnum=80
grepname=$1$2	

if [ ! "$1" == "" ]
then
	tmpsint=`echo $1|sed 's/[0-9]//g'`
	if [ "$tmpsint" == "" ]
	then
		grepname=""
		keepnum=$1
	fi
fi

echo keepnum is $keepnum
echo grepname is $grepname

for module in `ls -F | grep "/$" | grep "$grepname" | grep -v tools| awk -F / '{print $1}'`
do
	cd $module
	echo --------------------------------------------------------
	echo clean $module ...
	
	for logname in `ls flow/input_*.flow flow/output*.flow flow/*.*log *.log 2>/dev/null | awk -F . '{print $1}' | uniq`
	do
		nCount=0
		for count in `ls $logname.*.* 2>/dev/null | awk -F . '{print $2}' | sort -r -n`
		do
			nCount=`expr $nCount + 1`
			if [ $nCount -gt $keepnum ]
			then
				echo rm $logname.$count.*
				rm -rf $logname.$count.*
			fi
		done
	done

	for logname in `ls dump/*Failed.csv nohup.out 2>/dev/null`
	do
		nCount=`wc -l $logname | awk '{print $1}'`
		echo $logname $nCount
		if [ $nCount -gt 1000000 ]
		then
			echo cleaning $logname
			echo "" > $logname
			##cp -v /dev/null $logname
		fi
	done
	
	cd ..
done
