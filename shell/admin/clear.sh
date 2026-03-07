#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

User=`whoami`

proc=`ps -fu $User | grep -w "./wsclient" | grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
kill -9 $proc

rm -rf log/lastTrade*