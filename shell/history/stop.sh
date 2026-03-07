#!/bin/bash
#cd eth
#./stop.sh

cd short
./stop.sh

cd ../long
./stop.sh

proc=`ps -ux | grep -w "tmux new-session -d -s mysession"| grep -v grep | awk '{print $2}'`
if [ ! "$proc" == "" ]; then
	echo "Stop tmux ... $proc"
	tmux kill-session -t mysession
fi
