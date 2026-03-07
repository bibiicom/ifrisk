#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir
mkdir log  2>/dev/null

port=`grep httpfront_trade network.ini | awk -F : '{print $3}' | awk -F / '{print $1}' | head -n 1`
i=10

while [ 1 -eq 1 ]; do
	#echo "curl -s -d \"{}\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryCurrency"
	curl -s -d "{}" -X POST http://127.0.0.1:$port/action/v1.0/SendQryCurrency | sed "s/:{/\n/g" | tail -n +2 | sed "s/\"//g" | awk -F ",|:" '{printf("%2s %3s %8s %12.0f  %12.0f %12.0f %12.0f %12.0f %4.1f %4.1f %3.1f %6.0f %13s\n",NR,$2,$4,$10,$6,$8,$30,$28,$34*100,$36*100,$46,$48,$26)}' > ./log/Currencys
	if [ "$1" == "" ]; then
		break;
	fi
	tput home
	echo "No  SG Currency      Balance       Deposit     Withdraw    LoanTotal      LoanUse  LB%  LD%  HC    MAX  InstrumentID"
	cat ./log/Currencys
	#echo -e "\e[?25l"
	((i=$i+1))
	echo "$i                                                                 "
	sleep 2
done

number=`less ./log/Currencys | wc -l`

if [ $number -lt 1 ]; then
	echo no Currencys return 0
	exit 0
fi


while [ 1 -eq 1 ]; do
	echo "No  SG Currency      Balance       Deposit     Withdraw    LoanTotal      LoanUse  LB%  LD%  HC    MAX  InstrumentID"
	hnumber=`expr $i`
	if [ $hnumber -gt $number ]; then
		hnumber=$number
		tnumber=`expr $hnumber - $i + 10`
		x=`expr $i - 9`
		head -n $hnumber ./log/Currencys | tail -n $tnumber
		if [ $i -eq 10 ]; then
			echo "choose number, or q:quit"  
		else
			echo "choose number, or b:before/q:quit <- " 
		fi		
	else
		tnumber=10
		head -n $hnumber ./log/Currencys | tail -n $tnumber
		if [ $i -eq 10 ]; then
			echo "choose number, or n:next/q:quit -> "  
		else
			echo "choose number, or b:before/n:next/q:quit <- -> "  
		fi
	fi
	read FilterValue
	
	if [ "$FilterValue" == "n" ]; then
		if [ $i -gt $number ]; then
			echo to the end, please enter b or number
			continue
		fi
		i=`expr $i + 10`
	else
		if [ "$FilterValue" == "b" ]; then
			if [ $i -eq 10 ]; then
				echo to the begin, please enter n or number
				continue
			fi
			i=`expr $i - 10`
		else

			if [ "$FilterValue" == "q" -o "$FilterValue" == "" ]; then
				echo abord,return 0
				exit 0
			fi

			if [[ $FilterValue =~ ^[0-9]+$ ]]; then
				if [ $FilterValue -lt 0 -o $FilterValue -gt $number ];	then
					echo number must less than $number, please choose again!	
					continue
				else
					exit $FilterValue
				fi
			else
				echo not number,please choose again!
				continue
			fi
		fi
	fi
done
