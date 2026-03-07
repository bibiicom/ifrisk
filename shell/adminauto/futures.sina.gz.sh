#!/bin/bash

#获取Sina的期货合约，修改为季度合约之后，增加到合约列表中
#yum install jq

workdir=$(cd $(dirname $0); pwd)
cd $workdir

BaseCurrency=CNY
ClearCurrency=CNY

curl "https://vip.stock.finance.sina.com.cn/quotes_service/api/json_v2.php/Market_Center.getHQFuturesData?page=1&num=40&sort=symbol&asc=1&node=gz_qh&_s_r_a=init" > futures.json
jq -r '.[]|[.volume,.position,.symbol]|join(" ")' futures.json > futures.list
tail -n +2 futures.list | sort -n -r | head -n 4 > futures.grep

cntexpr=`wc -l futures.grep|awk '{print $1}'`
cnt=`expr $cntexpr`
i=0
success=0
rm -f futures.error
while [ $i -lt $cnt ]
do
	i=`expr $i + 1 `
	read volume position instId
	echo $volume $position $instId

	expTimeStr=`echo 20${instId:0-4:2}-${instId:0-2:2}-28 16:00:00`
	echo $expTimeStr
	expTime=`date -d "$expTimeStr" +%s`
	
	Name=$instId
	SubName=${instId}
	IsInverse=0
	PriceCurrency=$ClearCurrency
	SettlementGroup=Spot
	ProductGroup=Bond

	port=`grep httpfront_trade ../deploy/network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`
	httpurl=`echo {\"ExchangeID\":\"Test\"\,\"InstrumentID\":\"$instId\"\,\"InstrumentName\":\"$Name\"\,\"SettlementGroup\":\"$SettlementGroup\"\,\"ProductClass\":\"4\"\,\"ProductGroup\":\"$ProductGroup\"\,\"ProductType\":\"8\"\,\"VolumeMultiple\":1\,\"PriceCurrency\":\"$PriceCurrency\"\,\"ClearCurrency\":\"$ClearCurrency\"\,\"IsInverse\":$IsInverse\,\"BaseCurrency\":\"$BaseCurrency\"\,\"TradingModel\":\"0\"\,\"DayStartTime\":\"00:00:00\"\,\"ExpireTime\":\"$expTime\"\,\"InstrumentStatus\":\"2\"\,\"PriceTick\":\"0.01\"\,\"VolumeTick\":\"1\"\,\"DefaultLeverage\":\"20\",\"Remark\":\"Maker\|$SubName\_8\|books5\"\}`
	echo $httpurl
	#continue
	curl -d $httpurl -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Instrument
done < futures.grep

#如果启动成功，重启marketmarker

echo restart marketmaker
restart.sh Y marketmaker
