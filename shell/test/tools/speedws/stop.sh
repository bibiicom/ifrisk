#!/bin/bash

User=`whoami`
proc=`ps -fu $User | grep "./wsclient"| grep -v grep | grep -v bash| grep -v stop | awk '{print $2}'`
kill -9 $proc
