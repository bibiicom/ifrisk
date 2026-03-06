#!/bin/bash

cd build

for project in `ls -F | grep "/$"| awk -F / '{print $1}'`
do 
	cd $project
	if [ -f pack.sh ]
	then
		echo pack $project
		chmod u+x pack.sh
		./pack.sh
	fi
	cd ..
done


chmod u+x */*.so*
tarstr=`find . -maxdepth 4 -name "*" -type f -executable | grep -v "\.sh"`

srcversion=`cat ../version`
gccversion=`gcc --version | grep gcc | awk '{ print $3 }'`
buildversion=${srcversion}_$gccversion
tarname=$buildversion.bin.tar

tar -cvf $tarname $tarstr ./api/*/*.so ./api/*/include/ ./sql/*.sql 1>/dev/null
tar -rvf $tarname ./api/i5ting/preview ./api/i5ting/example ./*/*.example ./*/*.ini* 1>/dev/null
tar -rvf $tarname ./*.sh --exclude=./build.sh --exclude=./pack.sh --exclude=./gen.sh 1>/dev/null
tar -rvf $tarname ./*/*.sh --exclude=./*/build.sh --exclude=./*/pack.sh --exclude=./*/gen.sh 1>/dev/null
gzip $tarname
mv $tarname.gz ../
