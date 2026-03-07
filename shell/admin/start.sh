#!/bin/bash
echo "Start market ... "
nohup ./market.sh 5 l > ./log/market.nohup 2>&1 &
echo "Start currency ... "
nohup ./currency.sh l > ./log/currency.nohup 2>&1 &
echo "Start btc ... "

i=1
for instId in $(head -n 4 ./log/Instruments | awk '{print $3}'); do
	nohup ./orderbook.sh $instId > ./log/orderbook_$i.nohup 2>&1 &
	echo "Start $instId ... "
	((i++))
done