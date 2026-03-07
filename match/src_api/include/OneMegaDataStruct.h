/////////////////////////////////////////////////////////////////////////
///OneMegaDataStruct.h
///定义了客户端接口使用的业务数据结构
/////////////////////////////////////////////////////////////////////////

#ifndef __ONEMEGA_DATASTRUCT_H__
#define __ONEMEGA_DATASTRUCT_H__

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

namespace OneMegaByte
{
	#define ONE_CHAIN_FIRST			'F'		//第一个
	#define ONE_CHAIN_CONTINUE		'C'		//还有后续
	#define ONE_CHAIN_LAST			'L'		//最后一个
	#define ONE_CHAIN_SINGLE		'S'		//只有一个报文
	///订阅主题分类
	#define ONE_Topic_Private_Order "3"	//私有交易主题
	#define ONE_Topic_Private_SnapShot "12"	//私有快照主题
	#define ONE_Topic_Market_LastTrade "2"	//最近成交
	#define ONE_Topic_Market_LastKLine "11"	//最近K线(主要是1分钟)推送主题
	#define ONE_Topic_Market_KLine "4"	//历史K线主题
	#define ONE_Topic_Market_SnapShot "5"	//5档行情快照主题[可通过Numbers降频]
	#define ONE_Topic_Market_Broad "6"	//广播主题,合约变化,时钟心跳等
	#define ONE_Topic_Market_OverView "7"	//行情概要快照主题
	#define ONE_Topic_Market_OverView_Delay "17"	//行情概要延时快照主题
	#define ONE_Topic_Market_Depth_All "8"	//全档行情增量主题
	#define ONE_Topic_Market_Depth_Delay "18"	//全档行情增量延时发送主题
	#define ONE_Topic_Market_Depth_25 "25"	//25档行情增量主题
	#define ONE_Topic_Market_Depth_50 "50"	//50档行情增量主题
	#define ONE_Topic_Market_Depth_100 "100"	//100档行情增量主题
	#define ONE_Topic_Private_Risk "9"	//风控数据(持仓、资金、行情、时间),分控用户才能订阅
	#define ONE_Topic_Private_Trade "13"	//私有成交主题
	#define ONE_Topic_Private_TriggerOrder "14"	//策略触发程序使用的私有主题
	#define ONE_Topic_Private_FinishOrder "16"	//管理后台私有完成订单主题
	#define ONE_Topic_Private_Copy_Finish "10"	//跟单完成订单主题
	#define ONE_Topic_Private_UserSession "99"	//用户Session主题
	///如果有其他流水，应用程序中可以自行设置，但是不能和上面的重合

