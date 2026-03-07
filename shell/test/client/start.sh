#!/bin/bash

workdir=$(cd $(dirname $0); pwd)

User=`whoami`
proc=`ps -fu $User | grep -w $workdir/nginx | grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
if [ ! "$proc" == "" ]
then
    printf "%-14s is running\n" nginx
    exit
fi

./nginx/sbin/nginx -p $workdir/nginx
