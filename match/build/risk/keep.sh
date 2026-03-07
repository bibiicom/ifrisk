#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

dir=his1h
htmldir=../client/nginx/his1h

#keep3 0 days
ls -1t ./$dir/Account_*.csv 2>/dev/null | tail -n +$((30*24+1)) | xargs -r rm -f

#get by account

mkdir ${htmldir}_new

# 获取第一个 Account_*.csv 文件
first_file=$(ls ./$dir/Account_*.csv | tail -n1)
# 提取第一个文件的所有第二列值（去重）
values=$(awk -F',' '{print $2}' "$first_file" | sort -u)

awk -F',' 'FNR==NR {vals[$2]; next} $2 in vals {print $1 "," $3 > "'"${htmldir}_new"'/account."$2".csv"}' "$first_file" ./$dir/Account_*.csv

rm -rf $htmldir
mv ${htmldir}_new $htmldir
