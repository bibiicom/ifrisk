
/******************************
-- create ServiceConfig
--******************************/
drop table if exists perpetual.t_ServiceConfig;
create table perpetual.t_ServiceConfig
(
	ConfigName char(32) not null default '' COMMENT '配置名称'
	,Index1 char(32) not null default "default" COMMENT '索引1'
	,Index2 char(32) not null default "default" COMMENT '索引2'
	,ConfigValue varchar(64) not null default '' COMMENT '配置值'
	,Remark varchar(256) default null COMMENT '备注'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (ConfigName,Index1,Index2)
)ENGINE=InnoDB COMMENT='业务配置';

/******************************
-- create Currency
--******************************/
drop table if exists perpetual.t_Currency;
create table perpetual.t_Currency
(
	SettlementGroup char(8) not null default '' COMMENT '结算组编号'
	,Currency char(12) not null default '' COMMENT '币种'
	,Deposit numeric(38,10) not null default 0.0 COMMENT '入金金额'
	,Withdraw numeric(38,10) not null default 0.0 COMMENT '出金金额'
	,Balance numeric(38,10) not null default 0.0 COMMENT '静态权益'
	,PreBalance numeric(38,10) not null default 0.0 COMMENT '上次静态权益'
	,SortNumber numeric(10) default null COMMENT '排序字段'
	,CanReduce numeric(1) not null default 1 COMMENT '是否可以出金'
	,IsTrading numeric(1) not null default 1 COMMENT '是否允许相关交易'
	,SettleSegment char(20) default null COMMENT '结算段'
	,MoneyTick numeric(38,10) not null default 0.00000001 COMMENT '最小单位'
	,ExchangeID char(8) not null default '' COMMENT '价格参考交易所代码'
	,InstrumentID char(26) not null default '' COMMENT '价格参考标的代码'
	,LoanUse numeric(38,10) not null default 0.0 COMMENT '借款使用量'
	,LoanLimit numeric(26,10) not null default 0.8 COMMENT '最大可借比例'
	,LoanBorrow numeric(26,10) not null default 0.0 COMMENT '借款利息'
	,LoanDeposit numeric(26,10) not null default 0.0 COMMENT '存款利息'
	,LoanBase numeric(26,10) not null default 0.01 COMMENT '利息起点'
	,LoanRate numeric(26,10) not null default 0.1 COMMENT '利息斜率'
	,LoanHighBase numeric(26,10) not null default 0.9 COMMENT '高利息起点'
	,LoanHighRate numeric(26,10) not null default 1 COMMENT '高利息斜率'
	,Remark varchar(64) default null COMMENT '备注'
	,CurrValue numeric(38,10) not null default 0.0 COMMENT '当前价值'
	,HairCut numeric(26,10) not null default 0.9 COMMENT '抵押比例'
	,MaxValue numeric(38,10) not null default 10000 COMMENT '最大抵押数额'
	,MarginRateGroup char(8) default null COMMENT '保证金组'
	,PrivateEnd numeric(26,10) default null COMMENT '私有化停止交易线'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (Currency,SettlementGroup)
)ENGINE=InnoDB COMMENT='币种信息';

/******************************
-- create MarketData
--******************************/
drop table if exists perpetual.t_MarketData;
create table perpetual.t_MarketData
(
	ExchangeID char(8) not null default '' COMMENT '交易所代码'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,ProductGroup char(8) default null COMMENT '产品组'
	,UpdateTime numeric(20) not null default 0.0 COMMENT '更新时间'
	,PreClosePrice numeric(28,15) default null COMMENT '上次收盘'
	,PreSettlementPrice numeric(28,15) default null COMMENT '上次结算价'
	,PreOpenInterest numeric(38,10) not null default 0.0 COMMENT '上次持仓量'
	,PrePositionFeeRate numeric(26,10) default null COMMENT '上次持仓费按比例'
	,ClosePrice numeric(28,15) default null COMMENT '收盘价'
	,PositionFeeTime numeric(20) default null COMMENT '收取持仓费用时间'
	,SettlementTime numeric(20) default null COMMENT '结算时间'
	,UpperLimitPrice numeric(28,15) default null COMMENT '最高限价'
	,LowerLimitPrice numeric(28,15) default null COMMENT '最低限价'
	,UnderlyingPrice numeric(28,15) default null COMMENT '基础标的价格'
	,MarkedPrice numeric(28,15) default null COMMENT '标记价格'
	,PositionFeeRate numeric(26,10) default null COMMENT '预计持仓费按比例'
	,HighestPrice numeric(28,15) default null COMMENT '当日最高价'
	,LowestPrice numeric(28,15) default null COMMENT '当日最低价'
	,LastPrice numeric(28,15) default null COMMENT '最新价'
	,Volume numeric(36,8) not null default 0.0 COMMENT '当日数量'
	,Turnover numeric(36,8) not null default 0.0 COMMENT '当日成交金额'
	,OpenInterest numeric(38,10) not null default 0.0 COMMENT '持仓量'
	,SettlementPrice numeric(28,15) default null COMMENT '预估结算价'
	,OpenPrice numeric(28,15) default null COMMENT '当日开盘价'
	,IsPubMarketData numeric(1) not null default 0 COMMENT '行情发布状态'
	,InstrumentStatus char(1) not null default 0 COMMENT '标的交易状态(0:开盘前;1:非交易;2:连续交易;3:集合竞价报单;4:集合竞价价格平衡;5:集合竞价撮合;6:收盘;7:不活跃;)'
	,Remark varchar(64) default null COMMENT '备注'
	,BusinessNo numeric(16) default null COMMENT '业务序列号'
	,MarkedPriceTotal numeric(36,6) default null COMMENT '标记价格汇总'
	,MarkedPriceWeight numeric(36,6) default null COMMENT '标记价格权重'
	,L25UpperPrice numeric(28,15) default null COMMENT '25档行情最高价'
	,L25LowerPrice numeric(28,15) default null COMMENT '25档行情最低价'
	,L25UpperPrice1 numeric(28,15) default null COMMENT '25档行情最高价1'
	,L25LowerPrice1 numeric(28,15) default null COMMENT '25档行情最低价1'
	,L25UpperPrice2 numeric(28,15) default null COMMENT '25档行情最高价2'
	,L25LowerPrice2 numeric(28,15) default null COMMENT '25档行情最低价2'
	,BidPrice1 numeric(28,15) default null COMMENT '申买价一'
	,BidVolume1 numeric(38,10) default null COMMENT '申买量一'
	,AskPrice1 numeric(28,15) default null COMMENT '申卖价一'
	,AskVolume1 numeric(38,10) default null COMMENT '申卖量一'
	,TheoryPrice numeric(28,15) default null COMMENT '理论价'
	,HighestPrice24 numeric(28,15) default null COMMENT '24小时最高价'
	,LowestPrice24 numeric(28,15) default null COMMENT '24小时最低价'
	,Volume24 numeric(36,8) default null COMMENT '24小时数量'
	,Turnover24 numeric(36,8) default null COMMENT '24小时成交金额'
	,OpenPrice24 numeric(28,15) default null COMMENT '24小时开盘价'
	,Frequency numeric(10) default null COMMENT '每秒更新的次数'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (ExchangeID,InstrumentID)
)ENGINE=InnoDB COMMENT='市场行情';

