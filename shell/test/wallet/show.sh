#!/bin/bash
workdir=$(cd $(dirname $0); pwd)
cd $workdir

res=`ps ux |grep -w "./wallet.sh"|grep -v grep|awk '{printf "%-7s %-6s %-5s %-7s %-7s",$2,$3,$4,$9,$10}'`

if [ ! "$res" == "" ]
then
	md5=`md5sum wallet.sh | cut -d " " -f1`
	md5x=${md5:27}
	version=`cat version.sh`
	printf "%s %s:%s\n" "$res" $md5x $version
else
	printf "\n" 
fi


