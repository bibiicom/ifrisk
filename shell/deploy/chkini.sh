#!/bin/bash
workdir=$(cd $(dirname $0); pwd)
cd $workdir

for module in `ls -F ../run/ | grep "/$" | grep -w "$1$2"`
do
	echo check $module
	diff ../run/$module/network.ini network.ini
done


