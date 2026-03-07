#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

orderDepth=8
tradeDepth=5
tradeDepthDouble=`expr $tradeDepth + 10`
tailCount=100

port=`grep httpfront_trade network.ini | awk -F : '{print $3}' | awk -F / '{print $1}' | head -n 1`
wsport=`grep wsfront_trade network.ini | awk -F "=" '{print $NF}' | awk -F "|" '{print $NF}' | head -n 1`
chmod u+x wsclient

if [ "$1" == "" ]; then
	chmod u+x market.sh
	./market.sh 10
	choose=$?
	if [ "$choose" == "0" ]
	then
		exit
	else
		ExchangeID=`head -n $choose ./log/Instruments | tail -n 1 | awk '{print $15}'`
		InstrumentID=`head -n $choose ./log/Instruments | tail -n 1 | awk '{print $3}'`
	fi
else
		ExchangeID=`more env.ini | grep ExchangeID | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
		InstrumentID=$1
fi

User=`whoami`
proc=`ps -fu $User | grep -w "./wsclient" | grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
if [ "$proc" == "" ]
then
	json="{SendTopicAction:{Action:1,LocalNo:4,ResumeNo:-1,TopicID:2,Zip:4}}"
	echo ./wsclient ws://127.0.0.1:$wsport $json
	#stdbuf -o0 ./wsclient ws://127.0.0.1:$wsport $json | grep --line-buffered $InstrumentID > log/lastTrade &
	stdbuf -o0 ./wsclient ws://127.0.0.1:$wsport $json > log/lastTrade &
fi

echo -e "\e[?25l"
tput clear

while [ "1" == "1" ]
do
	tput home
	#echo " ====== ${ExchangeID}_$InstrumentID ======"
	title=${ExchangeID}_$InstrumentID
	title1=`expr 10 - ${#title} / 2`
	title2=`expr 10 - \( ${#title} + 1 \) / 2`
	#echo printf \"=====%-*s%s%*s=====\\n\" $title1 \" \" $title $title2 \" \"
	printf "=====%*s%s%*s=====\n" $title1 " " $title $title2 " "
	echo " [=     Price  |  Vloume     ]"
	
	tmp=`curl -s -d "{ExchangeID:$ExchangeID,InstrumentID:$InstrumentID,Depth:$orderDepth}" -H "Zip:4" -X POST http://127.0.0.1:$port/action/v1.0/SendQryMarketOrder`
	out=`echo $tmp | sed "s@\"$InstrumentID\",\"1\",@\n@g" | tail -n +2 | sort -r | awk -F ']|,' '{printf(" [\\033[31m+%10s\\033[0m  |  %-11s]  \\n",$1,$2)}'`
	echo -e "$out"
	echo " ----------------------------- "
	out=`tail -n 100 log/lastTrade | grep $InstrumentID | tail -n 1 | awk -F ',|]' '{if($7==1) printf(" [\\033[32m↓%9s  |  %-11s\\033[0m]  \\n",$8,$9); else printf(" [\\033[31m↑%9s  |  %-11s\\033[0m]\\n",$8,$9)}'`
	echo -e "$out"
	echo " -----------------------------"
	out=`echo $tmp | sed "s@\"$InstrumentID\",\"0\",@\n@g" | tail -n +2 | awk -F ']|,' '{printf(" [\\033[32m-%10s\\033[0m  |  %-11s]  \\n",$1,$2)}'`
	echo -e "$out"
	echo " ============================="

	out=`tail -n $tailCount log/lastTrade | grep $InstrumentID | awk -F ',|]' '{print $7,$8,$10}' |uniq -c | tac | head -n $tradeDepth | awk '{ if ($2==1) printf(" |%s  \\033[32m- %-9s %4s\\033[0m |\\n",strftime("%H:%M:%S",$4/1000),$3,$1); else printf(" |%s  \\033[31m+ %-9s %4s\\033[0m |\\n",strftime("%H:%M:%S",$4/1000),$3,$1)}'`
	echo -e "$out"	
	
	lastTradeCount=`tail -n $tailCount log/lastTrade | grep $InstrumentID | awk -F ',|]' '{print $7,$8,$10}' |uniq -c | tac | wc -l`
	sleeptimes=`expr $tailCount / 1000`
	sleeptime=`echo "scale=2; $sleeptimes + 0.3" | bc`
	#echo "$sleeptime:$tailCount/$lastTradeCount      "
	if [ $lastTradeCount -lt $tradeDepth ]
	then
		tailCount=`expr $tailCount + 200`
		if [ $tailCount -gt 100000 ]
		then
			User=`whoami`
			proc=`ps -fu $User | grep -w "./wsclient" | grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
			kill -9 $proc
			#rm -rf log/lastTrade
			json="{SendTopicAction:{Action:1,LocalNo:4,ResumeNo:-1,TopicID:2,Zip:4}}"
			#echo ./wsclient ws://127.0.0.1:$wsport $json
			stdbuf -o0 ./wsclient ws://127.0.0.1:$wsport $json > log/lastTrade &
		fi
	fi
	if [ $lastTradeCount -gt $tradeDepthDouble ]
	then
		tailCount=`expr $tailCount - 10`
	fi
	sleep $sleeptime
done
