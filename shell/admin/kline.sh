#!/bin/bash

if [ "$1" == "" -o "$2" == ""  -o "$3" == "" ]; then
	echo "kline.sh BTC/USDC BTCUSDT 1m"
	exit  
fi

ExchangeID=$1
InstrumentID=$2
symbol=$3
Bar=$4

echo "kline.sh $*"

interval=$Bar
if [ "$Bar" == "1o" ]; then
	interval="1M"
fi

#seconds -> 秒	1s
#minutes -> 分钟	1m， 3m， 5m， 15m， 30m
#hours -> 小时	1h， 2h， 4h， 6h， 8h， 12h
#days -> 天	1d， 3d
#weeks -> 周	1w
#months -> 月	1o

#seconds -> 秒	1s
#minutes -> 分钟	1m， 3m， 5m， 15m， 30m
#hours -> 小时	1h， 2h， 4h， 6h， 8h， 12h
#days -> 天	1d， 3d
#weeks -> 周	1w
#months -> 月	1M

>kline.sql
echo "curl -s \"https://www.binance.com/api/v3/klines?symbol=$symbol&interval=$interval&limit=1000\""
curl -s "https://www.binance.com/api/v3/klines?symbol=$symbol&interval=$interval&limit=1000" > kline.json
cat kline.json | sed "s/\[/\n/g" | tail -n +3 > kline.read

port=`grep httpfront_trade network.ini | awk -F : '{print $3}' | awk -F / '{print $1}' | head -n 1`

while IFS=',' read -r begintime open high low close volume endtime turnover n1 n2 n3; do
	echo "INSERT INTO perpetual.t_D_KLine (ExchangeID,InstrumentID,Bar,InsertTime,OpenPrice,ClosePrice,HighestPrice,LowestPrice,Volume,Turnover) VALUES (\"$ExchangeID\",\"$InstrumentID\",\"$Bar\",$begintime,$open,$close,$high,$low,$volume,$turnover);" >> kline.sql
done < kline.read

json="{ExchangeID:$ExchangeID,InstrumentID:$InstrumentID,Bar:$Bar,InsertTime:$begintime,OpenPrice:$open,HighestPrice:$high,LowestPrice:$low,Volume:$volume,Turnover:$turnover}"
echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendUpdate/LastKLine"
curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendUpdate/LastKLine
echo ""

mysql.sh kline.sql