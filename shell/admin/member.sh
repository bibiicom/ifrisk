#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir
mkdir log  2>/dev/null
MemberIDDefault=`cat ./log/MemberID`

echo "please input MemberID[null for $MemberIDDefault]:"
read MemberID

if [ "" == "$MemberID" ]; then
	MemberID=$MemberIDDefault
fi
if [ "" == "$MemberID" ]; then
	exit
fi

echo $MemberID > ./log/MemberID

while [ 1 -eq 1 ]
do
	json="{MemberID:$MemberID}"
	port=`grep httpfront_trade network.ini | awk -F : '{print $3}' | awk -F / '{print $1}' | head -n 1`
	out=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryOne/Member`
	MemberFound=`echo $out | grep RecordNotFound`
	if [ "$MemberFound" == "" ]
	then
		tput clear
		tput home
		json="{MemberID:$MemberID}"
		out=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryOne/Member`
		echo -e "\\033[33m[  Member]:\\033[0m" "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryOne/Member"
		echo -e "$out"
		echo -e "\\033[33m[Accounts]:\\033[0m" "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryAccount"
		echo -e "\\033[32mNo   SG Currency  Available   UnRealProfit FrozenMargin   FrozenMoney     FrozenFee       Balance     Borrow\\033[0m"
		outAccount=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryAccount | sed "s/:{/\n/g"|sed "s/\"//g" | head -n 12 | tail -n +2 | awk -F ",|:" '{printf("\\033[33m%2s\\033[0m %4s %6s %12.2f +%12.2f +%12.2f +%12.2f +%12.2f =%12.2f %10.2f\n",NR,$2,$6,$10,$82,$30,$32,$34,$16,$88)}'`
		echo -e "$outAccount"
		echo ""
		json="{MemberID:$MemberID,PositionClass:5}"
		echo -e "\\033[33m[   Loans]:\\033[0m" "curl -s -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryPosition"
		echo -e "\\033[32mNo        InstrumentID  l/s CM   Position LeX  UseMargin MaitMargin UnReProfit  CostPrice LiqudPrice SLTriPrice TPTriPrice\\033[0m"	
		out=`curl -s -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryPosition | sed "s/data\":/\n/g"| head -n 14 | tail -n +2 | sed "s/\"//g" |awk -F ":|," '{l="\\033[31mlong \\033[0m";s="\\033[32mshort\\033[0m";printf("\\033[33m%2s\\033[0m %19s %s %c %10.2f %3s %10s %10s %10s %10s %10s %10s %10s\n",NR,$10,$16==1||$20<0?s:l,$72==0?87:89,$20,$60,$38,$92,$94,$36,$96,$74,$76)}'`
	echo -e "$out"
		echo ""
		echo -e "\\033[33m[Currency]:\\033[0m" "curl -s -d \"{}\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryCurrency"
		echo -e "\\033[32mNo    SG Currency      Balance        Deposit    Withdraw    LoanTotal      LoanUse LoanB% LoanD%  HD    MAX  InstrumentID\\033[0m"
		out=`curl -s -d "{}" -X POST http://127.0.0.1:$port/action/v1.0/SendQryCurrency | sed "s/:{/\n/g" | tail -n +2 | sed "s/\"//g" | awk -F ",|:" '{printf("%2s %5s %8s %12.0f  %12.0f %12.0f %12.0f %12.0f %6.2f %6.2f %3.1f %6.0f %13s\n",NR,$2,$4,$10,$6,$8,$30,$28,$34*100,$36*100,$46,$48,$26)}'`
		echo -e "$out"
		echo "1: Deposit             入金"
		echo "2: Withdraw            出金"		
		echo "3: PositionType        改变持仓方式"
		echo "4: RiskLevel           改变风控级别"
		echo "5: FeeDeduce           改变手续费抵扣币种"
		echo "6: FeeGrade            改变手续费等级"
		echo "7: MarginRateGrade     改变手续费等级"
		echo "8: TradingRightGrade   改变权限等级"
		echo "b: LoanLend            借款"
		echo "c: LoanBack            回款"
		read ActionType

		case "$ActionType" in
			"b"|"c")
				while [ 1 -eq 1 ]
				do
					echo -e "\\033[32mNo   SG Currency  Available   UnRealProfit FrozenMargin   FrozenMoney     FrozenFee       Balance     Borrow\\033[0m"
					echo -e "$outAccount"
					echo "input number of Account"
					read number
					if [ "" == "$number" ]; then
						continue
					fi
					SettlementGroup=`echo $outAccount | head -n $number | tail -n 1 | awk '{print $3}'`
					Currency=`echo $outAccount | head -n $number | tail -n 1 | awk '{print $4}'`
					echo "input Amount(null to quit):"
					read Amount
					if [ "" == "$Amount" ]; then
						continue
					fi
					json="{AccountID:$MemberID,MemberID:$MemberID,ActionType:$ActionType,Currency:$Currency,Amount:$Amount,SettlementGroup:$SettlementGroup}"
					echo curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendAccountAction
					curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendAccountAction
					break;
				done
			;;
			"1")
				while [ 1 -eq 1 ]
				do
					echo "input Currency(null to quit):"
					chmod u+x currency.sh
					./currency.sh
					choose=$?
					if [ "$choose" == "0" ]; then
						break
					else
						Currency=`head -n $choose ./log/Currencys | tail -n 1 | awk '{print $3}'`
						SettlementGroup=`head -n $choose ./log/Currencys | tail -n 1 | awk '{print $2}'`
					fi
					echo "input Amount(null to quit):"
					read Amount
					if [ "" == "$Amount" ]; then
						continue
					fi
					json="{AccountID:$MemberID,MemberID:$MemberID,ActionType:$ActionType,Currency:$Currency,Amount:$Amount,SettlementGroup:$SettlementGroup}"
					echo curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendAccountAction
					curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendAccountAction
					break
				done
			;;	
			"2")
				while [ 1 -eq 1 ]
				do
					echo -e "\\033[32mNo   SG Currency  Available   UnRealProfit FrozenMargin   FrozenMoney     FrozenFee       Balance     Borrow\\033[0m"
					echo -e "$outAccount"
					echo "input number of Account"
					read number
					if [ "" == "$number" ]; then
						continue
					fi
					SettlementGroup=`echo $outAccount | head -n $number | tail -n 1 | awk '{print $3}'`
					Currency=`echo $outAccount | head -n $number | tail -n 1 | awk '{print $4}'`
					echo "input Amount(null to quit):"
					read Amount
					if [ "" == "$Amount" ]; then
						continue
					fi
					json="{AccountID:$MemberID,MemberID:$MemberID,ActionType:$ActionType,Currency:$Currency,Amount:$Amount,SettlementGroup:$SettlementGroup}"
					echo curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendAccountAction
					curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendAccountAction
					break
				done
			;;
			"3")
				echo "PositionType:"
				echo "1: 综合持仓(双边持仓)"
				echo "2: 净持仓(单边持仓)"
				read PositionType
				json="{MemberID:$MemberID,PositionType:$PositionType}"
				echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendUpdate/Member"
				curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendUpdate/Member
			;;
			"4")
				echo "RiskLevel:"
				echo "0: Currency   本币种资金权益"
				echo "1: Settlement 结算组所有资金权益"
				echo "2: Account    资金账号所有资金权益"
				echo "3: Member     会员账号所有资金权益"
				read RiskLevel
				json="{MemberID:$MemberID,RiskLevel:$RiskLevel}"
				echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendUpdate/Member"
				curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendUpdate/Member
			;;
		esac
        read waiting
	else
		echo "InputAction:"
		echo "1: Add Normal          增加普通用户"
		echo "2: InnerMarketMaker    增加内部做市商（不做资金控制）"
		echo "3: MarketMaker         增加做市商（做资金控制）"
		read Action
		case "$Action" in
			"1")
				json="{MemberID:$MemberID,MemberType:a}"
				jsonu="{UserID:$MemberID,Password:111111}"
				echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Member"
				curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Member
				echo "curl -d \"$jsonu\" -X POST http://127.0.0.1:$port/action/v1.0/SendInsUpd/UserSession"
				curl -d $jsonu -X POST http://127.0.0.1:$port/action/v1.0/SendInsUpd/UserSession
			;;
			"2")
				json="{MemberID:$MemberID,UserID:$MemberID,MemberType:b,Priority:0,Grade:mm,FeeGrade:mm,MarginRateGrade:mm,TradingRightGrade:mm,CFDGrade:mm,LiquidityGrade:mm,PositionType:2}"
				jsonu="{UserID:$MemberID,Password:111111,UserType:5}"	
				echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Member"
				curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Member
				echo "curl -d \"$jsonu\" -X POST http://127.0.0.1:$port/action/v1.0/SendInsUpd/UserSession"
				curl -d $jsonu -X POST http://127.0.0.1:$port/action/v1.0/SendInsUpd/UserSession
			;;
			"3")
				json="{MemberID:$MemberID,UserID:$MemberID,MemberType:a,Priority:0,Grade:mm,FeeGrade:mm,MarginRateGrade:mm,TradingRightGrade:mm,CFDGrade:mm,LiquidityGrade:mm,PositionType:2}"
				jsonu="{UserID:$MemberID,Password:111111,UserType:5}"
				echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Member"
				curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Member
				echo "curl -d \"$jsonu\" -X POST http://127.0.0.1:$port/action/v1.0/SendInsUpd/UserSession"
				curl -d $jsonu -X POST http://127.0.0.1:$port/action/v1.0/SendInsUpd/UserSession
			;;
		esac
        read waiting
	fi
done