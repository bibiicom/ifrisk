#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

errorMsg="";
PortFrom=`more env.ini | grep PortFrom | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
port=`grep httpfront_trade network.ini | awk -F : '{print $3}' | awk -F / '{print $1}' | head -n 1`
mkdir log  2>/dev/null

curl -s -H "Zip:3" http://127.0.0.1:$port/action/v1.0/SendQryMarketData?X=1 | jq -r '.result[] | [.G, .I, .N, .O, .U, .H, .L, .T, .P, .X] | join(" ")' | awk '{printf("\\033[33m%02d\\033[0m %6s %18s \\033[3%sm%8s\\033[0m \\033[4%sm%+6.2f%%\\033[0m %6s \\033[3%sm%8s\\033[0m \\033[3%sm%8s\\033[0m %7.2f%c %7.1fW %6s\n",NR,$1,$2,$4%2==0?1:2,$3,$3<$4?2:1,($3-$4)*100/$4,strftime("%H:%M:%S",$5/1000),$6<$4?2:1,$6,$7<$4?2:1,$7,$8>100000000?$8/100000000:$8/10000,$8>100000000?89:87,$9/10000,$10)}' > ./log/Instruments

number=`less ./log/Instruments | wc -l`

if [ $number -lt 1 ]; then
	echo no Instruments return 0
	exit 0
fi

if [ "$1" == "" ]; then
	page=3
else
	page=$1
fi

from=1
grep=""
tput sc

while [ 1 -eq 1 ]; do
	tput rc
	for ((i=0; i<page; i++)); do
		tput ed
	done
	tput rc
	echo -e "\\033[32mNo  PRODG        InstrumentID LasPrice Price/% UpdtTime HigPrice LowPrice Turnover OpenPosi ExchID\\033[0m"
	to=`expr $from + $page - 1`
	if [ $to -gt $number ]; then
		to=$number
	fi
	tnumber=`expr $to - $from + 1`
	
	if [ "$grep" == "" ]; then
		out=`head -n $to ./log/Instruments | tail -n $tnumber`
	else
		out=`grep "$grep" ./log/Instruments | head -n $to | tail -n $tnumber`
	fi
	
	echo -e "$out"
	echo "choose $tnumber records($grep), page [$from,$to] of $number, b:before/n:next/f:first/e:end/g:grep/q:quit" 
	if [ ! "$errorMsg" == "" ];	then
		echo $errorMsg;
	fi
	if [ "$2" == "" ]; then
		read FilterValue
	else
		FilterValue=l
	fi	
	errorMsg="";
	
	case $FilterValue in
		"")
			curl -s -H "Zip:3" http://127.0.0.1:$port/action/v1.0/SendQryMarketData?X=1 | jq -r '.result[] | [.G, .I, .N, .O, .U, .H, .L, .T, .P, .X] | join(" ")'| awk '{printf("\\033[33m%02d\\033[0m %6s %18s \\033[3%sm%8s\\033[0m \\033[4%sm%+6.2f%%\\033[0m %6s \\033[3%sm%8s\\033[0m \\033[3%sm%8s\\033[0m %7.2f%c %7.1fW %6s\n",NR,$1,$2,$4%2==0?1:2,$3,$3<$4?2:1,($3-$4)*100/$4,strftime("%H:%M:%S",$5/1000),$6<$4?2:1,$6,$7<$4?2:1,$7,$8>100000000?$8/100000000:$8/10000,$8>100000000?89:87,$9/10000,$10)}' > ./log/Instruments
			continue;		
		;;
		"l")
			echo -e "\e[?25l"
			tput clear
			while [ 1 -eq 1 ]
			do
				tput home
				echo -e "\\033[32mNo PRODG        InstrumentID LasPrice Price/% UpdtTime HigPrice LowPrice Turnover OpenPosi ExchID\\033[0m"
				if [ "$grep" == "" ]; then
					out=`curl -s -H "Zip:3" http://127.0.0.1:$port/action/v1.0/SendQryMarketData?X=1 | jq -r '.result[] | [.G, .I, .N, .O, .U, .H, .L, .T, .P, .X] | join(" ")'| awk '{printf("\\033[33m%02d\\033[0m %6s %18s \\033[3%sm%8s\\033[0m \\033[4%sm%+6.2f%%\\033[0m %6s \\033[3%sm%8s\\033[0m \\033[3%sm%8s\\033[0m %7.2f%c %7.1fW %6s\n",NR,$1,$2,$4%2==0?1:2,$3,$3<$4?2:1,($3-$4)*100/$4,strftime("%H:%M:%S",$5/1000),$6<$4?2:1,$6,$7<$4?2:1,$7,$8>100000000?$8/100000000:$8/10000,$8>100000000?89:87,$9/10000,$10)}'`
				else
					out=`curl -s -H "Zip:3" http://127.0.0.1:$port/action/v1.0/SendQryMarketData?X=1 | jq -r '.result[] | [.G, .I, .N, .O, .U, .H, .L, .T, .P, .X] | join(" ")' | grep "$grep"| awk '{printf("\\033[33m%02d\\033[0m %6s %18s \\033[3%sm%8s\\033[0m \\033[4%sm%+6.2f%%\\033[0m %6s \\033[3%sm%8s\\033[0m \\033[3%sm%8s\\033[0m %7.2f%c %7.1fW %6s\n",NR,$1,$2,$4%2==0?1:2,$3,$3<$4?2:1,($3-$4)*100/$4,strftime("%H:%M:%S",$5/1000),$6<$4?2:1,$6,$7<$4?2:1,$7,$8>100000000?$8/100000000:$8/10000,$8>100000000?89:87,$9/10000,$10)}' `
				fi
				echo -e "$out"
				sleep 0.3
			done
		;;
		"q")
			echo abord,return 0
			exit 0
		;;		
		"f")
			from=1
			continue		
		;;
		"e")
			from=`expr $number / $page \* $page`
			continue		
		;;		
		"n")
			if [ $to -eq $number ]; then
				errorMsg="ho to the end, please enter b or number"
				continue
			fi
			from=`expr $from + $page`
		;;
		"b")
			if [ $from -eq 1 ]; then
				errorMsg="to the begin, please enter n or number"
				continue
			fi
			from=`expr $from - $page`
		;;
		"g")
			echo enter grep words:
			read grep
			if [ "$grep" == "" ]; then
				number=`less ./log/Instruments | wc -l`
			else
				number=`grep "$grep" ./log/Instruments | wc -l`
			fi
			from=1
		;;
		*)
			if [[ $FilterValue =~ ^[0-9]+$ ]]; then
				if [ $FilterValue -lt 0 -o $FilterValue -gt $number ]; then
					errorMsg="number must less than $number, please choose again!"	
					continue
				else
					exit $FilterValue
				fi
			else
				errorMsg="not number,please choose again!"
				continue
			fi
		;;
	esac
done
