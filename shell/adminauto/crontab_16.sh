#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

cd ../run/trade
#httpfront_trade_0=manager|http://127.0.0.1:10100/action#1#precheck:2
port=`grep httpfront_trade_0 network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'` 
echo curl -d "{\\\"SettleAction\\\":\\\"3\\\"}"  -X POST  http://127.0.0.1:$port/v1.0/SendSettlement
#curl -d "{\"SettleAction\":\"3\"}"  -X POST  http://127.0.0.1:$port/action/v1.0/SendSettlement
curl -d "{\"SettleAction\":\"1\",\"ExchangeID\":\"Test\",\"SettlementGroup\":\"Swap\"}"  -X POST  http://127.0.0.1:$port/action/v1.0/SendSettlement
curl -d "{\"SettleAction\":\"1\",\"ExchangeID\":\"Test\",\"SettlementGroup\":\"SwapU\"}"  -X POST  http://127.0.0.1:$port/action/v1.0/SendSettlement
curl -d "{\"SettleAction\":\"1\",\"ExchangeID\":\"Test\",\"SettlementGroup\":\"Future\"}"  -X POST  http://127.0.0.1:$port/action/v1.0/SendSettlement
curl -d "{\"SettleAction\":\"1\",\"ExchangeID\":\"Test\",\"SettlementGroup\":\"FutureU\"}"  -X POST  http://127.0.0.1:$port/action/v1.0/SendSettlement
curl -d "{\"SettleAction\":\"2\",\"ExchangeID\":\"Test\",\"SettlementGroup\":\"Future\"}"  -X POST  http://127.0.0.1:$port/action/v1.0/SendSettlement
curl -d "{\"SettleAction\":\"2\",\"ExchangeID\":\"Test\",\"SettlementGroup\":\"FutureU\"}"  -X POST  http://127.0.0.1:$port/action/v1.0/SendSettlement
cd $workdir
