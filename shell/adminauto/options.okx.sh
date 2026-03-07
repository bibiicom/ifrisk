#!/bin/bash

#获取OKX的期权合约
#yum install jq

workdir=$(cd $(dirname $0); pwd)
cd $workdir

BaseCurrency=ETH
ClearCurrency=USDT

curl "https://www.oohking.com/api/v5/market/tickers?instType=OPTION" | jq '.data' > options.json
jq -r '.[]|[.vol24h,.instId]|join(" ")' options.json > options.market
grep $BaseCurrency options.market | sort -n -r | head -n 10 > options.grep

curl "https://www.oohking.com/priapi/v5/public/instruments?instType=OPTION" | jq '.data' > options.json
jq -r '.[]|[.expTime,.instId,.optType]|join(" ")' options.json > options.list

cntexpr=`wc -l options.grep|awk '{print $1}'`
cnt=`expr $cntexpr`
i=0
success=0
rm -f options.error
while [ $i -lt $cnt ]
do
	i=`expr $i + 1 `
	read vol24h instId
	echo $vol24h $instId

	expTime=`grep $instId options.list | awk '{print $1}'`
	expTime=${expTime:0:${#expTime}-3}
	cOptionsType=`grep $instId options.list | awk '{print $3}'`
	StrikePrice=`echo $instId | awk -F - '{print $4}'`
	
	PriceCurrency=USDT
	ClearCurrency=USDT
	SettlementGroup=SwapU
	ProductGroup=Options
	OptionsType=1
	if [ "$cOptionsType" == "P" ]
	then
		OptionsType=2
	fi
	
	port=`grep httpfront_trade ../deploy/network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`
	httpurl=`echo {\"ExchangeID\":\"Test\"\,\"InstrumentID\":\"$instId\"\,\"InstrumentName\":\"$BaseCurrency\"\,\"SettlementGroup\":\"$SettlementGroup\"\,\"ProductClass\":\"1\"\,\"ProductGroup\":\"$ProductGroup\"\,\"ProductType\":\"1\"\,\"VolumeMultiple\":1\,\"PriceCurrency\":\"$PriceCurrency\"\,\"ClearCurrency\":\"$ClearCurrency\"\,\"IsInverse\":0\,\"BaseCurrency\":\"$BaseCurrency\"\,\"TradingModel\":\"0\"\,\"OptionsType\":\"$OptionsType\"\,\"StrikePrice\":$StrikePrice\,\"DayStartTime\":\"00:00:00\"\,\"ExpireTime\":\"$expTime\"\,\"InstrumentStatus\":\"2\"\,\"PriceTick\":1\,\"VolumeTick\":10000\,\"DefaultLeverage\":100\,\"Remark\":\"Maker\|${instId}_4\|trades\|books5\|mark-price\"}`
	echo $httpurl
	#continue
	curl -d $httpurl -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Instrument
done < options.grep

#如果启动成功，重启marketmarker

echo restart marketmaker
restart.sh Y marketmaker

