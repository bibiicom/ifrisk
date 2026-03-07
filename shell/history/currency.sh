#!/bin/bash

mkdir -p ../hisdata

JSON=`curl -H "zip:3" http://127.0.0.1/public/v1.0/SendQryCurrency?All=all`
RAW_TOTAL=$(echo "$JSON" | jq '[.result[].V] | add')
TOTAL_V=$(printf "%.2f" "$RAW_TOTAL")
TIME=$(date +"%Y-%m-%d")
echo "$TIME,$TOTAL_V" >> ../hisdata/currency.csv