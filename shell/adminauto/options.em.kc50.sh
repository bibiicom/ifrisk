#!/bin/bash

#获取东方财富的期权合约
#yum install jq

workdir=$(cd $(dirname $0); pwd)
cd $workdir

curl "https://47.push2.eastmoney.com/api/qt/clist/get?pn=1&pz=300&po=1&np=1&ut=bd1d9ddb04089700cf9c27f6f7426281&fltt=2&invt=2&wbp2u=|0|0|0|web&fid=f5&fs=m:10+c:588000&fields=f108,f12,f14,f161,f162&_=1703068855030" | jq '.data' | jq '.diff' > options.json

jq -r '.[]|[.f108,.f12,.f14,.f162,.f161]|join(" ")' options.json > options.market
more options.market | sort -n -r | head -n 10 > options.grep

cntexpr=`wc -l options.grep|awk '{print $1}'`
cnt=`expr $cntexpr`
i=0
success=0
rm -f options.error
while [ $i -lt $cnt ]
do
	i=`expr $i + 1 `
	read ccl instId name syr price
	echo $ccl $instId $name $syr $price

	expTime=`date -d "$syr days" +%s`
	StrikePrice=$price
	
	instName=${name/科创/KC}
	instName=${instName/购/-C-}
	instName=${instName/沽/-P-}	
	instName=${instName/月/-}
	
	if [[ $name == *-P-* ]]
	then
		OptionsType=1
	else
		OptionsType=2
	fi
	
	BaseCurrency=CNY
	PriceCurrency=CNY
	ClearCurrency=CNY
	SettlementGroup=Spot
	ProductGroup=Stock

	port=`grep httpfront_trade ../deploy/network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`
	httpurl=`echo {\"ExchangeID\":\"Test\"\,\"InstrumentID\":\"$instId\"\,\"InstrumentName\":\"$instName\"\,\"SettlementGroup\":\"$SettlementGroup\"\,\"ProductClass\":\"1\"\,\"ProductGroup\":\"$ProductGroup\"\,\"ProductType\":\"1\"\,\"VolumeMultiple\":1\,\"PriceCurrency\":\"$PriceCurrency\"\,\"ClearCurrency\":\"$ClearCurrency\"\,\"IsInverse\":0\,\"BaseCurrency\":\"$BaseCurrency\"\,\"TradingModel\":\"0\"\,\"OptionsType\":\"$OptionsType\"\,\"StrikePrice\":$StrikePrice\,\"DayStartTime\":\"00:00:00\"\,\"ExpireTime\":\"$expTime\"\,\"InstrumentStatus\":\"2\"\,\"PriceTick\":1\,\"VolumeTick\":1\,\"DefaultLeverage\":100\,\"Remark\":\"Maker\|${instId}_8\|books5\"}`
	echo $httpurl
	#continue
	curl -d $httpurl -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Instrument
done < options.grep

#如果启动成功，重启marketmarker

#echo restart marketmaker
#restart.sh Y marketmaker