	///自定义类型类型
	typedef char MegaDefineTypeType;
	///布尔型类型
	typedef int MegaBoolType;
	///数字类型
	typedef int MegaNumberType;
	///累积量(只加不减)类型
	typedef double MegaCollectType;
	///汇总(加加减减)类型
	typedef double MegaTotalType;
	///成本(为了计算更加精确)类型
	typedef double MegaCostType;
	///杠杆(不累加的精度控制)类型
	typedef double MegaLeverageType;
	///价格类型
	typedef double MegaPriceType;
	///比率类型
	typedef double MegaRatioType;
	///资金类型
	typedef double MegaMoneyType;
	///数量类型
	typedef double MegaVolumeType;
	///精确到毫秒的时间戳类型
	typedef double MegaMilliSecondsType;
	///大额数字类型
	typedef double MegaLargeNumberType;
	///K线周期类型
	typedef char MegaBarType[4];
	///日期类型
	typedef char MegaDateType[9];
	///时间类型
	typedef char MegaTimeType[9];
	///币种类型
	typedef char MegaCurrencyType[13];
	///短唯一标识类型
	typedef char MegaShortIndexIDType[9];
	///交易所ID类型
	typedef char MegaExchangeIDType[9];
	///IP地址类型
	typedef char MegaIPAddressType[16];
	///唯一编号类型
	typedef char MegaIndexIDType[21];
	///Mac地址类型
	typedef char MegaMacAddressType[21];
	///名字类型
	typedef char MegaNameType[33];
	///标的ID类型
	typedef char MegaInstrumentIDType[27];
	///长唯一编号类型
	typedef char MegaLongIndexIDType[43];
	///密码类型
	typedef char MegaPasswordType[33];
	///资金账号类型
	typedef char MegaAccountIDType[45];
	///长名字类型
	typedef char MegaLongNameType[65];
	///超短内容类型
	typedef char MegaShortContentType[129];
	///短内容类型
	typedef char MegaContentType[257];
	///长内容类型
	typedef char MegaLongContentType[1025];
	///交易用户类型类型
	typedef char MegaUserTypeType;
	#define Mega_UserType_Trader '1'	///普通用户(订阅自己/可下自己订单)
	#define Mega_UserType_Observer '2'	///观察用户OB(订阅其他用户/不可下单)
	#define Mega_UserType_SuperManager '5'	///超级管理员(订阅所有/可给所有用户下单/可爆仓)
	///产品大类类型
	typedef char MegaProductClassType;
	#define Mega_ProductClass_Margin '1'	///保证金交易
	#define Mega_ProductClass_Premium '2'	///权利金交易
	#define Mega_ProductClass_AssetsExchange '3'	///资产转让（股票/基金/国债等）
	#define Mega_ProductClass_AccountExchange '4'	///兑换交易（资金之间的互换）
	#define Mega_ProductClass_Combination '5'	///组合
	#define Mega_ProductClass_Market '6'	///行情
	///产品类型类型
	typedef char MegaProductTypeType;
	#define Mega_ProductType_Delivery '1'	///交割期货
	#define Mega_ProductType_Swap '2'	///永续、利率互换期货
	#define Mega_ProductType_Spot '3'	///即期/日交割期货
	#define Mega_ProductType_Delay '4'	///递延交割期货
	#define Mega_ProductType_Warrants '5'	///权证
	#define Mega_ProductType_AmericanOptions '6'	///美式期权
	#define Mega_ProductType_EuropeanOptions '7'	///欧式期权
	#define Mega_ProductType_CryptoExchange '8'	///币币交易
	#define Mega_ProductType_ForeignExchange '9'	///外汇兑换
	#define Mega_ProductType_Stock 'a'	///股票
	#define Mega_ProductType_Bond 'b'	///债券
	#define Mega_ProductType_ETF 'c'	///基金
	///业务类别类型
	typedef char MegaBusinessTypeType;
	#define Mega_BusinessType_Normal '0'	///普通
	#define Mega_BusinessType_PurchaseOrRedeem '1'	///申赎
	#define Mega_BusinessType_OptionExercise '2'	///行权
	#define Mega_BusinessType_MiddlePositionApply '3'	///中立仓申报
	#define Mega_BusinessType_DeferDeliverApply '4'	///递延交割申报
	#define Mega_BusinessType_Swap '5'	///互换定单
	#define Mega_BusinessType_Mortgage '6'	///质押
	#define Mega_BusinessType_MergeOrSplit '7'	///合并分拆
	#define Mega_BusinessType_SwapEquity '8'	///转股
	#define Mega_BusinessType_SaleOrBuyBack '9'	///回售回购
	#define Mega_BusinessType_Vote 'a'	///投票
	#define Mega_BusinessType_MarginTrading 'b'	///融资融券
	#define Mega_BusinessType_OptionsExecution 'c'	///期权执行
	#define Mega_BusinessType_CloseOrderBusinessValue 'd'	///传递被平仓订单BusinessValue
	#define Mega_BusinessType_R11 'P'	///自定义11
	#define Mega_BusinessType_R10 'Q'	///自定义10
	#define Mega_BusinessType_R9 'R'	///自定义9
	#define Mega_BusinessType_R8 'S'	///连续交易
	#define Mega_BusinessType_R7 'T'	///自定义7
	#define Mega_BusinessType_R6 'U'	///画线委托
	#define Mega_BusinessType_R5 'V'	///限价市价
	#define Mega_BusinessType_R4 'W'	///条件
	#define Mega_BusinessType_R3 'X'	///止盈止损
	#define Mega_BusinessType_R2 'Y'	///追踪出场
	#define Mega_BusinessType_R1 'Z'	///指标类型
	///期权类型类型
	typedef char MegaOptionsTypeType;
	#define Mega_OptionsType_NotOptions '0'	///非期权
	#define Mega_OptionsType_CallOptions '1'	///看涨
	#define Mega_OptionsType_PutOptions '2'	///看跌
	///标的交易状态类型
	typedef char MegaInstrumentStatusType;
	#define Mega_InstrumentStatus_BeforeTrading '0'	///开盘前
	#define Mega_InstrumentStatus_NoTrading '1'	///非交易
	#define Mega_InstrumentStatus_Continous '2'	///连续交易
	#define Mega_InstrumentStatus_AuctionOrdering '3'	///集合竞价报单
	#define Mega_InstrumentStatus_AuctionBalance '4'	///集合竞价价格平衡
	#define Mega_InstrumentStatus_AuctionMatch '5'	///集合竞价撮合
	#define Mega_InstrumentStatus_Closed '6'	///收盘
	#define Mega_InstrumentStatus_NotActive '7'	///不活跃
	///买卖方向类型
	typedef char MegaDirectionType;
	#define Mega_Direction_Buy '0'	///买
	#define Mega_Direction_Sell '1'	///卖
	///持仓类型类型
	typedef char MegaPositionTypeType;
	#define Mega_PositionType_None '0'	///不设置
	#define Mega_PositionType_Gross '1'	///综合持仓(双边持仓)
	#define Mega_PositionType_Net '2'	///净持仓(单边持仓)
	///持仓大类类型
	typedef char MegaPositionClassType;
	#define Mega_PositionClass_None '0'	///不设置
	#define Mega_PositionClass_Risk '1'	///保证金交易
	#define Mega_PositionClass_Assets '3'	///资产
	#define Mega_PositionClass_AccountCost '4'	///持币成本
	///成员类型类型
	typedef char MegaMemberTypeType;
	#define Mega_MemberType_Normal 'a'	///普通
	#define Mega_MemberType_InnerMarketMaker 'b'	///内部做市商
	#define Mega_MemberType_Fee 'c'	///系统手续费使用
	#define Mega_MemberType_Risk 'd'	///风控使用
	#define Mega_MemberType_ExchangeUse 'e'	///运营使用
	#define Mega_MemberType_Reserve 'f'	///运营保留资金使用
	#define Mega_MemberType_OuterMarketMaker 'g'	///外部做市商
	///持仓多空方向类型
	typedef char MegaPosiDirectionType;
	#define Mega_PosiDirection_Long '0'	///多头
	#define Mega_PosiDirection_Short '1'	///空头
	#define Mega_PosiDirection_Net '2'	///净
	///取值方式类型
	typedef char MegaValueModeType;
	#define Mega_ValueMode_Percentage '0'	///百分比
	#define Mega_ValueMode_Absolute '1'	///绝对值
	///流动性基准价方式类型
	typedef char MegaLiquidityPriceTypeType;
	#define Mega_LiquidityPriceType_Bid1Ask1 '0'	///买一和买一中间值
	#define Mega_LiquidityPriceType_MarkedPrice '1'	///标记价
	#define Mega_LiquidityPriceType_LastPrice '2'	///最新价
	///帐号操作类型类型
	typedef char MegaAccountActionTypeType;
	#define Mega_AccountActionType_Deposit '1'	///入金
	#define Mega_AccountActionType_Withdraw '2'	///出金
	#define Mega_AccountActionType_FrozenMoney '3'	///冻结资金
	#define Mega_AccountActionType_FreeFrozenMoney '4'	///释放冻结资金
	#define Mega_AccountActionType_FrozenToWithdraw '5'	///释放冻结资金,同时出金
	#define Mega_AccountActionType_InnerTransfer '6'	///内部转账
	#define Mega_AccountActionType_SetReserveLimit '7'	///设置保留资金限制
	#define Mega_AccountActionType_SetReserveType '8'	///切换保留资金类型
	#define Mega_AccountActionType_SetReserveActive '9'	///保留资金激活失效
	#define Mega_AccountActionType_FrozenMoneyByOrder 'a'	///使用订单冻结资金
	#define Mega_AccountActionType_SetCanReduce 'b'	///切换是否可以出金
	///持仓操作类型类型
	typedef char MegaPositionActionTypeType;
	#define Mega_PositionActionType_ChangeLeverage '1'	///调整杠杆倍数
	#define Mega_PositionActionType_AddMargin '2'	///增加保证金
	#define Mega_PositionActionType_CutMargin '3'	///减少保证金
	#define Mega_PositionActionType_ChangeCross '4'	///全仓逐仓模式调换
	#define Mega_PositionActionType_ChangeMaxLeverage '5'	///调整不超过最大杠杆倍数
	#define Mega_PositionActionType_ChangeDefaultLeverage '6'	///调整默认杠杆倍数
	#define Mega_PositionActionType_ChangeDefaultCross '7'	///调整默认全逐仓
	#define Mega_PositionActionType_ChangeCostPrice '8'	///调整成本价
	#define Mega_PositionActionType_CloseAll 'a'	///全平
	#define Mega_PositionActionType_CloseMax 'b'	///最大平仓
	#define Mega_PositionActionType_ChangeAllCross 'c'	///清理之后才能切换的全逐仓切换
	///报单价格条件类型
	typedef char MegaOrderPriceTypeType;
	#define Mega_OrderPriceType_LimitPrice '0'	///限价(手动指定Price值)
	#define Mega_OrderPriceType_AnyPrice '1'	///任意价(Price按最高最低限价取值)
	#define Mega_OrderPriceType_BestPrice '2'	///最优价/对手价(Price按对方1档价取值)
	#define Mega_OrderPriceType_FiveLevelPrice '3'	///五档价(Price按对方5档价取值)
	#define Mega_OrderPriceType_TenLevelPrice '4'	///十档价(类同上)
	#define Mega_OrderPriceType_BestMakerPrice '7'	///最优Marker价(Price按本方1档价或其+1Tick取值)
	#define Mega_OrderPriceType_ByCost '8'	///Price按委托额Cost和订单簿情况确定值
	#define Mega_OrderPriceType_CFDPrice '9'	///保底点差价:成交价格Price不会劣于系统设置差价成交
	///开平标志类型
	typedef char MegaOffsetFlagType;
	#define Mega_OffsetFlag_Open '0'	///开仓
	#define Mega_OffsetFlag_Close '1'	///平仓
	#define Mega_OffsetFlag_ForceClose '2'	///强平
	#define Mega_OffsetFlag_CloseToday '3'	///平今
	#define Mega_OffsetFlag_ClosePrevious '4'	///平昨
	#define Mega_OffsetFlag_CloseAll '5'	///全平
	#define Mega_OffsetFlag_CloseOrderID '6'	///指定订单平仓
	#define Mega_OffsetFlag_CloseTradeID '7'	///指定成交平仓
	#define Mega_OffsetFlag_CloseMax '8'	///最大平仓
	///报单状态类型
	typedef char MegaOrderStatusType;
	#define Mega_OrderStatus_None '0'	///未设置状态
	#define Mega_OrderStatus_AllTraded '1'	///全部成交
	#define Mega_OrderStatus_PartTraded '2'	///部分成交未撤单
	#define Mega_OrderStatus_PartCanceled '3'	///部分成交已撤单
	#define Mega_OrderStatus_NoTraded '4'	///未成交未撤单
	#define Mega_OrderStatus_AllCanceled '6'	///无成交已撤单
	///触发报单状态类型
	typedef char MegaTriggerStatusType;
	#define Mega_TriggerStatus_None '0'	///未设置状态
	#define Mega_TriggerStatus_Active '1'	///活跃(运行)状态
	#define Mega_TriggerStatus_TriggerSuccess '2'	///已经触发
	#define Mega_TriggerStatus_TriggerFailed '3'	///触发失败
	#define Mega_TriggerStatus_Canceled '4'	///撤单
	///业务来源类型
	typedef char MegaDeriveSourceType;
	#define Mega_DeriveSource_Normal '0'	///正常
	#define Mega_DeriveSource_FromQuote '1'	///报价衍生
	#define Mega_DeriveSource_OptionsExecution '2'	///期权执行
	#define Mega_DeriveSource_FromCombination '3'	///组合衍生
	#define Mega_DeriveSource_FromOTCTrade_Liquidation '4'	///场外成交强平衍生
	#define Mega_DeriveSource_FromOTCTrade_BlockTrade '5'	///场外成交大宗交易衍生
	#define Mega_DeriveSource_FromOTCTrade_EFP '6'	///场外成交期转现衍生
	#define Mega_DeriveSource_FromOTCTrade_WashOnly '7'	///场外成交只刷单衍生
	#define Mega_DeriveSource_FromTriggerOrder '8'	///触发订单衍生
	#define Mega_DeriveSource_FromTriggerOrder_SL '9'	///止损触发订单衍生
	#define Mega_DeriveSource_FromTriggerOrder_TP 'a'	///止盈触发订单衍生
	#define Mega_DeriveSource_FromAccountAction 'b'	///资金操作衍生
	#define Mega_DeriveSource_FromOTCTrade_PositionMerge 'c'	///仓位合并衍生
	#define Mega_DeriveSource_FromOTCTrade_CFD 'd'	///CFD订单衍生
	#define Mega_DeriveSource_FromOTCTrade_ADL 'e'	///场外成交ADL衍生
	#define Mega_DeriveSource_FromOTCTrade_Private 'f'	///回购手续费不能是币
	///订单类型类型
	typedef char MegaOrderTypeType;
	#define Mega_OrderType_Normal '0'	///普通订单
	#define Mega_OrderType_FAK '1'	///(FillAndKill/IOC)立即完成并且剩余撤销
	#define Mega_OrderType_FOK '2'	///(FillOrKill)立即完成全部或者最小订单要求否则全部撤销
	#define Mega_OrderType_PostOnly '3'	///需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly)
	#define Mega_OrderType_FlashTrade '4'	///不需要撮合直接成交
	///触发的订单类型类型
	typedef char MegaTriggerOrderTypeType;
	#define Mega_TriggerOrderType_None '0'	///未定义
	#define Mega_TriggerOrderType_PositionTPSL '1'	///持仓止盈止损
	#define Mega_TriggerOrderType_OrderTPSL '2'	///订单止盈止损
	#define Mega_TriggerOrderType_Condition '3'	///条件单
	#define Mega_TriggerOrderType_Trailing '4'	///追踪(跟踪)订单
	#define Mega_TriggerOrderType_Algo '5'	///其他算法交易
	#define Mega_TriggerOrderType_Indicator '6'	///指标类型
	#define Mega_TriggerOrderType_IndicatorTPSL '7'	///指标类型止盈止损
	#define Mega_TriggerOrderType_CopyTrade '8'	///跟单类型
	#define Mega_TriggerOrderType_LineOrder '9'	///划线下单类型
	#define Mega_TriggerOrderType_Other1 'a'	///其他类型1
	#define Mega_TriggerOrderType_Other2 'b'	///其他类型2
	#define Mega_TriggerOrderType_Other3 'c'	///其他类型3
	#define Mega_TriggerOrderType_Other4 'd'	///其他类型4
	#define Mega_TriggerOrderType_Other5 'e'	///其他类型5
	///操作标志类型
	typedef char MegaActionFlagType;
	#define Mega_ActionFlag_Delete '1'	///删除
	#define Mega_ActionFlag_Suspend '2'	///挂起
	#define Mega_ActionFlag_Active '3'	///激活
	#define Mega_ActionFlag_Modify '4'	///修改
	#define Mega_ActionFlag_Trigger '5'	///触发
	#define Mega_ActionFlag_Change '6'	///增减
	///交易权限类型
	typedef char MegaTradingRightType;
	#define Mega_TradingRight_Allow '0'	///可以交易
	#define Mega_TradingRight_CloseOnly '1'	///只能平仓
	#define Mega_TradingRight_Forbidden '2'	///不能交易
	///交易模式类型
	typedef char MegaTradingModelType;
	#define Mega_TradingModel_PriceTime '0'	///价格优先时间优先
	#define Mega_TradingModel_Prorata '1'	///价格优先按比例分配
	#define Mega_TradingModel_OnlyQuote '2'	///只能和做市商报价成交
	#define Mega_TradingModel_NoMatch '3'	///没有撮合
	///成交角色类型
	typedef char MegaMatchRoleType;
	#define Mega_MatchRole_None '0'	///不区分
	#define Mega_MatchRole_Taker '1'	///主动成交方
	#define Mega_MatchRole_Maker '2'	///被动成交方
	#define Mega_MatchRole_Auction '3'	///集合竞价
	#define Mega_MatchRole_Delivery '4'	///交割
	#define Mega_MatchRole_OTC '5'	///场外交易
	///OTC类型类型
	typedef char MegaOTCTypeType;
	#define Mega_OTCType_Liquidation '0'	///强平
	#define Mega_OTCType_BlockTrade '1'	///大宗交易
	#define Mega_OTCType_EFP '2'	///期转现
	#define Mega_OTCType_WashOnly '3'	///只刷单
	#define Mega_OTCType_PositionMerge '4'	///仓位合并
	#define Mega_OTCType_CFD '5'	///差价交易
	#define Mega_OTCType_ADL '6'	///ADL减仓
	#define Mega_OTCType_Private '7'	///回购手续费不一样
	///保证金价格类型类型
	typedef char MegaMarginPriceTypeType;
	#define Mega_MarginPriceType_None '0'	///不按比例按固定值,无需价格
	#define Mega_MarginPriceType_PreSettlementPrice '1'	///昨结算价
	#define Mega_MarginPriceType_OpenPrice '2'	///开仓价
	#define Mega_MarginPriceType_MarkedPrice '3'	///标记价
	#define Mega_MarginPriceType_LastPrice '4'	///最新价
	///触发价格类型类型
	typedef char MegaTriggerPriceTypeType;
	#define Mega_TriggerPriceType_LastPrice '0'	///最新价
	#define Mega_TriggerPriceType_MarkedPrice '1'	///标记价
	#define Mega_TriggerPriceType_UnderlyingPrice '2'	///标的指数价
	///限价价格类型类型
	typedef char MegaLimitPriceTypeType;
	#define Mega_LimitPriceType_PreSettlementPrice '1'	///昨结算价
	#define Mega_LimitPriceType_LastPrice '2'	///最新价
	#define Mega_LimitPriceType_UnderlyingPrice '3'	///基础标的价
	#define Mega_LimitPriceType_MarkedPrice '4'	///标记价
	///加密类型类型
	typedef char MegaEncryptTypeType;
	#define Mega_EncryptType_None '0'	///不加密
	#define Mega_EncryptType_MD5 '1'	///MD5加密
	#define Mega_EncryptType_Base64 '2'	///Base64加密
	///席位状态类型
	typedef char MegaLinkStatusType;
	#define Mega_LinkStatus_NotActive '0'	///不活跃
	#define Mega_LinkStatus_Login '1'	///已登录
	#define Mega_LinkStatus_Logout '2'	///已登出
	///订阅操作类型
	typedef char MegaSubActionType;
	#define Mega_SubAction_Clear '0'	///全部退订
	#define Mega_SubAction_Sub '1'	///订阅
	#define Mega_SubAction_UnSub '2'	///退订
	///结算操作类型
	typedef char MegaSettleActionTypeType;
	#define Mega_SettleActionType_SettlementBegin '0'	///结算操作开始
	#define Mega_SettleActionType_Settlement '1'	///定期无负债结算
	#define Mega_SettleActionType_Delivery '2'	///到期交割(行权)
	#define Mega_SettleActionType_FundingRate '3'	///资金费率交换
	#define Mega_SettleActionType_OverNightFee '4'	///隔夜费划转
	#define Mega_SettleActionType_Clawback '5'	///分摊
	#define Mega_SettleActionType_AccountInit '6'	///资金初始化
	#define Mega_SettleActionType_SettlementEnd '7'	///结算操作结束
	#define Mega_SettleActionType_DataCheck '8'	///数据检查
	#define Mega_SettleActionType_DataRepair '9'	///数据修复
	#define Mega_SettleActionType_AccountBalance 'a'	///平账
	#define Mega_SettleActionType_DataClear 'b'	///清理
	#define Mega_SettleActionType_ClearNoPosition 'c'	///清空没有持仓的产品
	#define Mega_SettleActionType_LoanInterest 'd'	///借贷利息
	///下游系统同步类型类型
	typedef char MegaSinkTypeType;
	#define Mega_SinkType_SinkToDB '0'	///同步到数据库
	#define Mega_SinkType_SinkTillRedis '1'	///同步到Redis不到数据库
	#define Mega_SinkType_NoSink '2'	///不同步
	///保留资金使用类型类型
	typedef char MegaReserveTypeType;
	#define Mega_ReserveType_Inferior '0'	///先亏损保留资金再亏损真钱(劣后)
	#define Mega_ReserveType_Prior '1'	///先亏损真钱再亏损保留资金(优先)
	#define Mega_ReserveType_Guarantee '2'	///保留资金不做亏损(抵押金)
	///数列类型类型
	typedef char MegaSequenceTypeType;
	#define Mega_SequenceType_Arithmetical '1'	///等差数列
	#define Mega_SequenceType_Geometrical '2'	///等比数列
	///点差接管类型类型
	typedef char MegaCFDTypeType;
	#define Mega_CFDType_Normal '0'	///先撮合后点差接管
	#define Mega_CFDType_OnlyMatch '1'	///只撮合不点差接管
	#define Mega_CFDType_OnlyCFD '2'	///不撮合只点差接管
	///账户风控界别类型
	typedef char MegaRiskLevelType;
	#define Mega_RiskLevel_Currency '0'	///本币种资金权益
	#define Mega_RiskLevel_Settlement '1'	///结算组所有资金权益
	#define Mega_RiskLevel_Account '2'	///资金账号所有资金权益
	#define Mega_RiskLevel_Member '3'	///会员账号所有资金权益

	
	///主题订阅操作
	typedef struct MegaTopicActionStruct
	{
		MegaSubActionType Action;	///订阅操作
		MegaLongIndexIDType TopicID;	///请参考TOPICID类型定义
		MegaShortContentType Index;	///过滤值
		MegaLargeNumberType ResumeNo;	///续传号:0是从头开始,-1是从服务端最新位置续传
		MegaLargeNumberType BusinessNo;	///续传号:0是从头开始,不能为负数
		MegaShortContentType Token;	///令牌
		MegaNumberType Zip;	///压缩方式
		MegaNumberType Frequency;	///每间隔多少频率读取一次
	} MegaTopicActionStruct;
	
	///操作信息
	typedef struct MegaCommandHeadStruct
	{
		MegaBoolType IsLast;	///是否最后
	} MegaCommandHeadStruct;
	
	///响应信息
	typedef struct MegaErrorStruct
	{
		MegaNumberType ErrorNo;	///错误代码
		MegaShortContentType ErrorMsg;	///错误信息
	} MegaErrorStruct;
	
	///响应信息
	typedef struct MegaCheckTokenStruct
	{
		MegaShortContentType Token;	///令牌
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaIndexIDType MaxLocalID;	///最大本地标识
		MegaLongIndexIDType MemberID;	///成员代码
	} MegaCheckTokenStruct;
	
	///消息
	typedef struct MegaCommandStruct
	{
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaIndexIDType LocalID;	///命令本地号
		MegaNameType Command;	///命令
		MegaLongContentType Content;	///字段内容
	} MegaCommandStruct;
	
	///用户登录
	typedef struct MegaUserLoginStruct
	{
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaPasswordType Password;	///密码
		MegaNameType UserProductID;	///用户端产品信息
		MegaMacAddressType MacAddress;	///Mac地址
		MegaIPAddressType ClientIPAddress;	///终端IP地址
		MegaLongNameType HDSerialID;	///硬盘序列号
		MegaShortContentType AuthCode;	///授权编码
		MegaShortIndexIDType APPID;	///应用编号
		MegaShortContentType Token;	///令牌
		MegaLongNameType Remark;	///备注
	} MegaUserLoginStruct;
	
	///向上通道
	typedef struct MegaLinkStruct
	{
		MegaNumberType LinkNo;	///通道号
		MegaLongIndexIDType LinkUserID;	///通道用户
		MegaPasswordType LinkPassword;	///通道用户密码
		MegaEncryptTypeType EncryptType;	///密码加密算法
		MegaNameType LinkDllName;	///LinkDll名
		MegaNameType LinkDllLoad;	///LinkDll加载库
		MegaContentType LinkAddress;	///Link连接地址
		MegaLinkStatusType LinkStatus;	///Link状态
		MegaBoolType IsActive;	///是否活跃
		MegaContentType Config;	///报盘自定义的配置
		MegaLongNameType Remark;	///备注
	} MegaLinkStruct;
	
	///用户登录退出
	typedef struct MegaUserLogoutStruct
	{
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaShortIndexIDType APPID;	///应用编号
		MegaShortContentType Token;	///令牌
	} MegaUserLogoutStruct;
	
