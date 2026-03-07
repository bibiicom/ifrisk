#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir
tput clear
tput home
snap_path=../trade/snap

echo "choose table: (null to exit)"
snap=`ls -t $snap_path | head -n 1`
ls $snap_path/$snap | awk -F . '{print NR,$1}'
read table

if [ "$table" == "" ]
then
	exit
fi
table_choose=`ls $snap_path/$snap | head -n $table | tail -n 1 | awk -F . '{print $1}'`
number=`less $snap_path/$snap/$table_choose.csv | wc -l`

if [ "$1" == "" ]
then
	page=3
else
	page=$1
fi

from=1
grep=""

while [ 1 -eq 1 ]
do
	tput clear
	tput home
	echo "table choosed is: $table_choose (records:$number;grep:$grep)"
	head -n 1 $snap_path/$snap/$table_choose.csv

	if [ "$grep" == "" ]
	then
		to=`expr $from + $page`
		if [ $to -gt $number ]
		then
			to=$number
		fi
		tnumber=`expr $to - $from`
		out=`head -n $to $snap_path/$snap/$table_choose.csv | tail -n $tnumber | awk '{printf("\\033[31m%2d,\\033[0m%s\\n",NR+"'"$from"'"-1,$1)}'`
	else
		from_grep=`expr $from - 1`
		to=`expr $from_grep + $page`
		if [ $to -gt $number ]
		then
			to=$number
		fi
		tnumber=`expr $to - $from_grep`	
		out=`grep "$grep" $snap_path/$snap/$table_choose.csv | head -n $to | tail -n $tnumber | awk '{printf("\\033[31m%2d,\\033[0m%s\\n",NR+"'"$from"'"-1,$1)}'`
	fi
	echo -e "$out"
	echo "choose $tnumber records, page [$from,$to] of $number, b:before/n:next/f:first/e:end/g:grep/q:quit" 

	read FilterValue
		
	case "$FilterValue" in
		"")
			echo ===================
			continue
			;;
		"q")
			echo abord,return 0
			exit 0
		;;
		"f")
			from=1
			continue		
		;;
		"e")
			from=`expr $number / $page \* $page`
			continue		
		;;		
		"n")
			if [ $to -eq $number ]
			then
				echo to the end, please enter b or number
				continue
			fi
			from=`expr $from + $page`
		;;
		"b")
			if [ $from -eq 1 ]
			then
				echo to the begin, please enter n or number
				continue
			fi
			from=`expr $from - $page`
		;;
		"g")
			echo enter grep words:
			read grep
			if [ "$grep" == "" ]
			then
				number=`less $snap_path/$snap/$table_choose.csv | wc -l`
			else
				number=`grep "$grep" $snap_path/$snap/$table_choose.csv | wc -l`
			fi
			from=1
		;;		
		*)
			if [[ $FilterValue =~ ^[0-9]+$ ]]
			then
				if [ $FilterValue -lt 0 -o $FilterValue -gt $number ]
				then
					echo number must less than $number, please choose again!	
					continue
				else					
					json_tmp=`head -n 1 $snap_path/$snap/$table_choose.csv`
					json_tmp+=","
					echo record choosed is:
					if [ "$grep" == "" ]
					then
						choose=`expr $FilterValue + 1`
						json_tmp+=`head -n $choose $snap_path/$snap/$table_choose.csv | tail -n 1`
					else
						choose=$FilterValue
						json_tmp+=`grep "$grep" $snap_path/$snap/$table_choose.csv | head -n $choose | tail -n 1`
					fi
					echo $json_tmp
					json_tmp=`echo $json_tmp | sed "s/\"//g"`
					json_number=`echo $json_tmp | sed "s/,/\n/g" | wc -l`
					json_number=`expr $json_number / 2`
					
					i=0
					key="";
					keys=""
					change="";
					while [ $i -lt $json_number ]
					do
						i=`expr $i + 1`
						j=`expr $i + $json_number`
						fieldName=`echo $json_tmp | sed "s/,/\n/g" | head -n $i | tail -n 1`
						if [ "${fieldName: -1}" == "*" ]
						then
							if [ ! "$key" == "" ]
							then
								key+=","
								keys+=","
							fi
							key+=${fieldName%?}
							keys+=${fieldName%?}
							key+=":"
							keys+=":"
							keyvalue=`echo $json_tmp | sed "s/,/\n/g" | head -n $j | tail -n 1`
							key+=$keyvalue
							keys+="\\033[31m"$keyvalue"\\033[0m"
						else
							change+=$fieldName","
						fi
					done
					json="{"$key"}"
					port=`grep httpfront_trade ../trade/network.ini | awk -F : '{print $3}' | awk -F / '{print $1}' | head -n 1`
					echo -e "curl -s -d \"{$keys}\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryOne/$table_choose"
					curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryOne/$table_choose
					echo ""
					while [ 1 -eq 1 ]
					do
						echo choose Action:
						echo "u: Update"
						echo "c: Clear"
						echo "d: Delete"
						read action
						
						case "$action" in
							"")
								break
							;;
							"d")
								echo -e "curl -s -d \"{$keys}\" -X POST http://127.0.0.1:$port/action/v1.0/\\033[33mSendDelete\\033[0m/$table_choose"
								echo "Are you sure to send this message(y/other)?"
								read actiony
								if [ "$actiony" == "y" ]
								then
									echo yes
									#curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendDelete/$table_choose
								fi
							;;
							"u"|"c")
								if [ "$action" == "u" ]
								then
									actionx="Update"
								else
									actionx="Clear"
								fi
								while [ 1 -eq 1 ]
								do
									echo "choose field:"
									fieldNumber=`echo ${change%?} | sed "s/,/\n/g" | awk '{print NR,$1}' | wc -l`
									echo ${change%?} | sed "s/,/\n/g" | awk '{print NR,$1}'
									read field
									if [ "$field" == "" ]
									then
										break
									fi							
									if [[ ! $field =~ ^[0-9]+$ ]]
									then
										echo not number,change again!
										continue
									fi
									if [ $field -lt 1 -o $field -gt $fieldNumber ]
									then
										echo "$field<1 or $field>$fieldNumber"
										continue
									fi
									fieldx=`echo ${change%?} | sed "s/,/\n/g" | awk '{print $1}' | head -n $field | tail -n 1`
									echo "please input value of $fieldx:(sendClear must have a valid value too)"
									read fieldValue
									if [ "$fieldValue" == "" ]
									then
										continue
									fi
									json="{"$key",$fieldx:$fieldValue}"
									jsons="{"$key",\\033[31m$fieldx\\033[0m:\\033[32m$fieldValue\\033[0m}"
									echo -e "curl -s -d \"$jsons\" -X POST http://127.0.0.1:$port/action/v1.0/\\033[33mSend$actionx\\033[0m/$table_choose"
									echo "Are you sure to send this message(y/other)?"
									read actiony
									if [ "$actiony" == "y" ]
									then
										echo yes
										#curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/Send$actionx/$table_choose		
									fi									
								done					
							;;
						esac
					done
				fi
			else
				echo not number,please choose again!
			fi	
		;;	
	esac
done