/******************************
-- create Instrument
--******************************/
drop table if exists perpetual.t_Instrument;
create table perpetual.t_Instrument
(
	ExchangeID char(8) not null default '' COMMENT '交易所代码'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,InstrumentName varchar(64) default null COMMENT '标的名称'
	,SettlementGroup char(8) not null default '' COMMENT '结算组编号'
	,ProductGroup char(8) default null COMMENT '产品组'
	,UnderlyingID char(26) default null COMMENT '基础标的代码'
	,ProductClass char(1) not null default 1 COMMENT '产品大类(1:保证金交易;2:权利金交易;3:资产转让（股票/基金/国债等）;4:兑换交易（资金之间的互换）;5:组合;6:行情;)'
	,ProductType char(1) not null default 1 COMMENT '产品类型(1:交割期货;2:永续、利率互换期货;3:即期/日交割期货;4:递延交割期货;5:权证;6:美式期权;7:欧式期权;8:币币交易;9:外汇兑换;a:股票;b:债券;c:基金;)'
	,StrikePrice numeric(28,15) default null COMMENT '执行价'
	,OptionsType char(1) default null COMMENT '期权类型(0:非期权;1:看涨;2:看跌;)'
	,VolumeMultiple numeric(26,10) not null default 0.0 COMMENT '数量乘数'
	,StartTime numeric(20) default null COMMENT '启动时间'
	,ExpireTime numeric(20) default null COMMENT '到期时间'
	,PriceCurrency char(12) not null default '' COMMENT '计价币种'
	,ClearCurrency char(12) not null default '' COMMENT '清算币种'
	,IsInverse numeric(1) not null default 0 COMMENT '是否反向标的'
	,BaseCurrency char(12) not null default '' COMMENT '标的基础货币'
	,MarginPriceType char(1) not null default 0 COMMENT '保证金价格类型(0:不按比例按固定值,无需价格;1:昨结算价;2:开仓价;3:标记价;4:最新价;)'
	,BasisPrice numeric(28,15) default null COMMENT '挂牌基准价'
	,MinOrderVolume numeric(38,10) default null COMMENT '最小下单量'
	,MinOrderCost numeric(38,10) default null COMMENT '最小下单额'
	,MaxOrderVolume numeric(38,10) default null COMMENT '最大下单量'
	,PriceTick numeric(28,15) not null default 0.00000001 COMMENT '最小变动价位'
	,VolumeTick numeric(38,10) not null default 0.00000001 COMMENT '最小变动量'
	,ShowVolumeTick numeric(38,10) default null COMMENT '前端行情显示最小变动量(服务端不用)'
	,TradingModel char(1) not null default 0 COMMENT '交易模式(0:价格优先时间优先;1:价格优先按比例分配;2:只能和做市商报价成交;3:没有撮合;)'
	,DayStartTime varchar(128) default null COMMENT '每日开始交易时间'
	,SortNumber numeric(10) default null COMMENT '排序字段'
	,Remark varchar(256) default null COMMENT '备注'
	,DefaultLeverage numeric(20,8) default null COMMENT '默认杠杆倍数'
	,PriceLimitValueMode char(1) default null COMMENT '取值方式(0:百分比;1:绝对值;)'
	,PriceLimitPrice char(1) default null COMMENT '限价价格类型(1:昨结算价;2:最新价;3:基础标的价;4:标记价;)'
	,PriceLimitUpperValue numeric(28,15) default null COMMENT '上限'
	,PriceLimitLowerValue numeric(28,15) default null COMMENT '下限'
	,MarginRateGroup char(8) default null COMMENT '保证金组'
	,CFDGroup char(8) default null COMMENT '差价组'
	,FeeGroup char(8) default null COMMENT '手续费组'
	,TradingRightGroup char(8) default null COMMENT '权限组'
	,MaxOpenInterest numeric(38,10) default null COMMENT '最大持仓量'
	,FundingRateGroup char(8) default null COMMENT '资金费用组'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,MarkedPriceTimes numeric(10) default null COMMENT '标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价)'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (ExchangeID,InstrumentID)
	,KEY mykey_0 (ExchangeID,ProductGroup)
)ENGINE=InnoDB COMMENT='交易标的';

/******************************
-- create CurrentTime
--******************************/
drop table if exists perpetual.t_CurrentTime;
create table perpetual.t_CurrentTime
(
	SystemID char(8) not null default '' COMMENT '系统名'
	,ZoneDate char(8) not null default '' COMMENT '本地日期'
	,ZoneTime char(8) not null default '' COMMENT '本地时间'
	,ZoneDays char(8) not null default '' COMMENT '本地天数'
	,CurrTime numeric(20) not null default 0.0 COMMENT '当前时间（秒）'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间（毫秒）'
	,TimeZone numeric(10) not null default 0 COMMENT '本地时区'
	,LastTradeID char(20) default null COMMENT '最新最新成交序列号'
	,LastOrderID char(20) default null COMMENT '最新订单序列号'
	,LastAccountDetailID char(20) default null COMMENT '最新资金明细序列号'
	,LastTimeSortNo numeric(16) default null COMMENT '最新排序序列号'
	,LastBusinessNo numeric(16) default null COMMENT '最新事务序列号'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (SystemID)
)ENGINE=InnoDB COMMENT='当前时间';

