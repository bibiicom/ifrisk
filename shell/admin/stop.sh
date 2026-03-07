#!/bin/bash

proc=`ps -ux | grep -w "./orderbook.sh"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]; then
	echo "Stop orderbook ... $proc"
	kill -9 $proc
fi

proc=`ps -ux | grep -w "./market.sh"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]; then
	echo "Stop market ... $proc"
	kill -9 $proc
fi

proc=`ps -ux | grep -w "./currency.sh"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]; then
	echo "Stop currency ... $proc"
	kill -9 $proc
fi

proc=`ps -ux | grep -w "./wsclient"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]; then
	echo "Stop wsclient ... $proc"
	kill -9 $proc
fi

proc=`ps -ux | grep -w "tmux new-session -d -s mysession"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]; then
	echo "Stop tmux ... $proc"
	tmux kill-session -t mysession
fi
