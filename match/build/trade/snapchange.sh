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

##Order
curl -d "{\"SettlementGroup\":\"Spot\",\"AccountID\":\"521337\",\"Currency\":\"USDT\",\"MemberID\":\"521337\",\"FrozenMoney\":0}" -X POST  http://127.0.0.1:$port/$action/action/v1.0/SendUpdate/Account
#curl -d "{\"OrderID\":\"9\",\"Remark\":\"Order\"}"  -X POST  http://127.0.0.1:$port/$action/v1.0/SendUpdate/Order
#curl -d "{\"ExchangeID\":\"Test\",\"InstrumentID\":\"BTCUSD\",\"MemberID\":\"446902\",\"TradeUnitID\":\"446902\",\"PositionFlag\":\"1\",\"PosiDirection\":\"0\"}"  -X POST  http://127.0.0.1:$port/$action/action/v1.0/SendQryOne/Position
#
#curl -d "{\"UserID\":\"8001\",\"UserName\":\"NewName\"}"  -X POST  http://127.0.0.1:$port/$action/action/v1.0/SendUpdate/User
#curl -d "{\"MarketID\":\"DigitalCurrency\",\"ExchangeID\":\"Test\",\"InstrumentID\":\"BTC/USDT\",\"InstrumentName\":\"BTC/USDT\",\"SettlementGroup\":\"Spot\",\"ProductID\":\"Spot1\",\"ProductClass\":\"4\",\"ProductGroup\":\"Spot\",\"ProductType\":\"8\",\"VolumeMultiple\":1,\"UnderlyingMultiple\":1,\"PriceCurrency\":\"USDT\",\"ClearCurrency\":\"BTC\",\"IsInverse\":0,\"BaseCurrency\":\"BTC\",\"BasisPrice\":12000,\"MinOrderVolume\":1,\"MaxOrderVolume\":100000,\"PriceTick\":0.1,\"VolumeTick\":1,\"TradePriceMode\":\"1\",\"TradingModel\":\"0\",\"DayStartTime\":\"00:00:00\",\"SortNumber\":1,\"InstrumentStatus\":\"2\"}"  -X POST  http://127.0.0.1:$port/$action/action/v1.0/SendUpdate/Instrument
#curl -d "{\"MemberID\":\"default\",\"TradeUnitID\":\"default\",\"ExchangeID\":\"Test\",\"InstrumentID\":\"ETHUSD\",\"ValueMode\":\"0\",\"LongMarginRatio\":0.005,\"LongMaintMarginRatio\":0.005,\"LongMarginStep\":0.0,\"LongMaintMarginStep\":0.0,\"LongRiskLimit\":0.0,\"LongRiskStep\":0.0,\"ShortMarginRatio\":0.005,\"ShortMaintMarginRatio\":0.005,\"ShortMarginStep\":0.0,\"ShortMaintMarginStep\":0.0,\"ShortRiskLimit\":0.0,\"ShortRiskStep\":0.0\"}" -X POST  http://127.0.0.1:$port/$action/action/v1.0/SendUpdate/MarginRate