/******************************
-- create Member
--******************************/
drop table if exists perpetual.t_Member;
create table perpetual.t_Member
(
	MemberID char(42) not null default '' COMMENT '成员代码'
	,MemberType char(1) default null COMMENT '成员类型(a:普通;b:内部做市商;c:系统手续费使用;d:风控使用;e:运营使用;f:运营保留资金使用;g:外部做市商;)'
	,Grade char(8) default null COMMENT '等级'
	,Priority numeric(10) default null COMMENT '优先权'
	,FeeGrade char(8) default null COMMENT '手续费等级'
	,MarginRateGrade char(8) default null COMMENT '保证金等级'
	,TradingRightGrade char(8) default null COMMENT '权限等级'
	,CFDGrade char(8) default null COMMENT '差价等级'
	,LiquidityGrade char(8) default null COMMENT '流动性等级'
	,RiskLevel char(1) not null default 0 COMMENT '风控类型(0:本币种资金权益;1:结算组所有资金权益;2:资金账号所有资金权益;3:会员账号所有资金权益;)'
	,PositionType char(1) default null COMMENT '持仓类型(0:不设置;1:综合持仓(双边持仓);2:净持仓(单边持仓);)'
	,TriggerOrders numeric(10) default null COMMENT '触发订单个数'
	,OpenOrders numeric(10) default null COMMENT '未成交订单数'
	,OpenPositions numeric(10) default null COMMENT '用户持仓个数'
	,MaxLocalID char(20) default null COMMENT '最大本地标识'
	,SinkType char(1) default null COMMENT '下游系统同步类型(0:同步到数据库;1:同步到Redis不到数据库;2:不同步;)'
	,CopyMemberID char(42) default null COMMENT '带单员代码'
	,Region char(8) default null COMMENT ''
	,FeeDeduceDisCount numeric(26,10) default null COMMENT '作为手续费的抵扣优惠比例'
	,FeeDeduceCurrency char(12) default null COMMENT '作为手续费的抵扣比例'
	,CanBorrow numeric(1) default null COMMENT '是否需要借钱交易'
	,Remark char(32) default null COMMENT '备注'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (MemberID)
)ENGINE=InnoDB COMMENT='成员';

/******************************
-- create UserSession
--******************************/
drop table if exists perpetual.t_UserSession;
create table perpetual.t_UserSession
(
	UserID char(42) default null COMMENT '交易用户代码'
	,Password char(32) default null COMMENT '密码'
	,UserProductID char(32) default null COMMENT '用户端产品信息'
	,MacAddress char(20) default null COMMENT 'Mac地址'
	,ClientIPAddress char(15) default null COMMENT '终端IP地址'
	,HDSerialID varchar(64) default null COMMENT '硬盘序列号'
	,AuthCode varchar(128) default null COMMENT '授权编码'
	,APPID char(8) default null COMMENT '应用编号'
	,Token varchar(128) not null default '' COMMENT '令牌'
	,Remark varchar(64) default null COMMENT '备注'
	,APIID char(32) default null COMMENT '接口端产品信息'
	,LoginTime numeric(20) default null COMMENT '登录时间'
	,ExpireTime numeric(20) default null COMMENT '失效时间'
	,IPAddress varchar(256) default null COMMENT '允许登录的IP地址'
	,MaxLocalID char(20) default null COMMENT '最大本地标识'
	,SessionNo numeric(10) not null default 0 COMMENT '会话编号'
	,FrontNo numeric(10) not null default 0 COMMENT '前置编号'
	,AccessLimit numeric(10) default null COMMENT '每秒访问的次数限制'
	,UserType char(1) not null default 1 COMMENT '用户类型(1:普通用户(订阅自己/可下自己订单);2:观察用户OB(订阅其他用户/不可下单);5:超级管理员(订阅所有/可给所有用户下单/可爆仓);)'
	,MemberID char(42) default null COMMENT '成员代码'
	,LimitAccesses numeric(10) default null COMMENT '该秒已经访问的次数'
	,SinkType char(1) default null COMMENT '下游系统同步类型(0:同步到数据库;1:同步到Redis不到数据库;2:不同步;)'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (Token)
)ENGINE=InnoDB COMMENT='用户会话';

/******************************
-- create LastKLine
--******************************/
drop table if exists perpetual.t_LastKLine;
create table perpetual.t_LastKLine
(
	ExchangeID char(8) not null default '' COMMENT '交易所代码'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,Bar char(3) not null default '' COMMENT 'K线周期代码(n[m/h/d/o])'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,OpenPrice numeric(28,15) default null COMMENT '开盘价'
	,HighestPrice numeric(28,15) default null COMMENT '最高价'
	,LowestPrice numeric(28,15) default null COMMENT '最低价'
	,ClosePrice numeric(28,15) default null COMMENT '收盘价'
	,Volume numeric(36,8) not null default 0.0 COMMENT '数量'
	,Turnover numeric(36,8) not null default 0.0 COMMENT '成交金额'
	,UpdateTime numeric(20) not null default 0.0 COMMENT '更新时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (ExchangeID,InstrumentID,Bar)
)ENGINE=InnoDB COMMENT='最新K线';

/******************************
-- create CFDRate
--******************************/
drop table if exists perpetual.t_CFDRate;
create table perpetual.t_CFDRate
(
	MemberID char(42) not null default '' COMMENT '成员代码或组'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,TriggerOrderType char(42) not null default "default" COMMENT '报单价格类型'
	,RateBegin numeric(26,10) not null default 0.0 COMMENT '多头起始溢价率'
	,RateEnd numeric(26,10) not null default 0.0 COMMENT '多头最大溢价率'
	,CompVolume numeric(26,10) not null default 0.0 COMMENT '点差比较基准'
	,CompRate numeric(26,10) default null COMMENT '点差比较基准率'
	,HardRate numeric(26,10) default null COMMENT '硬顶率'
	,Remark varchar(64) default null COMMENT '备注'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (MemberID,InstrumentID,TriggerOrderType)
)ENGINE=InnoDB COMMENT='CFD溢价率';

/******************************
-- create TradingRight
--******************************/
drop table if exists perpetual.t_TradingRight;
create table perpetual.t_TradingRight
(
	MemberID char(42) not null default '' COMMENT '成员代码'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,Direction char(1) not null default 0 COMMENT '买卖方向(0:买;1:卖;)'
	,TradingRight char(1) not null default 0 COMMENT '交易权限(0:可以交易;1:只能平仓;2:不能交易;)'
	,Remark char(32) default null COMMENT '备注'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (MemberID,InstrumentID,Direction)
)ENGINE=InnoDB COMMENT='交易权限';

