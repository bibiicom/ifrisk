#!/bin/bash

srcversion=`cat version`
gccversion=`gcc --version | grep gcc | awk '{ print $3 }'`
buildversion=${srcversion}_$gccversion
echo build version is: [$buildversion]

echo '#define INNER_BASE_VERSION' \"$buildversion\" > src_base/function/versionValue.h
echo '#define INNER_DUE_DATE' \"20991230\" >> src_base/function/versionValue.h

echo "$buildversion" > version.sh

thispath=`pwd`
if [ -f ./libs/setstatic.sh ]
then
	cd ./libs
	chmod u+x setstatic.sh
	./setstatic.sh
	cd $thispath
fi

cd build

#编译所有的模块
for project in `ls -F | grep "/$"| awk -F / '{print $1}'`
do 
	cd $project
	if [ -f makefile.linux ]
	then
		echo make $project
		make -j3 -f makefile.linux 1>output 2>&1
		grep error: output
	fi
	if [ -f build.sh ]
	then
		echo build $project
		chmod u+x build.sh
		./build.sh
	fi
	for sub in `ls -F | grep "/$"| awk -F / '{print $1}'`
	do 
		cd $sub
		if [ -f makefile.linux ]
		then
			echo make $sub
			make -j3 -f makefile.linux 1>output 2>&1
			grep error: output
		fi
		if [ -f build.sh ]
		then
			echo build $sub
			chmod u+x build.sh
			./build.sh
		fi	
		cd ..
	done	
	cd ..
done 

