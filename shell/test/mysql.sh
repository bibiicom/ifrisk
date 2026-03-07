#!/bin/bash

workdir=$(cd $(dirname $0); pwd)

dbconfig="$workdir/../deploy/network.ini"

dbname=`grep 'dbname=' $dbconfig | grep -v '#' | awk -F = '{print $2}'`
dbuser=`grep 'dbuser=' $dbconfig | grep -v '#' | awk -F = '{print $2}'`
dbpasswd=`grep 'dbpasswd=' $dbconfig | awk -F = '{print $2}'`
ipall=${dbname##MYSQL://}
ipport=${ipall%@*}
database=${ipall#*@}
ip=${ipport%:*}
port=${ipport#*:}

#echo mysql -h$ip -P$port -u$dbuser -p$dbpasswd "<" $1
if [ "$1" == "" ]
then
	mysql -h$ip -P$port -u$dbuser -p$dbpasswd
else
	mysql -h$ip -P$port -u$dbuser -p$dbpasswd -f < $1
fi