/******************************
-- create MarginRate
--******************************/
drop table if exists perpetual.t_MarginRate;
create table perpetual.t_MarginRate
(
	MemberID char(42) not null default '' COMMENT '成员代码'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,ValueMode char(1) not null default 0 COMMENT '取值方式(0:百分比;1:绝对值;)'
	,MarginSequenceType char(1) default null COMMENT '保证金数列(1:等差数列;2:等比数列;)'
	,RiskSequenceType char(1) default null COMMENT '风险限额数列(1:等差数列;2:等比数列;)'
	,LongMarginRatio numeric(26,10) not null default 0.0 COMMENT '多头起始保证金率'
	,LongMaintMarginRatio numeric(26,10) not null default 0.0 COMMENT '多头起始维持保证金率'
	,LongMarginStep numeric(26,10) not null default 0.0 COMMENT '多头保证金率步长'
	,LongMaintMarginStep numeric(26,10) not null default 0.0 COMMENT '多头维持保证金率步长'
	,LongRiskLimit numeric(38,10) not null default 0.0 COMMENT '多头起始风险限额'
	,LongRiskStep numeric(38,10) not null default 0.0 COMMENT '多头风险限额步长'
	,ShortMarginRatio numeric(26,10) not null default 0.0 COMMENT '空头起始保证金率'
	,ShortMaintMarginRatio numeric(26,10) not null default 0.0 COMMENT '空头起始维持保证金率'
	,ShortMarginStep numeric(26,10) not null default 0.0 COMMENT '空头保证金率步长'
	,ShortMaintMarginStep numeric(26,10) not null default 0.0 COMMENT '空头维持保证金率步长'
	,ShortRiskLimit numeric(38,10) not null default 0.0 COMMENT '空头起始风险限额'
	,ShortRiskStep numeric(38,10) not null default 0.0 COMMENT '空头风险限额步长'
	,DefaultLeverage numeric(20,8) default null COMMENT '默认杠杆倍数'
	,Remark varchar(64) default null COMMENT '备注'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (MemberID,InstrumentID)
)ENGINE=InnoDB COMMENT='保证金率';

/******************************
-- create Fee
--******************************/
drop table if exists perpetual.t_Fee;
create table perpetual.t_Fee
(
	MemberID char(42) not null default '' COMMENT '成员代码'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,MatchRole char(1) not null default 0 COMMENT '成交角色(0:不区分;1:主动成交方;2:被动成交方;3:集合竞价;4:交割;5:场外交易;)'
	,OpenFeeRate numeric(26,10) not null default 0.0 COMMENT '开仓手续费按比例'
	,OpenFeeAmount numeric(26,10) not null default 0.0 COMMENT '开仓手续费按手数'
	,CloseFeeRate numeric(26,10) not null default 0.0 COMMENT '平仓手续费按比例'
	,CloseFeeAmount numeric(26,10) not null default 0.0 COMMENT '平仓手续费按手数'
	,CloseTodayFeeRate numeric(26,10) not null default 0.0 COMMENT '平今仓手续费按比例'
	,CloseTodayFeeAmount numeric(26,10) not null default 0.0 COMMENT '平今仓手续费按手数'
	,Remark varchar(64) default null COMMENT '备注'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (MemberID,InstrumentID,MatchRole)
)ENGINE=InnoDB COMMENT='手续费率';

/******************************
-- create Liquidity
--******************************/
drop table if exists perpetual.t_Liquidity;
create table perpetual.t_Liquidity
(
	LiquidityGrade char(8) not null default '' COMMENT '流动性等级'
	,LiquidityGroup char(42) not null default '' COMMENT '标的代码'
	,BasePriceType char(1) default null COMMENT '基于的价格(0:买一和买一中间值;1:标记价;2:最新价;)'
	,VolumeBegin numeric(38,10) default null COMMENT '起始量'
	,BuyOpen numeric(26,10) default null COMMENT '买开流动性比例'
	,BuyClose numeric(26,10) default null COMMENT '买平流动性比例'
	,SellOpen numeric(26,10) default null COMMENT '卖开流动性比例'
	,SellClose numeric(26,10) default null COMMENT '卖平流动性比例'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (LiquidityGrade,LiquidityGroup)
)ENGINE=InnoDB COMMENT='流动性设置表';

