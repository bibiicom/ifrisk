#!/bin/bash
echo "----------------------------------------------"
echo "---------- all releases are below  -----------"
echo "----------------------------------------------"
v=1
for release in `ls -tr ../release/*/*.tar.gz 2>/dev/null`
do
	choice=`echo $release | awk -F / '{print $4}'` 
	echo $v: $choice
	v=`expr $v + 1`
done 

echo "----------------------------------------------"
echo "input choice (q or null to quit):"

read number
if [ "q" == "$number" -o "" == "$number" ]
then
	cd ..
	exit
fi

v=1
for release in `ls -tr ../release/*/*.tar.gz 2>/dev/null`
do
	if [ "$v" == "$number" ]
	then
		tar -xvf $release -C ../test/
	fi	
	v=`expr $v + 1`
done 

echo ""
./get.sh
