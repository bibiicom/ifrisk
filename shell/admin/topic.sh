#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

if [ "$1" == "" ]; then
	echo "./topic.sh public_0|private_1|trade_0"
	grep wsfront_* network.ini
	exit
fi

wsport=`grep wsfront_$1 network.ini | awk -F "=" '{print $NF}' | awk -F "|" '{print $NF}' | head -n 1`
echo PortFrom:$wsport

MemberID=`more env.ini | grep MemberID | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
echo MemberID:$MemberID

echo 'Private_Order "3" 私有交易主题 FilterValue=MemberID'
echo 'Private_SnapShot "12" 私有快照主题 FilterValue=MemberID)'
echo 'Market_LastTrade "2" 最近成交 FilterValue=ExchangeID_InstrumentID'
echo 'Market_LastKLine "11" 最近K线(主要是1分钟)推送主题 FilterValue=ExchangeID_InstrumentID_Period'
echo 'Market_KLine "4" 历史K线主题 FilterValue=ExchangeID_InstrumentID_Period'
echo 'Market_SnapShot "5" 5档行情快照主题[可通过Numbers降频] FilterValue=ExchangeID_InstrumentID[_Numbers]'
echo 'Market_Broad "6" 广播主题,合约变化,时钟心跳等 '
echo 'Market_OverView "7" 行情概要快照主题 FilterValue=ExchangeID_InstrumentID'
echo 'Market_OverView_Delay "17" 行情概要延时快照主题 FilterValue=ExchangeID_InstrumentID)'
echo 'Market_Depth_All "8" 全档行情增量主题 FilterValue=ExchangeID_InstrumentID'
echo 'Market_Depth_Delay "18" 全档行情增量延时发送主题 FilterValue=ExchangeID_InstrumentID'
echo 'Market_Depth_25 "25" 25档行情增量主题 FilterValue=ExchangeID_InstrumentID'
echo 'Private_Risk "9" 风控数据(持仓、资金、行情、时间),分控用户才能订阅 '
echo 'Private_Trade "13" 私有成交主题 FilterValue=MemberID'
echo 'Private_TriggerOrder "14" 策略触发程序使用的私有主题 FilterValue=MemberID'
echo 'Private_FinishOrder "16" 管理后台私有完成订单主题 FilterValue=MemberID'
echo 'Private_Copy_Finish "10" 跟单完成订单主题 FilterValue=MemberID'		
echo 'Private_UserSession "99" 用户Session主题 '
echo "enter number, null for 25"
read TopicID
if [ "" == "$TopicID" ]
then
	TopicID=25
fi


if [ ! "$PortFrom" == "trade" ]
then
	case $TopicID in
		3|12)
			echo "enter MemeberID, null for all"
			read FilterValue
		;;
		2|5|7|17|25)
			out=`cat ./log/Instruments`
			echo -e "$out"
			echo "enter Number of Instruments, null for all"
			read choose
			if [ "$choose" == "" ];	then
				FilterValue=""
			else
				FilterValue=`head -n $choose ./log/Instruments | tail -n 1 | awk '{print $15"_"$3}'`
			fi
		;;
		*)
		echo no FilterValue
	esac

fi

json="{SendTopicAction:{Action:1,LocalNo:4,FilterValue:$FilterValue,ResumeNo:-1,TopicID:$TopicID}}"
chmod u+x wsclient

echo "./wsclient ws://127.0.0.1:$wsport \"$json\""
./wsclient ws://127.0.0.1:$wsport $json