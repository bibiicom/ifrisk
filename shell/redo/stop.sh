#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

User=`whoami`
proc=`ps -fu $User | grep -w "./redo.sh"| grep -v grep | awk '{print $2}'`
kill -9 $proc

proc=`ps -fu $User | grep -w "./trade -r"| grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
kill -9 $proc

proc=`ps -fu $User | grep -w "diff"| grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
kill -9 $proc

