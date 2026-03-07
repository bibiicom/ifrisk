#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

for module in `ls -F | grep "/$"| awk -F / '{print $1}'`
do
	if [ -f $module.tar.gz ]
	then
		echo rm $module ...
		rm -rf $module
	fi
done
