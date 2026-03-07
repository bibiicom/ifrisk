#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

if [ "$1" == "" ]
then
	backup_release_ipaddr=`more deploy/env.ini | grep backup_release_ipaddr | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
	echo backup_release_ipaddr:$backup_release_ipaddr
	backup_release_userid=`more deploy/env.ini | grep backup_release_userid | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
	echo backup_release_userid:$backup_release_userid
	backup=$backup_release_userid@$backup_release_ipaddr
else
	backup=$1
fi

echo Beginto BackUp...

ssh -n $backup "mkdir backup 2>/dev/null"
scp ../test/tools/gzip_dir.sh $backup:./backup/ 2>/dev/null
scp ../test/tools/clean_dir.sh $backup:./backup/ 2>/dev/null
ssh -n $backup "chmod u+x ./backup/*.sh"

ssh -n $backup "mkdir redo redo/trade redo/backup 2>/dev/null"
scp ../redo/*.sh $backup:./redo/
scp ../redo/trade/*.sh $backup:./redo/trade/
scp ../redo/backup/*.sh $backup:./redo/backup/
ssh -n $backup "chmod u+x ./redo/*.sh ./redo/trade/*.sh ./redo/backup/*.sh"

thispathall=`pwd`
thispath=${thispathall/$HOME/.}
ssh -n $backup "mkdir $thispath 2>/dev/null"
scp backup.sh $backup:$thispath/
ssh -n $backup "chmod u+x $thispath/backup.sh"
