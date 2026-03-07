#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

port=`grep httpfront_trade network.ini | awk -F : '{print $3}' | awk -F / '{print $1}' | head -n 1`

echo "please input MemberID:"
read MemberID

AccountID=$MemberID

choose=x
while [ 1 -eq 1 ]
do
	json="{MemberID:$MemberID}"
	tput clear
	tput home
	echo -e "=========================================  \\033[31m[  $MemberID  ]\\033[0m  =========================================="
	echo -e "\\033[33m[   Orders]:\\033[0m" "curl -s -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryOrder"
	echo -e "\\033[32mNo        InstrumentID  b/s   o/c     Price  Volume    InsertTime\\033[0m"
	out=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryOrder | sed "s/data\":/\n/g"| head -n 8 | tail -n +2 | sed "s/\"//g" | awk -F ":|," '{b="\\033[31mbuy \\033[0m";s="\\033[32msell\\033[0m";o="open ";c="close";printf("\\033[33m%2s\\033[0m %19s  %s %s %8s  %-11s %s\n",NR,$12,$16==0?b:s,$18==0?o:c,$20,$22,strftime("%H:%M:%S",$76/1000))}'`
	echo -e "$out"
	echo "-----------------------------------------------------------------------------------------------"
	echo -e "\\033[33m[Positions]:\\033[0m" "curl -s -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryPosition"
	echo -e "\\033[32mNo        InstrumentID  l/s CM Position LeX  UseMargin MaitMargin UnReProfit  CostPrice LiqudPrice SLTriPrice TPTriPrice\\033[0m"	
	out=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryPosition | sed "s/data\":/\n/g"| head -n 14 | tail -n +2 | sed "s/\"//g" |awk -F ":|," '{l="\\033[31mlong \\033[0m";s="\\033[32mshort\\033[0m";printf("\\033[33m%2s\\033[0m %19s %s %c %8s %3s %10s %10s %10s %10s %10s %10s %10s\n",NR,$10,$16==1||$20<0?s:l,$72==0?87:89,$20,$60,$38,$92,$94,$36,$96,$74,$76)}'`
	echo -e "$out"
	if [ ! "$choose" == "0" ]
	then
		echo "-----------------------------------------------------------------------------------------------"
		echo -e "\\033[33m[ Accounts]:\\033[0m" "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryAccount"
		echo -e "\\033[32mNo   SG Currency  Available   UnRealProfit FrozenMargin   FrozenMoney     FrozenFee       Balance     Borrow\\033[0m"
		outAccount=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryAccount | sed "s/:{/\n/g"|sed "s/\"//g" | head -n 12 | tail -n +2 | awk -F ",|:" '{printf("\\033[33m%2s\\033[0m %4s %6s %12.2f +%12.2f +%12.2f +%12.2f +%12.2f =%12.2f %10.2f\n",NR,$2,$6,$10,$82,$30,$32,$34,$16,$88)}'`
		echo -e "$outAccount"
		echo "-----------------------------------------------------------------------------------------------"
		echo -e "输入\\033[33m[下面的标的数字]\\033[0m:下单 或者 输入\\033[33m[q]\\033[0m:撤单/平仓 "
		echo "-----------------------------------------------------------------------------------------------"
		chmod u+x market.sh
		./market.sh 10
		choose=$?
		tput cnorm
	else
		echo "-----------------------------------------------------------------------------------------------"
	fi
	
	if [ "$choose" == "0" ]
	then
		echo -e "\\033[33mc:\\033[0m 撤单 OrderDelete  \\033[33mp:\\033[0m 平仓 PositionClose  \\033[33mq:\\033[0m 还原"
		echo -e "example: c 2019283/all  p 2019283/all  q"
		read Action number
		case "$Action" in
			"c")
			{
				if [ "all" == "$number" ]
				then
					json="{MemberID:$MemberID,ActionFlag:1}"
					echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendOrderAction"
					curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendOrderAction
					read waiting
				else
					if [[ $number =~ ^[0-9]+$ ]]
					then
						OrderID=`echo $orders | sed "s/\[/\n/g"| tail -n +4 | awk -F , '{print NR,$4,$6,$7,$8,$10,$11}' | head -n $order | tail -n 1 | awk '{print $2}'`
						json="{OrderID:$OrderID,ActionFlag:1}"
						echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendOrderAction"
						curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendOrderAction
						read waiting
					fi
				fi
			}
			;;
			"p")
			{
				echo "input PositionID(null to quit):"
				positions=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryPosition`
				echo $positions | sed "s/\[/\n/g"| tail -n +4 | awk -F , '{print "  " NR,$1,$5,$7,$8,$10,$11}'
				read position
				if [[ $position =~ ^[0-9]+$ ]]
				then
					PositionID=`echo $positions | sed "s/\[/\n/g"| tail -n +4 | awk -F , '{print NR,$1,$5,$7,$8,$10,$11}' | head -n $position | tail -n 1 | awk '{print $2}'`
					echo "input Price(null to quit):"
					read Price
					echo "input Volume(null to close all):"
					read Volume
					if [ "" == "$Volume" ]
					then
						json="{PositionID:$PositionID,OffsetFlag:1,Price:$Price}"
					else
						json="{PositionID:$PositionID,OffsetFlag:1,Price:$Price,Volume:$Volume}"
					fi	
					echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendOrderInsert"
					curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendOrderInsert
					read waitForInput
				fi		
			}
			;;
			"q")
				choose=x
			;;
		esac	
	else
		ExchangeID=`head -n $choose ./log/Instruments | tail -n 1 | awk '{print $15}'`
		InstrumentID=`head -n $choose ./log/Instruments | tail -n 1 | awk '{print $3}'`
		QryInstrument=`curl -s -d "{ExchangeID:$ExchangeID,InstrumentID:$InstrumentID}" -X POST http://127.0.0.1:$port/action/v1.0/SendQryInstrument | sed "s/\"//g"`
		SettlementGroup=`echo $QryInstrument | awk -F ",|:" '{print $20}' `
		ClearCurrency=`echo $QryInstrument | awk -F ",|:" '{print $42}' `
		BaseCurrency=`echo $QryInstrument | awk -F ",|:" '{print $46}' `
		ProductClass=`echo $QryInstrument | awk -F ",|:" '{print $26}' `
		while [ "1" == "1" ]
		do
			tput clear
			tput home
			echo -e "=========================================  \\033[31m[  $MemberID  ]\\033[0m  =========================================="
			json="{MemberID:$MemberID,ExchangeID:$ExchangeID,InstrumentID:$InstrumentID}"		
			echo -e "\\033[33m[   Orders]:\\033[0m" "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryOrder"
			echo -e "\\033[32mNo       InstrumentID  b/s   o/c     Price  Volume  InsrTime\\033[0m"
			out=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryOrder | sed "s/data\":/\n/g"| head -n 8 | tail -n +2 | sed "s/\"//g" | awk -F ":|," '{b="\\033[31mbuy \\033[0m";s="\\033[32msell\\033[0m";o="open ";c="close";printf("%2s %19s  %s %s %8s  %-11s  %s\n",NR,strftime("%H:%M:%S",$76/1000),$12,$16==0?b:s,$18==0?o:c,$20,$22)}'`
			echo -e "$out"
			echo "-----------------------------------------------------------------------------------------------"
			echo -e "\\033[33m[Positions]:\\033[0m" "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryPosition"
			echo -e "\\033[32mNo        InstrumentID  l/s CM Position LeX  UseMargin MaitMargin UnReProfit  CostPrice LiqudPrice SLTriPrice TPTriPrice\\033[0m"	
			out=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryPosition | sed "s/data\":/\n/g"| head -n 14 | tail -n +2 | sed "s/\"//g" |awk -F ":|," '{l="\\033[31mlong \\033[0m";s="\\033[32mshort\\033[0m";printf("%2s %19s %s %c %8s %3s %10s %10s %10s %10s %10s %10s %10s\n",NR,$10,$16==1||$20<0?s:l,$72==0?87:89,$20,$60,$38,$92,$94,$36,$96,$74,$76)}'`
			echo -e "$out"
			echo "-----------------------------------------------------------------------------------------------"
			json="{MemberID:$MemberID,AccountID:$AccountID,SettlementGroup:$SettlementGroup,Currency:$ClearCurrency}"
			echo -e "\\033[33m[ Accounts]:\\033[0m" "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryAccount"
			echo -e "\\033[32mNo   SG Currency  Available   UnRealProfit FrozenMargin   FrozenMoney     FrozenFee       Balance     Borrow\\033[0m"
			outAccount=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryAccount | sed "s/:{/\n/g"|sed "s/\"//g" | head -n 12 | tail -n +2 | awk -F ",|:" '{printf("%2s %4s %6s %12.2f +%12.2f +%12.2f +%12.2f +%12.2f =%12.2f %10.2f\n",NR,$2,$6,$10,$82,$30,$32,$34,$16,$88)}'`
			echo -e "$outAccount"
			if [ "$ProductClass" == "4" ]
			then
				json="{MemberID:$MemberID,AccountID:$AccountID,SettmentGroup:$SettmentGroup,Currency:$BaseCurrency}"
				outAccount=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryAccount | sed "s/:{/\n/g"|sed "s/\"//g" | head -n 12 | tail -n +2 | awk -F ",|:" '{printf("%2s %4s %6s %12.2f +%12.2f +%12.2f +%12.2f +%12.2f =%12.2f %10.2f\n",NR,$2,$6,$10,$82,$30,$32,$34,$16,$88)}'`
				echo -e "$outAccount"			
			fi
			title=${ExchangeID}_$InstrumentID
			title1=`expr 10 - ${#title} / 2`
			title2=`expr 10 - \( ${#title} + 1 \) / 2`
			printf "=====%*s%s%*s=====\n" $title1 " " $title $title2 " "
			echo " [=    Price    |  Vloume     ]"		
			tmp=`curl -s -d "{ExchangeID:$ExchangeID,InstrumentID:$InstrumentID,Depth:5}" -H "Zip:4" -X POST http://127.0.0.1:$port/action/v1.0/SendQryMarketOrder`
			out=`echo $tmp | sed "s@$InstrumentID,1,@\n@g" | tail -n +2 | awk -F ']|,' '{printf(" [\\033[31m+%s %9s\\033[0m  |  %-11s]\n",6-NR,$1,$2)}'`
			echo -e "$out"
			echo " -----------------------------"
			out=`echo $tmp | sed "s@$InstrumentID,0,@\n@g" | tail -n +2 | awk -F ']|,' '{printf(" [\\033[32m-%s %9s\\033[0m  |  %-11s]\n",NR,$1,$2)}'`
			echo -e "$out"
			echo " ============================="			
			#echo "input Direction(0:Buy/1:Sell) Price Volume"
			#echo "or a(sell)<num>/b(buy)<num> to quick choose orderbook's price and volume"
			#echo "or null(directly enter) to refresh"		
			echo -e "\\033[33m[Examples]:\\033[0m"
			echo "1: 0(buy) 9874.32 34.2 / 1(sell) 10000.32 88.8"
			echo "2: b(buy) -4(bid4 price and Volume) / a(sell) +3(ask3 price and Volume)"
			echo "3: c(cancle) 4(cancle Order No 4"
			echo "4: c(cancle) 4(cancle Order No 4"
			read Direction Price Volume
			if [ "$Direction" == "q" ]
			then
				break
			fi		
			if [ "$Direction" == "b" ]
			then
				Direction=0
				num=$Price
				Price=`echo $tmp | sed "s#$InstrumentID,#\nx#g"| tail -n +3 | grep "x0," | head -n $num | tail -n 1 | awk -F , '{print $2}'`
				Volume=`echo $tmp | sed "s#$InstrumentID,#\nx#g"| tail -n +3 | grep "x0," | head -n $num | tail -n 1 | awk -F , '{print $3}'`
			fi
			if [ "$Direction" == "a" ]
			then
				Direction=1
				num=$Price
				Price=`echo $tmp | sed "s#$InstrumentID,#\nx#g"| tail -n +3 | grep "x1," | tail -n $num | head -n 1 | awk -F , '{print $2}'`
				Volume=`echo $tmp | sed "s#$InstrumentID,#\nx#g"| tail -n +3 | grep "x1," | tail -n $num | head -n 1 | awk -F , '{print $3}'`
			fi
			if [ "$Price" == "" -o "$Volume" == "" ]
			then
				continue
			fi
			echo "Direction:$Direction,Price:$Price,Volume:$Volume"	
			json=`echo {\"MemberID\":\"$MemberID\"\,\"ExchangeID\":\"$ExchangeID\"\,\"InstrumentID\":\"$InstrumentID\"\,\"Direction\":\"$Direction\"\,\"Price\":\"$Price\"\,\"Volume\":\"$Volume\"}`
			echo "curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendOrderInsert"
			curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendOrderInsert
			read waitForInput
		done
	fi
echo ""
done
