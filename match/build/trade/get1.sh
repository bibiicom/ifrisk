#!/bin/bash

if [ "$1" == "" ]
then
	flowNo=`ls -d flow/inputall.*.flow 2>/dev/null | awk -F . '{print $2}' | sort -r -n | head -n 1`
	snapNo=$flowNo
else
	snapNo=$1
	for flow in `ls -d flow/inputall.*.flow 2>/dev/null | awk -F . '{print $2}' | sort -r -n`
	do
		if [ $flow -le $snapNo ] 
		then
			flowNo=$flow
			break
		fi
	done
fi
rm -rf 1.tar.gz
tar cvf 1.tar ./snap/snap.$snapNo* ./flow/inputall.$flowNo.flow *.ini
gzip 1.tar
#sz 1.tar.gz
