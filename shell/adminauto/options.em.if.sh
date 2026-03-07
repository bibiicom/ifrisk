#!/bin/bash

#获取东方财富的期权合约
#yum install jq

workdir=$(cd $(dirname $0); pwd)
cd $workdir

curl "https://futsseapi.eastmoney.com/list/option/221?variety=1&orderBy=zdf&sort=desc&pageSize=300&pageIndex=0&token=58b2fa8f54638b60b87d69b31969089c&field=dm%2Csc%2Cname%2Cp%2Czsjd%2Czde%2Czdf%2Cf152%2Cvol%2Ccje%2Cccl%2Cxqj%2Csyr%2Crz%2Czjsj%2Co&_=1703047291034" | jq '.list' > options.json

jq -r '.[]|[.cje,.dm,.syr,.ccl]|join(" ")' options.json > options.market
more options.market | sort -n -r | head -n 10 > options.grep

cntexpr=`wc -l options.grep|awk '{print $1}'`
cnt=`expr $cntexpr`
i=0
success=0
rm -f options.error
while [ $i -lt $cnt ]
do
	i=`expr $i + 1 `
	read cje instId syr ccl
	echo $cje $instId $syr $ccl

	expTime=`date -d "$syr days" +%s`
	cOptionsType=`echo $instId | awk -F - '{print $2}'`
	StrikePrice=`echo $instId | awk -F - '{print $3}'`
	
	BaseCurrency=CNY
	PriceCurrency=CNY
	ClearCurrency=CNY
	SettlementGroup=Spot
	ProductGroup=Stock
	OptionsType=1

	if [ "$cOptionsType" == "P" ]
	then
		OptionsType=2
	fi
	
	port=`grep httpfront_trade ../deploy/network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`
	httpurl=`echo {\"ExchangeID\":\"Test\"\,\"InstrumentID\":\"$instId\"\,\"InstrumentName\":\"$BaseCurrency\"\,\"SettlementGroup\":\"$SettlementGroup\"\,\"ProductClass\":\"1\"\,\"ProductGroup\":\"$ProductGroup\"\,\"ProductType\":\"1\"\,\"VolumeMultiple\":1\,\"PriceCurrency\":\"$PriceCurrency\"\,\"ClearCurrency\":\"$ClearCurrency\"\,\"IsInverse\":0\,\"BaseCurrency\":\"$BaseCurrency\"\,\"TradingModel\":\"0\"\,\"OptionsType\":\"$OptionsType\"\,\"StrikePrice\":$StrikePrice\,\"DayStartTime\":\"00:00:00\"\,\"ExpireTime\":\"$expTime\"\,\"InstrumentStatus\":\"2\"\,\"PriceTick\":1\,\"VolumeTick\":1\,\"DefaultLeverage\":100\,\"Remark\":\"Maker\|${instId}_8\|books5\"}`
	echo $httpurl
	#continue
	curl -d $httpurl -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Instrument
done < options.grep

#如果启动成功，重启marketmarker

#echo restart marketmaker
#restart.sh Y marketmaker

