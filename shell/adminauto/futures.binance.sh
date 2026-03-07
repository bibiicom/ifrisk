#!/bin/bash

#获取OKX的期货合约，修改为季度合约之后，增加到合约列表中
#yum install jq

workdir=$(cd $(dirname $0); pwd)
cd $workdir

port=`grep httpfront_trade ../deploy/network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`

curl "https://www.kanjiejing.com/priapi/v5/public/instruments?instType=FUTURES" | jq '.data' > future.json
jq -r '.[]|[.alias,.ctType,.ctValCcy,.instId,.settleCcy,.expTime]|join(" ")' future.json > future.list
grep BTC future.list | grep quarter > future_BTC.list

cntexpr=`wc -l future_BTC.list|awk '{print $1}'`
cnt=`expr $cntexpr`
i=0
success=0
rm -f addinstrument.json
while [ $i -lt $cnt ]
do
	i=`expr $i + 1 `
	read alias ctValCcy ctType instId settleCcy expTime
	echo $alias $ctValCcy $ctType $instId $settleCcy $expTime
	if [ "$ctType" == "inverse" ]
	then
		httpurl=`echo {\"MarketID\":\"DigitalCurrency\"\,\"ExchangeID\":\"Test\"\,\"InstrumentID\":\"$instId\"\,\"InstrumentName\":\"$instId\"\,\"SettlementGroup\":\"SwapU\"\,\"ProductClass\":\"4\"\,\"ProductGroup\":\"Swap\"\,\"ProductType\":\"8\"\,\"VolumeMultiple\":1\,\"UnderlyingMultiple\":1\,\"PriceCurrency\":\"USDT\"\,\"ClearCurrency\":\"BTC\"\,\"IsInverse\":0\,\"BaseCurrency\":\"BTC\"\,\"MinOrderVolume\":1\,\"MaxOrderVolume\":100000\,\"PriceTick\":0.1\,\"VolumeTick\":1\,\"TradingModel\":\"0\"\,\"DayStartTime\":\"00:00:00\"\,\"InstrumentStatus\":\"2\"\,\"DefaultLeverage\":\"100\"}`
		curl -d $httpurl -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Instrument > addinstrument.json
		errorCode=`jq -r '.errorCode' addinstrument.json`
	else
		httpurl=`echo {\"MarketID\":\"DigitalCurrency\"\,\"ExchangeID\":\"Test\"\,\"InstrumentID\":\"$instId\"\,\"InstrumentName\":\"$instId\"\,\"SettlementGroup\":\"SwapU\"\,\"ProductClass\":\"4\"\,\"ProductGroup\":\"Swap\"\,\"ProductType\":\"8\"\,\"VolumeMultiple\":1\,\"UnderlyingMultiple\":1\,\"PriceCurrency\":\"USDT\"\,\"ClearCurrency\":\"BTC\"\,\"IsInverse\":0\,\"BaseCurrency\":\"BTC\"\,\"MinOrderVolume\":1\,\"MaxOrderVolume\":100000\,\"PriceTick\":0.1\,\"VolumeTick\":1\,\"TradingModel\":\"0\"\,\"DayStartTime\":\"00:00:00\"\,\"InstrumentStatus\":\"2\"\,\"DefaultLeverage\":\"100\"\,\"Remark\":\"Maker:$instId:trades:books5"}`
		curl -d $httpurl -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Instrument > addinstrument.json
		errorCode=`jq -r '.errorCode' addinstrument.json`
	fi
	echo $errorCode 
	if [ "$errorCode" == "0" ]
	then
		success=1
	fi
done < future_BTC.list

#如果启动成功，重启marketmarker

if [ "success" == "1" ]
then
	echo restart marketmarker
	stop.sh marketmarker
	start.sh marketmarker
fi
