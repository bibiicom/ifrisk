#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir
	
port=`grep httpfront_trade ../deploy/network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`
httpurl=`echo {\"ExchangeID\":\"$1\"\,\"InstrumentID\":\"$2\"\,\"ExpireTime\":0}`
echo $httpurl
#continue
curl -d $httpurl -X POST http://127.0.0.1:$port/action/v1.0/SendUpdate/Instrument

httpurl=`echo {\"SettleAction\":\"2\"\,\"ExchangeID\":\"$1\"\,\"InstrumentID\":\"$2\"}`
echo $httpurl
curl -d $httpurl -X POST http://127.0.0.1:$port/action/v1.0/SendSettlement
