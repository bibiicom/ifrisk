#!/bin/bash

#编译所有的模块
for project in `ls -F | grep "/$"| awk -F / '{print $1}'`
do 
	cd $project
	if [ -f setstatic.sh ]
	then
		echo setstatic $project
		chmod u+x setstatic.sh
		./setstatic.sh
	fi
	cd ..
done 

