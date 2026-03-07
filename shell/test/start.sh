#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
ulimit -c unlimited
ulimit  unlimited
umask 0027
User=`whoami`

if [ ! "$1" == "Y" ]
then
	if [ "$2" == "" ]
	then
		grepname=$1
	else
		if [ "$2" -ge 0 ] 2>/dev/null 
		then
			grepname=$1$2
		else
			grepname=`echo ${1}_$2`
		fi
	fi
else
	if [ "$3" == "" ]
	then
		grepname=$2
	else
		if [ "$3" -ge 0 ] 2>/dev/null 
		then
			grepname=$2$3
		else
			grepname=`echo ${2}_$3`
		fi
	fi
fi

for module in `ls -F | grep "/$" | grep "$grepname" | awk -F / '{print $1}'`
do
	if [ "$module" == "tools" ]; then
		continue
	fi
	cd $module
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
	
	#echo start $appname $appnum
	#cd ..
	#continue
	if [ -f "start.sh" ]
	then
		chmod u+x start.sh
		./start.sh $appnum
	else
		if [ -f "$appname" ]
			then
			if [ "$appnum" == "" ]
			then
				res=`ps -fu $User|grep -w "./$appname"|grep -v grep|grep -v bash`
			else
				res=`ps -fu $User|grep -w "./$appname $appnum"|grep -v grep|grep -v bash`
			fi
			if [ "$res" == "" ]
			then
				echo "starting $appname $appnum"
				nohup ./$appname $appnum 1>>nohup.out 2>&1 &
			else
				printf "%-14s %s is running\n" $module
			fi
		fi
	fi
	cd ..
done

