#!/bin/bash

workdir=$(cd $(dirname $0); pwd)

res=`ps ux $User | grep -v grep|grep $workdir/nginx|grep -v worker|awk '{printf "%-7s %-6s %-5s %-7s %-7s",$2,$3,$4,$9,$10}'`

if [ "$res" == "" ]
then
    printf "\n"
    exit
fi
version=`cat ./nginx/html/version.txt`

printf "%s %-5s \n" "$res" "$version"