/******************************
-- create Account
--******************************/
drop table if exists perpetual.t_Account;
create table perpetual.t_Account
(
	SettlementGroup char(8) not null default '' COMMENT '结算组编号'
	,AccountID char(44) not null default '' COMMENT '资金账号'
	,Currency char(12) not null default '' COMMENT '币种'
	,MemberID char(42) not null default '' COMMENT '成员代码'
	,Available numeric(38,10) not null default 0.0 COMMENT '可用资金'
	,Withdrawable numeric(38,10) not null default 0.0 COMMENT '可取资金'
	,PreBalance numeric(38,10) not null default 0.0 COMMENT '上次静态权益'
	,Balance numeric(38,10) not null default 0.0 COMMENT '静态权益'
	,PositionCost numeric(38,10) not null default 0.0 COMMENT '持仓成本'
	,UseMargin numeric(38,10) not null default 0.0 COMMENT '所有占用保证金'
	,CrossMargin numeric(38,10) not null default 0.0 COMMENT '全仓保证金'
	,CloseProfit numeric(38,10) not null default 0.0 COMMENT '平仓盈亏'
	,Deposit numeric(38,10) not null default 0.0 COMMENT '入金金额'
	,Withdraw numeric(38,10) not null default 0.0 COMMENT '出金金额'
	,FrozenMargin numeric(38,10) not null default 0.0 COMMENT '冻结保证金'
	,FrozenMoney numeric(38,10) not null default 0.0 COMMENT '冻结资金'
	,FrozenFee numeric(38,10) not null default 0.0 COMMENT '冻结手续费'
	,Fee numeric(38,10) not null default 0.0 COMMENT '手续费'
	,MoneyChange numeric(38,10) not null default 0.0 COMMENT '资金变化'
	,Reserve numeric(38,10) not null default 0.0 COMMENT '保留资金'
	,ReserveAvailable numeric(38,10) not null default 0.0 COMMENT '保留资金可用'
	,ReserveLimit numeric(38,10) not null default 0.0 COMMENT '保留资金限制'
	,ReserveProfit numeric(38,10) not null default 0.0 COMMENT '保留资金盈亏'
	,ReserveType char(1) not null default 0 COMMENT '保留资金类型(0:先亏损保留资金再亏损真钱(劣后);1:先亏损真钱再亏损保留资金(优先);2:保留资金不做亏损(抵押金);)'
	,ReserveActive numeric(1) not null default 0 COMMENT '保留资金是否活跃'
	,TotalCloseProfit numeric(36,6) not null default 0.0 COMMENT '总平仓盈亏'
	,TotalDeposit numeric(36,6) not null default 0.0 COMMENT '总入金金额'
	,TotalWithdraw numeric(36,6) not null default 0.0 COMMENT '总出金金额'
	,TotalFee numeric(36,6) not null default 0.0 COMMENT '总手续费'
	,TotalMoneyChange numeric(36,6) not null default 0.0 COMMENT '总资金变化'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,AccountType char(8) default null COMMENT '账户类型'
	,BusinessNo numeric(16) default null COMMENT '业务序列号'
	,MarginRateGrade char(8) default null COMMENT '保证金等级'
	,TradingRightGrade char(8) default null COMMENT '权限等级'
	,Frequency numeric(10) default null COMMENT '每秒更新的次数'
	,MaintMargin numeric(38,10) not null default 0.0 COMMENT '维持保证金'
	,UnRealProfit numeric(38,10) not null default 0.0 COMMENT '未实现盈亏'
	,CopyProfit numeric(38,10) not null default 0.0 COMMENT '带单盈利分配'
	,CFDType char(1) default null COMMENT '点差接管类型(0:先撮合后点差接管;1:只撮合不点差接管;2:不撮合只点差接管;)'
	,FeeGrade char(8) default null COMMENT '手续费等级'
	,CFDGrade char(8) default null COMMENT '差价等级'
	,Borrow numeric(38,10) not null default 0.0 COMMENT '借款周期结算量'
	,Remark char(32) default null COMMENT '备注'
	,MaxLocalID char(20) default null COMMENT '最大本地标识'
	,CurrValue numeric(38,10) not null default 0.0 COMMENT '当前价值'
	,CanReduce numeric(1) not null default 1 COMMENT '是否可以出金'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (AccountID,Currency,SettlementGroup)
	,KEY mykey_0 (MemberID)
)ENGINE=InnoDB COMMENT='账户资金';

/******************************
-- create MemberPosition
--******************************/
drop table if exists perpetual.t_MemberPosition;
create table perpetual.t_MemberPosition
(
	MemberID char(42) not null default '' COMMENT '成员代码'
	,AccountID char(44) not null default '' COMMENT '资金账号'
	,ExchangeID char(8) not null default '' COMMENT '交易所代码'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,Position numeric(38,10) not null default 0.0 COMMENT '总持仓'
	,LongFrozen numeric(38,10) not null default 0.0 COMMENT '多头冻结'
	,ShortFrozen numeric(38,10) not null default 0.0 COMMENT '空头冻结'
	,DefaultLeverage numeric(20,8) default null COMMENT '默认杠杆倍数'
	,DefaultCrossMargin numeric(1) not null default 0 COMMENT '默认是否全仓'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (MemberID,AccountID,ExchangeID,InstrumentID)
)ENGINE=InnoDB COMMENT='成员持仓汇总';

/******************************
-- create Position
--******************************/
drop table if exists perpetual.t_Position;
create table perpetual.t_Position
(
	PositionID char(42) not null default '' COMMENT '持仓代码'
	,MemberID char(42) not null default '' COMMENT '成员代码'
	,TradeUnitID char(20) not null default '' COMMENT '交易单元代码'
	,ExchangeID char(8) not null default '' COMMENT '交易所代码'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,PositionClass char(1) default null COMMENT '持仓类型(0:不设置;1:保证金交易;3:资产;4:持币成本;)'
	,PosiDirection char(1) not null default 0 COMMENT '持仓多空方向(0:多头;1:空头;2:净;)'
	,PrePosition numeric(38,10) not null default 0.0 COMMENT '上次持仓'
	,Position numeric(38,10) not null default 0.0 COMMENT '总持仓'
	,LongFrozen numeric(38,10) not null default 0.0 COMMENT '多头冻结'
	,ShortFrozen numeric(38,10) not null default 0.0 COMMENT '空头冻结'
	,PreLongFrozen numeric(38,10) not null default 0.0 COMMENT '昨日多头冻结'
	,PreShortFrozen numeric(38,10) not null default 0.0 COMMENT '昨日空头冻结'
	,HighestPosition numeric(38,10) not null default 0.0 COMMENT '最大持仓'
	,ClosePosition numeric(38,10) default null COMMENT '可平持仓'
	,PositionCost numeric(36,15) not null default 0.0 COMMENT '持仓成本'
	,CostPrice numeric(28,15) default null COMMENT '成本价'
	,UseMargin numeric(38,10) not null default 0.0 COMMENT '占用保证金'
	,FrozenMargin numeric(38,10) not null default 0.0 COMMENT '冻结保证金'
	,LongFrozenMargin numeric(38,10) not null default 0.0 COMMENT '多头冻结保证金'
	,ShortFrozenMargin numeric(38,10) not null default 0.0 COMMENT '空头冻结保证金'
	,CloseProfit numeric(38,10) not null default 0.0 COMMENT '平仓盈亏'
	,TotalPositionCost numeric(36,6) not null default 0.0 COMMENT '开仓成本'
	,TotalCloseProfit numeric(36,6) not null default 0.0 COMMENT '总平仓盈亏'
	,OpenPrice numeric(28,15) default null COMMENT '开仓均价'
	,ClosePrice numeric(28,15) default null COMMENT '平仓均价'
	,TradeFee numeric(38,10) not null default 0.0 COMMENT '交易费用'
	,PositionFee numeric(38,10) not null default 0.0 COMMENT '持仓费用或者股票分红等'
	,Leverage numeric(20,8) default null COMMENT '杠杆倍数'
	,AccountID char(44) not null default '' COMMENT '资金账号'
	,PriceCurrency char(12) default null COMMENT '计价币种'
	,ClearCurrency char(12) default null COMMENT '清算币种'
	,SettlementGroup char(8) not null default '' COMMENT '结算组编号'
	,IsCrossMargin numeric(1) not null default 0 COMMENT '是否全仓'
	,CloseOrderID char(20) default null COMMENT '平仓单代码'
	,SLTriggerPrice numeric(28,15) default null COMMENT '止损触发价'
	,TPTriggerPrice numeric(28,15) default null COMMENT '止盈触发价'
	,BeginTime numeric(20) default null COMMENT '持仓不为0的开始时间'
	,InsertTime numeric(20) default null COMMENT '插入时间(新持仓时间)'
	,LastOpenTime numeric(20) default null COMMENT '最后开仓时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,BusinessNo numeric(16) default null COMMENT '业务序列号'
	,IsAutoAddMargin numeric(1) default null COMMENT '是否自动追加保证金'
	,Frequency numeric(10) default null COMMENT '每秒更新的次数'
	,MaintMargin numeric(38,10) default null COMMENT '维持保证金'
	,UnRealProfit numeric(38,10) not null default 0.0 COMMENT '未实现盈亏'
	,LiquidPrice numeric(28,15) default null COMMENT '清算价格'
	,CreateTime char(20) default null COMMENT '创建时间'
	,CopyMemberID char(42) default null COMMENT '带单员代码'
	,CopyProfitRate numeric(26,10) default null COMMENT '带单分配比例'
	,CopyProfit numeric(38,10) not null default 0.0 COMMENT '带单盈利分配'
	,FirstTradeID char(42) default null COMMENT '初始成交订单号'
	,LastTradeID char(42) default null COMMENT '结束成交订单号'
	,BusinessType char(1) default null COMMENT '业务类别(0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;)'
	,BusinessValue char(32) default null COMMENT '业务值'
	,Reserve numeric(38,10) default null COMMENT '保留资金'
	,ReserveProfit numeric(38,10) default null COMMENT '保留资金盈亏'
	,Remark char(32) default null COMMENT '备注'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (PositionID)
)ENGINE=InnoDB COMMENT='标的持仓';

