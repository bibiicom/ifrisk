#!/bin/bash
echo "----------------------------------------------"
echo "---------- all releases are below  -----------"
echo "----------------------------------------------"
v=1
name=""
for release in ../release/*/*bin*.tar.gz
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
for release in ../release/*/*bin*.tar.gz
do
	if [ "$v" == "$number" ]
	then
		tar -xvf $release -C ./
	fi	
	v=`expr $v + 1`
done 


echo ""
./get_release.sh
