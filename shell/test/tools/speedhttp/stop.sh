#!/bin/bash

User=`whoami`
proc=`ps -fu $User | grep -w "./run.sh"| grep -v grep| awk '{print $2}'`
kill -9 $proc
proc=`ps -fu $User | grep -w "./speedtest.sh"| grep -v grep| awk '{print $2}'`
kill -9 $proc
proc=`ps -fu $User | grep -w "curl"| grep -v grep| awk '{print $2}'`
kill -9 $proc

