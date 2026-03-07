#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
ulimit -c unlimited
ulimit  unlimited
umask 0027
User=`whoami`

path=`pwd`
##echo $path
number=${path##*trade}

##echo start $appname $number

if [ "$number" == "" ]
then
	res=`ps -fu $User|grep -w "./trade"|grep -v grep|grep -v bash`
else
	res=`ps -fu $User|grep -w "./trade $number"|grep -v grep|grep -v bash`
fi
if [ "$res" == "" ]
then
	if [ ! -d "snap" ]
	then
		mkdir -p snap/snap.0
	else
		snapnum=`ls snap | wc -l`
		if [ $snapnum -eq 0 ]
		then
			mkdir -p snap/snap.0
		fi
	fi
	echo "starting trade $number ...."
	nohup ./trade $number 1>>nohup.out 2>&1 &
else
	printf "%-14s %s is running\n" trade
fi

if [ "$number" == "" ]
then
	res=`ps -fu $User|grep -w "./trade"|grep -v grep|grep -v bash`
else
	res=`ps -fu $User|grep -w "./trade $number"|grep -v grep|grep -v bash`
fi

if [ ! "$res" == "" ]
then
	rm -rf snapgen.sh.lock
fi
