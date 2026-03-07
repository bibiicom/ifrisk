BEGIN
	DECLARE GLOBAL_MaxTime INT DEFAULT 1668759381; -- 当前时间的timestamp：select UNIX_TIMESTAMP(now()) from dual;
	DECLARE GLOBAL_MinTime INT DEFAULT 1626358443; -- 历史记录中的最小值
	DECLARE maxTime INT;
	DECLARE minTime INT;
	DECLARE startTime INT;
	DECLARE endTime INT;
	
	-- t_D_FinishOrder
	-- 上次迁移的最小时间
	select MIN(min_time) into maxTime from dc_history_migratelog where table_name = 't_D_FinishOrder_20221130';
	if maxTime is NULL then
		set maxTime = GLOBAL_MaxTime;
	end if;
	if maxTime > GLOBAL_MinTime then
		select UNIX_TIMESTAMP(now()) into startTime from dual;
		
		-- 本次迁移的最小时间
		set minTime = maxTime - p_interval;

		-- 迁移sql
		insert ignore into t_D_FinishOrder(MemberID,TradeUnitID,AccountID,UserID,LocalID,ExchangeID,InstrumentID,OrderPriceType,Direction,OffsetFlag,Price,Volume,VolumeDisplay,VolumeMode,Cost,OrderType,TimeCondition,GTDTime,MinVolume,BusinessType,BusinessValue,CloseOrderID,IsCrossMargin,Remark,OrderID,OrderID,PositionID,OrderStatus,DeriveSource,VolumeTraded,VolumeRemain,VolumeCancled,InsertTime,UpdateTime,Priority,TimeSortNo,FrontNo,SessionNo,Currency,PriceCurrency,FeeCurrency,ClearCurrency,FrozenMoney,FrozenFee,FrozenMargin,Fee,CloseProfit,Turnover,Leverage,RelatedOrderID,RelatedOrderID,BusinessResult,BusinessNo,TriggerPrice,Tradable,SettlementGroup,APPID,ProductID,ProductGroup,MatchGroup,PosiDirection,TradePrice,OpenPrice,TriggerOrderID,SLTriggerPrice,TPTriggerPrice,LastPriceByInsert,BidPrice1ByInsert,AskPrice1ByInsert,ImplySortNo,CreateTime)
		select MemberID,TradeUnitID,AccountID,UserID,LocalID,ExchangeID,InstrumentID,OrderPriceType,Direction,OffsetFlag,Price,Volume,VolumeDisplay,VolumeMode,Cost,OrderType,TimeCondition,GTDTime,MinVolume,BusinessType,BusinessValue,CloseOrderID,IsCrossMargin,Remark,OrderID,OrderID,PositionID,OrderStatus,DeriveSource,VolumeTraded,VolumeRemain,VolumeCancled,InsertTime,UpdateTime,Priority,TimeSortNo,FrontNo,SessionNo,Currency,PriceCurrency,FeeCurrency,ClearCurrency,FrozenMoney,FrozenFee,FrozenMargin,Fee,CloseProfit,Turnover,Leverage,RelatedOrderID,RelatedOrderID,BusinessResult,BusinessNo,TriggerPrice,Tradable,SettlementGroup,APPID,ProductID,ProductGroup,MatchGroup,PosiDirection,TradePrice,OpenPrice,TriggerOrderID,SLTriggerPrice,TPTriggerPrice,LastPriceByInsert,BidPrice1ByInsert,AskPrice1ByInsert,ImplySortNo,CreateTime
		from t_D_FinishOrder_20221130
		where UpdateTime >= minTime and UpdateTime < maxTime;
		-- 写入日志
		select UNIX_TIMESTAMP(now()) into endTime from dual;
		insert into dc_history_migratelog(table_name, min_time, max_time, cost_sec) VALUES ('t_D_FinishOrder_20221130', minTime, maxTime, endTime-startTime);
	else 
		-- 结束日志
		insert into dc_history_migratelog(table_name, min_time, max_time, cost_sec) VALUES ('t_D_FinishOrder_20221130_Done', maxTime, maxTime, 0);
	end if;
	
	-- t_D_Trade
	-- 上次迁移的最小时间
	select MIN(min_time) into maxTime from dc_history_migratelog where table_name = 't_D_Trade_20221130';
	if maxTime is NULL then
		set maxTime = GLOBAL_MaxTime;
	end if;
	if maxTime > GLOBAL_MinTime then
		select UNIX_TIMESTAMP(now()) into startTime from dual;
		
		-- 本次迁移的最小时间
		set minTime = maxTime - p_interval;

		-- 迁移sql
		insert ignore into t_D_Trade(TradeID,Direction,OrderID,OrderID,MemberID,PositionID,TradeUnitID,AccountID,ExchangeID,InstrumentID,ProductGroup,SettlementGroup,ProductID,OffsetFlag,Price,Volume,TradeTime,DeriveSource,PriceSource,UserID,OrderLocalID,MatchRole,Currency,PriceCurrency,ClearCurrency,Fee,FeeCurrency,ReserveFee,CloseProfit,ReserveProfit,Turnover,UseMargin,ReserveMargin,Leverage,OrderPrice,TriggerPrice,IsSelfTrade,Remark,BusinessNo,OpenPrice,InsertTime,CreateTime)
		select TradeID,Direction,OrderID,OrderID,MemberID,PositionID,TradeUnitID,AccountID,ExchangeID,InstrumentID,ProductGroup,SettlementGroup,ProductID,OffsetFlag,Price,Volume,TradeTime,DeriveSource,PriceSource,UserID,OrderLocalID,MatchRole,Currency,PriceCurrency,ClearCurrency,Fee,FeeCurrency,ReserveFee,CloseProfit,ReserveProfit,Turnover,UseMargin,ReserveMargin,Leverage,OrderPrice,TriggerPrice,IsSelfTrade,Remark,BusinessNo,OpenPrice,InsertTime,CreateTime
		from t_D_Trade_20221130
		where TradeTime >= minTime and TradeTime < maxTime;
		-- 写入日志
		select UNIX_TIMESTAMP(now()) into endTime from dual;
		insert into dc_history_migratelog(table_name, min_time, max_time, cost_sec) VALUES ('t_D_Trade_20221130', minTime, maxTime, endTime-startTime);
	else 
		-- 结束日志
		insert into dc_history_migratelog(table_name, min_time, max_time, cost_sec) VALUES ('t_D_Trade_20221130_Done', maxTime, maxTime, 0);
	end if;
	
	-- t_D_FinishPosition
	-- 上次迁移的最小时间
	select MIN(min_time) into maxTime from dc_history_migratelog where table_name = 't_D_FinishPosition_20221130';
	if maxTime is NULL then
		set maxTime = GLOBAL_MaxTime;
	end if;
	if maxTime > GLOBAL_MinTime then
		select UNIX_TIMESTAMP(now()) into startTime from dual;
		
		-- 本次迁移的最小时间
		set minTime = maxTime - p_interval;

		-- 迁移sql
		insert ignore into t_D_FinishPosition(PositionID,MemberID,TradeUnitID,ExchangeID,InstrumentID,ProductID,ProductGroup,PosiDirection,PrePosition,Position,LongFrozen,ShortFrozen,PreLongFrozen,PreShortFrozen,HighestPosition,ClosePosition,PositionCost,CostPrice,UseMargin,FrozenMargin,LongFrozenMargin,ShortFrozenMargin,CloseProfit,TotalPositionCost,TotalCloseProfit,OpenPrice,ClosePrice,TradeFee,PositionFee,Leverage,AccountID,Currency,PriceCurrency,ClearCurrency,SettlementGroup,IsCrossMargin,CloseOrderID,CloseOrderID,SLTriggerPrice,TPTriggerPrice,UserID,BeginTime,InsertTime,LastOpenTime,UpdateTime,BusinessNo,IsAutoAddMargin,CreateTime)
		select PositionID,MemberID,TradeUnitID,ExchangeID,InstrumentID,ProductID,ProductGroup,PosiDirection,PrePosition,Position,LongFrozen,ShortFrozen,PreLongFrozen,PreShortFrozen,HighestPosition,ClosePosition,PositionCost,CostPrice,UseMargin,FrozenMargin,LongFrozenMargin,ShortFrozenMargin,CloseProfit,TotalPositionCost,TotalCloseProfit,OpenPrice,ClosePrice,TradeFee,PositionFee,Leverage,AccountID,Currency,PriceCurrency,ClearCurrency,SettlementGroup,IsCrossMargin,CloseOrderID,CloseOrderID,SLTriggerPrice,TPTriggerPrice,UserID,BeginTime,InsertTime,LastOpenTime,UpdateTime,BusinessNo,IsAutoAddMargin,CreateTime
		from t_D_FinishPosition_20221130
		where UpdateTime >= minTime and UpdateTime < maxTime;
		-- 写入日志
		select UNIX_TIMESTAMP(now()) into endTime from dual;
		insert into dc_history_migratelog(table_name, min_time, max_time, cost_sec) VALUES ('t_D_FinishPosition_20221130', minTime, maxTime, endTime-startTime);
	else 
		-- 结束日志
		insert into dc_history_migratelog(table_name, min_time, max_time, cost_sec) VALUES ('t_D_FinishPosition_20221130_Done', maxTime, maxTime, 0);
	end if;
	
	-- t_D_AccountDetail
	-- 上次迁移的最小时间
	select MIN(min_time) into maxTime from dc_history_migratelog where table_name = 't_D_AccountDetail_20221130';
	if maxTime is NULL then
		set maxTime = GLOBAL_MaxTime;
	end if;
	if maxTime > GLOBAL_MinTime then
		select UNIX_TIMESTAMP(now()) into startTime from dual;
		
		-- 本次迁移的最小时间
		set minTime = maxTime - p_interval;

		-- 迁移sql
		insert ignore into t_D_AccountDetail(AccountDetailID,MemberID,TradeUnitID,ExchangeID,InstrumentID,SettlementGroup,ProductGroup,AccountID,Currency,Amount,PreBalance,Balance,Source,Remark,UpdateUserID,UpdateTime,UserID,InsertTime,LocalID,SettleSegment,BusinessNo,CreateTime,IsShow)
		select AccountDetailID,MemberID,TradeUnitID,ExchangeID,InstrumentID,SettlementGroup,ProductGroup,AccountID,Currency,Amount,PreBalance,Balance,Source,Remark,UpdateUserID,UpdateTime,UserID,InsertTime,LocalID,SettleSegment,BusinessNo,CreateTime,IsShow
		from t_D_AccountDetail_20221130
		where UpdateTime >= minTime and UpdateTime < maxTime;
		-- 写入日志
		select UNIX_TIMESTAMP(now()) into endTime from dual;
		insert into dc_history_migratelog(table_name, min_time, max_time, cost_sec) VALUES ('t_D_AccountDetail_20221130', minTime, maxTime, endTime-startTime);
	else 
		-- 结束日志
		insert into dc_history_migratelog(table_name, min_time, max_time, cost_sec) VALUES ('t_D_AccountDetail_20221130_Done', maxTime, maxTime, 0);
	end if;
	
	-- t_D_FinishTriggerOrder
	-- 上次迁移的最小时间
	select MIN(min_time) into maxTime from dc_history_migratelog where table_name = 't_D_FinishTriggerOrder_20221130';
	if maxTime is NULL then
		set maxTime = GLOBAL_MaxTime;
	end if;
	if maxTime > GLOBAL_MinTime then
		select UNIX_TIMESTAMP(now()) into startTime from dual;
		
		-- 本次迁移的最小时间
		set minTime = maxTime - p_interval;

		-- 迁移sql
		insert ignore into t_D_FinishTriggerOrder(MemberID,TradeUnitID,AccountID,UserID,LocalID,ExchangeID,InstrumentID,OrderPriceType,Direction,OffsetFlag,Price,Volume,VolumeDisplay,VolumeMode,Cost,OrderType,TimeCondition,GTDTime,MinVolume,BusinessType,BusinessValue,CloseOrderID,IsCrossMargin,Remark,OrderID,OrderID,PositionID,SLPrice,SLTriggerPrice,TPPrice,TPTriggerPrice,TriggerPrice,RiskBefore,TriggerOrderType,TriggerPriceType,TriggerValue1,TriggerValue2,TriggerValue3,TriggerValue4,AlgoID,CloseSLPrice,CloseSLTriggerPrice,CloseTPPrice,CloseTPTriggerPrice,CloseOrderPriceType,ClosePrice,CloseTriggerPrice,RelatedOrderID,RelatedOrderID,ActiveTime,TriggerTime,TimeSortNo,TriggerStatus,PosiDirection,ProductID,ProductGroup,FrontNo,SessionNo,APPID,ErrorNo,ErrorMsg,Leverage,InsertTime,UpdateTime,BusinessNo,CreateTime)
		select MemberID,TradeUnitID,AccountID,UserID,LocalID,ExchangeID,InstrumentID,OrderPriceType,Direction,OffsetFlag,Price,Volume,VolumeDisplay,VolumeMode,Cost,OrderType,TimeCondition,GTDTime,MinVolume,BusinessType,BusinessValue,CloseOrderID,IsCrossMargin,Remark,OrderID,OrderID,PositionID,SLPrice,SLTriggerPrice,TPPrice,TPTriggerPrice,TriggerPrice,RiskBefore,TriggerOrderType,TriggerPriceType,TriggerValue1,TriggerValue2,TriggerValue3,TriggerValue4,AlgoID,CloseSLPrice,CloseSLTriggerPrice,CloseTPPrice,CloseTPTriggerPrice,CloseOrderPriceType,ClosePrice,CloseTriggerPrice,RelatedOrderID,RelatedOrderID,ActiveTime,TriggerTime,TimeSortNo,TriggerStatus,PosiDirection,ProductID,ProductGroup,FrontNo,SessionNo,APPID,ErrorNo,ErrorMsg,Leverage,InsertTime,UpdateTime,BusinessNo,CreateTime
		from t_D_FinishTriggerOrder_20221130
		where UpdateTime >= minTime and UpdateTime < maxTime;
		-- 写入日志
		select UNIX_TIMESTAMP(now()) into endTime from dual;
		insert into dc_history_migratelog(table_name, min_time, max_time, cost_sec) VALUES ('t_D_FinishTriggerOrder_20221130', minTime, maxTime, endTime-startTime);
	else 
		-- 结束日志
		insert into dc_history_migratelog(table_name, min_time, max_time, cost_sec) VALUES ('t_D_FinishTriggerOrder_20221130_Done', maxTime, maxTime, 0);
	end if;

END