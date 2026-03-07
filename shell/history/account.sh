#!/bin/bash

mkdir -p ../hisdata
mkdir -p ../hisdata/day
mkdir -p ../hisdata/week
mkdir -p ../hisdata/month

JSON=`curl -H "zip:3" http://127.0.0.1/public/v1.0/SendQryAccount.sh?All=all`
TOTAL_V=$(echo "$JSON" | jq '[.result[].V] | add')
TIME=$(date +"%Y-%m-%d %H:%M:%S")
echo "$TIME,$TOTAL_V" >> ../hisdata/currency.csv