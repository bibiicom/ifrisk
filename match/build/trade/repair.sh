#!/bin/bash

read -r -p "Are You Sure? [Y/n] " input	
case $input in
	[yY][eE][sS]|[yY])
		echo "Yes"
		;;
	[nN][oO]|[nN])
		echo "No"
		exit 1
		;;
	*)
		echo "Invalid input..."
		exit 1
		;;
esac

chmod u+x flowrepair

thistime=`date +%Y%m%d%H%M%S`

lastFlow=`ls -d flow/inputall.*.flow 2>/dev/null | awk -F . '{print $2}' | sort -r -n | head -n 1`
mv flow/inputall.$lastFlow.flow flow/inputall.$lastFlow.flow.$thistime
./flowrepair flow/inputall.$lastFlow.flow.$thistime flow/inputall.$lastFlow.flow

