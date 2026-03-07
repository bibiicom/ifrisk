#!/bin/bash

#最后最新的文件夹不打包，大概率查询的时候会用到，免得又需要解压

workdir=$(cd $(dirname $0); pwd)
cd $workdir

if [ "$1" == "" ]
then
	keepnum=5
else
	keepnum=$1
fi

maxsnap=`ls -F | grep "/$" | awk -F / '{print $1}' | sort -r | head -n $keepnum | tail -n 1`

for module in `ls -F | grep "/$" | awk -F / '{print $1}' | sort -r`
do
	if [ $module -ge $maxsnap ]
	then
		echo $module bigger than $maxsnap skip ..
		continue
	fi
	
	if [ -f $module.tar.gz ]
	then
		echo $module.tar.gz is exit, skip gzip
		continue
	fi
	
	if [ -f $module.tar ]
	then
		echo $module.tar is exit, skip tar
		continue
	fi
	
	checkempty=`ls -F $module`
	if [ "$(ls -A $module)" = "" ]
	then
		echo  $module is empty, do nothing and rm it 
		rm -rf $module
	else
		tar cvf $module.tar ./$module
		gzip $module.tar
		rm -rf $module
	fi
done
