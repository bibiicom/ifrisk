#!/bin/bash

if [ ! "$1" == "Y" ]
then
	read -r -p "Are You Sure? [Y/n] " input

	case $input in
		[yY][eE][sS]|[yY])
			echo "Yes"
			;;

		[nN][oO]|[nN])
			echo "No"
			exit 1
			;;

		*)
			echo "Invalid input..."
			exit 1
			;;
	esac
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

echo $grepname

for module in `ls -F ../run/ | grep "/$" | grep "$grepname" |  awk -F / '{print $1}'`
do
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

	printf "publish %12s %2s ----------------------------------------------\n" $appname $appnum
	User=`whoami`
	if [ "$appnum" == "" ]
	then
		proc=`ps -fu $User | grep -w "./$appname"| grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
	else
		proc=`ps -fu $User | grep -w "./$appname $appnum"| grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
	fi
	if [ ! "$proc" == "" ]
	then
		printf "%-14s is running\n" $module
		continue
	fi

	for run in `ls ../test/$appname/*.so  ../test/$appname/*.so.* 2>/dev/null`
	do
		cp -pv $run ../run/$module/
	done

	if [ -f ../run/$module/omq.ini ]
	then
		mkdir ../run/$module/flow 2>/dev/null
	fi

	if [ -f ../test/$appname/publish.sh ]
	then
		chmod u+x ../test/$appname/publish.sh
		../test/$appname/publish.sh ../../run/$module/
	fi
	
	for run in ../test/$appname/*
	do
		binfile=`file $run | grep -v "shell script" | grep executable`
		if [ ! "$binfile" == "" ] 
		then
			if [[ ! $run == *.exe ]] 
			then
				#echo "   " cp $run 
				chmod u+x $run
				cp -pv $run ../run/$module/
				if [ ! $? -eq 0 ]
				then
					sleep 3
					cp -pv $run ../run/$module/
				fi
			fi
		fi
	done	
done

../run/chkver.sh $grepname
