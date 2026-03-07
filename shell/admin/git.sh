#!/bin/bash
#change mdb from *.csv

workdir=$(cd $(dirname $0); pwd)
cd $workdir

git=`more env.ini | grep git | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
echo git:$git
git config --global credential.helper store

if [ -d "config" ]
then
	cd config
	git pull
else
	git clone $git ./config
	cd config
fi

version=`git rev-parse --short HEAD`
cd ..
echo "new version is $version"
if [ ! -e "configLast" ]
then
	echo $version > configLast
fi

versionLast=`cat configLast`
if [ ! "$version" == "$versionLast" ]
then
	echo "change $versionLast to $version"
	
	for csv in `ls config/table_insert_update/*.csv`
	do
		#./csv.sh $csv
		echo $csv
	done
	for csv in `ls config/table_delete/*.csv`
	do
		#./csv.sh $csv Delete
		echo $csv delete
	done	
	echo $version > configLast
fi