/******************************
-- create Order
--******************************/
drop table if exists perpetual.t_Order;
create table perpetual.t_Order
(
	MemberID char(42) not null default '' COMMENT '成员代码'
	,TradeUnitID char(20) default null COMMENT '交易单元代码'
	,AccountID char(44) default null COMMENT '资金账号'
	,LocalID char(20) default null COMMENT '报单本地标识'
	,ExchangeID char(8) not null default '' COMMENT '交易所代码'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,OrderPriceType char(1) not null default 0 COMMENT '报单价格类型(0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;)'
	,Direction char(1) not null default 0 COMMENT '买卖方向(0:买;1:卖;)'
	,OffsetFlag char(1) not null default 0 COMMENT '开平标志(0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;)'
	,Price numeric(28,15) default null COMMENT '报单价格'
	,Volume numeric(38,10) default null COMMENT '数量'
	,VolumeDisplay numeric(38,10) default null COMMENT '显示数量'
	,VolumeMode char(1) default null COMMENT '数量取值方式(0:百分比;1:绝对值;)'
	,Cost numeric(38,10) default null COMMENT '委托额'
	,OrderType char(1) not null default 0 COMMENT '订单类型(0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;)'
	,GTDTime numeric(20) default null COMMENT 'GTD时间'
	,MinVolume numeric(38,10) default null COMMENT '最小成交量'
	,BusinessType char(1) not null default 0 COMMENT '业务类别(0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;)'
	,BusinessValue char(32) default null COMMENT '业务值'
	,CloseOrderID char(20) default null COMMENT '平仓指定开仓的订单号'
	,IsCrossMargin numeric(1) default null COMMENT '是否全仓'
	,Remark char(32) default null COMMENT '备注'
	,OrderID char(20) not null default '' COMMENT '报单系统唯一代码'
	,CopyMemberID char(42) default null COMMENT '带单员代码'
	,CopyOrderID char(20) default null COMMENT '带单员报单号'
	,Leverage numeric(20,8) default null COMMENT '委托单杠杆倍数'
	,CopyProfitRate numeric(26,10) default null COMMENT '带单分配比例'
	,APPID char(8) default null COMMENT '应用编号'
	,PositionID char(42) not null default '' COMMENT '持仓代码'
	,TriggerPrice numeric(28,15) default null COMMENT '触发价'
	,Reserve numeric(38,10) default null COMMENT '保留资金'
	,OrderStatus char(1) not null default 0 COMMENT '报单状态(0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;)'
	,DeriveSource char(1) not null default 0 COMMENT '衍生来源(0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;)'
	,DeriveDetail char(20) default null COMMENT '衍生明细'
	,VolumeTraded numeric(38,10) not null default 0.0 COMMENT '成交数量'
	,VolumeRemain numeric(38,10) not null default 0.0 COMMENT '剩余数量'
	,VolumeCancled numeric(38,10) not null default 0.0 COMMENT '已经撤单数量'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,Priority numeric(10) not null default 0 COMMENT '优先权'
	,TimeSortNo numeric(16) not null default 0.0 COMMENT '按时间排队的序号'
	,FrontNo numeric(10) default null COMMENT '前置编号'
	,PriceCurrency char(12) default null COMMENT '计价币种'
	,FeeCurrency char(12) default null COMMENT '手续费币种'
	,ClearCurrency char(12) default null COMMENT '清算币种'
	,FrozenMoney numeric(38,10) not null default 0.0 COMMENT '冻结资金'
	,FrozenFee numeric(38,10) not null default 0.0 COMMENT '冻结手续费'
	,FrozenMargin numeric(38,10) not null default 0.0 COMMENT '冻结保证金'
	,Fee numeric(38,10) not null default 0.0 COMMENT '手续费'
	,CloseProfit numeric(38,10) not null default 0.0 COMMENT '平仓盈亏'
	,Turnover numeric(38,10) not null default 0.0 COMMENT '成交金额'
	,RelatedOrderID char(20) default null COMMENT '关联报单号'
	,BusinessResult char(32) default null COMMENT '业务执行结果'
	,BusinessNo numeric(16) default null COMMENT '业务序列号'
	,Tradable numeric(1) not null default 1 COMMENT '是否可以参加交易'
	,SettlementGroup char(8) not null default '' COMMENT '结算组编号'
	,PosiDirection char(1) not null default 0 COMMENT '持仓多空方向(0:多头;1:空头;2:净;)'
	,TradePrice numeric(28,15) default null COMMENT '成交均价'
	,OpenPrice numeric(28,15) default null COMMENT '平仓成交时的开仓均价'
	,TriggerOrderID char(20) default null COMMENT '触发报单号'
	,SLTriggerPrice numeric(28,15) default null COMMENT '开仓报单成交之后的止损触发价'
	,TPTriggerPrice numeric(28,15) default null COMMENT '开仓报单成交之后的止盈触发价'
	,CopyProfit numeric(38,10) not null default 0.0 COMMENT '带单盈利分配'
	,Position numeric(38,10) default null COMMENT '成交时的持仓量'
	,UserID char(42) not null default '' COMMENT '交易用户代码'
	,LastPriceByInsert numeric(28,15) default null COMMENT '插入时的最新价'
	,BidPrice1ByInsert numeric(28,15) default null COMMENT '插入时的买一价'
	,AskPrice1ByInsert numeric(28,15) default null COMMENT '插入时的卖一价'
	,Available numeric(38,10) default null COMMENT '可用资金'
	,CreateTime char(20) default null COMMENT '创建时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (OrderID)
	,KEY mykey_0 (MemberID,CopyMemberID)
)ENGINE=InnoDB COMMENT='报单';

