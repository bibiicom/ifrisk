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
	backup_release_ipaddr=`more ../deploy/env.ini | grep backup_release_ipaddr | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
	echo backup_release_ipaddr:$backup_release_ipaddr
	backup_release_userid=`more ../deploy/env.ini | grep backup_release_userid | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
	echo backup_release_userid:$backup_release_userid
	backup=$backup_release_userid@$backup_release_ipaddr
else
	backup=$1
fi

echo Beginto Sync...

thispathall=`pwd`
thispath=${thispathall/$HOME/.}
ssh -n $backup "mkdir $thispath 2>/dev/null"
scp *.sh $backup:$thispath/

maxsnap=`ls -F | grep "/$" | awk -F / '{print $1}' | sort -r | head -n 1`
for module in `ls -F | grep "/$" | grep -v $maxsnap | awk -F / '{print $1}'`
do	
	#echo $module
	checkempty=`ls -F $module`
	if [ "$(ls -A $module)" = "" ]
	then
		echo  $module is empty, do nothing and rm it 
		rm -rf $module
	else
		scp -l 200000 -rp $module $backup:$thispath/
		rm -rf $module
	fi
done

cd $workdir
rm -rf $0.lock
