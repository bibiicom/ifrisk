#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd $workdir

SettlementGroup=`more env.ini | grep SettlementGroup | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
if [ "$SettlementGroup" == "" ]; then
	SettlementGroup="SG1"
fi
echo SettlementGroup:$SettlementGroup

TradingRightGroup=`more env.ini | grep TradingRightGroup | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
echo TradingRightGroup:$TradingRightGroup

ExchangeID=`more env.ini | grep ExchangeID | grep -v \# | awk -F = '{print $2}'| tr -d '\r'`
if [ "$ExchangeID" == "" ]
then
	ExchangeID="Test"
fi
ProductGroup=Swap
PriceCurrency=USD

port=`grep httpfront_trade network.ini | awk -F : '{print $3}' | awk -F / '{print $1}' | head -n 1`

while [ 1 -eq 1 ]; do
	echo All Instruments in $ProductGroup are belowing:
	json="{ProductGroup:$ProductGroup}"
	echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendQryInstrument"
	echo "---------------------------------------------------------------------------------------------------------------------"
	curl -s -H "Zip:4" -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendQryInstrument | sed "s/\[/\n/g" | tail -n +4
	echo ""
	echo "---------------------------------------------------------------------------------------------------------------------"
	echo "input BaseCurrency Crypto (null to quit):"
	read BaseCurrency
	if [ "" == "$BaseCurrency" ]; then
		exit
	fi
	echo "input MM Cropto No (null to quit)"
	echo "1: ${BaseCurrency}-${ClearCurrency}-SWAP_3  [OKX0.01]"
	echo "2: ${BaseCurrency,,}${ClearCurrency,,}_4  [binance0.001]"
	echo "3: ${BaseCurrency,,}${ClearCurrency,,}_5  [binance0.01]"
	echo "4: ${BaseCurrency}-${ClearCurrency}_6  [Stable]"
	echo "others: input by hand"
	read MMNo
	case "$MMNo" in
    "1")
        MM=${BaseCurrency}-${ClearCurrency}_3
        ;;
    "2")
		MM=${BaseCurrency,,}${ClearCurrency,,}_4
        ;;
    "3")
        MM=${BaseCurrency,,}${ClearCurrency,,}_5
        ;;
    "4")
        MM=${BaseCurrency}-${ClearCurrency}_6
        ;;
    *)
		read MM
        ;;
	esac

	ClearCurrency=$BaseCurrency
	echo "input MM Cropto Name (null to quit/BTC-USDT_3[OKX0.01]/btcusdt_4[binance0.001]/btcusdt_5[binance0.01]/USD-USDC_6[Stable]):"
	read MM
	if [ "" == "$MM" ];	then
		exit
	fi

	InstrumentID="$BaseCurrency$PriceCurrency"
	InstrumentName="$BaseCurrency$PriceCurrency"
	Remark="M|$MM"
	ExpireTime=""
	IsInverse=1
	ProductClass=1
	## <EnumChar typename="ProductClass" label="产品大类" prefix="PC" type="string">
	## 	<enum name="Margin" value="1" label="保证金交易"/>
	## 	<enum name="Premium" value="2" label="权利金交易"/>
	## 	<enum name="Exchange" value="4" label="兑换交易"/>
	## 	<enum name="Combination" value="5" label="组合"/>
	## 	<enum name="Market" value="6" label="行情"/>
	## </EnumChar>
	ProductType=2
	## <EnumChar typename="ProductType" label="产品类型" prefix="PT" type="string">
	## 	<enum name="Delivery" value="1" label="交割期货"/>
	## 	<enum name="Swap" value="2" label="永续、利率互换期货"/>
	## 	<enum name="Spot" value="3" label="即期/日交割期货"/>
	## 	<enum name="Delay" value="4" label="递延交割期货"/>
	## 	<enum name="Warrants" value="5" label="权证"/>
	## 	<enum name="AmericanOptions" value="6" label="美式期权"/>
	## 	<enum name="EuropeanOptions" value="7" label="欧式期权"/>
	## 	<enum name="CryptoExchange" value="8" label="币币交易"/>
	## 	<enum name="ForeignExchange" value="9" label="外汇兑换"/>
	## 	<enum name="Stock" value="a" label="股票"/>
	## 	<enum name="Bond" value="b" label="债券"/>
	## 	<enum name="ETF" value="c" label="基金"/>
	## 	<enum name="IndexMarket" value="A" label="指数行情"/>
	## 	<enum name="LinkedMarket" value="B" label="连续行情"/>
	## </EnumChar>

	json="{ExchangeID:$ExchangeID,InstrumentID:$InstrumentID,InstrumentName:$InstrumentName,SettlementGroup:$SettlementGroup,ProductClass:$ProductClass,ProductGroup:$ProductGroup,ProductType:$ProductType,VolumeMultiple:1,PriceCurrency:$PriceCurrency,ClearCurrency:$ClearCurrency,IsInverse:$IsInverse,BaseCurrency:$BaseCurrency,TradingModel:0,DayStartTime:00:00:00,ExpireTime:$ExpireTime,InstrumentStatus:2,PriceTick:0.01,VolumeTick:1,TradingRightGroup:$TradingRightGroup,DefaultLeverage:5,Remark:$Remark}"

	echo "curl -d \"$json\" -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Instrument"

	echo "are you sure? (y/a:again/c:continue/null:exit)"
	read sure	
	case $sure in
	"y")
		curl -d $json -X POST http://127.0.0.1:$port/action/v1.0/SendInsert/Instrument
		echo ""	
		;;
	"a")
		./instrumentSwapInverse.sh
		;;
	"c")
		continue
		;;
	*)
		exit
		;;
	esac
done