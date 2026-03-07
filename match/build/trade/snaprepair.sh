#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

path=`pwd`
echo $path

number=${path##*trade}
echo $number
if [ "$number" == "" ]
then
	number=0
fi

echo httpfront_trade_$number 
#httpfront_marketdata_0=manager|http://127.0.0.1:10100/action#1
port=`grep httpfront_trade_$number network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`
action=`grep httpfront_trade_$number network.ini | awk -F / '{print $4}' | awk -F '#' '{print $1}'`

echo 'curl -d "{\"SettleAction\":\"9\",\"Remark\":\"Member\"}"' -X POST  http://127.0.0.1:$port/$action/v1.0/SendSettlement
curl -d "{\"SettleAction\":\"9\",\"Remark\":\"Member\"}"  -X POST  http://127.0.0.1:$port/$action/v1.0/SendSettlement
echo ""
echo 'curl -d "{\"SettleAction\":\"9\",\"Remark\":\"Order\"}"' -X POST  http://127.0.0.1:$port/$action/v1.0/SendSettlement
curl -d "{\"SettleAction\":\"9\",\"Remark\":\"Order\"}"  -X POST  http://127.0.0.1:$port/$action/v1.0/SendSettlement
echo ""
echo 'curl -d "{\"SettleAction\":\"9\",\"Remark\":\"Position\"}"' -X POST  http://127.0.0.1:$port/$action/v1.0/SendSettlement
curl -d "{\"SettleAction\":\"9\",\"Remark\":\"Position\"}"  -X POST  http://127.0.0.1:$port/$action/v1.0/SendSettlement
echo ""
echo 'curl -d "{\"SettleAction\":\"9\",\"Remark\":\"Account\"}"' -X POST  http://127.0.0.1:$port/$action/v1.0/SendSettlement
curl -d "{\"SettleAction\":\"9\",\"Remark\":\"Account\"}"  -X POST  http://127.0.0.1:$port/$action/v1.0/SendSettlement
echo ""
echo 'curl -d "{\"SettleAction\":\"9\",\"Remark\":\"MemberPosition\"}"' -X POST  http://127.0.0.1:$port/$action/v1.0/SendSettlement
curl -d "{\"SettleAction\":\"9\",\"Remark\":\"MemberPosition\"}"  -X POST  http://127.0.0.1:$port/$action/v1.0/SendSettlement
