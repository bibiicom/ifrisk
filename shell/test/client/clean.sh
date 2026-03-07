#!/bin/bash

User=`whoami`
proc=`ps -fu $User | grep "/client/nginx" | grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
if [ ! "$proc" == "" ]
then
    printf "%-14s is running\n" nginx
    exit
fi
rm -rf ./nginx/logs/*