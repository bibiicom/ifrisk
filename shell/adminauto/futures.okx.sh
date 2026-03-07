#!/bin/bash

#获取OKX的期货合约，修改为季度合约之后，增加到合约列表中
#yum install jq

workdir=$(cd $(dirname $0); pwd)
cd $workdir

ExchangeID=NX
BaseCurrency=ETH
ClearCurrency=USDT

#curl "https://www.oohking.com/priapi/v5/public/instruments?instType=FUTURES" | jq '.data' > futures.json
curl "https://www.okx.com/priapi/v5/public/instruments?instType=FUTURES" | jq '.data' > futures.json
jq -r '.[]|[.alias,.ctType,.ctValCcy,.instId,.settleCcy,.expTime]|join(" ")' futures.json > futures.list
grep $BaseCurrency futures.list | grep quarter | grep $ClearCurrency > futures.grep
grep $BaseCurrency futures.list | grep quarter | grep -w USD >> futures.grep

cntexpr=`wc -l futures.grep|awk '{print $1}'`
cnt=`expr $cntexpr`
i=0
success=0
rm -f futures.error
while [ $i -lt $cnt ]
do
	i=`expr $i + 1 `
	read alias ctValCcy ctType instId settleCcy expTime
	echo $alias $ctValCcy $ctType $instId $settleCcy $expTime
	expTime=${expTime:0:${#expTime}-3}
	if [ "$ctValCcy" == "inverse" ]
	then
		IsInverse=1
		PriceCurrency=USD
		ClearCurrency=$BaseCurrency
		SettlementGroup=SG1
		ProductGroup=FTUR
	else
		IsInverse=0
		PriceCurrency=$ClearCurrency
		SettlementGroup=SG1
		ProductGroup=FTURU
	fi

	InstrumentID=${instId:}

	if [ "$alias" == "quarter" ]
	then
		Name=$BaseCurrency"CS"
	else
		Name=$BaseCurrency"NS"
	fi
	
	port=`grep httpfront_trade ../deploy/network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`
	httpurl="{ExchangeID:$ExchangeID,InstrumentID:$instId,InstrumentName:$Name,SettlementGroup:$SettlementGroup,ProductClass:1,ProductGroup:$ProductGroup,ProductType:1,VolumeMultiple:1,PriceCurrency:$PriceCurrency,ClearCurrency:$ClearCurrency,IsInverse:$IsInverse,BaseCurrency:$BaseCurrency,TradingModel:0,DayStartTime:00:00:00,ExpireTime:$expTime,InstrumentStatus:2,PriceTick:1,VolumeTick:10000,DefaultLeverage:100,Remark:Maker|${instId}_3|trades|books5}"
	echo $httpurl
	#continue
	curl -d $httpurl -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Instrument
done < futures.grep

#如果启动成功，重启marketmarker

#echo restart marketmaker
#restart.sh Y marketmaker
