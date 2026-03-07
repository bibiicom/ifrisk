#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

SettlementGroup=`more env.ini | grep SettlementGroup | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
if [ "$SettlementGroup" == "" ]
then
	SettlementGroup="SG1"
fi

port=`grep httpfront_trade network.ini | awk -F : '{print $3}' | awk -F / '{print $1}'`

echo "input SettleAction(null to quit):"
echo " 0: define SAT_SettlementBegin 结算操作开始"
echo " 2: Delivery 到期交割(行权)"
echo " 3: FundingRate 资金费率交换"
echo " 4: OverNightFee 隔夜费划转"
echo " 5: Clawback 分摊"
echo " 6: AccountInit 资金初始化"
echo " 7: SettlementEnd 结算操作结束"
echo " 8: DataCheck 数据检查"
echo " 9: DataRepair 数据修复"
echo " a: AccountBalance 平账"
echo " b: DataClear 清理"
echo " c: ClearNoPosition 清空没有持仓的产品"
echo " d: LoanInterest 借贷利息"

read SettleAction

case "$SettleAction" in
	"")
		exit
	;;
	"9")
		echo "input Table to DataRepair(null to quit):"
		echo " 1: Member"
		echo " 2: Order"
		echo " 3: SpotPosition 修复现货持仓冻结只对现货，同时写死了交易所"
		echo " 4: Position"
		echo " 5: MemberPosition"
		echo " 6: Account 修复资金冻结"
		echo " 7: TriggerOrder"
		echo " 8: ClearLink 如果找不到外键，将自己的主键置为空"
		read RemarkNo
		
		case "$RemarkNo" in
			"1")
			Remark=Member
			;;
			"2")
			Remark=Order
			;;
			"3")
			Remark=SpotPosition
			;;
			"4")
			Remark=Position
			;;
			"5")
			Remark=MemberPosition
			;;
			"6")
			Remark=Account
			;;
			"7")
			Remark=TriggerOrder
			;;
			"8")
			Remark=ClearLink
			;;	
		esac
		json="{SettlementGroup:$SettlementGroup,SettleAction:$SettleAction,Remark:$Remark}"
	;;	
	"b")
		echo "input Table to Clear(null to quit):"
		echo " 1: Position"
		echo " 2: MemberPosition"
		echo " 3: UserSession"
		echo " 4: Account"
		echo " 5: User"
		read RemarkNo
		
		case "$RemarkNo" in
			"1")
			Remark=Position
			;;
			"2")
			Remark=MemberPosition
			;;
			"3")
			Remark=UserSession
			;;
			"4")
			Remark=Account
			;;
			"5")
			Remark=User
			;;
		esac
		json="{SettlementGroup:$SettlementGroup,SettleAction:$SettleAction,Remark:$Remark}"
	;;
	*)
		json="{SettlementGroup:$SettlementGroup,SettleAction:$SettleAction}"
	;;
esac

echo curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendSettlement
curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendSettlement
echo ""
