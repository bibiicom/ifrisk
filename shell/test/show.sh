#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

printf "%2s %-13s %-7s %-6s %-5s %-7s %-7s %s\n" No Module Pid Cpu% Mem% Start Time+ Version
num=1
for module in `ls -F | grep "/$"| awk -F / '{print $1}'`
do
	if [ "$module" == "tools" ]; then
		continue
	fi
	cd $module
	if [ -f "show.sh" ]
	then
		chmod u+x show.sh
		printf "%2s %-13s " $num $module
		./show.sh
	else
		appnum=${module##*_}
		if [ "$appnum" == "$module" ]
		then
			appnum=${module##*[a-zA-Z]}
			appname=${module%$appnum}
		else
			appname=${module%_$appnum}
		fi
		
		if [ "$appnum" == "module" ]
		then
			appnum=""
		fi
	
		if [ "$appnum" == "" ]
		then
			res=`ps ux |grep -w "./$appname" | grep -v grep|grep -v bash|awk '{printf "%-7s %-6s %-5s %-7s %-7s",$2,$3,$4,$9,$10}'`
		else
			res=`ps ux |grep -w "./$appname $appnum" | grep -v grep|grep -v bash|awk '{printf "%-7s %-6s %-5s %-7s %-7s",$2,$3,$4,$9,$10}'`
		fi
		if [ ! "$res" == "" ]
		then
			md5=`md5sum $appname | cut -d " " -f1`
			md5x=${md5:27}
			version=`./$appname -v`
			versionx=`echo $version | awk -F ":" '{printf $2}'`
			printf "%2s %-13s %s %s:%s\n" $num $module "$res" $md5x $versionx
		else
			printf "%2s %-13s %s\n" $num $module
		fi
		
	fi
	num=`expr $num + 1`
	cd ..
done