/******************************
-- create TriggerOrder
--******************************/
drop table if exists perpetual.t_TriggerOrder;
create table perpetual.t_TriggerOrder
(
	MemberID char(42) not null default '' COMMENT '成员代码'
	,TradeUnitID char(20) default null COMMENT '交易单元代码'
	,AccountID char(44) default null COMMENT '资金账号'
	,LocalID char(20) default null COMMENT '报单本地标识'
	,ExchangeID char(8) not null default '' COMMENT '交易所代码'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,OrderPriceType char(1) not null default 0 COMMENT '报单价格类型(0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;)'
	,Direction char(1) not null default 0 COMMENT '买卖方向(0:买;1:卖;)'
	,OffsetFlag char(1) not null default 0 COMMENT '开平标志(0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;)'
	,Price numeric(28,15) default null COMMENT '报单价格'
	,Volume numeric(38,10) default null COMMENT '数量'
	,VolumeDisplay numeric(38,10) default null COMMENT '显示数量'
	,VolumeMode char(1) default null COMMENT '数量取值方式(0:百分比;1:绝对值;)'
	,Cost numeric(38,10) default null COMMENT '委托额'
	,OrderType char(1) not null default 0 COMMENT '订单类型(0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;)'
	,GTDTime numeric(20) default null COMMENT 'GTD时间'
	,MinVolume numeric(38,10) default null COMMENT '最小成交量'
	,BusinessType char(1) not null default 0 COMMENT '业务类别(0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;)'
	,BusinessValue char(32) default null COMMENT '业务值'
	,CloseOrderID char(20) default null COMMENT '平仓指定开仓的订单号'
	,IsCrossMargin numeric(1) default null COMMENT '是否全仓'
	,Remark char(32) default null COMMENT '备注'
	,OrderID char(20) not null default '' COMMENT '报单系统唯一代码'
	,CopyMemberID char(42) default null COMMENT '带单员代码'
	,CopyOrderID char(20) default null COMMENT '带单员报单号'
	,Leverage numeric(20,8) default null COMMENT '委托单杠杆倍数'
	,CopyProfitRate numeric(26,10) default null COMMENT '带单分配比例'
	,APPID char(8) default null COMMENT '应用编号'
	,PositionID char(42) not null default '' COMMENT '持仓代码'
	,TriggerPrice numeric(28,15) default null COMMENT '触发价'
	,Reserve numeric(38,10) default null COMMENT '保留资金'
	,SLPrice numeric(28,15) default null COMMENT '止损价'
	,SLTriggerPrice numeric(28,15) default null COMMENT '止损触发价'
	,TPPrice numeric(28,15) default null COMMENT '止盈价'
	,TPTriggerPrice numeric(28,15) default null COMMENT '止盈触发价'
	,RiskBefore numeric(1) not null default 0 COMMENT '是否提前检查风控'
	,TriggerOrderType char(1) not null default 0 COMMENT '触发的订单类型(0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;)'
	,TriggerDetail char(20) default null COMMENT '触发类型明细'
	,TriggerPriceType char(1) not null default "0" COMMENT '触发价类型(0:最新价;1:标记价;2:标的指数价;)'
	,TriggerValue varchar(1024) default null COMMENT '触发单具体设置信息'
	,CloseSLPrice numeric(28,15) default null COMMENT '平仓止损价'
	,CloseSLTriggerPrice numeric(28,15) default null COMMENT '平仓止损触发价'
	,CloseTPPrice numeric(28,15) default null COMMENT '平仓止盈价'
	,CloseTPTriggerPrice numeric(28,15) default null COMMENT '平仓止盈触发价'
	,CloseOrderPriceType char(1) default null COMMENT '报单价格类型(0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;)'
	,ClosePrice numeric(28,15) default null COMMENT '平仓价'
	,CloseTriggerPrice numeric(28,15) default null COMMENT '平仓触发价'
	,RelatedOrderID char(20) default null COMMENT '关联报单号'
	,ActiveTime numeric(20) default null COMMENT '激活时间'
	,TriggerTime numeric(20) default null COMMENT '触发时间'
	,TimeSortNo numeric(16) not null default 0.0 COMMENT '按时间排队的序号'
	,TriggerStatus char(1) not null default 0 COMMENT '触发报单状态(0:未设置状态;1:活跃(运行)状态;2:已经触发;3:触发失败;4:撤单;)'
	,PosiDirection char(1) default null COMMENT '持仓多空方向(0:多头;1:空头;2:净;)'
	,FrontNo numeric(10) default null COMMENT '前置编号'
	,ErrorNo numeric(10) default null COMMENT '错误代码'
	,ErrorMsg varchar(128) default null COMMENT '错误信息'
	,InsertTime numeric(20) not null default 0.0 COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,BusinessNo numeric(16) default null COMMENT '业务序列号'
	,CreateTime char(20) default null COMMENT '创建时间'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (OrderID)
	,KEY mykey_0 (MemberID,CopyMemberID)
)ENGINE=InnoDB COMMENT='条件报单';

