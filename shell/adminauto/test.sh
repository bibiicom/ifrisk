#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

source env.sh
source base.sh

#api_path="/api/v5/trade/fills?instType=SWAP&subType=4&limit=5"
#api_path="/api/v5/trade/orders-history?instType=SWAP&state=filled"
#get x.json
#jq -r '.data[] | [.fillTime, .side, .posSide, .instId, .fillPx, .fillSz] | join(" ")' x.json | grep "short" | grep "buy" | awk 'BEGIN { FS=" " } {timestamp = $1; local_time = strftime("%m%d-%H:%M:%S", timestamp/1000); key = $2 "-" $3 "-" $4; if (!seen[key]++) {print local_time, $2, $3, $4, $5, $6;}}'

account test
#echo mgnRatio:$mgnRatio
#echo upWin:$upWin
#echo upLost:$upLost
#echo downWin:$downWin
#echo downLost:$downLost
#echo moneyAll:$moneyAll
#echo shortOpen:$shortOpen
#echo longOpen:$longOpen
#echo shortClose:$shortClose
#echo longClose:$longClose


#get_kline BTC-USDT-SWAP 15m 30 test_kline15m