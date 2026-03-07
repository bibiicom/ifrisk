#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

instId=ETHUSDT
Name=ETHUSDT
subInstID=ETH-USDT-SWAP

BaseCurrency=ETH
ClearCurrency=USDT

IsInverse=0
PriceCurrency=$ClearCurrency
SettlementGroup=SwapU
ProductGroup=FutureU

port=`grep httpfront_trade ../deploy/network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`

httpurl=`echo {\"ExchangeID\":\"Test\"\,\"InstrumentID\":\"$instId\"\,\"InstrumentName\":\"$Name\"\,\"SettlementGroup\":\"$SettlementGroup\"\,\"ProductClass\":\"1\"\,\"ProductGroup\":\"$ProductGroup\"\,\"ProductType\":\"1\"\,\"VolumeMultiple\":1\,\"PriceCurrency\":\"$PriceCurrency\"\,\"ClearCurrency\":\"$ClearCurrency\"\,\"IsInverse\":$IsInverse\,\"BaseCurrency\":\"$BaseCurrency\"\,\"TradingModel\":\"0\"\,\"DayStartTime\":\"00:00:00\"\,\"InstrumentStatus\":\"2\"\,\"PriceTick\":\"1\"\,\"VolumeTick\":\"10000\"\,\"DefaultLeverage\":\"100\"\,\"Remark\":\"Maker\|${subInstID}_4\|trades\|books5\"}`
echo $httpurl
#continue

curl -d $httpurl -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Instrument

#echo restart marketmaker
#restart.sh Y marketmaker