/******************************
-- create PositionOrder
--******************************/
drop table if exists perpetual.t_PositionOrder;
create table perpetual.t_PositionOrder
(
	MemberID char(42) not null default '' COMMENT '成员代码'
	,TradeUnitID char(20) default null COMMENT '交易单元代码'
	,AccountID char(44) default null COMMENT '资金账号'
	,LocalID char(20) default null COMMENT '报单本地标识'
	,ExchangeID char(8) not null default '' COMMENT '交易所代码'
	,InstrumentID char(26) not null default '' COMMENT '标的代码'
	,OrderPriceType char(1) not null default 0 COMMENT '报单价格类型(0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;)'
	,Direction char(1) not null default 0 COMMENT '买卖方向(0:买;1:卖;)'
	,OffsetFlag char(1) not null default 0 COMMENT '开平标志(0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;)'
	,Price numeric(28,15) default null COMMENT '报单价格'
	,Volume numeric(38,10) default null COMMENT '数量'
	,VolumeDisplay numeric(38,10) default null COMMENT '显示数量'
	,VolumeMode char(1) default null COMMENT '数量取值方式(0:百分比;1:绝对值;)'
	,Cost numeric(38,10) default null COMMENT '委托额'
	,OrderType char(1) not null default 0 COMMENT '订单类型(0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;)'
	,GTDTime numeric(20) default null COMMENT 'GTD时间'
	,MinVolume numeric(38,10) default null COMMENT '最小成交量'
	,BusinessType char(1) not null default 0 COMMENT '业务类别(0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;)'
	,BusinessValue char(32) default null COMMENT '业务值'
	,CloseOrderID char(20) default null COMMENT '平仓指定开仓的订单号'
	,IsCrossMargin numeric(1) default null COMMENT '是否全仓'
	,Remark char(32) default null COMMENT '备注'
	,OrderID char(20) not null default '' COMMENT '报单系统唯一代码'
	,CopyMemberID char(42) default null COMMENT '带单员代码'
	,CopyOrderID char(20) default null COMMENT '带单员报单号'
	,Leverage numeric(20,8) default null COMMENT '委托单杠杆倍数'
	,CopyProfitRate numeric(26,10) default null COMMENT '带单分配比例'
	,APPID char(8) default null COMMENT '应用编号'
	,PositionID char(42) not null default '' COMMENT '持仓代码'
	,TriggerPrice numeric(28,15) default null COMMENT '触发价'
	,Reserve numeric(38,10) default null COMMENT '保留资金'
	,OrderStatus char(1) not null default 0 COMMENT '报单状态(0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;)'
	,DeriveSource char(1) not null default 0 COMMENT '衍生来源(0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;)'
	,DeriveDetail char(20) default null COMMENT '衍生明细'
	,VolumeTraded numeric(38,10) not null default 0.0 COMMENT '成交数量'
	,VolumeRemain numeric(38,10) not null default 0.0 COMMENT '剩余数量'
	,VolumeCancled numeric(38,10) not null default 0.0 COMMENT '已经撤单数量'
	,InsertTime numeric(20) default null COMMENT '插入时间'
	,UpdateTime numeric(20) default null COMMENT '更新时间'
	,Priority numeric(10) not null default 0 COMMENT '优先权'
	,TimeSortNo numeric(16) not null default 0.0 COMMENT '按时间排队的序号'
	,FrontNo numeric(10) default null COMMENT '前置编号'
	,PriceCurrency char(12) default null COMMENT '计价币种'
	,FeeCurrency char(12) default null COMMENT '手续费币种'
	,ClearCurrency char(12) default null COMMENT '清算币种'
	,FrozenMoney numeric(38,10) not null default 0.0 COMMENT '冻结资金'
	,FrozenFee numeric(38,10) not null default 0.0 COMMENT '冻结手续费'
	,FrozenMargin numeric(38,10) not null default 0.0 COMMENT '冻结保证金'
	,Fee numeric(38,10) not null default 0.0 COMMENT '手续费'
	,CloseProfit numeric(38,10) not null default 0.0 COMMENT '平仓盈亏'
	,Turnover numeric(38,10) not null default 0.0 COMMENT '成交金额'
	,RelatedOrderID char(20) default null COMMENT '关联报单号'
	,BusinessResult char(32) default null COMMENT '业务执行结果'
	,BusinessNo numeric(16) default null COMMENT '业务序列号'
	,Tradable numeric(1) not null default 1 COMMENT '是否可以参加交易'
	,SettlementGroup char(8) not null default '' COMMENT '结算组编号'
	,PosiDirection char(1) not null default 0 COMMENT '持仓多空方向(0:多头;1:空头;2:净;)'
	,TradePrice numeric(28,15) default null COMMENT '成交均价'
	,OpenPrice numeric(28,15) default null COMMENT '平仓成交时的开仓均价'
	,TriggerOrderID char(20) default null COMMENT '触发报单号'
	,SLTriggerPrice numeric(28,15) default null COMMENT '开仓报单成交之后的止损触发价'
	,TPTriggerPrice numeric(28,15) default null COMMENT '开仓报单成交之后的止盈触发价'
	,CopyProfit numeric(38,10) not null default 0.0 COMMENT '带单盈利分配'
	,Position numeric(38,10) default null COMMENT '成交时的持仓量'
	,UserID char(42) not null default '' COMMENT '交易用户代码'
	,LastPriceByInsert numeric(28,15) default null COMMENT '插入时的最新价'
	,BidPrice1ByInsert numeric(28,15) default null COMMENT '插入时的买一价'
	,AskPrice1ByInsert numeric(28,15) default null COMMENT '插入时的卖一价'
	,Available numeric(38,10) default null COMMENT '可用资金'
	,CreateTime char(20) default null COMMENT '创建时间'
	,VolumeToClose numeric(38,10) not null default 0.0 COMMENT '需要平仓的数量'
	,VolumeClosed numeric(38,10) not null default 0.0 COMMENT '已经平仓的数量'
	,VolumeOnClose numeric(38,10) not null default 0.0 COMMENT '正在平仓未成交的数量'
	,UpdateTimes numeric(28,0) DEFAULT 0 COMMENT '更新次数'
	,DBTime datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP on update current_timestamp COMMENT '数据库修改时间'
	,primary key (OrderID)
)ENGINE=InnoDB COMMENT='持仓订单组成';
