#!/bin/bash

User=`whoami`
workdir=$(cd $(dirname $0); pwd)
cd $workdir

# 获取最后一个参数
last_param="${!#}"

# 如果最后一个参数是 "Y"，去除最后一个参数
if [ "$last_param" == "Y" ]
then
    # 去掉最后一个参数，拼接剩余的参数
    set -- "${@:1:$(($#-1))}"  # 移除最后一个参数
    echo "Skipping confirmation..."
else
    # 如果最后一个参数不是 "Y"，进行用户确认
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
fi

# 拼接剩余的参数
grepname="$*"

if [ -f before_stop.sh ]
then
	chmod u+x before_stop.sh
	./before_stop.sh $grepname
fi
cd $workdir

for module in `ls -F | grep "/$"| grep "$grepname" |  awk -F / '{print $1}'`
do
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
		
	if [ -f "stop.sh" ]
	then
		chmod u+x stop.sh
		./stop.sh $appnum
	else
		if [ -f "$appname" ]
		then
			if [ "$appnum" == "" ]
			then
				proc=`ps -fu $User | grep -w "./$appname"| grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
			else
				proc=`ps -fu $User | grep -w "./$appname $appnum"| grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
			fi

			if [ "$proc" == "" ]
			then
				printf "%-14s is not running\n" $module
			else
				echo "Stop $module ..."
				kill -9 $proc
			fi
		fi
	fi
	cd ..
done
