#!/bin/bash

if [ "$1" == "" ]; then
	echo "./klineall.sh BTC/USDC BTC[for binance]"
	echo "./klineall.sh all"
	exit
fi

port=`grep httpfront_trade network.ini | awk -F : '{print $3}' | awk -F / '{print $1}' | head -n 1`
curl -s -H "Zip:2" http://127.0.0.1:$port/action/v1.0/SendQryInstrument?X=1 | jq -r '.result[] | [.InstrumentID,.BaseCurrency,.ExchangeID] | join(" ")'  > klineall.list

while IFS=' ' read -r InstrumentID BaseCurrency ExchangeID; do
	if [ ! "$1" == "all" ]; then
		if [[ ! $InstrumentID == *$1* ]]; then
			continue
		fi
	fi
	if [ "$2" == "" ]; then
		./kline.sh $ExchangeID $InstrumentID "$BaseCurrency"USDT 1d
		./kline.sh $ExchangeID $InstrumentID "$BaseCurrency"USDT 1w
		./kline.sh $ExchangeID $InstrumentID "$BaseCurrency"USDT 1o
		./kline.sh $ExchangeID $InstrumentID "$BaseCurrency"USDT 1h
	else
		./kline.sh $ExchangeID $InstrumentID "$2" 1d
		./kline.sh $ExchangeID $InstrumentID "$2" 1w
		./kline.sh $ExchangeID $InstrumentID "$2" 1o
		./kline.sh $ExchangeID $InstrumentID "$2" 1h
	fi
done < klineall.list
