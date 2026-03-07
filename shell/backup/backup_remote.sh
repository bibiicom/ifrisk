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

echo Beginto BackUp...

thispathall=`pwd`
thispath=${thispathall/$HOME/.}
ssh -n $backup "mkdir $thispath 2>/dev/null"

maxsnap=`ls -F *.tar.gz | awk -F / '{print $1}' | sort -r | head -n 1`
for module in `ls -F *.tar.gz | grep -v $maxsnap`
do
	#echo $module
	scp -l 300000 $module $backup:$thispath/
	rm -rf $module
done

cd $workdir
rm -rf $0.lock
