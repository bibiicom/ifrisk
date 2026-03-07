#!/bin/bash

thispath=`pwd`
if [ -f libs/setstatic.sh ]
then
	cd libs
	chmod u+x setstatic.sh
	./setstatic.sh
	cd $thispath
fi

cd xmls
echo gen xmls
chmod u+x gen.sh
./gen.sh
cd ..

for project in `ls -F | grep build | grep "/$"| awk -F / '{print $1}'`
do 
	cd $project
	if [ -f gen.sh ]
	then
		echo gen $project
		chmod u+x gen.sh
		./gen.sh
	fi
	cd ..
done
