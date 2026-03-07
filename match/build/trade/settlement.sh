#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

path=`pwd`
echo $path

if [ "$1" == "" ]; then
	echo "///结算操作开始"
	echo "#define SAT_SettlementBegin '0'"
	echo "///定期无负债结算"
	echo "#define SAT_Settlement '1'"
	echo "///到期交割(行权)"
	echo "#define SAT_Delivery '2'"
	echo "///资金费率交换"
	echo "#define SAT_FundingRate '3'"
	echo "///隔夜费划转"
	echo "#define SAT_OverNightFee '4'"
	echo "///分摊"
	echo "#define SAT_Clawback '5'"
	echo "///资金初始化"
	echo "#define SAT_AccountInit '6'"
	echo "///结算操作结束"
	echo "#define SAT_SettlementEnd '7'"
	echo "///数据检查"
	echo "#define SAT_DataCheck '8'"
	echo "///数据修复"
	echo "#define SAT_DataRepair '9'"
	echo "///平账"
	echo "#define SAT_AccountBalance 'a'"
	echo "///清理"
	echo "#define SAT_DataClear 'b'"
	exit
fi

number=${path##*trade}
echo $number
if [ "$number" == "" ]; then
	number=0
fi

echo httpfront_trade_$number 
#httpfront_marketdata_0=manager|http://127.0.0.1:10100/action#1
port=`grep httpfront_trade_$number network.ini | head -n 1 | awk -F : '{print $3}' | awk -F / '{print $1}'`

for sg in $(curl -s http://127.0.0.1:$port/action/v1.0/SendQryCurrency?X=1 | jq -r '.result[].data.SettlementGroup' | sort -u); do
	payload="{\"SettleAction\":\"$1\",\"SettlementGroup\":\"$sg\"}"
    echo "curl -d '$payload' -X POST http://127.0.0.1:$port/action/v1.0/SendSettlement"
    curl -d "$payload" -X POST http://127.0.0.1:$port/action/v1.0/SendSettlement
	echo ""
done