	///用户口令修改
	typedef struct MegaUserPasswordUpdateStruct
	{
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaPasswordType OldPassword;	///旧密码
		MegaPasswordType NewPassword;	///新密码
	} MegaUserPasswordUpdateStruct;
	
	///输入报单
	typedef struct MegaTriggerOrderInsertStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType LocalID;	///报单本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaOrderPriceTypeType OrderPriceType;	///报单价格类型
		MegaDirectionType Direction;	///买卖方向
		MegaOffsetFlagType OffsetFlag;	///开平标志
		MegaPriceType Price;	///报单价格
		MegaVolumeType Volume;	///数量
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaValueModeType VolumeMode;	///数量取值方式
		MegaMoneyType Cost;	///委托额
		MegaOrderTypeType OrderType;	///订单类型
		MegaMilliSecondsType GTDTime;	///GTD时间
		MegaVolumeType MinVolume;	///最小成交量
		MegaBusinessTypeType BusinessType;	///业务类别
		MegaNameType BusinessValue;	///业务值
		MegaIndexIDType CloseOrderID;	///平仓指定开仓的订单号
		MegaBoolType IsCrossMargin;	///是否全仓
		MegaNameType Remark;	///备注
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaIndexIDType CopyOrderID;	///带单员报单号
		MegaLeverageType Leverage;	///委托单杠杆倍数
		MegaRatioType CopyProfitRate;	///带单分配比例
		MegaShortIndexIDType APPID;	///应用编号
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaPriceType TriggerPrice;	///触发价
		MegaMoneyType Reserve;	///保留资金
		MegaPriceType SLPrice;	///止损价
		MegaPriceType SLTriggerPrice;	///止损触发价
		MegaPriceType TPPrice;	///止盈价
		MegaPriceType TPTriggerPrice;	///止盈触发价
		MegaBoolType RiskBefore;	///是否提前检查风控
		MegaTriggerOrderTypeType TriggerOrderType;	///触发的订单类型
		MegaIndexIDType TriggerDetail;	///触发类型明细
		MegaTriggerPriceTypeType TriggerPriceType;	///触发价类型
		MegaLongContentType TriggerValue;	///触发单具体设置信息
		MegaPriceType CloseSLPrice;	///平仓止损价
		MegaPriceType CloseSLTriggerPrice;	///平仓止损触发价
		MegaPriceType CloseTPPrice;	///平仓止盈价
		MegaPriceType CloseTPTriggerPrice;	///平仓止盈触发价
		MegaOrderPriceTypeType CloseOrderPriceType;	///报单价格类型
		MegaPriceType ClosePrice;	///平仓价
		MegaPriceType CloseTriggerPrice;	///平仓触发价
		MegaIndexIDType RelatedOrderID;	///关联报单号
		MegaMilliSecondsType ActiveTime;	///激活时间
		MegaMilliSecondsType TriggerTime;	///触发时间
	} MegaTriggerOrderInsertStruct;
	
	///输入止盈止损报单
	typedef struct MegaCloseOrderInsertStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType LocalID;	///报单本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaOrderPriceTypeType OrderPriceType;	///报单价格类型
		MegaDirectionType Direction;	///买卖方向
		MegaOffsetFlagType OffsetFlag;	///开平标志
		MegaPriceType Price;	///报单价格
		MegaVolumeType Volume;	///数量
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaValueModeType VolumeMode;	///数量取值方式
		MegaMoneyType Cost;	///委托额
		MegaOrderTypeType OrderType;	///订单类型
		MegaMilliSecondsType GTDTime;	///GTD时间
		MegaVolumeType MinVolume;	///最小成交量
		MegaBusinessTypeType BusinessType;	///业务类别
		MegaNameType BusinessValue;	///业务值
		MegaIndexIDType CloseOrderID;	///平仓指定开仓的订单号
		MegaBoolType IsCrossMargin;	///是否全仓
		MegaNameType Remark;	///备注
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaIndexIDType CopyOrderID;	///带单员报单号
		MegaLeverageType Leverage;	///委托单杠杆倍数
		MegaRatioType CopyProfitRate;	///带单分配比例
		MegaShortIndexIDType APPID;	///应用编号
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaPriceType TriggerPrice;	///触发价
		MegaMoneyType Reserve;	///保留资金
		MegaPriceType CloseSLPrice;	///平仓止损价
		MegaPriceType SLTriggerPrice;	///平仓止损触发价
		MegaPriceType CloseSLTriggerPrice;	///平仓止损触发价
		MegaPriceType CloseTPPrice;	///平仓止盈价
		MegaPriceType TPTriggerPrice;	///平仓止盈触发价
		MegaPriceType CloseTPTriggerPrice;	///平仓止盈触发价
		MegaOrderPriceTypeType CloseOrderPriceType;	///报单价格类型
		MegaPriceType ClosePrice;	///平仓价
		MegaPriceType CloseTriggerPrice;	///平仓触发价
		MegaTriggerPriceTypeType CloseTriggerPriceType;	///触发价类型
	} MegaCloseOrderInsertStruct;
	
	///触发单操作
	typedef struct MegaTriggerOrderActionStruct
	{
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaIndexIDType OrderLocalID;	///报单本地标识
		MegaActionFlagType ActionFlag;	///报单操作标志
		MegaIndexIDType LocalID;	///操作本地标识
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaPriceType Price;	///价格
		MegaVolumeType Volume;	///数量变化
		MegaDirectionType Direction;	///买卖方向
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaMoneyType Cost;	///委托额
		MegaNameType Remark;	///备注
		MegaPriceType SLPrice;	///止损价
		MegaPriceType SLTriggerPrice;	///止损触发价
		MegaPriceType TPPrice;	///止盈价
		MegaPriceType TPTriggerPrice;	///止盈触发价
		MegaPriceType TriggerPrice;	///触发价
		MegaShortIndexIDType TriggerOrderType;	///触发的订单类型
		MegaLongContentType TriggerValue;	///触发单具体设置信息
		MegaIndexIDType TriggerDetail;	///触发类型明细
		MegaTriggerPriceTypeType TriggerPriceType;	///触发价类型
		MegaNumberType ErrorNo;	///错误代码
		MegaShortContentType ErrorMsg;	///错误信息
	} MegaTriggerOrderActionStruct;
	
	///输入报单
	typedef struct MegaOrderInsertStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType LocalID;	///报单本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaOrderPriceTypeType OrderPriceType;	///报单价格类型
		MegaDirectionType Direction;	///买卖方向
		MegaOffsetFlagType OffsetFlag;	///开平标志
		MegaPriceType Price;	///报单价格
		MegaVolumeType Volume;	///数量
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaValueModeType VolumeMode;	///数量取值方式
		MegaMoneyType Cost;	///委托额
		MegaOrderTypeType OrderType;	///订单类型
		MegaMilliSecondsType GTDTime;	///GTD时间
		MegaVolumeType MinVolume;	///最小成交量
		MegaBusinessTypeType BusinessType;	///业务类别
		MegaNameType BusinessValue;	///业务值
		MegaIndexIDType CloseOrderID;	///平仓指定开仓的订单号
		MegaBoolType IsCrossMargin;	///是否全仓
		MegaNameType Remark;	///备注
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaIndexIDType CopyOrderID;	///带单员报单号
		MegaLeverageType Leverage;	///委托单杠杆倍数
		MegaRatioType CopyProfitRate;	///带单分配比例
		MegaShortIndexIDType APPID;	///应用编号
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaPriceType TriggerPrice;	///触发价
		MegaMoneyType Reserve;	///保留资金
	} MegaOrderInsertStruct;
	
	///报单操作
	typedef struct MegaOrderActionStruct
	{
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaIndexIDType OrderLocalID;	///报单本地标识
		MegaActionFlagType ActionFlag;	///报单操作标志
		MegaIndexIDType LocalID;	///操作本地标识
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaPriceType Price;	///价格
		MegaVolumeType Volume;	///数量变化
		MegaDirectionType Direction;	///买卖方向
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaMoneyType Cost;	///委托额
		MegaNameType Remark;	///备注
	} MegaOrderActionStruct;
	
	///输入报价
	typedef struct MegaQuoteInsertStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaIndexIDType LocalID;	///报价本地标识
		MegaVolumeType Volume;	///数量
		MegaOffsetFlagType BuyOffsetFlag;	///买方组合开平标志
		MegaPriceType BuyPrice;	///买方价格
		MegaOffsetFlagType SellOffsetFlag;	///卖方组合开平标志
		MegaPriceType SellPrice;	///卖方价格
		MegaNameType Remark;	///备注
	} MegaQuoteInsertStruct;
	
	///报价操作
	typedef struct MegaQuoteActionStruct
	{
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaIndexIDType OrderLocalID;	///报单本地标识
		MegaActionFlagType ActionFlag;	///报单操作标志
		MegaIndexIDType LocalID;	///操作本地标识
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaPriceType Price;	///价格
		MegaVolumeType Volume;	///数量变化
		MegaDirectionType Direction;	///买卖方向
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaMoneyType Cost;	///委托额
		MegaNameType Remark;	///备注
		MegaIndexIDType TradeUnitID;	///交易单元代码
	} MegaQuoteActionStruct;
	
	///输入场外成交对
	typedef struct MegaOTCTradeInsertStruct
	{
		MegaIndexIDType LocalID;	///成交对本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaPriceType Price;	///价格
		MegaVolumeType Volume;	///数量
		MegaLongIndexIDType BuyMemberID;	///买方成员代码
		MegaAccountIDType BuyAccountID;	///买方交易单元代码
		MegaIndexIDType BuyTradeUnitID;	///买方交易单元代码
		MegaOffsetFlagType BuyOffsetFlag;	///买方组合开平标志
		MegaLongIndexIDType SellMemberID;	///卖方方成员代码
		MegaAccountIDType SellAccountID;	///卖方交易单元代码
		MegaIndexIDType SellTradeUnitID;	///卖方交易单元代码
		MegaOffsetFlagType SellOffsetFlag;	///卖方组合开平标志
		MegaOTCTypeType OTCType;	///OTC类型
		MegaDirectionType TakerDirection;	///Taker的方向
		MegaPriceType TriggerPrice;	///触发价
		MegaNameType Remark;	///备注
	} MegaOTCTradeInsertStruct;
	
	///输入场外成交对
	typedef struct MegaOTCTradeStruct
	{
		MegaIndexIDType LocalID;	///成交对本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaPriceType Price;	///价格
		MegaVolumeType Volume;	///数量
		MegaLongIndexIDType BuyMemberID;	///买方成员代码
		MegaAccountIDType BuyAccountID;	///买方交易单元代码
		MegaIndexIDType BuyTradeUnitID;	///买方交易单元代码
		MegaOffsetFlagType BuyOffsetFlag;	///买方组合开平标志
		MegaLongIndexIDType SellMemberID;	///卖方方成员代码
		MegaAccountIDType SellAccountID;	///卖方交易单元代码
		MegaIndexIDType SellTradeUnitID;	///卖方交易单元代码
		MegaOffsetFlagType SellOffsetFlag;	///卖方组合开平标志
		MegaOTCTypeType OTCType;	///OTC类型
		MegaDirectionType TakerDirection;	///Taker的方向
		MegaPriceType TriggerPrice;	///触发价
		MegaNameType Remark;	///备注
		MegaIndexIDType TradeID;	///成交代码
		MegaMilliSecondsType TradeTime;	///成交时间
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaDeriveSourceType DeriveSource;	///成交类型
		MegaIndexIDType BuyOrderID;	///买方报单系统唯一代码
		MegaIndexIDType SellOrderID;	///卖方报单系统唯一代码
	} MegaOTCTradeStruct;
	
	///内部做市商报单
	typedef struct MegaRobotOrderStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaAccountIDType AccountID;	///资金账号
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaIndexIDType LocalID;	///报单本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaOrderPriceTypeType OrderPriceType;	///报单价格类型
		MegaDirectionType Direction;	///买卖方向
		MegaOffsetFlagType OffsetFlag;	///开平标志
		MegaPriceType Price;	///报单价格
		MegaVolumeType Volume;	///数量
		MegaVolumeType VolumeDisplay;	///显示数量
	} MegaRobotOrderStruct;
	
	///仓位操作
	typedef struct MegaPositionActionStruct
	{
		MegaIndexIDType LocalID;	///出入金本地号
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaAccountIDType AccountID;	///资金账号
		MegaPosiDirectionType PosiDirection;	///持仓多空方向，0:多;1:空;2:净;空值:多空方向同时操作
		MegaMoneyType Amount;	///发生额
		MegaLeverageType LongLeverage;	///杠杆倍数
		MegaLeverageType ShortLeverage;	///杠杆倍数
		MegaLeverageType NetLeverage;	///杠杆倍数
		MegaPositionActionTypeType ActionType;	///操作类型
		MegaBoolType IsCrossMargin;	///是否全仓
		MegaBoolType IsAutoAddMargin;	///是否自动追加保证金
		MegaNameType Remark;	///备注
	} MegaPositionActionStruct;
	
	///资金账户出入金
	typedef struct MegaAccountActionStruct
	{
		MegaIndexIDType ActionLocalID;	///关联的之前的资金操作本地号
		MegaIndexIDType LocalID;	///出入金本地号
		MegaIndexIDType OrderID;	///关联订单号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaAccountIDType OtherAccountID;	///内部转账资金账号
		MegaShortIndexIDType OtherSettlementGroup;	///内部转账结算组编号
		MegaLongIndexIDType OtherMemberID;	///内部转账成员代码
		MegaMoneyType Amount;	///发生额
		MegaMoneyType FrozenMoney;	///冻结资金
		MegaCurrencyType Currency;	///币种
		MegaAccountActionTypeType ActionType;	///资金操作类型
		MegaBoolType IsReserve;	///是否保留资金
		MegaIndexIDType AccountType;	///账户类型
		MegaShortIndexIDType MarginRateGrade;	///保证金等级
		MegaShortIndexIDType TradingRightGrade;	///权限等级
		MegaPriceType Price;	///价格
		MegaVolumeType Volume;	///数量
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaDefineTypeType Source;	///财务流水类型
		MegaLongNameType RelatedID;	///内外对账ID
		MegaRatioType FeeDeduceRate;	///作为手续费的抵扣比例
		MegaNameType Remark;	///备注
	} MegaAccountActionStruct;
	
	///资金账户出入金
	typedef struct MegaMemberActionStruct
	{
		MegaIndexIDType LocalID;	///出入金本地号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaRiskLevelType RiskLevel;	///风控类型
		MegaPositionTypeType PositionType;	///持仓类型
	} MegaMemberActionStruct;
	
	///行情概要
	typedef struct MegaMarketOverViewStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaPriceType UpperLimitPrice;	///最高限价
		MegaPriceType LowerLimitPrice;	///最低限价
		MegaPriceType UnderlyingPrice;	///基础标的价格
		MegaPriceType MarkedPrice;	///标记价格
		MegaRatioType PositionFeeRate;	///预计持仓费按比例
		MegaPriceType HighestPrice;	///当日最高价
		MegaPriceType LowestPrice;	///当日最低价
		MegaPriceType LastPrice;	///最新价
		MegaCollectType Volume;	///当日数量
		MegaCollectType Turnover;	///当日成交金额
		MegaVolumeType OpenInterest;	///持仓量
		MegaPriceType SettlementPrice;	///预估结算价
		MegaPriceType OpenPrice;	///当日开盘价
		MegaInstrumentStatusType InstrumentStatus;	///标的交易状态
		MegaRatioType PrePositionFeeRate;	///上次持仓费按比例
		MegaMilliSecondsType PositionFeeTime;	///收取持仓费用时间
		MegaPriceType BidPrice1;	///申买价一
		MegaPriceType AskPrice1;	///申卖价一
		MegaPriceType HighestPrice24;	///24小时最高价
		MegaPriceType LowestPrice24;	///24小时最低价
		MegaCollectType Volume24;	///24小时数量
		MegaCollectType Turnover24;	///24小时成交金额
		MegaPriceType OpenPrice24;	///24小时开盘价
	} MegaMarketOverViewStruct;
	
	///5档深度行情
	typedef struct MegaFiveDepthMarketDataStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaPriceType BidPrice1;	///申买价一
		MegaVolumeType BidVolume1;	///申买量一
		MegaPriceType AskPrice1;	///申卖价一
		MegaVolumeType AskVolume1;	///申卖量一
		MegaPriceType BidPrice2;	///申买价二
		MegaVolumeType BidVolume2;	///申买量二
		MegaPriceType AskPrice2;	///申卖价二
		MegaVolumeType AskVolume2;	///申卖量二
		MegaPriceType BidPrice3;	///申买价三
		MegaVolumeType BidVolume3;	///申买量三
		MegaPriceType AskPrice3;	///申卖价三
		MegaVolumeType AskVolume3;	///申卖量三
		MegaPriceType BidPrice4;	///申买价四
		MegaVolumeType BidVolume4;	///申买量四
		MegaPriceType AskPrice4;	///申卖价四
		MegaVolumeType AskVolume4;	///申卖量四
		MegaPriceType BidPrice5;	///申买价五
		MegaVolumeType BidVolume5;	///申买量五
		MegaPriceType AskPrice5;	///申卖价五
		MegaVolumeType AskVolume5;	///申卖量五
	} MegaFiveDepthMarketDataStruct;
	
	///10档深度行情
	typedef struct MegaTenDepthMarketDataStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaPriceType BidPrice1;	///申买价一
		MegaVolumeType BidVolume1;	///申买量一
		MegaPriceType AskPrice1;	///申卖价一
		MegaVolumeType AskVolume1;	///申卖量一
		MegaPriceType BidPrice2;	///申买价二
		MegaVolumeType BidVolume2;	///申买量二
		MegaPriceType AskPrice2;	///申卖价二
		MegaVolumeType AskVolume2;	///申卖量二
		MegaPriceType BidPrice3;	///申买价三
		MegaVolumeType BidVolume3;	///申买量三
		MegaPriceType AskPrice3;	///申卖价三
		MegaVolumeType AskVolume3;	///申卖量三
		MegaPriceType BidPrice4;	///申买价四
		MegaVolumeType BidVolume4;	///申买量四
		MegaPriceType AskPrice4;	///申卖价四
		MegaVolumeType AskVolume4;	///申卖量四
		MegaPriceType BidPrice5;	///申买价五
		MegaVolumeType BidVolume5;	///申买量五
		MegaPriceType AskPrice5;	///申卖价五
		MegaVolumeType AskVolume5;	///申卖量五
		MegaPriceType BidPrice6;	///申买价六
		MegaVolumeType BidVolume6;	///申买量六
		MegaPriceType AskPrice6;	///申卖价六
		MegaVolumeType AskVolume6;	///申卖量六
		MegaPriceType BidPrice7;	///申买价七
		MegaVolumeType BidVolume7;	///申买量七
		MegaPriceType AskPrice7;	///申卖价七
		MegaVolumeType AskVolume7;	///申卖量七
		MegaPriceType BidPrice8;	///申买价八
		MegaVolumeType BidVolume8;	///申买量八
		MegaPriceType AskPrice8;	///申卖价八
		MegaVolumeType AskVolume8;	///申卖量八
		MegaPriceType BidPrice9;	///申买价九
		MegaVolumeType BidVolume9;	///申买量九
		MegaPriceType AskPrice9;	///申卖价九
		MegaVolumeType AskVolume9;	///申卖量九
		MegaPriceType BidPrice10;	///申买价十
		MegaVolumeType BidVolume10;	///申买量十
		MegaPriceType AskPrice10;	///申卖价十
		MegaVolumeType AskVolume10;	///申卖量十
	} MegaTenDepthMarketDataStruct;
	
	///期权行情
	typedef struct MegaOptionsMarketDataStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaPriceType PreClosePrice;	///上次收盘
		MegaPriceType PreSettlementPrice;	///上次结算价
		MegaVolumeType PreOpenInterest;	///上次持仓量
		MegaRatioType PrePositionFeeRate;	///上次持仓费按比例
		MegaPriceType ClosePrice;	///收盘价
		MegaMilliSecondsType PositionFeeTime;	///收取持仓费用时间
		MegaMilliSecondsType SettlementTime;	///结算时间
		MegaPriceType UpperLimitPrice;	///最高限价
		MegaPriceType LowerLimitPrice;	///最低限价
		MegaPriceType UnderlyingPrice;	///基础标的价格
		MegaPriceType MarkedPrice;	///标记价格
		MegaRatioType PositionFeeRate;	///预计持仓费按比例
		MegaPriceType HighestPrice;	///当日最高价
		MegaPriceType LowestPrice;	///当日最低价
		MegaPriceType LastPrice;	///最新价
		MegaCollectType Volume;	///当日数量
		MegaCollectType Turnover;	///当日成交金额
		MegaVolumeType OpenInterest;	///持仓量
		MegaPriceType SettlementPrice;	///预估结算价
		MegaPriceType OpenPrice;	///当日开盘价
		MegaRatioType Delta;	///虚实度
		MegaRatioType Gamma;	///曲线凸度
		MegaRatioType Vega;	///波动率度
		MegaRatioType Theta;	///时间价值度
		MegaRatioType Rho;	///利率度
		MegaRatioType LastVolatility;	///最新波动率
		MegaRatioType Bid1Volatility;	///买一波动率
		MegaRatioType Ask1Volatility;	///卖一波动率
		MegaRatioType MaskedVolatility;	///标记波动率
	} MegaOptionsMarketDataStruct;
	
	///更新合约
	typedef struct MegaInstrumentUpdateStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaVolumeType MinOrderVolume;	///最小下单量
		MegaVolumeType MinOrderCost;	///最小下单额
		MegaVolumeType MaxOrderVolume;	///最大下单量
		MegaPriceType PriceTick;	///最小变动价位
		MegaVolumeType VolumeTick;	///最小变动量
		MegaVolumeType MaxOpenInterest;	///最大持仓量
		MegaPriceType BasisPrice;	///挂牌基准价
		MegaPriceType UpperLimitPrice;	///最高限价
		MegaPriceType LowerLimitPrice;	///最低限价
	} MegaInstrumentUpdateStruct;
	
	///更新行情
	typedef struct MegaMarketDataUpdateStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaPriceType MarkedPrice;	///合理价格
		MegaPriceType TheoryPrice;	///理论价
		MegaPriceType PreSettlementPrice;	///上次结算价
		MegaPriceType SettlementPrice;	///结算价
		MegaPriceType UnderlyingPrice;	///基础标的价格
		MegaRatioType PrePositionFeeRate;	///持仓费按比例
		MegaRatioType PositionFeeRate;	///预计持仓费按比例
		MegaInstrumentStatusType InstrumentStatus;	///标的交易状态
		MegaCollectType Volume;	///数量
		MegaCollectType Turnover;	///成交金额
		MegaLongNameType Remark;	///备注
	} MegaMarketDataUpdateStruct;
	
	///成交明细
	typedef struct MegaMarketTradeStruct
	{
		MegaIndexIDType TradeID;	///成交代码
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaDirectionType Direction;	///Taker方买卖方向
		MegaPriceType Price;	///成交价格
		MegaVolumeType Volume;	///成交数量
		MegaMilliSecondsType InsertTime;	///插入时间
	} MegaMarketTradeStruct;
	
	///资金结算
	typedef struct MegaSettlementStruct
	{
		MegaShortIndexIDType APPID;	///应用编号
		MegaIndexIDType LocalID;	///成交对本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaSettleActionTypeType SettleAction;	///结算操作
		MegaRatioType Value;	///操作使用值
		MegaRatioType Value1;	///发生值1
		MegaRatioType Value2;	///发生值2
		MegaRatioType Value3;	///发生值3
		MegaNameType Remark;	///备注
	} MegaSettlementStruct;
	
	///输入APIKey
	typedef struct MegaAPIKeyInsertStruct
	{
		MegaNameType Name;	///备注
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaShortIndexIDType APPID;	///应用编号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaShortContentType AccessKey;	///访问密钥
		MegaShortContentType SecretKey;	///签名加密密钥
		MegaNameType Auth;	///权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭
		MegaNumberType AccessLimit;	///每秒访问的次数限制
		MegaMilliSecondsType CreateTime;	///创建时间
		MegaMilliSecondsType ExpireTime;	///失效时间
		MegaContentType IPAddress;	///绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2
	} MegaAPIKeyInsertStruct;
	
	///APIKey
	typedef struct MegaAPIKeyStruct
	{
		MegaNameType Name;	///备注
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaShortIndexIDType APPID;	///应用编号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaShortContentType AccessKey;	///访问密钥
		MegaShortContentType SecretKey;	///签名加密密钥
		MegaNameType Auth;	///权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭
		MegaNumberType AccessLimit;	///每秒访问的次数限制
		MegaMilliSecondsType CreateTime;	///创建时间
		MegaMilliSecondsType ExpireTime;	///失效时间
		MegaContentType IPAddress;	///绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2
		MegaNumberType RemainDays;	///剩余有效天数
		MegaNumberType Status;	///状态，1：正常，0：失效
	} MegaAPIKeyStruct;
	
	///操作APIKey
	typedef struct MegaAPIKeyActionStruct
	{
		MegaNameType Name;	///备注
		MegaShortContentType AccessKey;	///访问密钥
		MegaNameType Auth;	///权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭
		MegaNumberType AccessLimit;	///每秒访问的次数限制
		MegaContentType IPAddress;	///绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2
		MegaMilliSecondsType ExpireTime;	///失效时间
		MegaActionFlagType ActionFlag;	///操作标志
	} MegaAPIKeyActionStruct;
	
	///清理行情数据
	typedef struct MegaClearMarketStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
	} MegaClearMarketStruct;
	
	///清理最新K线数据
	typedef struct MegaClearLastKLineStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaBarType Bar;	///K线周期代码(n[m/h/d/o])
	} MegaClearLastKLineStruct;
	
	///输入OToken
	typedef struct MegaOTokenInsertStruct
	{
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaShortIndexIDType APPID;	///应用编号
		MegaShortContentType Token;	///令牌
		MegaLongIndexIDType MemberID;	///成员代码
		MegaMilliSecondsType LoginTime;	///登录时间
		MegaMilliSecondsType ExpireTime;	///失效时间
		MegaNumberType AccessLimit;	///每秒访问的次数限制
	} MegaOTokenInsertStruct;
	
	///操作OToken
	typedef struct MegaOTokenActionStruct
	{
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaShortIndexIDType APPID;	///应用编号
		MegaActionFlagType ActionFlag;	///操作标志
	} MegaOTokenActionStruct;
	
	///重置LocalID
	typedef struct MegaResetLocalIDStruct
	{
		MegaShortContentType Token;	///令牌
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaShortIndexIDType APPID;	///应用编号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaAccountIDType AccountID;	///资金账号
		MegaCurrencyType Currency;	///币种
	} MegaResetLocalIDStruct;
	
	///合并持仓
	typedef struct MegaPositionMergeStruct
	{
		MegaIndexIDType LocalID;	///操作本地号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaLongIndexIDType PositionID1;	///持仓代码1
		MegaLongIndexIDType PositionID2;	///持仓代码2
		MegaLongIndexIDType PositionID3;	///持仓代码3
		MegaLongIndexIDType PositionID4;	///持仓代码4
		MegaLongIndexIDType PositionID5;	///持仓代码5
		MegaLongIndexIDType PositionID6;	///持仓代码6
		MegaLongIndexIDType PositionID7;	///持仓代码7
		MegaLongIndexIDType PositionID8;	///持仓代码8
		MegaLongIndexIDType PositionID9;	///持仓代码9
		MegaLongIndexIDType PositionID10;	///持仓代码10
	} MegaPositionMergeStruct;
	
	///用户入金
	typedef struct MegaDepositStruct
	{
		MegaIndexIDType LocalID;	///出入金本地号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType AccountID;	///资金账号
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaMoneyType Amount;	///发生额
		MegaCurrencyType Currency;	///币种
		MegaNameType Remark;	///备注
	} MegaDepositStruct;
	
	///用户出金
	typedef struct MegaWithdrawStruct
	{
		MegaIndexIDType LocalID;	///出入金本地号
		MegaIndexIDType OrderID;	///冻结无需填,真正出金的订单号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaShortIndexIDType SettlementGroup;	///转出结算组编号
		MegaShortIndexIDType OtherSettlementGroup;	///转入结算组编号
		MegaMoneyType Amount;	///发生额
		MegaCurrencyType Currency;	///币种
		MegaNameType Remark;	///备注
	} MegaWithdrawStruct;
	
	///用户转账
	typedef struct MegaTransferStruct
	{
		MegaIndexIDType LocalID;	///出入金本地号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaAccountIDType OtherAccountID;	///内部转账资金账号
		MegaShortIndexIDType OtherSettlementGroup;	///内部转账结算组编号
		MegaLongIndexIDType OtherMemberID;	///内部转账成员代码
		MegaMoneyType Amount;	///发生额
		MegaCurrencyType Currency;	///币种
		MegaNameType Remark;	///备注
	} MegaTransferStruct;
	
	///报单止盈止损操作
	typedef struct MegaCloseOrderActionStruct
	{
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaIndexIDType OrderLocalID;	///报单本地标识
		MegaActionFlagType ActionFlag;	///报单操作标志
		MegaIndexIDType LocalID;	///操作本地标识
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaPriceType Price;	///价格
		MegaVolumeType Volume;	///数量变化
		MegaDirectionType Direction;	///买卖方向
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaMoneyType Cost;	///委托额
		MegaNameType Remark;	///备注
		MegaPriceType SLPrice;	///止损价
		MegaPriceType SLTriggerPrice;	///止损触发价
		MegaPriceType TPPrice;	///止盈价
		MegaPriceType TPTriggerPrice;	///止盈触发价
		MegaPriceType TriggerPrice;	///触发价
		MegaShortIndexIDType TriggerOrderType;	///触发的订单类型
		MegaLongContentType TriggerValue;	///触发单具体设置信息
		MegaIndexIDType TriggerDetail;	///触发类型明细
		MegaTriggerPriceTypeType TriggerPriceType;	///触发价类型
		MegaNumberType ErrorNo;	///错误代码
		MegaShortContentType ErrorMsg;	///错误信息
	} MegaCloseOrderActionStruct;
	
	///杠杆倍数
	typedef struct MegaLeverageStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaPosiDirectionType PosiDirection;	///持仓多空方向
		MegaLeverageType DefaultMaxLeverage;	///默认最大杠杆倍数
		MegaLeverageType LongLeverage;	///多头杠杆倍数
		MegaLeverageType LongMaxLeverage;	///当前多头梯度最大杠杆倍数
		MegaVolumeType LongMaxVolume;	///当前多头梯度最大张数
		MegaVolumeType LongLastVolume;	///当前多头梯度剩余可开张数
		MegaNumberType LongLevel;	///多头梯度档位
		MegaRatioType LongMaintRatio;	///当前多头维持保证金率
		MegaLeverageType ShortLeverage;	///空头杠杆倍数
		MegaLeverageType ShortMaxLeverage;	///当前空头梯度最大杠杆倍数
		MegaVolumeType ShortMaxVolume;	///当前空头梯度最大张数
		MegaVolumeType ShortLastVolume;	///当前空头梯度剩余可开张数
		MegaNumberType ShortLevel;	///空头梯度档位
		MegaRatioType ShortMaintRatio;	///当前空头维持保证金率
		MegaBoolType IsCrossMargin;	///是否全仓
	} MegaLeverageStruct;
	
	///反向开仓
	typedef struct MegaPositionReverseStruct
	{
		MegaIndexIDType LocalID;	///操作本地号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaVolumeType Volume;	///反向开仓数量
		MegaNameType Remark;	///备注
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaShortIndexIDType APPID;	///应用编号
	} MegaPositionReverseStruct;
	
	///私有主题分类
	typedef struct MegaPrivateDispatchStruct
	{
		MegaLongIndexIDType TopicID;	///请参考TOPICID类型定义
		MegaLongIndexIDType MemberID;	///成员代码
		MegaLongIndexIDType PrivateBase;	///私有主题起点
	} MegaPrivateDispatchStruct;
	
	///TICK订单簿变化
	typedef struct MegaTickMarketOrderStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaDirectionType Direction;	///买卖方向
		MegaPriceType Price;	///价格
		MegaVolumeType Volume;	///数量
		MegaNumberType Orders;	///订单数量
		MegaLargeNumberType BusinessNo;	///最后变化序列号
		MegaVolumeType TrueVolume;	///真实报单数量
		MegaPriceType Tick;	///最小变动价位
		MegaShortContentType SubIndex;	///索引
	} MegaTickMarketOrderStruct;
	
	///基础K线
	typedef struct MegaKLineBaseStruct
	{
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaPriceType OpenPrice;	///开盘价
		MegaPriceType HighestPrice;	///最高价
		MegaPriceType LowestPrice;	///最低价
		MegaPriceType ClosePrice;	///收盘价
		MegaCollectType Volume;	///数量
		MegaCollectType Turnover;	///成交金额
	} MegaKLineBaseStruct;
	
	///历史行情线
	typedef struct MegaLineStruct
	{
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaPriceType ClosePrice;	///收盘价
	} MegaLineStruct;
	
	///充币兑换
	typedef struct MegaChangeCurrencyStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType LocalID;	///报单本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID1;	///第一个标的代码
		MegaInstrumentIDType InstrumentID2;	///第二个标的代码
		MegaCurrencyType Currency;	///币种
		MegaMoneyType Amount;	///发生额
		MegaNameType Remark;	///备注
	} MegaChangeCurrencyStruct;
	
	///钱包登录随机数
	typedef struct MegaWalletNonceStruct
	{
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaLongIndexIDType MemberID;	///成员代码
		MegaLongIndexIDType Nonce;	///随机数
	} MegaWalletNonceStruct;
	
	///钱包登录信息
	typedef struct MegaWalletLoginStruct
	{
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaContentType Signature;	///签名
	} MegaWalletLoginStruct;
	
	///报单查询
	typedef struct MegaQryOrderStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaIndexIDType LocalID;	///报单本地标识
		MegaIndexIDType OrderID;	///订单号
	} MegaQryOrderStruct;
	
	///持仓汇总查询
	typedef struct MegaQryMemberPositionStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
	} MegaQryMemberPositionStruct;
	
	///交易单元持仓查询
	typedef struct MegaQryPositionStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaPositionClassType PositionClass;	///持仓大类
	} MegaQryPositionStruct;
	
	///交易资金查询
	typedef struct MegaQryAccountStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaShortIndexIDType SettlementGroup;	///结算组
		MegaCurrencyType Currency;	///币种
	} MegaQryAccountStruct;
	
	///标的查询
	typedef struct MegaQryInstrumentStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaInstrumentIDType InstrumentID;	///标的代码
	} MegaQryInstrumentStruct;
	
	///标的查询
	typedef struct MegaQryCurrencyStruct
	{
		MegaShortIndexIDType SettlementGroup;	///结算组
		MegaCurrencyType Currency;	///币种
	} MegaQryCurrencyStruct;
	
	///成员查询
	typedef struct MegaQryMemberStruct
	{
		MegaMemberTypeType MemberType;	///成员类型
		MegaLongIndexIDType MemberID;	///成员代码
	} MegaQryMemberStruct;
	
	///标的价位查询
	typedef struct MegaQryMarketOrderStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaDirectionType Direction;	///买卖方向
		MegaNumberType Depth;	///深度
		MegaPriceType Tick;	///最小变动价位
	} MegaQryMarketOrderStruct;
	
	///查询APIKEY
	typedef struct MegaQryAPIKeyStruct
	{
		MegaLongIndexIDType UserID;	///交易用户代码
	} MegaQryAPIKeyStruct;
	
	///查询保证金率
	typedef struct MegaQryMarginRateStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaAccountIDType AccountID;	///资金账号
	} MegaQryMarginRateStruct;
	
	///查询手续费率
	typedef struct MegaQryFeeStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaMatchRoleType MatchRole;	///成交角色
	} MegaQryFeeStruct;
	
	///查询触发订单
	typedef struct MegaQryTriggerOrderStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaIndexIDType LocalID;	///报单本地标识
	} MegaQryTriggerOrderStruct;
	
	///查询杠杆倍数
	typedef struct MegaQryLeverageStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaPosiDirectionType PosiDirection;	///持仓多空方向
	} MegaQryLeverageStruct;
	
	///查询行情
	typedef struct MegaQryMarketDataStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaNumberType From;	///开始序号
		MegaNumberType Limit;	///限制数量
	} MegaQryMarketDataStruct;
	
	///查询成交
	typedef struct MegaQryTradeStruct
	{
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaNumberType Limit;	///限制数量
		MegaIndexIDType Before;	///请求此 ID 之前（更旧的数据）的分页内容
		MegaIndexIDType After;	///请求此 ID 之后（更新的数据）的分页内容
		MegaMilliSecondsType StartTime;	///起始时间
		MegaMilliSecondsType EndTime;	///结束时间
	} MegaQryTradeStruct;
	
	///查询历史委托
	typedef struct MegaQryFinishOrderStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaAccountIDType AccountID;	///资金账号
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaOrderStatusType OrderStatus;	///报单状态
		MegaNumberType Limit;	///限制数量
		MegaIndexIDType Before;	///请求此 ID 之前（更旧的数据）的分页内容
		MegaIndexIDType After;	///请求此 ID 之后（更新的数据）的分页内容
		MegaMilliSecondsType StartTime;	///起始时间
		MegaMilliSecondsType EndTime;	///结束时间
	} MegaQryFinishOrderStruct;
	
	///查询历史条件单
	typedef struct MegaQryFinishTriggerOrderStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaAccountIDType AccountID;	///资金账号
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaTriggerStatusType TriggerStatus;	///触发报单状态
		MegaTriggerOrderTypeType TriggerOrderType;	///触发的订单类型
		MegaNumberType Limit;	///限制数量
		MegaIndexIDType Before;	///请求此 ID 之前（更旧的数据）的分页内容
		MegaIndexIDType After;	///请求此 ID 之后（更新的数据）的分页内容
		MegaMilliSecondsType StartTime;	///起始时间
		MegaMilliSecondsType EndTime;	///结束时间
	} MegaQryFinishTriggerOrderStruct;
	
	///指定杠杠下梯度信息
	typedef struct MegaQryWithLeverageStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaPosiDirectionType PosiDirection;	///持仓多空方向
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaLeverageType LongLeverage;	///多头杠杆倍数
		MegaLeverageType ShortLeverage;	///空头杠杆倍数
		MegaLeverageType NetLeverage;	///净仓杠杆倍数
	} MegaQryWithLeverageStruct;
	
	///查询K线
	typedef struct MegaQryKLineStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaBarType Bar;	///K线周期代码(n[m/h/d/o])
		MegaMilliSecondsType EndTime;	///结束时间
		MegaNumberType Limit;	///数量
	} MegaQryKLineStruct;
	
	///查询线
	typedef struct MegaQryLineStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaBarType Bar;	///K线周期代码(n[m/h/d/o])
		MegaNumberType Limit;	///数量
	} MegaQryLineStruct;
	
	///查询资金流水
	typedef struct MegaQryAccountDetailStruct
	{
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaAccountIDType AccountID;	///资金账号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaCurrencyType Currency;	///币种
		MegaShortIndexIDType SettlementGroup;	///结算组
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaIndexIDType Source;	///财务流水类型
		MegaNumberType Limit;	///限制数量
		MegaIndexIDType Before;	///请求此 ID 之前（更旧的数据）的分页内容
		MegaIndexIDType After;	///请求此 ID 之后（更新的数据）的分页内容
		MegaMilliSecondsType StartTime;	///起始时间
		MegaMilliSecondsType EndTime;	///结束时间
	} MegaQryAccountDetailStruct;
	
	///查询结算流水
	typedef struct MegaQrySettleDetailStruct
	{
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaSettleActionTypeType SettleAction;	///结算操作
	} MegaQrySettleDetailStruct;
	
	///业务配置
	typedef struct MegaServiceConfigStruct
	{
		MegaNameType ConfigName;	///配置名称
		MegaNameType Index1;	///索引1
		MegaNameType Index2;	///索引2
		MegaLongNameType ConfigValue;	///配置值
		MegaContentType Remark;	///备注
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
	} MegaServiceConfigStruct;
	
	///币种信息
	typedef struct MegaCurrencyStruct
	{
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaCurrencyType Currency;	///币种
		MegaMoneyType Deposit;	///入金金额
		MegaMoneyType Withdraw;	///出金金额
		MegaMoneyType Balance;	///静态权益
		MegaMoneyType PreBalance;	///上次静态权益
		MegaNumberType SortNumber;	///排序字段
		MegaBoolType CanReduce;	///是否可以出金
		MegaBoolType IsTrading;	///是否允许相关交易
		MegaIndexIDType SettleSegment;	///结算段
		MegaMoneyType MoneyTick;	///最小单位
		MegaExchangeIDType ExchangeID;	///价格参考交易所代码
		MegaInstrumentIDType InstrumentID;	///价格参考标的代码
		MegaMoneyType LoanUse;	///借款使用量
		MegaRatioType LoanLimit;	///最大可借比例
		MegaRatioType LoanBorrow;	///借款利息
		MegaRatioType LoanDeposit;	///存款利息
		MegaRatioType LoanBase;	///利息起点
		MegaRatioType LoanRate;	///利息斜率
		MegaRatioType LoanHighBase;	///高利息起点
		MegaRatioType LoanHighRate;	///高利息斜率
		MegaLongNameType Remark;	///备注
		MegaMoneyType CurrValue;	///当前价值
		MegaRatioType HairCut;	///抵押比例
		MegaMoneyType MaxValue;	///最大抵押数额
		MegaShortIndexIDType MarginRateGroup;	///保证金组
		MegaRatioType PrivateEnd;	///私有化停止交易线
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
	} MegaCurrencyStruct;
	
	///市场行情
	typedef struct MegaMarketDataStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaPriceType PreClosePrice;	///上次收盘
		MegaPriceType PreSettlementPrice;	///上次结算价
		MegaVolumeType PreOpenInterest;	///上次持仓量
		MegaRatioType PrePositionFeeRate;	///上次持仓费按比例
		MegaPriceType ClosePrice;	///收盘价
		MegaMilliSecondsType PositionFeeTime;	///收取持仓费用时间
		MegaMilliSecondsType SettlementTime;	///结算时间
		MegaPriceType UpperLimitPrice;	///最高限价
		MegaPriceType LowerLimitPrice;	///最低限价
		MegaPriceType UnderlyingPrice;	///基础标的价格
		MegaPriceType MarkedPrice;	///标记价格
		MegaRatioType PositionFeeRate;	///预计持仓费按比例
		MegaPriceType HighestPrice;	///当日最高价
		MegaPriceType LowestPrice;	///当日最低价
		MegaPriceType LastPrice;	///最新价
		MegaCollectType Volume;	///当日数量
		MegaCollectType Turnover;	///当日成交金额
		MegaVolumeType OpenInterest;	///持仓量
		MegaPriceType SettlementPrice;	///预估结算价
		MegaPriceType OpenPrice;	///当日开盘价
		MegaBoolType IsPubMarketData;	///行情发布状态
		MegaInstrumentStatusType InstrumentStatus;	///标的交易状态
		MegaLongNameType Remark;	///备注
		MegaLargeNumberType BusinessNo;	///业务序列号
		MegaTotalType MarkedPriceTotal;	///标记价格汇总
		MegaTotalType MarkedPriceWeight;	///标记价格权重
		MegaPriceType L25UpperPrice;	///25档行情最高价
		MegaPriceType L25LowerPrice;	///25档行情最低价
		MegaPriceType L25UpperPrice1;	///25档行情最高价1
		MegaPriceType L25LowerPrice1;	///25档行情最低价1
		MegaPriceType L25UpperPrice2;	///25档行情最高价2
		MegaPriceType L25LowerPrice2;	///25档行情最低价2
		MegaPriceType BidPrice1;	///申买价一
		MegaVolumeType BidVolume1;	///申买量一
		MegaPriceType AskPrice1;	///申卖价一
		MegaVolumeType AskVolume1;	///申卖量一
		MegaPriceType TheoryPrice;	///理论价
		MegaPriceType HighestPrice24;	///24小时最高价
		MegaPriceType LowestPrice24;	///24小时最低价
		MegaCollectType Volume24;	///24小时数量
		MegaCollectType Turnover24;	///24小时成交金额
		MegaPriceType OpenPrice24;	///24小时开盘价
		MegaNumberType Frequency;	///每秒更新的次数
		MegaMilliSecondsType InsertTime;	///插入时间
	} MegaMarketDataStruct;
	
	///交易标的
	typedef struct MegaInstrumentStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaLongNameType InstrumentName;	///标的名称
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaShortIndexIDType ProductGroup;	///产品组
		MegaInstrumentIDType UnderlyingID;	///基础标的代码
		MegaProductClassType ProductClass;	///产品大类
		MegaProductTypeType ProductType;	///产品类型
		MegaPriceType StrikePrice;	///执行价
		MegaOptionsTypeType OptionsType;	///期权类型
		MegaRatioType VolumeMultiple;	///数量乘数
		MegaMilliSecondsType StartTime;	///启动时间
		MegaMilliSecondsType ExpireTime;	///到期时间
		MegaCurrencyType PriceCurrency;	///计价币种
		MegaCurrencyType ClearCurrency;	///清算币种
		MegaBoolType IsInverse;	///是否反向标的
		MegaCurrencyType BaseCurrency;	///标的基础货币
		MegaMarginPriceTypeType MarginPriceType;	///保证金价格类型
		MegaPriceType BasisPrice;	///挂牌基准价
		MegaVolumeType MinOrderVolume;	///最小下单量
		MegaVolumeType MinOrderCost;	///最小下单额
		MegaVolumeType MaxOrderVolume;	///最大下单量
		MegaPriceType PriceTick;	///最小变动价位
		MegaVolumeType VolumeTick;	///最小变动量
		MegaVolumeType ShowVolumeTick;	///前端行情显示最小变动量(服务端不用)
		MegaTradingModelType TradingModel;	///交易模式
		MegaShortContentType DayStartTime;	///每日开始交易时间
		MegaNumberType SortNumber;	///排序字段
		MegaContentType Remark;	///备注
		MegaLeverageType DefaultLeverage;	///默认杠杆倍数
		MegaValueModeType PriceLimitValueMode;	///取值方式
		MegaLimitPriceTypeType PriceLimitPrice;	///限价价格类型
		MegaPriceType PriceLimitUpperValue;	///上限
		MegaPriceType PriceLimitLowerValue;	///下限
		MegaShortIndexIDType MarginRateGroup;	///保证金组
		MegaShortIndexIDType CFDGroup;	///差价组
		MegaShortIndexIDType FeeGroup;	///手续费组
		MegaShortIndexIDType TradingRightGroup;	///权限组
		MegaVolumeType MaxOpenInterest;	///最大持仓量
		MegaShortIndexIDType FundingRateGroup;	///资金费用组
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaNumberType MarkedPriceTimes;	///标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价)
	} MegaInstrumentStruct;
	
	///当前时间
	typedef struct MegaCurrentTimeStruct
	{
		MegaShortIndexIDType SystemID;	///系统名
		MegaDateType ZoneDate;	///本地日期
		MegaTimeType ZoneTime;	///本地时间
		MegaDateType ZoneDays;	///本地天数
		MegaMilliSecondsType CurrTime;	///当前时间（秒）
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间（毫秒）
		MegaNumberType TimeZone;	///本地时区
		MegaIndexIDType LastTradeID;	///最新最新成交序列号
		MegaIndexIDType LastOrderID;	///最新订单序列号
		MegaIndexIDType LastAccountDetailID;	///最新资金明细序列号
		MegaLargeNumberType LastTimeSortNo;	///最新排序序列号
		MegaLargeNumberType LastBusinessNo;	///最新事务序列号
	} MegaCurrentTimeStruct;
	
	///成员
	typedef struct MegaMemberStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaMemberTypeType MemberType;	///成员类型
		MegaShortIndexIDType Grade;	///等级
		MegaNumberType Priority;	///优先权
		MegaShortIndexIDType FeeGrade;	///手续费等级
		MegaShortIndexIDType MarginRateGrade;	///保证金等级
		MegaShortIndexIDType TradingRightGrade;	///权限等级
		MegaShortIndexIDType CFDGrade;	///差价等级
		MegaShortIndexIDType LiquidityGrade;	///流动性等级
		MegaRiskLevelType RiskLevel;	///风控类型
		MegaPositionTypeType PositionType;	///持仓类型
		MegaNumberType TriggerOrders;	///触发订单个数
		MegaNumberType OpenOrders;	///未成交订单数
		MegaNumberType OpenPositions;	///用户持仓个数
		MegaIndexIDType MaxLocalID;	///最大本地标识
		MegaSinkTypeType SinkType;	///下游系统同步类型
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaShortIndexIDType Region;	///
		MegaRatioType FeeDeduceDisCount;	///作为手续费的抵扣优惠比例
		MegaCurrencyType FeeDeduceCurrency;	///作为手续费的抵扣比例
		MegaBoolType CanBorrow;	///是否需要借钱交易
		MegaNameType Remark;	///备注
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
	} MegaMemberStruct;
	
	///用户会话
	typedef struct MegaUserSessionStruct
	{
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaPasswordType Password;	///密码
		MegaNameType UserProductID;	///用户端产品信息
		MegaMacAddressType MacAddress;	///Mac地址
		MegaIPAddressType ClientIPAddress;	///终端IP地址
		MegaLongNameType HDSerialID;	///硬盘序列号
		MegaShortContentType AuthCode;	///授权编码
		MegaShortIndexIDType APPID;	///应用编号
		MegaShortContentType Token;	///令牌
		MegaLongNameType Remark;	///备注
		MegaNameType APIID;	///接口端产品信息
		MegaMilliSecondsType LoginTime;	///登录时间
		MegaMilliSecondsType ExpireTime;	///失效时间
		MegaContentType IPAddress;	///允许登录的IP地址
		MegaIndexIDType MaxLocalID;	///最大本地标识
		MegaNumberType SessionNo;	///会话编号
		MegaNumberType FrontNo;	///前置编号
		MegaNumberType AccessLimit;	///每秒访问的次数限制
		MegaUserTypeType UserType;	///用户类型
		MegaLongIndexIDType MemberID;	///成员代码
		MegaNumberType LimitAccesses;	///该秒已经访问的次数
		MegaSinkTypeType SinkType;	///下游系统同步类型
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
	} MegaUserSessionStruct;
	
	///最新K线
	typedef struct MegaLastKLineStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaBarType Bar;	///K线周期代码(n[m/h/d/o])
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaPriceType OpenPrice;	///开盘价
		MegaPriceType HighestPrice;	///最高价
		MegaPriceType LowestPrice;	///最低价
		MegaPriceType ClosePrice;	///收盘价
		MegaCollectType Volume;	///数量
		MegaCollectType Turnover;	///成交金额
		MegaMilliSecondsType UpdateTime;	///更新时间
	} MegaLastKLineStruct;
	
	///CFD溢价率
	typedef struct MegaCFDRateStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码或组
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaLongIndexIDType TriggerOrderType;	///报单价格类型
		MegaRatioType RateBegin;	///多头起始溢价率
		MegaRatioType RateEnd;	///多头最大溢价率
		MegaRatioType CompVolume;	///点差比较基准
		MegaRatioType CompRate;	///点差比较基准率
		MegaRatioType HardRate;	///硬顶率
		MegaLongNameType Remark;	///备注
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
	} MegaCFDRateStruct;
	
	///交易权限
	typedef struct MegaTradingRightStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaDirectionType Direction;	///买卖方向
		MegaTradingRightType TradingRight;	///交易权限
		MegaNameType Remark;	///备注
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
	} MegaTradingRightStruct;
	
	///保证金率
	typedef struct MegaMarginRateStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaValueModeType ValueMode;	///取值方式
		MegaSequenceTypeType MarginSequenceType;	///保证金数列
		MegaSequenceTypeType RiskSequenceType;	///风险限额数列
		MegaRatioType LongMarginRatio;	///多头起始保证金率
		MegaRatioType LongMaintMarginRatio;	///多头起始维持保证金率
		MegaRatioType LongMarginStep;	///多头保证金率步长
		MegaRatioType LongMaintMarginStep;	///多头维持保证金率步长
		MegaMoneyType LongRiskLimit;	///多头起始风险限额
		MegaMoneyType LongRiskStep;	///多头风险限额步长
		MegaRatioType ShortMarginRatio;	///空头起始保证金率
		MegaRatioType ShortMaintMarginRatio;	///空头起始维持保证金率
		MegaRatioType ShortMarginStep;	///空头保证金率步长
		MegaRatioType ShortMaintMarginStep;	///空头维持保证金率步长
		MegaMoneyType ShortRiskLimit;	///空头起始风险限额
		MegaMoneyType ShortRiskStep;	///空头风险限额步长
		MegaLeverageType DefaultLeverage;	///默认杠杆倍数
		MegaLongNameType Remark;	///备注
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
	} MegaMarginRateStruct;
	
	///手续费率
	typedef struct MegaFeeStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaMatchRoleType MatchRole;	///成交角色
		MegaRatioType OpenFeeRate;	///开仓手续费按比例
		MegaRatioType OpenFeeAmount;	///开仓手续费按手数
		MegaRatioType CloseFeeRate;	///平仓手续费按比例
		MegaRatioType CloseFeeAmount;	///平仓手续费按手数
		MegaRatioType CloseTodayFeeRate;	///平今仓手续费按比例
		MegaRatioType CloseTodayFeeAmount;	///平今仓手续费按手数
		MegaLongNameType Remark;	///备注
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
	} MegaFeeStruct;
	
	///流动性设置表
	typedef struct MegaLiquidityStruct
	{
		MegaShortIndexIDType LiquidityGrade;	///流动性等级
		MegaLongIndexIDType LiquidityGroup;	///标的代码
		MegaLiquidityPriceTypeType BasePriceType;	///基于的价格
		MegaVolumeType VolumeBegin;	///起始量
		MegaRatioType BuyOpen;	///买开流动性比例
		MegaRatioType BuyClose;	///买平流动性比例
		MegaRatioType SellOpen;	///卖开流动性比例
		MegaRatioType SellClose;	///卖平流动性比例
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
	} MegaLiquidityStruct;
	
	///账户资金
	typedef struct MegaAccountStruct
	{
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaAccountIDType AccountID;	///资金账号
		MegaCurrencyType Currency;	///币种
		MegaLongIndexIDType MemberID;	///成员代码
		MegaMoneyType Available;	///可用资金
		MegaMoneyType Withdrawable;	///可取资金
		MegaMoneyType PreBalance;	///上次静态权益
		MegaMoneyType Balance;	///静态权益
		MegaMoneyType PositionCost;	///持仓成本
		MegaMoneyType UseMargin;	///所有占用保证金
		MegaMoneyType CrossMargin;	///全仓保证金
		MegaMoneyType CloseProfit;	///平仓盈亏
		MegaMoneyType Deposit;	///入金金额
		MegaMoneyType Withdraw;	///出金金额
		MegaMoneyType FrozenMargin;	///冻结保证金
		MegaMoneyType FrozenMoney;	///冻结资金
		MegaMoneyType FrozenFee;	///冻结手续费
		MegaMoneyType Fee;	///手续费
		MegaMoneyType MoneyChange;	///资金变化
		MegaMoneyType Reserve;	///保留资金
		MegaMoneyType ReserveAvailable;	///保留资金可用
		MegaMoneyType ReserveLimit;	///保留资金限制
		MegaMoneyType ReserveProfit;	///保留资金盈亏
		MegaReserveTypeType ReserveType;	///保留资金类型
		MegaBoolType ReserveActive;	///保留资金是否活跃
		MegaTotalType TotalCloseProfit;	///总平仓盈亏
		MegaTotalType TotalDeposit;	///总入金金额
		MegaTotalType TotalWithdraw;	///总出金金额
		MegaTotalType TotalFee;	///总手续费
		MegaTotalType TotalMoneyChange;	///总资金变化
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaShortIndexIDType AccountType;	///账户类型
		MegaLargeNumberType BusinessNo;	///业务序列号
		MegaShortIndexIDType MarginRateGrade;	///保证金等级
		MegaShortIndexIDType TradingRightGrade;	///权限等级
		MegaNumberType Frequency;	///每秒更新的次数
		MegaMoneyType MaintMargin;	///维持保证金
		MegaMoneyType UnRealProfit;	///未实现盈亏
		MegaMoneyType CopyProfit;	///带单盈利分配
		MegaCFDTypeType CFDType;	///点差接管类型
		MegaShortIndexIDType FeeGrade;	///手续费等级
		MegaShortIndexIDType CFDGrade;	///差价等级
		MegaMoneyType Borrow;	///借款周期结算量
		MegaNameType Remark;	///备注
		MegaIndexIDType MaxLocalID;	///最大本地标识
		MegaMoneyType CurrValue;	///当前价值
		MegaBoolType CanReduce;	///是否可以出金
	} MegaAccountStruct;
	
	///成员持仓汇总
	typedef struct MegaMemberPositionStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaAccountIDType AccountID;	///资金账号
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaVolumeType Position;	///总持仓
		MegaVolumeType LongFrozen;	///多头冻结
		MegaVolumeType ShortFrozen;	///空头冻结
		MegaLeverageType DefaultLeverage;	///默认杠杆倍数
		MegaBoolType DefaultCrossMargin;	///默认是否全仓
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
	} MegaMemberPositionStruct;
	
	///标的持仓
	typedef struct MegaPositionStruct
	{
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaPositionClassType PositionClass;	///持仓类型
		MegaPosiDirectionType PosiDirection;	///持仓多空方向
		MegaVolumeType PrePosition;	///上次持仓
		MegaVolumeType Position;	///总持仓
		MegaVolumeType LongFrozen;	///多头冻结
		MegaVolumeType ShortFrozen;	///空头冻结
		MegaVolumeType PreLongFrozen;	///昨日多头冻结
		MegaVolumeType PreShortFrozen;	///昨日空头冻结
		MegaVolumeType HighestPosition;	///最大持仓
		MegaVolumeType ClosePosition;	///可平持仓
		MegaCostType PositionCost;	///持仓成本
		MegaPriceType CostPrice;	///成本价
		MegaMoneyType UseMargin;	///占用保证金
		MegaMoneyType FrozenMargin;	///冻结保证金
		MegaMoneyType LongFrozenMargin;	///多头冻结保证金
		MegaMoneyType ShortFrozenMargin;	///空头冻结保证金
		MegaMoneyType CloseProfit;	///平仓盈亏
		MegaTotalType TotalPositionCost;	///开仓成本
		MegaTotalType TotalCloseProfit;	///总平仓盈亏
		MegaPriceType OpenPrice;	///开仓均价
		MegaPriceType ClosePrice;	///平仓均价
		MegaMoneyType TradeFee;	///交易费用
		MegaMoneyType PositionFee;	///持仓费用或者股票分红等
		MegaLeverageType Leverage;	///杠杆倍数
		MegaAccountIDType AccountID;	///资金账号
		MegaCurrencyType PriceCurrency;	///计价币种
		MegaCurrencyType ClearCurrency;	///清算币种
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaBoolType IsCrossMargin;	///是否全仓
		MegaIndexIDType CloseOrderID;	///平仓单代码
		MegaPriceType SLTriggerPrice;	///止损触发价
		MegaPriceType TPTriggerPrice;	///止盈触发价
		MegaMilliSecondsType BeginTime;	///持仓不为0的开始时间
		MegaMilliSecondsType InsertTime;	///插入时间(新持仓时间)
		MegaMilliSecondsType LastOpenTime;	///最后开仓时间
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaLargeNumberType BusinessNo;	///业务序列号
		MegaBoolType IsAutoAddMargin;	///是否自动追加保证金
		MegaNumberType Frequency;	///每秒更新的次数
		MegaMoneyType MaintMargin;	///维持保证金
		MegaMoneyType UnRealProfit;	///未实现盈亏
		MegaPriceType LiquidPrice;	///清算价格
		MegaIndexIDType CreateTime;	///创建时间
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaRatioType CopyProfitRate;	///带单分配比例
		MegaMoneyType CopyProfit;	///带单盈利分配
		MegaLongIndexIDType FirstTradeID;	///初始成交订单号
		MegaLongIndexIDType LastTradeID;	///结束成交订单号
		MegaBusinessTypeType BusinessType;	///业务类别
		MegaNameType BusinessValue;	///业务值
		MegaMoneyType Reserve;	///保留资金
		MegaMoneyType ReserveProfit;	///保留资金盈亏
		MegaNameType Remark;	///备注
	} MegaPositionStruct;
	
	///报单
	typedef struct MegaOrderStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType LocalID;	///报单本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaOrderPriceTypeType OrderPriceType;	///报单价格类型
		MegaDirectionType Direction;	///买卖方向
		MegaOffsetFlagType OffsetFlag;	///开平标志
		MegaPriceType Price;	///报单价格
		MegaVolumeType Volume;	///数量
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaValueModeType VolumeMode;	///数量取值方式
		MegaMoneyType Cost;	///委托额
		MegaOrderTypeType OrderType;	///订单类型
		MegaMilliSecondsType GTDTime;	///GTD时间
		MegaVolumeType MinVolume;	///最小成交量
		MegaBusinessTypeType BusinessType;	///业务类别
		MegaNameType BusinessValue;	///业务值
		MegaIndexIDType CloseOrderID;	///平仓指定开仓的订单号
		MegaBoolType IsCrossMargin;	///是否全仓
		MegaNameType Remark;	///备注
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaIndexIDType CopyOrderID;	///带单员报单号
		MegaLeverageType Leverage;	///委托单杠杆倍数
		MegaRatioType CopyProfitRate;	///带单分配比例
		MegaShortIndexIDType APPID;	///应用编号
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaPriceType TriggerPrice;	///触发价
		MegaMoneyType Reserve;	///保留资金
		MegaOrderStatusType OrderStatus;	///报单状态
		MegaDeriveSourceType DeriveSource;	///衍生来源
		MegaIndexIDType DeriveDetail;	///衍生明细
		MegaVolumeType VolumeTraded;	///成交数量
		MegaVolumeType VolumeRemain;	///剩余数量
		MegaVolumeType VolumeCancled;	///已经撤单数量
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaNumberType Priority;	///优先权
		MegaLargeNumberType TimeSortNo;	///按时间排队的序号
		MegaNumberType FrontNo;	///前置编号
		MegaCurrencyType PriceCurrency;	///计价币种
		MegaCurrencyType FeeCurrency;	///手续费币种
		MegaCurrencyType ClearCurrency;	///清算币种
		MegaMoneyType FrozenMoney;	///冻结资金
		MegaMoneyType FrozenFee;	///冻结手续费
		MegaMoneyType FrozenMargin;	///冻结保证金
		MegaMoneyType Fee;	///手续费
		MegaMoneyType CloseProfit;	///平仓盈亏
		MegaMoneyType Turnover;	///成交金额
		MegaIndexIDType RelatedOrderID;	///关联报单号
		MegaNameType BusinessResult;	///业务执行结果
		MegaLargeNumberType BusinessNo;	///业务序列号
		MegaBoolType Tradable;	///是否可以参加交易
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaPosiDirectionType PosiDirection;	///持仓多空方向
		MegaPriceType TradePrice;	///成交均价
		MegaPriceType OpenPrice;	///平仓成交时的开仓均价
		MegaIndexIDType TriggerOrderID;	///触发报单号
		MegaPriceType SLTriggerPrice;	///开仓报单成交之后的止损触发价
		MegaPriceType TPTriggerPrice;	///开仓报单成交之后的止盈触发价
		MegaMoneyType CopyProfit;	///带单盈利分配
		MegaVolumeType Position;	///成交时的持仓量
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaPriceType LastPriceByInsert;	///插入时的最新价
		MegaPriceType BidPrice1ByInsert;	///插入时的买一价
		MegaPriceType AskPrice1ByInsert;	///插入时的卖一价
		MegaMoneyType Available;	///可用资金
		MegaIndexIDType CreateTime;	///创建时间
	} MegaOrderStruct;
	
	///条件报单
	typedef struct MegaTriggerOrderStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType LocalID;	///报单本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaOrderPriceTypeType OrderPriceType;	///报单价格类型
		MegaDirectionType Direction;	///买卖方向
		MegaOffsetFlagType OffsetFlag;	///开平标志
		MegaPriceType Price;	///报单价格
		MegaVolumeType Volume;	///数量
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaValueModeType VolumeMode;	///数量取值方式
		MegaMoneyType Cost;	///委托额
		MegaOrderTypeType OrderType;	///订单类型
		MegaMilliSecondsType GTDTime;	///GTD时间
		MegaVolumeType MinVolume;	///最小成交量
		MegaBusinessTypeType BusinessType;	///业务类别
		MegaNameType BusinessValue;	///业务值
		MegaIndexIDType CloseOrderID;	///平仓指定开仓的订单号
		MegaBoolType IsCrossMargin;	///是否全仓
		MegaNameType Remark;	///备注
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaIndexIDType CopyOrderID;	///带单员报单号
		MegaLeverageType Leverage;	///委托单杠杆倍数
		MegaRatioType CopyProfitRate;	///带单分配比例
		MegaShortIndexIDType APPID;	///应用编号
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaPriceType TriggerPrice;	///触发价
		MegaMoneyType Reserve;	///保留资金
		MegaPriceType SLPrice;	///止损价
		MegaPriceType SLTriggerPrice;	///止损触发价
		MegaPriceType TPPrice;	///止盈价
		MegaPriceType TPTriggerPrice;	///止盈触发价
		MegaBoolType RiskBefore;	///是否提前检查风控
		MegaTriggerOrderTypeType TriggerOrderType;	///触发的订单类型
		MegaIndexIDType TriggerDetail;	///触发类型明细
		MegaTriggerPriceTypeType TriggerPriceType;	///触发价类型
		MegaLongContentType TriggerValue;	///触发单具体设置信息
		MegaPriceType CloseSLPrice;	///平仓止损价
		MegaPriceType CloseSLTriggerPrice;	///平仓止损触发价
		MegaPriceType CloseTPPrice;	///平仓止盈价
		MegaPriceType CloseTPTriggerPrice;	///平仓止盈触发价
		MegaOrderPriceTypeType CloseOrderPriceType;	///报单价格类型
		MegaPriceType ClosePrice;	///平仓价
		MegaPriceType CloseTriggerPrice;	///平仓触发价
		MegaIndexIDType RelatedOrderID;	///关联报单号
		MegaMilliSecondsType ActiveTime;	///激活时间
		MegaMilliSecondsType TriggerTime;	///触发时间
		MegaLargeNumberType TimeSortNo;	///按时间排队的序号
		MegaTriggerStatusType TriggerStatus;	///触发报单状态
		MegaPosiDirectionType PosiDirection;	///持仓多空方向
		MegaNumberType FrontNo;	///前置编号
		MegaNumberType ErrorNo;	///错误代码
		MegaShortContentType ErrorMsg;	///错误信息
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaLargeNumberType BusinessNo;	///业务序列号
		MegaIndexIDType CreateTime;	///创建时间
	} MegaTriggerOrderStruct;
	
	///持仓订单组成
	typedef struct MegaPositionOrderStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType LocalID;	///报单本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaOrderPriceTypeType OrderPriceType;	///报单价格类型
		MegaDirectionType Direction;	///买卖方向
		MegaOffsetFlagType OffsetFlag;	///开平标志
		MegaPriceType Price;	///报单价格
		MegaVolumeType Volume;	///数量
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaValueModeType VolumeMode;	///数量取值方式
		MegaMoneyType Cost;	///委托额
		MegaOrderTypeType OrderType;	///订单类型
		MegaMilliSecondsType GTDTime;	///GTD时间
		MegaVolumeType MinVolume;	///最小成交量
		MegaBusinessTypeType BusinessType;	///业务类别
		MegaNameType BusinessValue;	///业务值
		MegaIndexIDType CloseOrderID;	///平仓指定开仓的订单号
		MegaBoolType IsCrossMargin;	///是否全仓
		MegaNameType Remark;	///备注
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaIndexIDType CopyOrderID;	///带单员报单号
		MegaLeverageType Leverage;	///委托单杠杆倍数
		MegaRatioType CopyProfitRate;	///带单分配比例
		MegaShortIndexIDType APPID;	///应用编号
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaPriceType TriggerPrice;	///触发价
		MegaMoneyType Reserve;	///保留资金
		MegaOrderStatusType OrderStatus;	///报单状态
		MegaDeriveSourceType DeriveSource;	///衍生来源
		MegaIndexIDType DeriveDetail;	///衍生明细
		MegaVolumeType VolumeTraded;	///成交数量
		MegaVolumeType VolumeRemain;	///剩余数量
		MegaVolumeType VolumeCancled;	///已经撤单数量
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaNumberType Priority;	///优先权
		MegaLargeNumberType TimeSortNo;	///按时间排队的序号
		MegaNumberType FrontNo;	///前置编号
		MegaCurrencyType PriceCurrency;	///计价币种
		MegaCurrencyType FeeCurrency;	///手续费币种
		MegaCurrencyType ClearCurrency;	///清算币种
		MegaMoneyType FrozenMoney;	///冻结资金
		MegaMoneyType FrozenFee;	///冻结手续费
		MegaMoneyType FrozenMargin;	///冻结保证金
		MegaMoneyType Fee;	///手续费
		MegaMoneyType CloseProfit;	///平仓盈亏
		MegaMoneyType Turnover;	///成交金额
		MegaIndexIDType RelatedOrderID;	///关联报单号
		MegaNameType BusinessResult;	///业务执行结果
		MegaLargeNumberType BusinessNo;	///业务序列号
		MegaBoolType Tradable;	///是否可以参加交易
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaPosiDirectionType PosiDirection;	///持仓多空方向
		MegaPriceType TradePrice;	///成交均价
		MegaPriceType OpenPrice;	///平仓成交时的开仓均价
		MegaIndexIDType TriggerOrderID;	///触发报单号
		MegaPriceType SLTriggerPrice;	///开仓报单成交之后的止损触发价
		MegaPriceType TPTriggerPrice;	///开仓报单成交之后的止盈触发价
		MegaMoneyType CopyProfit;	///带单盈利分配
		MegaVolumeType Position;	///成交时的持仓量
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaPriceType LastPriceByInsert;	///插入时的最新价
		MegaPriceType BidPrice1ByInsert;	///插入时的买一价
		MegaPriceType AskPrice1ByInsert;	///插入时的卖一价
		MegaMoneyType Available;	///可用资金
		MegaIndexIDType CreateTime;	///创建时间
		MegaVolumeType VolumeToClose;	///需要平仓的数量
		MegaVolumeType VolumeClosed;	///已经平仓的数量
		MegaVolumeType VolumeOnClose;	///正在平仓未成交的数量
	} MegaPositionOrderStruct;
	
	///完成的报单
	typedef struct MegaFinishOrderStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType LocalID;	///报单本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaOrderPriceTypeType OrderPriceType;	///报单价格类型
		MegaDirectionType Direction;	///买卖方向
		MegaOffsetFlagType OffsetFlag;	///开平标志
		MegaPriceType Price;	///报单价格
		MegaVolumeType Volume;	///数量
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaValueModeType VolumeMode;	///数量取值方式
		MegaMoneyType Cost;	///委托额
		MegaOrderTypeType OrderType;	///订单类型
		MegaMilliSecondsType GTDTime;	///GTD时间
		MegaVolumeType MinVolume;	///最小成交量
		MegaBusinessTypeType BusinessType;	///业务类别
		MegaNameType BusinessValue;	///业务值
		MegaIndexIDType CloseOrderID;	///平仓指定开仓的订单号
		MegaBoolType IsCrossMargin;	///是否全仓
		MegaNameType Remark;	///备注
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaIndexIDType CopyOrderID;	///带单员报单号
		MegaLeverageType Leverage;	///委托单杠杆倍数
		MegaRatioType CopyProfitRate;	///带单分配比例
		MegaShortIndexIDType APPID;	///应用编号
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaPriceType TriggerPrice;	///触发价
		MegaMoneyType Reserve;	///保留资金
		MegaOrderStatusType OrderStatus;	///报单状态
		MegaDeriveSourceType DeriveSource;	///衍生来源
		MegaIndexIDType DeriveDetail;	///衍生明细
		MegaVolumeType VolumeTraded;	///成交数量
		MegaVolumeType VolumeRemain;	///剩余数量
		MegaVolumeType VolumeCancled;	///已经撤单数量
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaNumberType Priority;	///优先权
		MegaLargeNumberType TimeSortNo;	///按时间排队的序号
		MegaNumberType FrontNo;	///前置编号
		MegaCurrencyType PriceCurrency;	///计价币种
		MegaCurrencyType FeeCurrency;	///手续费币种
		MegaCurrencyType ClearCurrency;	///清算币种
		MegaMoneyType FrozenMoney;	///冻结资金
		MegaMoneyType FrozenFee;	///冻结手续费
		MegaMoneyType FrozenMargin;	///冻结保证金
		MegaMoneyType Fee;	///手续费
		MegaMoneyType CloseProfit;	///平仓盈亏
		MegaMoneyType Turnover;	///成交金额
		MegaIndexIDType RelatedOrderID;	///关联报单号
		MegaNameType BusinessResult;	///业务执行结果
		MegaLargeNumberType BusinessNo;	///业务序列号
		MegaBoolType Tradable;	///是否可以参加交易
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaPosiDirectionType PosiDirection;	///持仓多空方向
		MegaPriceType TradePrice;	///成交均价
		MegaPriceType OpenPrice;	///平仓成交时的开仓均价
		MegaIndexIDType TriggerOrderID;	///触发报单号
		MegaPriceType SLTriggerPrice;	///开仓报单成交之后的止损触发价
		MegaPriceType TPTriggerPrice;	///开仓报单成交之后的止盈触发价
		MegaMoneyType CopyProfit;	///带单盈利分配
		MegaVolumeType Position;	///成交时的持仓量
		MegaLongIndexIDType UserID;	///交易用户代码
		MegaPriceType LastPriceByInsert;	///插入时的最新价
		MegaPriceType BidPrice1ByInsert;	///插入时的买一价
		MegaPriceType AskPrice1ByInsert;	///插入时的卖一价
		MegaMoneyType Available;	///可用资金
		MegaIndexIDType CreateTime;	///创建时间
	} MegaFinishOrderStruct;
	
	///成交
	typedef struct MegaTradeStruct
	{
		MegaIndexIDType TradeID;	///成交代码
		MegaDirectionType Direction;	///买卖方向
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaLongIndexIDType MemberID;	///成员代码
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaAccountIDType AccountID;	///资金账号
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaOffsetFlagType OffsetFlag;	///开平标志
		MegaPriceType Price;	///成交价格
		MegaVolumeType Volume;	///成交数量
		MegaDeriveSourceType DeriveSource;	///衍生类型
		MegaMatchRoleType MatchRole;	///成交角色
		MegaCurrencyType PriceCurrency;	///计价币种
		MegaCurrencyType ClearCurrency;	///清算币种
		MegaMoneyType Fee;	///手续费
		MegaCurrencyType FeeCurrency;	///手续费币种
		MegaMoneyType CloseProfit;	///平仓盈亏
		MegaMoneyType Turnover;	///成交金额
		MegaMoneyType UseMargin;	///占用或者减少的保证金
		MegaLeverageType Leverage;	///杠杆倍数
		MegaPriceType OrderPrice;	///委托价格
		MegaPriceType TriggerPrice;	///触发价
		MegaBoolType IsSelfTrade;	///是否自成交
		MegaNameType Remark;	///备注
		MegaLargeNumberType BusinessNo;	///业务序列号
		MegaPriceType OpenPrice;	///开仓均价
		MegaShortIndexIDType APPID;	///应用编号
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaIndexIDType CreateTime;	///创建时间
		MegaIndexIDType CopyOrderID;	///带单员报单号
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaMoneyType CopyProfit;	///带单盈利分配
		MegaVolumeType Position;	///成交时的持仓量
		MegaMoneyType ReserveProfit;	///保留资金盈亏
		MegaMoneyType ReserveFee;	///保留资金手续费
		MegaNameType TradeRemark;	///Trade备注
		MegaBusinessTypeType BusinessType;	///业务类别
		MegaNameType BusinessValue;	///业务值
	} MegaTradeStruct;
	
	///持仓历史
	typedef struct MegaFinishPositionStruct
	{
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaPositionClassType PositionClass;	///持仓类型
		MegaPosiDirectionType PosiDirection;	///持仓多空方向
		MegaVolumeType PrePosition;	///上次持仓
		MegaVolumeType Position;	///总持仓
		MegaVolumeType LongFrozen;	///多头冻结
		MegaVolumeType ShortFrozen;	///空头冻结
		MegaVolumeType PreLongFrozen;	///昨日多头冻结
		MegaVolumeType PreShortFrozen;	///昨日空头冻结
		MegaVolumeType HighestPosition;	///最大持仓
		MegaVolumeType ClosePosition;	///可平持仓
		MegaCostType PositionCost;	///持仓成本
		MegaPriceType CostPrice;	///成本价
		MegaMoneyType UseMargin;	///占用保证金
		MegaMoneyType FrozenMargin;	///冻结保证金
		MegaMoneyType LongFrozenMargin;	///多头冻结保证金
		MegaMoneyType ShortFrozenMargin;	///空头冻结保证金
		MegaMoneyType CloseProfit;	///平仓盈亏
		MegaTotalType TotalPositionCost;	///开仓成本
		MegaTotalType TotalCloseProfit;	///总平仓盈亏
		MegaPriceType OpenPrice;	///开仓均价
		MegaPriceType ClosePrice;	///平仓均价
		MegaMoneyType TradeFee;	///交易费用
		MegaMoneyType PositionFee;	///持仓费用或者股票分红等
		MegaLeverageType Leverage;	///杠杆倍数
		MegaAccountIDType AccountID;	///资金账号
		MegaCurrencyType PriceCurrency;	///计价币种
		MegaCurrencyType ClearCurrency;	///清算币种
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaBoolType IsCrossMargin;	///是否全仓
		MegaIndexIDType CloseOrderID;	///平仓单代码
		MegaPriceType SLTriggerPrice;	///止损触发价
		MegaPriceType TPTriggerPrice;	///止盈触发价
		MegaMilliSecondsType BeginTime;	///持仓不为0的开始时间
		MegaMilliSecondsType InsertTime;	///插入时间(新持仓时间)
		MegaMilliSecondsType LastOpenTime;	///最后开仓时间
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaLargeNumberType BusinessNo;	///业务序列号
		MegaBoolType IsAutoAddMargin;	///是否自动追加保证金
		MegaNumberType Frequency;	///每秒更新的次数
		MegaMoneyType MaintMargin;	///维持保证金
		MegaMoneyType UnRealProfit;	///未实现盈亏
		MegaPriceType LiquidPrice;	///清算价格
		MegaIndexIDType CreateTime;	///创建时间
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaRatioType CopyProfitRate;	///带单分配比例
		MegaMoneyType CopyProfit;	///带单盈利分配
		MegaLongIndexIDType FirstTradeID;	///初始成交订单号
		MegaLongIndexIDType LastTradeID;	///结束成交订单号
		MegaBusinessTypeType BusinessType;	///业务类别
		MegaNameType BusinessValue;	///业务值
		MegaMoneyType Reserve;	///保留资金
		MegaMoneyType ReserveProfit;	///保留资金盈亏
		MegaNameType Remark;	///备注
	} MegaFinishPositionStruct;
	
	///账户流水
	typedef struct MegaAccountDetailStruct
	{
		MegaIndexIDType AccountDetailID;	///资金明细号
		MegaLongIndexIDType MemberID;	///成员代码
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaAccountIDType AccountID;	///资金账号
		MegaCurrencyType Currency;	///币种
		MegaMoneyType Amount;	///发生额
		MegaMoneyType PreBalance;	///上次静态权益
		MegaMoneyType Balance;	///静态权益
		MegaMoneyType ReserveAmount;	///体验金发生额
		MegaMoneyType ReserveBalance;	///体验金静态权益
		MegaDefineTypeType Source;	///财务流水类型
		MegaNameType Remark;	///备注
		MegaIndexIDType LocalID;	///本地标识
		MegaIndexIDType SettleSegment;	///结算段
		MegaLargeNumberType BusinessNo;	///业务序列号
		MegaLongNameType RelatedID;	///内外对账ID
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaIndexIDType CreateTime;	///创建时间
	} MegaAccountDetailStruct;
	
	///历史K线
	typedef struct MegaKLineStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaBarType Bar;	///K线周期代码(n[m/h/d/o])
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaPriceType OpenPrice;	///开盘价
		MegaPriceType HighestPrice;	///最高价
		MegaPriceType LowestPrice;	///最低价
		MegaPriceType ClosePrice;	///收盘价
		MegaCollectType Volume;	///数量
		MegaCollectType Turnover;	///成交金额
	} MegaKLineStruct;
	
	///操作流水
	typedef struct MegaSettleDetailStruct
	{
		MegaIndexIDType SettleDetailID;	///操作明细号
		MegaShortIndexIDType APPID;	///应用编号
		MegaIndexIDType LocalID;	///成交对本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaShortIndexIDType SettlementGroup;	///结算组编号
		MegaSettleActionTypeType SettleAction;	///结算操作
		MegaRatioType Value;	///操作使用值
		MegaRatioType Value1;	///发生值1
		MegaRatioType Value2;	///发生值2
		MegaRatioType Value3;	///发生值3
		MegaNameType Remark;	///备注
		MegaShortIndexIDType FundingRateGroup;	///资金费用组
		MegaIndexIDType SettleSegment;	///结算段
		MegaIndexIDType InsertTime;	///创建时间
		MegaLargeNumberType BusinessNo;	///业务序列号
	} MegaSettleDetailStruct;
	
	///完成的条件报单
	typedef struct MegaFinishTriggerOrderStruct
	{
		MegaLongIndexIDType MemberID;	///成员代码
		MegaIndexIDType TradeUnitID;	///交易单元代码
		MegaAccountIDType AccountID;	///资金账号
		MegaIndexIDType LocalID;	///报单本地标识
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaOrderPriceTypeType OrderPriceType;	///报单价格类型
		MegaDirectionType Direction;	///买卖方向
		MegaOffsetFlagType OffsetFlag;	///开平标志
		MegaPriceType Price;	///报单价格
		MegaVolumeType Volume;	///数量
		MegaVolumeType VolumeDisplay;	///显示数量
		MegaValueModeType VolumeMode;	///数量取值方式
		MegaMoneyType Cost;	///委托额
		MegaOrderTypeType OrderType;	///订单类型
		MegaMilliSecondsType GTDTime;	///GTD时间
		MegaVolumeType MinVolume;	///最小成交量
		MegaBusinessTypeType BusinessType;	///业务类别
		MegaNameType BusinessValue;	///业务值
		MegaIndexIDType CloseOrderID;	///平仓指定开仓的订单号
		MegaBoolType IsCrossMargin;	///是否全仓
		MegaNameType Remark;	///备注
		MegaIndexIDType OrderID;	///报单系统唯一代码
		MegaLongIndexIDType CopyMemberID;	///带单员代码
		MegaIndexIDType CopyOrderID;	///带单员报单号
		MegaLeverageType Leverage;	///委托单杠杆倍数
		MegaRatioType CopyProfitRate;	///带单分配比例
		MegaShortIndexIDType APPID;	///应用编号
		MegaLongIndexIDType PositionID;	///持仓代码
		MegaPriceType TriggerPrice;	///触发价
		MegaMoneyType Reserve;	///保留资金
		MegaPriceType SLPrice;	///止损价
		MegaPriceType SLTriggerPrice;	///止损触发价
		MegaPriceType TPPrice;	///止盈价
		MegaPriceType TPTriggerPrice;	///止盈触发价
		MegaBoolType RiskBefore;	///是否提前检查风控
		MegaTriggerOrderTypeType TriggerOrderType;	///触发的订单类型
		MegaIndexIDType TriggerDetail;	///触发类型明细
		MegaTriggerPriceTypeType TriggerPriceType;	///触发价类型
		MegaLongContentType TriggerValue;	///触发单具体设置信息
		MegaPriceType CloseSLPrice;	///平仓止损价
		MegaPriceType CloseSLTriggerPrice;	///平仓止损触发价
		MegaPriceType CloseTPPrice;	///平仓止盈价
		MegaPriceType CloseTPTriggerPrice;	///平仓止盈触发价
		MegaOrderPriceTypeType CloseOrderPriceType;	///报单价格类型
		MegaPriceType ClosePrice;	///平仓价
		MegaPriceType CloseTriggerPrice;	///平仓触发价
		MegaIndexIDType RelatedOrderID;	///关联报单号
		MegaMilliSecondsType ActiveTime;	///激活时间
		MegaMilliSecondsType TriggerTime;	///触发时间
		MegaLargeNumberType TimeSortNo;	///按时间排队的序号
		MegaTriggerStatusType TriggerStatus;	///触发报单状态
		MegaPosiDirectionType PosiDirection;	///持仓多空方向
		MegaNumberType FrontNo;	///前置编号
		MegaNumberType ErrorNo;	///错误代码
		MegaShortContentType ErrorMsg;	///错误信息
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaMilliSecondsType UpdateTime;	///更新时间
		MegaLargeNumberType BusinessNo;	///业务序列号
		MegaIndexIDType CreateTime;	///创建时间
	} MegaFinishTriggerOrderStruct;
	
	///订单簿变化
	typedef struct MegaMarketOrderStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaDirectionType Direction;	///买卖方向
		MegaPriceType Price;	///价格
		MegaVolumeType Volume;	///数量
		MegaNumberType Orders;	///订单数量
		MegaLargeNumberType BusinessNo;	///最后变化序列号
		MegaVolumeType TrueVolume;	///真实报单数量
	} MegaMarketOrderStruct;
	
	///发布K线
	typedef struct MegaPublishKLineStruct
	{
		MegaExchangeIDType ExchangeID;	///交易所代码
		MegaInstrumentIDType InstrumentID;	///标的代码
		MegaBarType Bar;	///K线周期代码(n[m/h/d/o])
		MegaMilliSecondsType InsertTime;	///插入时间
		MegaPriceType OpenPrice;	///开盘价
		MegaPriceType HighestPrice;	///最高价
		MegaPriceType LowestPrice;	///最低价
		MegaPriceType ClosePrice;	///收盘价
		MegaCollectType Volume;	///数量
		MegaCollectType Turnover;	///成交金额
	} MegaPublishKLineStruct;
}

#ifdef __cplusplus
}
#endif

#endif
