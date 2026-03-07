#!/bin/bash
#change mdb from *.csv

workdir=$(cd $(dirname $0); pwd)
cd $workdir

if [ "$1" == "" ]
then
	echo "usage: csv.sh *.csv"
	echo "pinks: change mdb from *.csv"
	exit
else
	csv=$1
fi

if [ "$2" == "" ]
then
	Action=InsUpd
else
	Action=$2
fi

head -n 20 $1
echo "are you sure to chage mdb from $1 (yes/other)?"
read sure
if [ ! "$sure" == "yes" ]
then
	exit
fi

csvhead=`head -n 1 $1`
csvnumber=`echo $csvhead | sed "s/,/\n/g" | wc -l`

i=0
while IFS= read -r line
do
	if [ $i -eq 0 ]
	then
		i=`expr $i + 1`
		continue;
	fi
	
	json="{"
	j=1
	while [ $j -lt $csvnumber ]
	do
		if [ ! "$json" == "{" ]
		then
			json+=","
		fi
		fieldName=`echo $csvhead | sed "s/,/\n/g" | head -n $j | tail -n 1`
		fieldValue=`echo $line | sed "s/,/\n/g" | head -n $j | tail -n 1`
		if [ "${fieldName: -1}" == "*" ]
		then
			fieldName=${fieldName%?}
		fi
		if [ "${fieldName: -2}" == "*\"" ]
		then
			fieldName=${fieldName:0:-2}"\""
		fi		
		json+="\""$fieldName"\":\""$fieldValue"\""
		j=`expr $j + 1`
	done
	json+="}"
	table=${csv##*/}
	table=${table%.csv}
	port=`grep httpfront_trade ../trade/network.ini | awk -F : '{print $3}' | awk -F / '{print $1}' | head -n 1`
	echo "curl -s -d '$json' -X POST http://127.0.0.1:$port/action/v1.0/Send$Action/$table"
	curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/Send$Action/$table
	echo ""
	i=`expr $i + 1`
done < "$csv"

