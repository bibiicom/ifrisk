#!/bin/bash

#最后最新的文件夹不打包，大概率查询的时候会用到，免得又需要解压

workdir=$(cd $(dirname $0); pwd)
cd $workdir

if [ "$1" == "" ]
then
	days=60
else
	days=$1
fi

daysago=`date -d "$days days ago" +%Y%m%d%H%M`
echo $daysago ========

for module in `ls -F *.tar.gz | awk -F . '{print $1}'`
do
        if [ $module -lt $daysago ]
        then
                echo echo rm $module.tar.gz
				rm -rf $module.tar.gz
        fi
done
