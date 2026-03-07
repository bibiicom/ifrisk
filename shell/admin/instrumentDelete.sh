#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

echo "input InstrumentID Like:"
read instrID

port=`grep httpfront_trade network.ini | awk -F : '{print $3}' | awk -F / '{print $1}' | head -n 1`

while [ 1 -eq 1 ]; do
	echo All Instruments Like $instrID are belowing:
	echo "curl http://127.0.0.1:$port/action/v1.0/SendQryInstrument"
	echo "---------------------------------------------------------------------"
	echo -e "No PRODG  ExchID   InstrumentID  ExpireTime TimeLeft"
	if [ "$instrID" == "" ]; then
		curl -s -H "Zip:2" http://127.0.0.1:$port/action/v1.0/SendQryInstrument?X=1 | jq -r '.r[].d[] | [.G, .X, .I, .E] | join(" ")' | awk '{printf("%2s %5s %5s %8s %8s \n"),NR,$1,$2,$3,$4}' > ./log/instrumentDelete.tmp
	else
		curl -s -H "Zip:2" http://127.0.0.1:$port/action/v1.0/SendQryInstrument?X=1 | jq -r '.r[].d[] | [.G, .X, .I, .E] | join(" ")' | grep $instrID | awk '{printf("%2s %5s %5s %8s %8s \n"),NR,$1,$2,$3,$4}' > ./log/instrumentDelete.tmp
	fi
	cat ./log/instrumentDelete.tmp
	echo "---------------------------------------------------------------------"
	echo "choose number of Instrument:"
	read choose
	head -n $choose ./log/instrumentDelete.tmp | tail -n 1
	# 提示用户输入删除工具的时间设定
	echo "input when to delete this Instrument:"
	echo "m 5 : 5 minutes later"
	echo "h 2 : 2 hours later"
	echo "d 3 : 3 days later at 16:00"
	echo "a 20250410 12:00:00 : at the exact time of '20250410 12:00:00'"
	read a b c

	case "$a" in
		"m")
			# 使用date命令计算分钟后的Unix时间戳
			ExpireTime=$(date -d "+$b minutes" +%s)
			;;
		"h")
			# 使用date命令计算小时后的Unix时间戳
			ExpireTime=$(date -d "+$b hours" +%s)
			;;
		"d")
			# 这里假设是在当天16:00，可根据需求调整逻辑
			ExpireTime=$(date -d "$b days 16:00" +%s)
			;;
		"a")
			# 将用户输入的日期时间转换为Unix时间戳
			ExpireTime=$(date -d "$b $c" +%s)
			;;
		*)
			ExpireTime=9999999999
			;;
	esac

	ExchangeID=`head -n $choose ./log/instrumentDelete.tmp | tail -n 1 | awk '{print $3}'`
	InstrumentID=`head -n $choose ./log/instrumentDelete.tmp | tail -n 1 | awk '{print $4}'`
	json="{ExchangeID:$ExchangeID,InstrumentID:$InstrumentID,ExpireTime:${ExpireTime}000}"
	echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendUpdate/Instrument"
	echo "are you sure? (y/a:again/c:continue/null:exit)"
	read sure
	case $sure in
	"y")
		curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendUpdate/Instrument
		echo ""	
		;;
	"a")
		./instrumentDelete.sh
		;;
	"c")
		continue
		;;
	*)
		exit
		;;
	esac
done
