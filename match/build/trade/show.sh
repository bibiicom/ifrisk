#!/bin/bash
workdir=$(cd $(dirname $0); pwd)
cd $workdir

path=`pwd`
number=${path##*trade}

if [ "$number" == "" ]
then
	res=`ps ux |grep -w "./trade"|grep -v '\-r'|grep -v grep|grep -v bash|awk '{printf "%-7s %-6s %-5s %-7s %-7s",$2,$3,$4,$9,$10}'`
else
	res=`ps ux |grep -w "./trade $number"|grep -v \-r|grep -v grep|grep -v bash|awk '{printf "%-7s %-6s %-5s %-7s %-7s",$2,$3,$4,$9,$10}'`
fi
if [ ! "$res" == "" ]
then
	md5=`md5sum trade | cut -d " " -f1`
	md5x=${md5:27}
	version=`./trade -v`
	versionx=`echo $version | awk -F ":" '{printf $2}'`
	printf "%s %s:%s\n" "$res" $md5x $versionx 
else
	printf "\n" 
fi


