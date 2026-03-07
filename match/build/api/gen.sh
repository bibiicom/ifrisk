#!/bin/bash

gentpl ../../xmls/entity 1>/dev/null

for project in `ls -F | grep "/$"| awk -F / '{print $1}'`
do 
	cd $project
	if [ -f "gen.sh" ]
	then
		echo gen $project
		chmod u+x gen.sh
		./gen.sh
	fi
	cd ..
done 


#编译所有的模块
for project in cplusplus linkswap dataswap*
do
	cd $project
	echo gen $project
	rm -rf makefile* *.dsw *.dsp 1>/dev/null 2>/dev/null
	genprj list.xml all 1>/dev/null
	cd ..
done
