
#ifndef _EntityCopy_H
#define _EntityCopy_H

#ifndef CopyErrorEntity
#define CopyErrorEntity(target,source){	\
			(target)->ErrorNo=(source)->ErrorNo;	\
			(target)->ErrorMsg=(source)->ErrorMsg;	\
	}

#endif

#ifndef CopyTrueErrorEntity
#define CopyTrueErrorEntity(target,source){	\
			if(!(source)->ErrorNo.isNull()) (target)->ErrorNo=(source)->ErrorNo;	\
			if(!(source)->ErrorMsg.isNull()) (target)->ErrorMsg=(source)->ErrorMsg;	\
	}

#endif

#ifndef CopyTopicEntity
#define CopyTopicEntity(target,source){	\
			(target)->Action=(source)->Action;	\
			(target)->TopicID=(source)->TopicID;	\
			(target)->Index=(source)->Index;	\
			(target)->ResumeNo=(source)->ResumeNo;	\
			(target)->BusinessNo=(source)->BusinessNo;	\
			(target)->Token=(source)->Token;	\
			(target)->Zip=(source)->Zip;	\
			(target)->Frequency=(source)->Frequency;	\
	}

#endif

#ifndef CopyTrueTopicEntity
#define CopyTrueTopicEntity(target,source){	\
			if(!(source)->Action.isNull()) (target)->Action=(source)->Action;	\
			if(!(source)->TopicID.isNull()) (target)->TopicID=(source)->TopicID;	\
			if(!(source)->Index.isNull()) (target)->Index=(source)->Index;	\
			if(!(source)->ResumeNo.isNull()) (target)->ResumeNo=(source)->ResumeNo;	\
			if(!(source)->BusinessNo.isNull()) (target)->BusinessNo=(source)->BusinessNo;	\
			if(!(source)->Token.isNull()) (target)->Token=(source)->Token;	\
			if(!(source)->Zip.isNull()) (target)->Zip=(source)->Zip;	\
			if(!(source)->Frequency.isNull()) (target)->Frequency=(source)->Frequency;	\
	}

#endif

#ifndef CopyPrivateDispatchEntity
#define CopyPrivateDispatchEntity(target,source){	\
			(target)->TopicID=(source)->TopicID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->PrivateBase=(source)->PrivateBase;	\
	}

#endif

#ifndef CopyTruePrivateDispatchEntity
#define CopyTruePrivateDispatchEntity(target,source){	\
			if(!(source)->TopicID.isNull()) (target)->TopicID=(source)->TopicID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->PrivateBase.isNull()) (target)->PrivateBase=(source)->PrivateBase;	\
	}

#endif

#ifndef CopyLinkEntity
#define CopyLinkEntity(target,source){	\
			(target)->LinkNo=(source)->LinkNo;	\
			(target)->LinkUserID=(source)->LinkUserID;	\
			(target)->LinkPassword=(source)->LinkPassword;	\
			(target)->EncryptType=(source)->EncryptType;	\
			(target)->LinkDllName=(source)->LinkDllName;	\
			(target)->LinkDllLoad=(source)->LinkDllLoad;	\
			(target)->LinkAddress=(source)->LinkAddress;	\
			(target)->LinkStatus=(source)->LinkStatus;	\
			(target)->IsActive=(source)->IsActive;	\
			(target)->Config=(source)->Config;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueLinkEntity
#define CopyTrueLinkEntity(target,source){	\
			if(!(source)->LinkNo.isNull()) (target)->LinkNo=(source)->LinkNo;	\
			if(!(source)->LinkUserID.isNull()) (target)->LinkUserID=(source)->LinkUserID;	\
			if(!(source)->LinkPassword.isNull()) (target)->LinkPassword=(source)->LinkPassword;	\
			if(!(source)->EncryptType.isNull()) (target)->EncryptType=(source)->EncryptType;	\
			if(!(source)->LinkDllName.isNull()) (target)->LinkDllName=(source)->LinkDllName;	\
			if(!(source)->LinkDllLoad.isNull()) (target)->LinkDllLoad=(source)->LinkDllLoad;	\
			if(!(source)->LinkAddress.isNull()) (target)->LinkAddress=(source)->LinkAddress;	\
			if(!(source)->LinkStatus.isNull()) (target)->LinkStatus=(source)->LinkStatus;	\
			if(!(source)->IsActive.isNull()) (target)->IsActive=(source)->IsActive;	\
			if(!(source)->Config.isNull()) (target)->Config=(source)->Config;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyCurrentTimeOutEntity
#define CopyCurrentTimeOutEntity(target,source){	\
			(target)->SystemID=(source)->SystemID;	\
			(target)->ZoneDate=(source)->ZoneDate;	\
			(target)->ZoneTime=(source)->ZoneTime;	\
			(target)->ZoneDays=(source)->ZoneDays;	\
			(target)->CurrTime=(source)->CurrTime;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueCurrentTimeOutEntity
#define CopyTrueCurrentTimeOutEntity(target,source){	\
			if(!(source)->SystemID.isNull()) (target)->SystemID=(source)->SystemID;	\
			if(!(source)->ZoneDate.isNull()) (target)->ZoneDate=(source)->ZoneDate;	\
			if(!(source)->ZoneTime.isNull()) (target)->ZoneTime=(source)->ZoneTime;	\
			if(!(source)->ZoneDays.isNull()) (target)->ZoneDays=(source)->ZoneDays;	\
			if(!(source)->CurrTime.isNull()) (target)->CurrTime=(source)->CurrTime;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyCurrentTimeEntity
#define CopyCurrentTimeEntity(target,source){	\
			CopyCurrentTimeOutEntity(target,source);	\
			(target)->TimeZone=(source)->TimeZone;	\
			(target)->LastTradeID=(source)->LastTradeID;	\
			(target)->LastOrderID=(source)->LastOrderID;	\
			(target)->LastAccountDetailID=(source)->LastAccountDetailID;	\
			(target)->LastTimeSortNo=(source)->LastTimeSortNo;	\
			(target)->LastBusinessNo=(source)->LastBusinessNo;	\
	}

#endif

#ifndef CopyTrueCurrentTimeEntity
#define CopyTrueCurrentTimeEntity(target,source){	\
			CopyTrueCurrentTimeOutEntity(target,source);	\
			if(!(source)->TimeZone.isNull()) (target)->TimeZone=(source)->TimeZone;	\
			if(!(source)->LastTradeID.isNull()) (target)->LastTradeID=(source)->LastTradeID;	\
			if(!(source)->LastOrderID.isNull()) (target)->LastOrderID=(source)->LastOrderID;	\
			if(!(source)->LastAccountDetailID.isNull()) (target)->LastAccountDetailID=(source)->LastAccountDetailID;	\
			if(!(source)->LastTimeSortNo.isNull()) (target)->LastTimeSortNo=(source)->LastTimeSortNo;	\
			if(!(source)->LastBusinessNo.isNull()) (target)->LastBusinessNo=(source)->LastBusinessNo;	\
	}

#endif

#ifndef CopyInstrumentEntity
#define CopyInstrumentEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->InstrumentName=(source)->InstrumentName;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->ProductGroup=(source)->ProductGroup;	\
			(target)->UnderlyingID=(source)->UnderlyingID;	\
			(target)->ProductClass=(source)->ProductClass;	\
			(target)->ProductType=(source)->ProductType;	\
			(target)->StrikePrice=(source)->StrikePrice;	\
			(target)->OptionsType=(source)->OptionsType;	\
			(target)->VolumeMultiple=(source)->VolumeMultiple;	\
			(target)->StartTime=(source)->StartTime;	\
			(target)->ExpireTime=(source)->ExpireTime;	\
			(target)->PriceCurrency=(source)->PriceCurrency;	\
			(target)->ClearCurrency=(source)->ClearCurrency;	\
			(target)->IsInverse=(source)->IsInverse;	\
			(target)->BaseCurrency=(source)->BaseCurrency;	\
			(target)->MarginPriceType=(source)->MarginPriceType;	\
			(target)->BasisPrice=(source)->BasisPrice;	\
			(target)->MinOrderVolume=(source)->MinOrderVolume;	\
			(target)->MinOrderCost=(source)->MinOrderCost;	\
			(target)->MaxOrderVolume=(source)->MaxOrderVolume;	\
			(target)->PriceTick=(source)->PriceTick;	\
			(target)->VolumeTick=(source)->VolumeTick;	\
			(target)->ShowVolumeTick=(source)->ShowVolumeTick;	\
			(target)->TradingModel=(source)->TradingModel;	\
			(target)->DayStartTime=(source)->DayStartTime;	\
			(target)->SortNumber=(source)->SortNumber;	\
			(target)->Remark=(source)->Remark;	\
			(target)->DefaultLeverage=(source)->DefaultLeverage;	\
			(target)->PriceLimitValueMode=(source)->PriceLimitValueMode;	\
			(target)->PriceLimitPrice=(source)->PriceLimitPrice;	\
			(target)->PriceLimitUpperValue=(source)->PriceLimitUpperValue;	\
			(target)->PriceLimitLowerValue=(source)->PriceLimitLowerValue;	\
			(target)->MarginRateGroup=(source)->MarginRateGroup;	\
			(target)->CFDGroup=(source)->CFDGroup;	\
			(target)->FeeGroup=(source)->FeeGroup;	\
			(target)->TradingRightGroup=(source)->TradingRightGroup;	\
			(target)->MaxOpenInterest=(source)->MaxOpenInterest;	\
			(target)->FundingRateGroup=(source)->FundingRateGroup;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
			(target)->MarkedPriceTimes=(source)->MarkedPriceTimes;	\
	}

#endif

#ifndef CopyTrueInstrumentEntity
#define CopyTrueInstrumentEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->InstrumentName.isNull()) (target)->InstrumentName=(source)->InstrumentName;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->ProductGroup.isNull()) (target)->ProductGroup=(source)->ProductGroup;	\
			if(!(source)->UnderlyingID.isNull()) (target)->UnderlyingID=(source)->UnderlyingID;	\
			if(!(source)->ProductClass.isNull()) (target)->ProductClass=(source)->ProductClass;	\
			if(!(source)->ProductType.isNull()) (target)->ProductType=(source)->ProductType;	\
			if(!(source)->StrikePrice.isNull()) (target)->StrikePrice=(source)->StrikePrice;	\
			if(!(source)->OptionsType.isNull()) (target)->OptionsType=(source)->OptionsType;	\
			if(!(source)->VolumeMultiple.isNull()) (target)->VolumeMultiple=(source)->VolumeMultiple;	\
			if(!(source)->StartTime.isNull()) (target)->StartTime=(source)->StartTime;	\
			if(!(source)->ExpireTime.isNull()) (target)->ExpireTime=(source)->ExpireTime;	\
			if(!(source)->PriceCurrency.isNull()) (target)->PriceCurrency=(source)->PriceCurrency;	\
			if(!(source)->ClearCurrency.isNull()) (target)->ClearCurrency=(source)->ClearCurrency;	\
			if(!(source)->IsInverse.isNull()) (target)->IsInverse=(source)->IsInverse;	\
			if(!(source)->BaseCurrency.isNull()) (target)->BaseCurrency=(source)->BaseCurrency;	\
			if(!(source)->MarginPriceType.isNull()) (target)->MarginPriceType=(source)->MarginPriceType;	\
			if(!(source)->BasisPrice.isNull()) (target)->BasisPrice=(source)->BasisPrice;	\
			if(!(source)->MinOrderVolume.isNull()) (target)->MinOrderVolume=(source)->MinOrderVolume;	\
			if(!(source)->MinOrderCost.isNull()) (target)->MinOrderCost=(source)->MinOrderCost;	\
			if(!(source)->MaxOrderVolume.isNull()) (target)->MaxOrderVolume=(source)->MaxOrderVolume;	\
			if(!(source)->PriceTick.isNull()) (target)->PriceTick=(source)->PriceTick;	\
			if(!(source)->VolumeTick.isNull()) (target)->VolumeTick=(source)->VolumeTick;	\
			if(!(source)->ShowVolumeTick.isNull()) (target)->ShowVolumeTick=(source)->ShowVolumeTick;	\
			if(!(source)->TradingModel.isNull()) (target)->TradingModel=(source)->TradingModel;	\
			if(!(source)->DayStartTime.isNull()) (target)->DayStartTime=(source)->DayStartTime;	\
			if(!(source)->SortNumber.isNull()) (target)->SortNumber=(source)->SortNumber;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->DefaultLeverage.isNull()) (target)->DefaultLeverage=(source)->DefaultLeverage;	\
			if(!(source)->PriceLimitValueMode.isNull()) (target)->PriceLimitValueMode=(source)->PriceLimitValueMode;	\
			if(!(source)->PriceLimitPrice.isNull()) (target)->PriceLimitPrice=(source)->PriceLimitPrice;	\
			if(!(source)->PriceLimitUpperValue.isNull()) (target)->PriceLimitUpperValue=(source)->PriceLimitUpperValue;	\
			if(!(source)->PriceLimitLowerValue.isNull()) (target)->PriceLimitLowerValue=(source)->PriceLimitLowerValue;	\
			if(!(source)->MarginRateGroup.isNull()) (target)->MarginRateGroup=(source)->MarginRateGroup;	\
			if(!(source)->CFDGroup.isNull()) (target)->CFDGroup=(source)->CFDGroup;	\
			if(!(source)->FeeGroup.isNull()) (target)->FeeGroup=(source)->FeeGroup;	\
			if(!(source)->TradingRightGroup.isNull()) (target)->TradingRightGroup=(source)->TradingRightGroup;	\
			if(!(source)->MaxOpenInterest.isNull()) (target)->MaxOpenInterest=(source)->MaxOpenInterest;	\
			if(!(source)->FundingRateGroup.isNull()) (target)->FundingRateGroup=(source)->FundingRateGroup;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
			if(!(source)->MarkedPriceTimes.isNull()) (target)->MarkedPriceTimes=(source)->MarkedPriceTimes;	\
	}

#endif

#ifndef CopyInstrumentUpdateEntity
#define CopyInstrumentUpdateEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->MinOrderVolume=(source)->MinOrderVolume;	\
			(target)->MinOrderCost=(source)->MinOrderCost;	\
			(target)->MaxOrderVolume=(source)->MaxOrderVolume;	\
			(target)->PriceTick=(source)->PriceTick;	\
			(target)->VolumeTick=(source)->VolumeTick;	\
			(target)->MaxOpenInterest=(source)->MaxOpenInterest;	\
			(target)->BasisPrice=(source)->BasisPrice;	\
			(target)->UpperLimitPrice=(source)->UpperLimitPrice;	\
			(target)->LowerLimitPrice=(source)->LowerLimitPrice;	\
	}

#endif

#ifndef CopyTrueInstrumentUpdateEntity
#define CopyTrueInstrumentUpdateEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->MinOrderVolume.isNull()) (target)->MinOrderVolume=(source)->MinOrderVolume;	\
			if(!(source)->MinOrderCost.isNull()) (target)->MinOrderCost=(source)->MinOrderCost;	\
			if(!(source)->MaxOrderVolume.isNull()) (target)->MaxOrderVolume=(source)->MaxOrderVolume;	\
			if(!(source)->PriceTick.isNull()) (target)->PriceTick=(source)->PriceTick;	\
			if(!(source)->VolumeTick.isNull()) (target)->VolumeTick=(source)->VolumeTick;	\
			if(!(source)->MaxOpenInterest.isNull()) (target)->MaxOpenInterest=(source)->MaxOpenInterest;	\
			if(!(source)->BasisPrice.isNull()) (target)->BasisPrice=(source)->BasisPrice;	\
			if(!(source)->UpperLimitPrice.isNull()) (target)->UpperLimitPrice=(source)->UpperLimitPrice;	\
			if(!(source)->LowerLimitPrice.isNull()) (target)->LowerLimitPrice=(source)->LowerLimitPrice;	\
	}

#endif

#ifndef CopyServiceConfigEntity
#define CopyServiceConfigEntity(target,source){	\
			(target)->ConfigName=(source)->ConfigName;	\
			(target)->Index1=(source)->Index1;	\
			(target)->Index2=(source)->Index2;	\
			(target)->ConfigValue=(source)->ConfigValue;	\
			(target)->Remark=(source)->Remark;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueServiceConfigEntity
#define CopyTrueServiceConfigEntity(target,source){	\
			if(!(source)->ConfigName.isNull()) (target)->ConfigName=(source)->ConfigName;	\
			if(!(source)->Index1.isNull()) (target)->Index1=(source)->Index1;	\
			if(!(source)->Index2.isNull()) (target)->Index2=(source)->Index2;	\
			if(!(source)->ConfigValue.isNull()) (target)->ConfigValue=(source)->ConfigValue;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyMemberActionEntity
#define CopyMemberActionEntity(target,source){	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->RiskLevel=(source)->RiskLevel;	\
			(target)->PositionType=(source)->PositionType;	\
	}

#endif

#ifndef CopyTrueMemberActionEntity
#define CopyTrueMemberActionEntity(target,source){	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->RiskLevel.isNull()) (target)->RiskLevel=(source)->RiskLevel;	\
			if(!(source)->PositionType.isNull()) (target)->PositionType=(source)->PositionType;	\
	}

#endif

#ifndef CopyMemberEntity
#define CopyMemberEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->MemberType=(source)->MemberType;	\
			(target)->Grade=(source)->Grade;	\
			(target)->Priority=(source)->Priority;	\
			(target)->FeeGrade=(source)->FeeGrade;	\
			(target)->MarginRateGrade=(source)->MarginRateGrade;	\
			(target)->TradingRightGrade=(source)->TradingRightGrade;	\
			(target)->CFDGrade=(source)->CFDGrade;	\
			(target)->LiquidityGrade=(source)->LiquidityGrade;	\
			(target)->RiskLevel=(source)->RiskLevel;	\
			(target)->PositionType=(source)->PositionType;	\
			(target)->TriggerOrders=(source)->TriggerOrders;	\
			(target)->OpenOrders=(source)->OpenOrders;	\
			(target)->OpenPositions=(source)->OpenPositions;	\
			(target)->MaxLocalID=(source)->MaxLocalID;	\
			(target)->SinkType=(source)->SinkType;	\
			(target)->CopyMemberID=(source)->CopyMemberID;	\
			(target)->Region=(source)->Region;	\
			(target)->FeeDeduceDisCount=(source)->FeeDeduceDisCount;	\
			(target)->FeeDeduceCurrency=(source)->FeeDeduceCurrency;	\
			(target)->CanBorrow=(source)->CanBorrow;	\
			(target)->Remark=(source)->Remark;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueMemberEntity
#define CopyTrueMemberEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->MemberType.isNull()) (target)->MemberType=(source)->MemberType;	\
			if(!(source)->Grade.isNull()) (target)->Grade=(source)->Grade;	\
			if(!(source)->Priority.isNull()) (target)->Priority=(source)->Priority;	\
			if(!(source)->FeeGrade.isNull()) (target)->FeeGrade=(source)->FeeGrade;	\
			if(!(source)->MarginRateGrade.isNull()) (target)->MarginRateGrade=(source)->MarginRateGrade;	\
			if(!(source)->TradingRightGrade.isNull()) (target)->TradingRightGrade=(source)->TradingRightGrade;	\
			if(!(source)->CFDGrade.isNull()) (target)->CFDGrade=(source)->CFDGrade;	\
			if(!(source)->LiquidityGrade.isNull()) (target)->LiquidityGrade=(source)->LiquidityGrade;	\
			if(!(source)->RiskLevel.isNull()) (target)->RiskLevel=(source)->RiskLevel;	\
			if(!(source)->PositionType.isNull()) (target)->PositionType=(source)->PositionType;	\
			if(!(source)->TriggerOrders.isNull()) (target)->TriggerOrders=(source)->TriggerOrders;	\
			if(!(source)->OpenOrders.isNull()) (target)->OpenOrders=(source)->OpenOrders;	\
			if(!(source)->OpenPositions.isNull()) (target)->OpenPositions=(source)->OpenPositions;	\
			if(!(source)->MaxLocalID.isNull()) (target)->MaxLocalID=(source)->MaxLocalID;	\
			if(!(source)->SinkType.isNull()) (target)->SinkType=(source)->SinkType;	\
			if(!(source)->CopyMemberID.isNull()) (target)->CopyMemberID=(source)->CopyMemberID;	\
			if(!(source)->Region.isNull()) (target)->Region=(source)->Region;	\
			if(!(source)->FeeDeduceDisCount.isNull()) (target)->FeeDeduceDisCount=(source)->FeeDeduceDisCount;	\
			if(!(source)->FeeDeduceCurrency.isNull()) (target)->FeeDeduceCurrency=(source)->FeeDeduceCurrency;	\
			if(!(source)->CanBorrow.isNull()) (target)->CanBorrow=(source)->CanBorrow;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTradingRightEntity
#define CopyTradingRightEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Direction=(source)->Direction;	\
			(target)->TradingRight=(source)->TradingRight;	\
			(target)->Remark=(source)->Remark;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueTradingRightEntity
#define CopyTrueTradingRightEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Direction.isNull()) (target)->Direction=(source)->Direction;	\
			if(!(source)->TradingRight.isNull()) (target)->TradingRight=(source)->TradingRight;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyMarginRateEntity
#define CopyMarginRateEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->ValueMode=(source)->ValueMode;	\
			(target)->MarginSequenceType=(source)->MarginSequenceType;	\
			(target)->RiskSequenceType=(source)->RiskSequenceType;	\
			(target)->LongMarginRatio=(source)->LongMarginRatio;	\
			(target)->LongMaintMarginRatio=(source)->LongMaintMarginRatio;	\
			(target)->LongMarginStep=(source)->LongMarginStep;	\
			(target)->LongMaintMarginStep=(source)->LongMaintMarginStep;	\
			(target)->LongRiskLimit=(source)->LongRiskLimit;	\
			(target)->LongRiskStep=(source)->LongRiskStep;	\
			(target)->ShortMarginRatio=(source)->ShortMarginRatio;	\
			(target)->ShortMaintMarginRatio=(source)->ShortMaintMarginRatio;	\
			(target)->ShortMarginStep=(source)->ShortMarginStep;	\
			(target)->ShortMaintMarginStep=(source)->ShortMaintMarginStep;	\
			(target)->ShortRiskLimit=(source)->ShortRiskLimit;	\
			(target)->ShortRiskStep=(source)->ShortRiskStep;	\
			(target)->DefaultLeverage=(source)->DefaultLeverage;	\
			(target)->Remark=(source)->Remark;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueMarginRateEntity
#define CopyTrueMarginRateEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->ValueMode.isNull()) (target)->ValueMode=(source)->ValueMode;	\
			if(!(source)->MarginSequenceType.isNull()) (target)->MarginSequenceType=(source)->MarginSequenceType;	\
			if(!(source)->RiskSequenceType.isNull()) (target)->RiskSequenceType=(source)->RiskSequenceType;	\
			if(!(source)->LongMarginRatio.isNull()) (target)->LongMarginRatio=(source)->LongMarginRatio;	\
			if(!(source)->LongMaintMarginRatio.isNull()) (target)->LongMaintMarginRatio=(source)->LongMaintMarginRatio;	\
			if(!(source)->LongMarginStep.isNull()) (target)->LongMarginStep=(source)->LongMarginStep;	\
			if(!(source)->LongMaintMarginStep.isNull()) (target)->LongMaintMarginStep=(source)->LongMaintMarginStep;	\
			if(!(source)->LongRiskLimit.isNull()) (target)->LongRiskLimit=(source)->LongRiskLimit;	\
			if(!(source)->LongRiskStep.isNull()) (target)->LongRiskStep=(source)->LongRiskStep;	\
			if(!(source)->ShortMarginRatio.isNull()) (target)->ShortMarginRatio=(source)->ShortMarginRatio;	\
			if(!(source)->ShortMaintMarginRatio.isNull()) (target)->ShortMaintMarginRatio=(source)->ShortMaintMarginRatio;	\
			if(!(source)->ShortMarginStep.isNull()) (target)->ShortMarginStep=(source)->ShortMarginStep;	\
			if(!(source)->ShortMaintMarginStep.isNull()) (target)->ShortMaintMarginStep=(source)->ShortMaintMarginStep;	\
			if(!(source)->ShortRiskLimit.isNull()) (target)->ShortRiskLimit=(source)->ShortRiskLimit;	\
			if(!(source)->ShortRiskStep.isNull()) (target)->ShortRiskStep=(source)->ShortRiskStep;	\
			if(!(source)->DefaultLeverage.isNull()) (target)->DefaultLeverage=(source)->DefaultLeverage;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyCFDRateEntity
#define CopyCFDRateEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->TriggerOrderType=(source)->TriggerOrderType;	\
			(target)->RateBegin=(source)->RateBegin;	\
			(target)->RateEnd=(source)->RateEnd;	\
			(target)->CompVolume=(source)->CompVolume;	\
			(target)->CompRate=(source)->CompRate;	\
			(target)->HardRate=(source)->HardRate;	\
			(target)->Remark=(source)->Remark;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueCFDRateEntity
#define CopyTrueCFDRateEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->TriggerOrderType.isNull()) (target)->TriggerOrderType=(source)->TriggerOrderType;	\
			if(!(source)->RateBegin.isNull()) (target)->RateBegin=(source)->RateBegin;	\
			if(!(source)->RateEnd.isNull()) (target)->RateEnd=(source)->RateEnd;	\
			if(!(source)->CompVolume.isNull()) (target)->CompVolume=(source)->CompVolume;	\
			if(!(source)->CompRate.isNull()) (target)->CompRate=(source)->CompRate;	\
			if(!(source)->HardRate.isNull()) (target)->HardRate=(source)->HardRate;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyFeeEntity
#define CopyFeeEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->MatchRole=(source)->MatchRole;	\
			(target)->OpenFeeRate=(source)->OpenFeeRate;	\
			(target)->OpenFeeAmount=(source)->OpenFeeAmount;	\
			(target)->CloseFeeRate=(source)->CloseFeeRate;	\
			(target)->CloseFeeAmount=(source)->CloseFeeAmount;	\
			(target)->CloseTodayFeeRate=(source)->CloseTodayFeeRate;	\
			(target)->CloseTodayFeeAmount=(source)->CloseTodayFeeAmount;	\
			(target)->Remark=(source)->Remark;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueFeeEntity
#define CopyTrueFeeEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->MatchRole.isNull()) (target)->MatchRole=(source)->MatchRole;	\
			if(!(source)->OpenFeeRate.isNull()) (target)->OpenFeeRate=(source)->OpenFeeRate;	\
			if(!(source)->OpenFeeAmount.isNull()) (target)->OpenFeeAmount=(source)->OpenFeeAmount;	\
			if(!(source)->CloseFeeRate.isNull()) (target)->CloseFeeRate=(source)->CloseFeeRate;	\
			if(!(source)->CloseFeeAmount.isNull()) (target)->CloseFeeAmount=(source)->CloseFeeAmount;	\
			if(!(source)->CloseTodayFeeRate.isNull()) (target)->CloseTodayFeeRate=(source)->CloseTodayFeeRate;	\
			if(!(source)->CloseTodayFeeAmount.isNull()) (target)->CloseTodayFeeAmount=(source)->CloseTodayFeeAmount;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyCurrencyEntity
#define CopyCurrencyEntity(target,source){	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->Currency=(source)->Currency;	\
			(target)->Deposit=(source)->Deposit;	\
			(target)->Withdraw=(source)->Withdraw;	\
			(target)->Balance=(source)->Balance;	\
			(target)->PreBalance=(source)->PreBalance;	\
			(target)->SortNumber=(source)->SortNumber;	\
			(target)->CanReduce=(source)->CanReduce;	\
			(target)->IsTrading=(source)->IsTrading;	\
			(target)->SettleSegment=(source)->SettleSegment;	\
			(target)->MoneyTick=(source)->MoneyTick;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->LoanUse=(source)->LoanUse;	\
			(target)->LoanLimit=(source)->LoanLimit;	\
			(target)->LoanBorrow=(source)->LoanBorrow;	\
			(target)->LoanDeposit=(source)->LoanDeposit;	\
			(target)->LoanBase=(source)->LoanBase;	\
			(target)->LoanRate=(source)->LoanRate;	\
			(target)->LoanHighBase=(source)->LoanHighBase;	\
			(target)->LoanHighRate=(source)->LoanHighRate;	\
			(target)->Remark=(source)->Remark;	\
			(target)->CurrValue=(source)->CurrValue;	\
			(target)->HairCut=(source)->HairCut;	\
			(target)->MaxValue=(source)->MaxValue;	\
			(target)->MarginRateGroup=(source)->MarginRateGroup;	\
			(target)->PrivateEnd=(source)->PrivateEnd;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueCurrencyEntity
#define CopyTrueCurrencyEntity(target,source){	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
			if(!(source)->Deposit.isNull()) (target)->Deposit=(source)->Deposit;	\
			if(!(source)->Withdraw.isNull()) (target)->Withdraw=(source)->Withdraw;	\
			if(!(source)->Balance.isNull()) (target)->Balance=(source)->Balance;	\
			if(!(source)->PreBalance.isNull()) (target)->PreBalance=(source)->PreBalance;	\
			if(!(source)->SortNumber.isNull()) (target)->SortNumber=(source)->SortNumber;	\
			if(!(source)->CanReduce.isNull()) (target)->CanReduce=(source)->CanReduce;	\
			if(!(source)->IsTrading.isNull()) (target)->IsTrading=(source)->IsTrading;	\
			if(!(source)->SettleSegment.isNull()) (target)->SettleSegment=(source)->SettleSegment;	\
			if(!(source)->MoneyTick.isNull()) (target)->MoneyTick=(source)->MoneyTick;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->LoanUse.isNull()) (target)->LoanUse=(source)->LoanUse;	\
			if(!(source)->LoanLimit.isNull()) (target)->LoanLimit=(source)->LoanLimit;	\
			if(!(source)->LoanBorrow.isNull()) (target)->LoanBorrow=(source)->LoanBorrow;	\
			if(!(source)->LoanDeposit.isNull()) (target)->LoanDeposit=(source)->LoanDeposit;	\
			if(!(source)->LoanBase.isNull()) (target)->LoanBase=(source)->LoanBase;	\
			if(!(source)->LoanRate.isNull()) (target)->LoanRate=(source)->LoanRate;	\
			if(!(source)->LoanHighBase.isNull()) (target)->LoanHighBase=(source)->LoanHighBase;	\
			if(!(source)->LoanHighRate.isNull()) (target)->LoanHighRate=(source)->LoanHighRate;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->CurrValue.isNull()) (target)->CurrValue=(source)->CurrValue;	\
			if(!(source)->HairCut.isNull()) (target)->HairCut=(source)->HairCut;	\
			if(!(source)->MaxValue.isNull()) (target)->MaxValue=(source)->MaxValue;	\
			if(!(source)->MarginRateGroup.isNull()) (target)->MarginRateGroup=(source)->MarginRateGroup;	\
			if(!(source)->PrivateEnd.isNull()) (target)->PrivateEnd=(source)->PrivateEnd;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyAccountEntity
#define CopyAccountEntity(target,source){	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->Currency=(source)->Currency;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->Available=(source)->Available;	\
			(target)->Withdrawable=(source)->Withdrawable;	\
			(target)->PreBalance=(source)->PreBalance;	\
			(target)->Balance=(source)->Balance;	\
			(target)->PositionCost=(source)->PositionCost;	\
			(target)->UseMargin=(source)->UseMargin;	\
			(target)->CrossMargin=(source)->CrossMargin;	\
			(target)->CloseProfit=(source)->CloseProfit;	\
			(target)->Deposit=(source)->Deposit;	\
			(target)->Withdraw=(source)->Withdraw;	\
			(target)->FrozenMargin=(source)->FrozenMargin;	\
			(target)->FrozenMoney=(source)->FrozenMoney;	\
			(target)->FrozenFee=(source)->FrozenFee;	\
			(target)->Fee=(source)->Fee;	\
			(target)->MoneyChange=(source)->MoneyChange;	\
			(target)->Reserve=(source)->Reserve;	\
			(target)->ReserveAvailable=(source)->ReserveAvailable;	\
			(target)->ReserveLimit=(source)->ReserveLimit;	\
			(target)->ReserveProfit=(source)->ReserveProfit;	\
			(target)->ReserveType=(source)->ReserveType;	\
			(target)->ReserveActive=(source)->ReserveActive;	\
			(target)->TotalCloseProfit=(source)->TotalCloseProfit;	\
			(target)->TotalDeposit=(source)->TotalDeposit;	\
			(target)->TotalWithdraw=(source)->TotalWithdraw;	\
			(target)->TotalFee=(source)->TotalFee;	\
			(target)->TotalMoneyChange=(source)->TotalMoneyChange;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
			(target)->AccountType=(source)->AccountType;	\
			(target)->BusinessNo=(source)->BusinessNo;	\
			(target)->MarginRateGrade=(source)->MarginRateGrade;	\
			(target)->TradingRightGrade=(source)->TradingRightGrade;	\
			(target)->Frequency=(source)->Frequency;	\
			(target)->MaintMargin=(source)->MaintMargin;	\
			(target)->UnRealProfit=(source)->UnRealProfit;	\
			(target)->CopyProfit=(source)->CopyProfit;	\
			(target)->CFDType=(source)->CFDType;	\
			(target)->FeeGrade=(source)->FeeGrade;	\
			(target)->CFDGrade=(source)->CFDGrade;	\
			(target)->Borrow=(source)->Borrow;	\
			(target)->Remark=(source)->Remark;	\
			(target)->MaxLocalID=(source)->MaxLocalID;	\
			(target)->CurrValue=(source)->CurrValue;	\
			(target)->CanReduce=(source)->CanReduce;	\
	}

#endif

#ifndef CopyTrueAccountEntity
#define CopyTrueAccountEntity(target,source){	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->Available.isNull()) (target)->Available=(source)->Available;	\
			if(!(source)->Withdrawable.isNull()) (target)->Withdrawable=(source)->Withdrawable;	\
			if(!(source)->PreBalance.isNull()) (target)->PreBalance=(source)->PreBalance;	\
			if(!(source)->Balance.isNull()) (target)->Balance=(source)->Balance;	\
			if(!(source)->PositionCost.isNull()) (target)->PositionCost=(source)->PositionCost;	\
			if(!(source)->UseMargin.isNull()) (target)->UseMargin=(source)->UseMargin;	\
			if(!(source)->CrossMargin.isNull()) (target)->CrossMargin=(source)->CrossMargin;	\
			if(!(source)->CloseProfit.isNull()) (target)->CloseProfit=(source)->CloseProfit;	\
			if(!(source)->Deposit.isNull()) (target)->Deposit=(source)->Deposit;	\
			if(!(source)->Withdraw.isNull()) (target)->Withdraw=(source)->Withdraw;	\
			if(!(source)->FrozenMargin.isNull()) (target)->FrozenMargin=(source)->FrozenMargin;	\
			if(!(source)->FrozenMoney.isNull()) (target)->FrozenMoney=(source)->FrozenMoney;	\
			if(!(source)->FrozenFee.isNull()) (target)->FrozenFee=(source)->FrozenFee;	\
			if(!(source)->Fee.isNull()) (target)->Fee=(source)->Fee;	\
			if(!(source)->MoneyChange.isNull()) (target)->MoneyChange=(source)->MoneyChange;	\
			if(!(source)->Reserve.isNull()) (target)->Reserve=(source)->Reserve;	\
			if(!(source)->ReserveAvailable.isNull()) (target)->ReserveAvailable=(source)->ReserveAvailable;	\
			if(!(source)->ReserveLimit.isNull()) (target)->ReserveLimit=(source)->ReserveLimit;	\
			if(!(source)->ReserveProfit.isNull()) (target)->ReserveProfit=(source)->ReserveProfit;	\
			if(!(source)->ReserveType.isNull()) (target)->ReserveType=(source)->ReserveType;	\
			if(!(source)->ReserveActive.isNull()) (target)->ReserveActive=(source)->ReserveActive;	\
			if(!(source)->TotalCloseProfit.isNull()) (target)->TotalCloseProfit=(source)->TotalCloseProfit;	\
			if(!(source)->TotalDeposit.isNull()) (target)->TotalDeposit=(source)->TotalDeposit;	\
			if(!(source)->TotalWithdraw.isNull()) (target)->TotalWithdraw=(source)->TotalWithdraw;	\
			if(!(source)->TotalFee.isNull()) (target)->TotalFee=(source)->TotalFee;	\
			if(!(source)->TotalMoneyChange.isNull()) (target)->TotalMoneyChange=(source)->TotalMoneyChange;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
			if(!(source)->AccountType.isNull()) (target)->AccountType=(source)->AccountType;	\
			if(!(source)->BusinessNo.isNull()) (target)->BusinessNo=(source)->BusinessNo;	\
			if(!(source)->MarginRateGrade.isNull()) (target)->MarginRateGrade=(source)->MarginRateGrade;	\
			if(!(source)->TradingRightGrade.isNull()) (target)->TradingRightGrade=(source)->TradingRightGrade;	\
			if(!(source)->Frequency.isNull()) (target)->Frequency=(source)->Frequency;	\
			if(!(source)->MaintMargin.isNull()) (target)->MaintMargin=(source)->MaintMargin;	\
			if(!(source)->UnRealProfit.isNull()) (target)->UnRealProfit=(source)->UnRealProfit;	\
			if(!(source)->CopyProfit.isNull()) (target)->CopyProfit=(source)->CopyProfit;	\
			if(!(source)->CFDType.isNull()) (target)->CFDType=(source)->CFDType;	\
			if(!(source)->FeeGrade.isNull()) (target)->FeeGrade=(source)->FeeGrade;	\
			if(!(source)->CFDGrade.isNull()) (target)->CFDGrade=(source)->CFDGrade;	\
			if(!(source)->Borrow.isNull()) (target)->Borrow=(source)->Borrow;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->MaxLocalID.isNull()) (target)->MaxLocalID=(source)->MaxLocalID;	\
			if(!(source)->CurrValue.isNull()) (target)->CurrValue=(source)->CurrValue;	\
			if(!(source)->CanReduce.isNull()) (target)->CanReduce=(source)->CanReduce;	\
	}

#endif

#ifndef CopyAccountActionEntity
#define CopyAccountActionEntity(target,source){	\
			(target)->ActionLocalID=(source)->ActionLocalID;	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->OrderID=(source)->OrderID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->OtherAccountID=(source)->OtherAccountID;	\
			(target)->OtherSettlementGroup=(source)->OtherSettlementGroup;	\
			(target)->OtherMemberID=(source)->OtherMemberID;	\
			(target)->Amount=(source)->Amount;	\
			(target)->FrozenMoney=(source)->FrozenMoney;	\
			(target)->Currency=(source)->Currency;	\
			(target)->ActionType=(source)->ActionType;	\
			(target)->IsReserve=(source)->IsReserve;	\
			(target)->AccountType=(source)->AccountType;	\
			(target)->MarginRateGrade=(source)->MarginRateGrade;	\
			(target)->TradingRightGrade=(source)->TradingRightGrade;	\
			(target)->Price=(source)->Price;	\
			(target)->Volume=(source)->Volume;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Source=(source)->Source;	\
			(target)->RelatedID=(source)->RelatedID;	\
			(target)->FeeDeduceRate=(source)->FeeDeduceRate;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueAccountActionEntity
#define CopyTrueAccountActionEntity(target,source){	\
			if(!(source)->ActionLocalID.isNull()) (target)->ActionLocalID=(source)->ActionLocalID;	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->OrderID.isNull()) (target)->OrderID=(source)->OrderID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->OtherAccountID.isNull()) (target)->OtherAccountID=(source)->OtherAccountID;	\
			if(!(source)->OtherSettlementGroup.isNull()) (target)->OtherSettlementGroup=(source)->OtherSettlementGroup;	\
			if(!(source)->OtherMemberID.isNull()) (target)->OtherMemberID=(source)->OtherMemberID;	\
			if(!(source)->Amount.isNull()) (target)->Amount=(source)->Amount;	\
			if(!(source)->FrozenMoney.isNull()) (target)->FrozenMoney=(source)->FrozenMoney;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
			if(!(source)->ActionType.isNull()) (target)->ActionType=(source)->ActionType;	\
			if(!(source)->IsReserve.isNull()) (target)->IsReserve=(source)->IsReserve;	\
			if(!(source)->AccountType.isNull()) (target)->AccountType=(source)->AccountType;	\
			if(!(source)->MarginRateGrade.isNull()) (target)->MarginRateGrade=(source)->MarginRateGrade;	\
			if(!(source)->TradingRightGrade.isNull()) (target)->TradingRightGrade=(source)->TradingRightGrade;	\
			if(!(source)->Price.isNull()) (target)->Price=(source)->Price;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Source.isNull()) (target)->Source=(source)->Source;	\
			if(!(source)->RelatedID.isNull()) (target)->RelatedID=(source)->RelatedID;	\
			if(!(source)->FeeDeduceRate.isNull()) (target)->FeeDeduceRate=(source)->FeeDeduceRate;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyDepositEntity
#define CopyDepositEntity(target,source){	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->Amount=(source)->Amount;	\
			(target)->Currency=(source)->Currency;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueDepositEntity
#define CopyTrueDepositEntity(target,source){	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->Amount.isNull()) (target)->Amount=(source)->Amount;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyWithdrawEntity
#define CopyWithdrawEntity(target,source){	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->OrderID=(source)->OrderID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->OtherSettlementGroup=(source)->OtherSettlementGroup;	\
			(target)->Amount=(source)->Amount;	\
			(target)->Currency=(source)->Currency;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueWithdrawEntity
#define CopyTrueWithdrawEntity(target,source){	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->OrderID.isNull()) (target)->OrderID=(source)->OrderID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->OtherSettlementGroup.isNull()) (target)->OtherSettlementGroup=(source)->OtherSettlementGroup;	\
			if(!(source)->Amount.isNull()) (target)->Amount=(source)->Amount;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTransferEntity
#define CopyTransferEntity(target,source){	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->OtherAccountID=(source)->OtherAccountID;	\
			(target)->OtherSettlementGroup=(source)->OtherSettlementGroup;	\
			(target)->OtherMemberID=(source)->OtherMemberID;	\
			(target)->Amount=(source)->Amount;	\
			(target)->Currency=(source)->Currency;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueTransferEntity
#define CopyTrueTransferEntity(target,source){	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->OtherAccountID.isNull()) (target)->OtherAccountID=(source)->OtherAccountID;	\
			if(!(source)->OtherSettlementGroup.isNull()) (target)->OtherSettlementGroup=(source)->OtherSettlementGroup;	\
			if(!(source)->OtherMemberID.isNull()) (target)->OtherMemberID=(source)->OtherMemberID;	\
			if(!(source)->Amount.isNull()) (target)->Amount=(source)->Amount;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyAccountDetailEntity
#define CopyAccountDetailEntity(target,source){	\
			(target)->AccountDetailID=(source)->AccountDetailID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->Currency=(source)->Currency;	\
			(target)->Amount=(source)->Amount;	\
			(target)->PreBalance=(source)->PreBalance;	\
			(target)->Balance=(source)->Balance;	\
			(target)->ReserveAmount=(source)->ReserveAmount;	\
			(target)->ReserveBalance=(source)->ReserveBalance;	\
			(target)->Source=(source)->Source;	\
			(target)->Remark=(source)->Remark;	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->SettleSegment=(source)->SettleSegment;	\
			(target)->BusinessNo=(source)->BusinessNo;	\
			(target)->RelatedID=(source)->RelatedID;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->CreateTime=(source)->CreateTime;	\
	}

#endif

#ifndef CopyTrueAccountDetailEntity
#define CopyTrueAccountDetailEntity(target,source){	\
			if(!(source)->AccountDetailID.isNull()) (target)->AccountDetailID=(source)->AccountDetailID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
			if(!(source)->Amount.isNull()) (target)->Amount=(source)->Amount;	\
			if(!(source)->PreBalance.isNull()) (target)->PreBalance=(source)->PreBalance;	\
			if(!(source)->Balance.isNull()) (target)->Balance=(source)->Balance;	\
			if(!(source)->ReserveAmount.isNull()) (target)->ReserveAmount=(source)->ReserveAmount;	\
			if(!(source)->ReserveBalance.isNull()) (target)->ReserveBalance=(source)->ReserveBalance;	\
			if(!(source)->Source.isNull()) (target)->Source=(source)->Source;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->SettleSegment.isNull()) (target)->SettleSegment=(source)->SettleSegment;	\
			if(!(source)->BusinessNo.isNull()) (target)->BusinessNo=(source)->BusinessNo;	\
			if(!(source)->RelatedID.isNull()) (target)->RelatedID=(source)->RelatedID;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->CreateTime.isNull()) (target)->CreateTime=(source)->CreateTime;	\
	}

#endif

#ifndef CopySettlementEntity
#define CopySettlementEntity(target,source){	\
			(target)->APPID=(source)->APPID;	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->SettleAction=(source)->SettleAction;	\
			(target)->Value=(source)->Value;	\
			(target)->Value1=(source)->Value1;	\
			(target)->Value2=(source)->Value2;	\
			(target)->Value3=(source)->Value3;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueSettlementEntity
#define CopyTrueSettlementEntity(target,source){	\
			if(!(source)->APPID.isNull()) (target)->APPID=(source)->APPID;	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->SettleAction.isNull()) (target)->SettleAction=(source)->SettleAction;	\
			if(!(source)->Value.isNull()) (target)->Value=(source)->Value;	\
			if(!(source)->Value1.isNull()) (target)->Value1=(source)->Value1;	\
			if(!(source)->Value2.isNull()) (target)->Value2=(source)->Value2;	\
			if(!(source)->Value3.isNull()) (target)->Value3=(source)->Value3;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopySettleDetailEntity
#define CopySettleDetailEntity(target,source){	\
			(target)->SettleDetailID=(source)->SettleDetailID;	\
			CopySettlementEntity(target,source);	\
			(target)->FundingRateGroup=(source)->FundingRateGroup;	\
			(target)->SettleSegment=(source)->SettleSegment;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->BusinessNo=(source)->BusinessNo;	\
	}

#endif

#ifndef CopyTrueSettleDetailEntity
#define CopyTrueSettleDetailEntity(target,source){	\
			if(!(source)->SettleDetailID.isNull()) (target)->SettleDetailID=(source)->SettleDetailID;	\
			CopyTrueSettlementEntity(target,source);	\
			if(!(source)->FundingRateGroup.isNull()) (target)->FundingRateGroup=(source)->FundingRateGroup;	\
			if(!(source)->SettleSegment.isNull()) (target)->SettleSegment=(source)->SettleSegment;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->BusinessNo.isNull()) (target)->BusinessNo=(source)->BusinessNo;	\
	}

#endif

#ifndef CopyLiquidityEntity
#define CopyLiquidityEntity(target,source){	\
			(target)->LiquidityGrade=(source)->LiquidityGrade;	\
			(target)->LiquidityGroup=(source)->LiquidityGroup;	\
			(target)->BasePriceType=(source)->BasePriceType;	\
			(target)->VolumeBegin=(source)->VolumeBegin;	\
			(target)->BuyOpen=(source)->BuyOpen;	\
			(target)->BuyClose=(source)->BuyClose;	\
			(target)->SellOpen=(source)->SellOpen;	\
			(target)->SellClose=(source)->SellClose;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueLiquidityEntity
#define CopyTrueLiquidityEntity(target,source){	\
			if(!(source)->LiquidityGrade.isNull()) (target)->LiquidityGrade=(source)->LiquidityGrade;	\
			if(!(source)->LiquidityGroup.isNull()) (target)->LiquidityGroup=(source)->LiquidityGroup;	\
			if(!(source)->BasePriceType.isNull()) (target)->BasePriceType=(source)->BasePriceType;	\
			if(!(source)->VolumeBegin.isNull()) (target)->VolumeBegin=(source)->VolumeBegin;	\
			if(!(source)->BuyOpen.isNull()) (target)->BuyOpen=(source)->BuyOpen;	\
			if(!(source)->BuyClose.isNull()) (target)->BuyClose=(source)->BuyClose;	\
			if(!(source)->SellOpen.isNull()) (target)->SellOpen=(source)->SellOpen;	\
			if(!(source)->SellClose.isNull()) (target)->SellClose=(source)->SellClose;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyBasePositionEntity
#define CopyBasePositionEntity(target,source){	\
			(target)->PosiDirection=(source)->PosiDirection;	\
			(target)->PrePosition=(source)->PrePosition;	\
			(target)->Position=(source)->Position;	\
			(target)->LongFrozen=(source)->LongFrozen;	\
			(target)->ShortFrozen=(source)->ShortFrozen;	\
			(target)->PreLongFrozen=(source)->PreLongFrozen;	\
			(target)->PreShortFrozen=(source)->PreShortFrozen;	\
	}

#endif

#ifndef CopyTrueBasePositionEntity
#define CopyTrueBasePositionEntity(target,source){	\
			if(!(source)->PosiDirection.isNull()) (target)->PosiDirection=(source)->PosiDirection;	\
			if(!(source)->PrePosition.isNull()) (target)->PrePosition=(source)->PrePosition;	\
			if(!(source)->Position.isNull()) (target)->Position=(source)->Position;	\
			if(!(source)->LongFrozen.isNull()) (target)->LongFrozen=(source)->LongFrozen;	\
			if(!(source)->ShortFrozen.isNull()) (target)->ShortFrozen=(source)->ShortFrozen;	\
			if(!(source)->PreLongFrozen.isNull()) (target)->PreLongFrozen=(source)->PreLongFrozen;	\
			if(!(source)->PreShortFrozen.isNull()) (target)->PreShortFrozen=(source)->PreShortFrozen;	\
	}

#endif

#ifndef CopyMemberPositionEntity
#define CopyMemberPositionEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Position=(source)->Position;	\
			(target)->LongFrozen=(source)->LongFrozen;	\
			(target)->ShortFrozen=(source)->ShortFrozen;	\
			(target)->DefaultLeverage=(source)->DefaultLeverage;	\
			(target)->DefaultCrossMargin=(source)->DefaultCrossMargin;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueMemberPositionEntity
#define CopyTrueMemberPositionEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Position.isNull()) (target)->Position=(source)->Position;	\
			if(!(source)->LongFrozen.isNull()) (target)->LongFrozen=(source)->LongFrozen;	\
			if(!(source)->ShortFrozen.isNull()) (target)->ShortFrozen=(source)->ShortFrozen;	\
			if(!(source)->DefaultLeverage.isNull()) (target)->DefaultLeverage=(source)->DefaultLeverage;	\
			if(!(source)->DefaultCrossMargin.isNull()) (target)->DefaultCrossMargin=(source)->DefaultCrossMargin;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyPositionEntity
#define CopyPositionEntity(target,source){	\
			(target)->PositionID=(source)->PositionID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->TradeUnitID=(source)->TradeUnitID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->PositionClass=(source)->PositionClass;	\
			CopyBasePositionEntity(target,source);	\
			(target)->HighestPosition=(source)->HighestPosition;	\
			(target)->ClosePosition=(source)->ClosePosition;	\
			(target)->PositionCost=(source)->PositionCost;	\
			(target)->CostPrice=(source)->CostPrice;	\
			(target)->UseMargin=(source)->UseMargin;	\
			(target)->FrozenMargin=(source)->FrozenMargin;	\
			(target)->LongFrozenMargin=(source)->LongFrozenMargin;	\
			(target)->ShortFrozenMargin=(source)->ShortFrozenMargin;	\
			(target)->CloseProfit=(source)->CloseProfit;	\
			(target)->TotalPositionCost=(source)->TotalPositionCost;	\
			(target)->TotalCloseProfit=(source)->TotalCloseProfit;	\
			(target)->OpenPrice=(source)->OpenPrice;	\
			(target)->ClosePrice=(source)->ClosePrice;	\
			(target)->TradeFee=(source)->TradeFee;	\
			(target)->PositionFee=(source)->PositionFee;	\
			(target)->Leverage=(source)->Leverage;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->PriceCurrency=(source)->PriceCurrency;	\
			(target)->ClearCurrency=(source)->ClearCurrency;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->IsCrossMargin=(source)->IsCrossMargin;	\
			(target)->CloseOrderID=(source)->CloseOrderID;	\
			(target)->SLTriggerPrice=(source)->SLTriggerPrice;	\
			(target)->TPTriggerPrice=(source)->TPTriggerPrice;	\
			(target)->BeginTime=(source)->BeginTime;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->LastOpenTime=(source)->LastOpenTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
			(target)->BusinessNo=(source)->BusinessNo;	\
			(target)->IsAutoAddMargin=(source)->IsAutoAddMargin;	\
			(target)->Frequency=(source)->Frequency;	\
			(target)->MaintMargin=(source)->MaintMargin;	\
			(target)->UnRealProfit=(source)->UnRealProfit;	\
			(target)->LiquidPrice=(source)->LiquidPrice;	\
			(target)->CreateTime=(source)->CreateTime;	\
			(target)->CopyMemberID=(source)->CopyMemberID;	\
			(target)->CopyProfitRate=(source)->CopyProfitRate;	\
			(target)->CopyProfit=(source)->CopyProfit;	\
			(target)->FirstTradeID=(source)->FirstTradeID;	\
			(target)->LastTradeID=(source)->LastTradeID;	\
			(target)->BusinessType=(source)->BusinessType;	\
			(target)->BusinessValue=(source)->BusinessValue;	\
			(target)->Reserve=(source)->Reserve;	\
			(target)->ReserveProfit=(source)->ReserveProfit;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTruePositionEntity
#define CopyTruePositionEntity(target,source){	\
			if(!(source)->PositionID.isNull()) (target)->PositionID=(source)->PositionID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->TradeUnitID.isNull()) (target)->TradeUnitID=(source)->TradeUnitID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->PositionClass.isNull()) (target)->PositionClass=(source)->PositionClass;	\
			CopyTrueBasePositionEntity(target,source);	\
			if(!(source)->HighestPosition.isNull()) (target)->HighestPosition=(source)->HighestPosition;	\
			if(!(source)->ClosePosition.isNull()) (target)->ClosePosition=(source)->ClosePosition;	\
			if(!(source)->PositionCost.isNull()) (target)->PositionCost=(source)->PositionCost;	\
			if(!(source)->CostPrice.isNull()) (target)->CostPrice=(source)->CostPrice;	\
			if(!(source)->UseMargin.isNull()) (target)->UseMargin=(source)->UseMargin;	\
			if(!(source)->FrozenMargin.isNull()) (target)->FrozenMargin=(source)->FrozenMargin;	\
			if(!(source)->LongFrozenMargin.isNull()) (target)->LongFrozenMargin=(source)->LongFrozenMargin;	\
			if(!(source)->ShortFrozenMargin.isNull()) (target)->ShortFrozenMargin=(source)->ShortFrozenMargin;	\
			if(!(source)->CloseProfit.isNull()) (target)->CloseProfit=(source)->CloseProfit;	\
			if(!(source)->TotalPositionCost.isNull()) (target)->TotalPositionCost=(source)->TotalPositionCost;	\
			if(!(source)->TotalCloseProfit.isNull()) (target)->TotalCloseProfit=(source)->TotalCloseProfit;	\
			if(!(source)->OpenPrice.isNull()) (target)->OpenPrice=(source)->OpenPrice;	\
			if(!(source)->ClosePrice.isNull()) (target)->ClosePrice=(source)->ClosePrice;	\
			if(!(source)->TradeFee.isNull()) (target)->TradeFee=(source)->TradeFee;	\
			if(!(source)->PositionFee.isNull()) (target)->PositionFee=(source)->PositionFee;	\
			if(!(source)->Leverage.isNull()) (target)->Leverage=(source)->Leverage;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->PriceCurrency.isNull()) (target)->PriceCurrency=(source)->PriceCurrency;	\
			if(!(source)->ClearCurrency.isNull()) (target)->ClearCurrency=(source)->ClearCurrency;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->IsCrossMargin.isNull()) (target)->IsCrossMargin=(source)->IsCrossMargin;	\
			if(!(source)->CloseOrderID.isNull()) (target)->CloseOrderID=(source)->CloseOrderID;	\
			if(!(source)->SLTriggerPrice.isNull()) (target)->SLTriggerPrice=(source)->SLTriggerPrice;	\
			if(!(source)->TPTriggerPrice.isNull()) (target)->TPTriggerPrice=(source)->TPTriggerPrice;	\
			if(!(source)->BeginTime.isNull()) (target)->BeginTime=(source)->BeginTime;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->LastOpenTime.isNull()) (target)->LastOpenTime=(source)->LastOpenTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
			if(!(source)->BusinessNo.isNull()) (target)->BusinessNo=(source)->BusinessNo;	\
			if(!(source)->IsAutoAddMargin.isNull()) (target)->IsAutoAddMargin=(source)->IsAutoAddMargin;	\
			if(!(source)->Frequency.isNull()) (target)->Frequency=(source)->Frequency;	\
			if(!(source)->MaintMargin.isNull()) (target)->MaintMargin=(source)->MaintMargin;	\
			if(!(source)->UnRealProfit.isNull()) (target)->UnRealProfit=(source)->UnRealProfit;	\
			if(!(source)->LiquidPrice.isNull()) (target)->LiquidPrice=(source)->LiquidPrice;	\
			if(!(source)->CreateTime.isNull()) (target)->CreateTime=(source)->CreateTime;	\
			if(!(source)->CopyMemberID.isNull()) (target)->CopyMemberID=(source)->CopyMemberID;	\
			if(!(source)->CopyProfitRate.isNull()) (target)->CopyProfitRate=(source)->CopyProfitRate;	\
			if(!(source)->CopyProfit.isNull()) (target)->CopyProfit=(source)->CopyProfit;	\
			if(!(source)->FirstTradeID.isNull()) (target)->FirstTradeID=(source)->FirstTradeID;	\
			if(!(source)->LastTradeID.isNull()) (target)->LastTradeID=(source)->LastTradeID;	\
			if(!(source)->BusinessType.isNull()) (target)->BusinessType=(source)->BusinessType;	\
			if(!(source)->BusinessValue.isNull()) (target)->BusinessValue=(source)->BusinessValue;	\
			if(!(source)->Reserve.isNull()) (target)->Reserve=(source)->Reserve;	\
			if(!(source)->ReserveProfit.isNull()) (target)->ReserveProfit=(source)->ReserveProfit;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyFinishPositionEntity
#define CopyFinishPositionEntity(target,source){	\
			CopyPositionEntity(target,source);	\
	}

#endif

#ifndef CopyTrueFinishPositionEntity
#define CopyTrueFinishPositionEntity(target,source){	\
			CopyTruePositionEntity(target,source);	\
	}

#endif

#ifndef CopyPositionMergeEntity
#define CopyPositionMergeEntity(target,source){	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->PositionID=(source)->PositionID;	\
			(target)->PositionID1=(source)->PositionID1;	\
			(target)->PositionID2=(source)->PositionID2;	\
			(target)->PositionID3=(source)->PositionID3;	\
			(target)->PositionID4=(source)->PositionID4;	\
			(target)->PositionID5=(source)->PositionID5;	\
			(target)->PositionID6=(source)->PositionID6;	\
			(target)->PositionID7=(source)->PositionID7;	\
			(target)->PositionID8=(source)->PositionID8;	\
			(target)->PositionID9=(source)->PositionID9;	\
			(target)->PositionID10=(source)->PositionID10;	\
	}

#endif

#ifndef CopyTruePositionMergeEntity
#define CopyTruePositionMergeEntity(target,source){	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->PositionID.isNull()) (target)->PositionID=(source)->PositionID;	\
			if(!(source)->PositionID1.isNull()) (target)->PositionID1=(source)->PositionID1;	\
			if(!(source)->PositionID2.isNull()) (target)->PositionID2=(source)->PositionID2;	\
			if(!(source)->PositionID3.isNull()) (target)->PositionID3=(source)->PositionID3;	\
			if(!(source)->PositionID4.isNull()) (target)->PositionID4=(source)->PositionID4;	\
			if(!(source)->PositionID5.isNull()) (target)->PositionID5=(source)->PositionID5;	\
			if(!(source)->PositionID6.isNull()) (target)->PositionID6=(source)->PositionID6;	\
			if(!(source)->PositionID7.isNull()) (target)->PositionID7=(source)->PositionID7;	\
			if(!(source)->PositionID8.isNull()) (target)->PositionID8=(source)->PositionID8;	\
			if(!(source)->PositionID9.isNull()) (target)->PositionID9=(source)->PositionID9;	\
			if(!(source)->PositionID10.isNull()) (target)->PositionID10=(source)->PositionID10;	\
	}

#endif

#ifndef CopyPositionOrderEntity
#define CopyPositionOrderEntity(target,source){	\
			CopyOrderEntity(target,source);	\
			(target)->VolumeToClose=(source)->VolumeToClose;	\
			(target)->VolumeClosed=(source)->VolumeClosed;	\
			(target)->VolumeOnClose=(source)->VolumeOnClose;	\
	}

#endif

#ifndef CopyTruePositionOrderEntity
#define CopyTruePositionOrderEntity(target,source){	\
			CopyTrueOrderEntity(target,source);	\
			if(!(source)->VolumeToClose.isNull()) (target)->VolumeToClose=(source)->VolumeToClose;	\
			if(!(source)->VolumeClosed.isNull()) (target)->VolumeClosed=(source)->VolumeClosed;	\
			if(!(source)->VolumeOnClose.isNull()) (target)->VolumeOnClose=(source)->VolumeOnClose;	\
	}

#endif

#ifndef CopyInputOrderEntity
#define CopyInputOrderEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->TradeUnitID=(source)->TradeUnitID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->OrderPriceType=(source)->OrderPriceType;	\
			(target)->Direction=(source)->Direction;	\
			(target)->OffsetFlag=(source)->OffsetFlag;	\
			(target)->Price=(source)->Price;	\
			(target)->Volume=(source)->Volume;	\
			(target)->VolumeDisplay=(source)->VolumeDisplay;	\
			(target)->VolumeMode=(source)->VolumeMode;	\
			(target)->Cost=(source)->Cost;	\
			(target)->OrderType=(source)->OrderType;	\
			(target)->GTDTime=(source)->GTDTime;	\
			(target)->MinVolume=(source)->MinVolume;	\
			(target)->BusinessType=(source)->BusinessType;	\
			(target)->BusinessValue=(source)->BusinessValue;	\
			(target)->CloseOrderID=(source)->CloseOrderID;	\
			(target)->IsCrossMargin=(source)->IsCrossMargin;	\
			(target)->Remark=(source)->Remark;	\
			(target)->OrderID=(source)->OrderID;	\
			(target)->CopyMemberID=(source)->CopyMemberID;	\
			(target)->CopyOrderID=(source)->CopyOrderID;	\
			(target)->Leverage=(source)->Leverage;	\
			(target)->CopyProfitRate=(source)->CopyProfitRate;	\
			(target)->APPID=(source)->APPID;	\
			(target)->PositionID=(source)->PositionID;	\
			(target)->TriggerPrice=(source)->TriggerPrice;	\
			(target)->Reserve=(source)->Reserve;	\
	}

#endif

#ifndef CopyTrueInputOrderEntity
#define CopyTrueInputOrderEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->TradeUnitID.isNull()) (target)->TradeUnitID=(source)->TradeUnitID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->OrderPriceType.isNull()) (target)->OrderPriceType=(source)->OrderPriceType;	\
			if(!(source)->Direction.isNull()) (target)->Direction=(source)->Direction;	\
			if(!(source)->OffsetFlag.isNull()) (target)->OffsetFlag=(source)->OffsetFlag;	\
			if(!(source)->Price.isNull()) (target)->Price=(source)->Price;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->VolumeDisplay.isNull()) (target)->VolumeDisplay=(source)->VolumeDisplay;	\
			if(!(source)->VolumeMode.isNull()) (target)->VolumeMode=(source)->VolumeMode;	\
			if(!(source)->Cost.isNull()) (target)->Cost=(source)->Cost;	\
			if(!(source)->OrderType.isNull()) (target)->OrderType=(source)->OrderType;	\
			if(!(source)->GTDTime.isNull()) (target)->GTDTime=(source)->GTDTime;	\
			if(!(source)->MinVolume.isNull()) (target)->MinVolume=(source)->MinVolume;	\
			if(!(source)->BusinessType.isNull()) (target)->BusinessType=(source)->BusinessType;	\
			if(!(source)->BusinessValue.isNull()) (target)->BusinessValue=(source)->BusinessValue;	\
			if(!(source)->CloseOrderID.isNull()) (target)->CloseOrderID=(source)->CloseOrderID;	\
			if(!(source)->IsCrossMargin.isNull()) (target)->IsCrossMargin=(source)->IsCrossMargin;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->OrderID.isNull()) (target)->OrderID=(source)->OrderID;	\
			if(!(source)->CopyMemberID.isNull()) (target)->CopyMemberID=(source)->CopyMemberID;	\
			if(!(source)->CopyOrderID.isNull()) (target)->CopyOrderID=(source)->CopyOrderID;	\
			if(!(source)->Leverage.isNull()) (target)->Leverage=(source)->Leverage;	\
			if(!(source)->CopyProfitRate.isNull()) (target)->CopyProfitRate=(source)->CopyProfitRate;	\
			if(!(source)->APPID.isNull()) (target)->APPID=(source)->APPID;	\
			if(!(source)->PositionID.isNull()) (target)->PositionID=(source)->PositionID;	\
			if(!(source)->TriggerPrice.isNull()) (target)->TriggerPrice=(source)->TriggerPrice;	\
			if(!(source)->Reserve.isNull()) (target)->Reserve=(source)->Reserve;	\
	}

#endif

#ifndef CopyTriggerOrderInsertEntity
#define CopyTriggerOrderInsertEntity(target,source){	\
			CopyInputOrderEntity(target,source);	\
			(target)->SLPrice=(source)->SLPrice;	\
			(target)->SLTriggerPrice=(source)->SLTriggerPrice;	\
			(target)->TPPrice=(source)->TPPrice;	\
			(target)->TPTriggerPrice=(source)->TPTriggerPrice;	\
			(target)->RiskBefore=(source)->RiskBefore;	\
			(target)->TriggerOrderType=(source)->TriggerOrderType;	\
			(target)->TriggerDetail=(source)->TriggerDetail;	\
			(target)->TriggerPriceType=(source)->TriggerPriceType;	\
			(target)->TriggerValue=(source)->TriggerValue;	\
			(target)->CloseSLPrice=(source)->CloseSLPrice;	\
			(target)->CloseSLTriggerPrice=(source)->CloseSLTriggerPrice;	\
			(target)->CloseTPPrice=(source)->CloseTPPrice;	\
			(target)->CloseTPTriggerPrice=(source)->CloseTPTriggerPrice;	\
			(target)->CloseOrderPriceType=(source)->CloseOrderPriceType;	\
			(target)->ClosePrice=(source)->ClosePrice;	\
			(target)->CloseTriggerPrice=(source)->CloseTriggerPrice;	\
			(target)->RelatedOrderID=(source)->RelatedOrderID;	\
			(target)->ActiveTime=(source)->ActiveTime;	\
			(target)->TriggerTime=(source)->TriggerTime;	\
	}

#endif

#ifndef CopyTrueTriggerOrderInsertEntity
#define CopyTrueTriggerOrderInsertEntity(target,source){	\
			CopyTrueInputOrderEntity(target,source);	\
			if(!(source)->SLPrice.isNull()) (target)->SLPrice=(source)->SLPrice;	\
			if(!(source)->SLTriggerPrice.isNull()) (target)->SLTriggerPrice=(source)->SLTriggerPrice;	\
			if(!(source)->TPPrice.isNull()) (target)->TPPrice=(source)->TPPrice;	\
			if(!(source)->TPTriggerPrice.isNull()) (target)->TPTriggerPrice=(source)->TPTriggerPrice;	\
			if(!(source)->RiskBefore.isNull()) (target)->RiskBefore=(source)->RiskBefore;	\
			if(!(source)->TriggerOrderType.isNull()) (target)->TriggerOrderType=(source)->TriggerOrderType;	\
			if(!(source)->TriggerDetail.isNull()) (target)->TriggerDetail=(source)->TriggerDetail;	\
			if(!(source)->TriggerPriceType.isNull()) (target)->TriggerPriceType=(source)->TriggerPriceType;	\
			if(!(source)->TriggerValue.isNull()) (target)->TriggerValue=(source)->TriggerValue;	\
			if(!(source)->CloseSLPrice.isNull()) (target)->CloseSLPrice=(source)->CloseSLPrice;	\
			if(!(source)->CloseSLTriggerPrice.isNull()) (target)->CloseSLTriggerPrice=(source)->CloseSLTriggerPrice;	\
			if(!(source)->CloseTPPrice.isNull()) (target)->CloseTPPrice=(source)->CloseTPPrice;	\
			if(!(source)->CloseTPTriggerPrice.isNull()) (target)->CloseTPTriggerPrice=(source)->CloseTPTriggerPrice;	\
			if(!(source)->CloseOrderPriceType.isNull()) (target)->CloseOrderPriceType=(source)->CloseOrderPriceType;	\
			if(!(source)->ClosePrice.isNull()) (target)->ClosePrice=(source)->ClosePrice;	\
			if(!(source)->CloseTriggerPrice.isNull()) (target)->CloseTriggerPrice=(source)->CloseTriggerPrice;	\
			if(!(source)->RelatedOrderID.isNull()) (target)->RelatedOrderID=(source)->RelatedOrderID;	\
			if(!(source)->ActiveTime.isNull()) (target)->ActiveTime=(source)->ActiveTime;	\
			if(!(source)->TriggerTime.isNull()) (target)->TriggerTime=(source)->TriggerTime;	\
	}

#endif

#ifndef CopyTriggerOrderEntity
#define CopyTriggerOrderEntity(target,source){	\
			CopyTriggerOrderInsertEntity(target,source);	\
			(target)->TimeSortNo=(source)->TimeSortNo;	\
			(target)->TriggerStatus=(source)->TriggerStatus;	\
			(target)->PosiDirection=(source)->PosiDirection;	\
			(target)->FrontNo=(source)->FrontNo;	\
			(target)->ErrorNo=(source)->ErrorNo;	\
			(target)->ErrorMsg=(source)->ErrorMsg;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
			(target)->BusinessNo=(source)->BusinessNo;	\
			(target)->CreateTime=(source)->CreateTime;	\
	}

#endif

#ifndef CopyTrueTriggerOrderEntity
#define CopyTrueTriggerOrderEntity(target,source){	\
			CopyTrueTriggerOrderInsertEntity(target,source);	\
			if(!(source)->TimeSortNo.isNull()) (target)->TimeSortNo=(source)->TimeSortNo;	\
			if(!(source)->TriggerStatus.isNull()) (target)->TriggerStatus=(source)->TriggerStatus;	\
			if(!(source)->PosiDirection.isNull()) (target)->PosiDirection=(source)->PosiDirection;	\
			if(!(source)->FrontNo.isNull()) (target)->FrontNo=(source)->FrontNo;	\
			if(!(source)->ErrorNo.isNull()) (target)->ErrorNo=(source)->ErrorNo;	\
			if(!(source)->ErrorMsg.isNull()) (target)->ErrorMsg=(source)->ErrorMsg;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
			if(!(source)->BusinessNo.isNull()) (target)->BusinessNo=(source)->BusinessNo;	\
			if(!(source)->CreateTime.isNull()) (target)->CreateTime=(source)->CreateTime;	\
	}

#endif

#ifndef CopyFinishTriggerOrderEntity
#define CopyFinishTriggerOrderEntity(target,source){	\
			CopyTriggerOrderEntity(target,source);	\
	}

#endif

#ifndef CopyTrueFinishTriggerOrderEntity
#define CopyTrueFinishTriggerOrderEntity(target,source){	\
			CopyTrueTriggerOrderEntity(target,source);	\
	}

#endif

#ifndef CopyCloseOrderInsertEntity
#define CopyCloseOrderInsertEntity(target,source){	\
			CopyInputOrderEntity(target,source);	\
			(target)->CloseSLPrice=(source)->CloseSLPrice;	\
			(target)->SLTriggerPrice=(source)->SLTriggerPrice;	\
			(target)->CloseSLTriggerPrice=(source)->CloseSLTriggerPrice;	\
			(target)->CloseTPPrice=(source)->CloseTPPrice;	\
			(target)->TPTriggerPrice=(source)->TPTriggerPrice;	\
			(target)->CloseTPTriggerPrice=(source)->CloseTPTriggerPrice;	\
			(target)->CloseOrderPriceType=(source)->CloseOrderPriceType;	\
			(target)->ClosePrice=(source)->ClosePrice;	\
			(target)->CloseTriggerPrice=(source)->CloseTriggerPrice;	\
			(target)->CloseTriggerPriceType=(source)->CloseTriggerPriceType;	\
	}

#endif

#ifndef CopyTrueCloseOrderInsertEntity
#define CopyTrueCloseOrderInsertEntity(target,source){	\
			CopyTrueInputOrderEntity(target,source);	\
			if(!(source)->CloseSLPrice.isNull()) (target)->CloseSLPrice=(source)->CloseSLPrice;	\
			if(!(source)->SLTriggerPrice.isNull()) (target)->SLTriggerPrice=(source)->SLTriggerPrice;	\
			if(!(source)->CloseSLTriggerPrice.isNull()) (target)->CloseSLTriggerPrice=(source)->CloseSLTriggerPrice;	\
			if(!(source)->CloseTPPrice.isNull()) (target)->CloseTPPrice=(source)->CloseTPPrice;	\
			if(!(source)->TPTriggerPrice.isNull()) (target)->TPTriggerPrice=(source)->TPTriggerPrice;	\
			if(!(source)->CloseTPTriggerPrice.isNull()) (target)->CloseTPTriggerPrice=(source)->CloseTPTriggerPrice;	\
			if(!(source)->CloseOrderPriceType.isNull()) (target)->CloseOrderPriceType=(source)->CloseOrderPriceType;	\
			if(!(source)->ClosePrice.isNull()) (target)->ClosePrice=(source)->ClosePrice;	\
			if(!(source)->CloseTriggerPrice.isNull()) (target)->CloseTriggerPrice=(source)->CloseTriggerPrice;	\
			if(!(source)->CloseTriggerPriceType.isNull()) (target)->CloseTriggerPriceType=(source)->CloseTriggerPriceType;	\
	}

#endif

#ifndef CopyInnerOrderEntity
#define CopyInnerOrderEntity(target,source){	\
			(target)->OrderStatus=(source)->OrderStatus;	\
			(target)->DeriveSource=(source)->DeriveSource;	\
			(target)->DeriveDetail=(source)->DeriveDetail;	\
			(target)->VolumeTraded=(source)->VolumeTraded;	\
			(target)->VolumeRemain=(source)->VolumeRemain;	\
			(target)->VolumeCancled=(source)->VolumeCancled;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
			(target)->Priority=(source)->Priority;	\
			(target)->TimeSortNo=(source)->TimeSortNo;	\
			(target)->FrontNo=(source)->FrontNo;	\
			(target)->PriceCurrency=(source)->PriceCurrency;	\
			(target)->FeeCurrency=(source)->FeeCurrency;	\
			(target)->ClearCurrency=(source)->ClearCurrency;	\
			(target)->FrozenMoney=(source)->FrozenMoney;	\
			(target)->FrozenFee=(source)->FrozenFee;	\
			(target)->FrozenMargin=(source)->FrozenMargin;	\
			(target)->Fee=(source)->Fee;	\
			(target)->CloseProfit=(source)->CloseProfit;	\
			(target)->Turnover=(source)->Turnover;	\
			(target)->RelatedOrderID=(source)->RelatedOrderID;	\
			(target)->BusinessResult=(source)->BusinessResult;	\
			(target)->BusinessNo=(source)->BusinessNo;	\
			(target)->Tradable=(source)->Tradable;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->PosiDirection=(source)->PosiDirection;	\
			(target)->TradePrice=(source)->TradePrice;	\
			(target)->OpenPrice=(source)->OpenPrice;	\
			(target)->TriggerOrderID=(source)->TriggerOrderID;	\
			(target)->SLTriggerPrice=(source)->SLTriggerPrice;	\
			(target)->TPTriggerPrice=(source)->TPTriggerPrice;	\
			(target)->CopyProfit=(source)->CopyProfit;	\
			(target)->Position=(source)->Position;	\
			(target)->UserID=(source)->UserID;	\
	}

#endif

#ifndef CopyTrueInnerOrderEntity
#define CopyTrueInnerOrderEntity(target,source){	\
			if(!(source)->OrderStatus.isNull()) (target)->OrderStatus=(source)->OrderStatus;	\
			if(!(source)->DeriveSource.isNull()) (target)->DeriveSource=(source)->DeriveSource;	\
			if(!(source)->DeriveDetail.isNull()) (target)->DeriveDetail=(source)->DeriveDetail;	\
			if(!(source)->VolumeTraded.isNull()) (target)->VolumeTraded=(source)->VolumeTraded;	\
			if(!(source)->VolumeRemain.isNull()) (target)->VolumeRemain=(source)->VolumeRemain;	\
			if(!(source)->VolumeCancled.isNull()) (target)->VolumeCancled=(source)->VolumeCancled;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
			if(!(source)->Priority.isNull()) (target)->Priority=(source)->Priority;	\
			if(!(source)->TimeSortNo.isNull()) (target)->TimeSortNo=(source)->TimeSortNo;	\
			if(!(source)->FrontNo.isNull()) (target)->FrontNo=(source)->FrontNo;	\
			if(!(source)->PriceCurrency.isNull()) (target)->PriceCurrency=(source)->PriceCurrency;	\
			if(!(source)->FeeCurrency.isNull()) (target)->FeeCurrency=(source)->FeeCurrency;	\
			if(!(source)->ClearCurrency.isNull()) (target)->ClearCurrency=(source)->ClearCurrency;	\
			if(!(source)->FrozenMoney.isNull()) (target)->FrozenMoney=(source)->FrozenMoney;	\
			if(!(source)->FrozenFee.isNull()) (target)->FrozenFee=(source)->FrozenFee;	\
			if(!(source)->FrozenMargin.isNull()) (target)->FrozenMargin=(source)->FrozenMargin;	\
			if(!(source)->Fee.isNull()) (target)->Fee=(source)->Fee;	\
			if(!(source)->CloseProfit.isNull()) (target)->CloseProfit=(source)->CloseProfit;	\
			if(!(source)->Turnover.isNull()) (target)->Turnover=(source)->Turnover;	\
			if(!(source)->RelatedOrderID.isNull()) (target)->RelatedOrderID=(source)->RelatedOrderID;	\
			if(!(source)->BusinessResult.isNull()) (target)->BusinessResult=(source)->BusinessResult;	\
			if(!(source)->BusinessNo.isNull()) (target)->BusinessNo=(source)->BusinessNo;	\
			if(!(source)->Tradable.isNull()) (target)->Tradable=(source)->Tradable;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->PosiDirection.isNull()) (target)->PosiDirection=(source)->PosiDirection;	\
			if(!(source)->TradePrice.isNull()) (target)->TradePrice=(source)->TradePrice;	\
			if(!(source)->OpenPrice.isNull()) (target)->OpenPrice=(source)->OpenPrice;	\
			if(!(source)->TriggerOrderID.isNull()) (target)->TriggerOrderID=(source)->TriggerOrderID;	\
			if(!(source)->SLTriggerPrice.isNull()) (target)->SLTriggerPrice=(source)->SLTriggerPrice;	\
			if(!(source)->TPTriggerPrice.isNull()) (target)->TPTriggerPrice=(source)->TPTriggerPrice;	\
			if(!(source)->CopyProfit.isNull()) (target)->CopyProfit=(source)->CopyProfit;	\
			if(!(source)->Position.isNull()) (target)->Position=(source)->Position;	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
	}

#endif

#ifndef CopyOrderEntity
#define CopyOrderEntity(target,source){	\
			CopyInputOrderEntity(target,source);	\
			CopyInnerOrderEntity(target,source);	\
			(target)->LastPriceByInsert=(source)->LastPriceByInsert;	\
			(target)->BidPrice1ByInsert=(source)->BidPrice1ByInsert;	\
			(target)->AskPrice1ByInsert=(source)->AskPrice1ByInsert;	\
			(target)->Available=(source)->Available;	\
			(target)->CreateTime=(source)->CreateTime;	\
	}

#endif

#ifndef CopyTrueOrderEntity
#define CopyTrueOrderEntity(target,source){	\
			CopyTrueInputOrderEntity(target,source);	\
			CopyTrueInnerOrderEntity(target,source);	\
			if(!(source)->LastPriceByInsert.isNull()) (target)->LastPriceByInsert=(source)->LastPriceByInsert;	\
			if(!(source)->BidPrice1ByInsert.isNull()) (target)->BidPrice1ByInsert=(source)->BidPrice1ByInsert;	\
			if(!(source)->AskPrice1ByInsert.isNull()) (target)->AskPrice1ByInsert=(source)->AskPrice1ByInsert;	\
			if(!(source)->Available.isNull()) (target)->Available=(source)->Available;	\
			if(!(source)->CreateTime.isNull()) (target)->CreateTime=(source)->CreateTime;	\
	}

#endif

#ifndef CopyFinishOrderEntity
#define CopyFinishOrderEntity(target,source){	\
			CopyOrderEntity(target,source);	\
	}

#endif

#ifndef CopyTrueFinishOrderEntity
#define CopyTrueFinishOrderEntity(target,source){	\
			CopyTrueOrderEntity(target,source);	\
	}

#endif

#ifndef CopyInputActionEntity
#define CopyInputActionEntity(target,source){	\
			(target)->OrderID=(source)->OrderID;	\
			(target)->OrderLocalID=(source)->OrderLocalID;	\
			(target)->ActionFlag=(source)->ActionFlag;	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->Price=(source)->Price;	\
			(target)->Volume=(source)->Volume;	\
			(target)->Direction=(source)->Direction;	\
			(target)->VolumeDisplay=(source)->VolumeDisplay;	\
			(target)->ProductGroup=(source)->ProductGroup;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Cost=(source)->Cost;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueInputActionEntity
#define CopyTrueInputActionEntity(target,source){	\
			if(!(source)->OrderID.isNull()) (target)->OrderID=(source)->OrderID;	\
			if(!(source)->OrderLocalID.isNull()) (target)->OrderLocalID=(source)->OrderLocalID;	\
			if(!(source)->ActionFlag.isNull()) (target)->ActionFlag=(source)->ActionFlag;	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->Price.isNull()) (target)->Price=(source)->Price;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->Direction.isNull()) (target)->Direction=(source)->Direction;	\
			if(!(source)->VolumeDisplay.isNull()) (target)->VolumeDisplay=(source)->VolumeDisplay;	\
			if(!(source)->ProductGroup.isNull()) (target)->ProductGroup=(source)->ProductGroup;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Cost.isNull()) (target)->Cost=(source)->Cost;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyOrderActionEntity
#define CopyOrderActionEntity(target,source){	\
			CopyInputActionEntity(target,source);	\
	}

#endif

#ifndef CopyTrueOrderActionEntity
#define CopyTrueOrderActionEntity(target,source){	\
			CopyTrueInputActionEntity(target,source);	\
	}

#endif

#ifndef CopyQuoteActionEntity
#define CopyQuoteActionEntity(target,source){	\
			CopyInputActionEntity(target,source);	\
			(target)->TradeUnitID=(source)->TradeUnitID;	\
	}

#endif

#ifndef CopyTrueQuoteActionEntity
#define CopyTrueQuoteActionEntity(target,source){	\
			CopyTrueInputActionEntity(target,source);	\
			if(!(source)->TradeUnitID.isNull()) (target)->TradeUnitID=(source)->TradeUnitID;	\
	}

#endif

#ifndef CopyTriggerOrderActionEntity
#define CopyTriggerOrderActionEntity(target,source){	\
			CopyInputActionEntity(target,source);	\
			(target)->SLPrice=(source)->SLPrice;	\
			(target)->SLTriggerPrice=(source)->SLTriggerPrice;	\
			(target)->TPPrice=(source)->TPPrice;	\
			(target)->TPTriggerPrice=(source)->TPTriggerPrice;	\
			(target)->TriggerPrice=(source)->TriggerPrice;	\
			(target)->TriggerOrderType=(source)->TriggerOrderType;	\
			(target)->TriggerValue=(source)->TriggerValue;	\
			(target)->TriggerDetail=(source)->TriggerDetail;	\
			(target)->TriggerPriceType=(source)->TriggerPriceType;	\
			(target)->ErrorNo=(source)->ErrorNo;	\
			(target)->ErrorMsg=(source)->ErrorMsg;	\
	}

#endif

#ifndef CopyTrueTriggerOrderActionEntity
#define CopyTrueTriggerOrderActionEntity(target,source){	\
			CopyTrueInputActionEntity(target,source);	\
			if(!(source)->SLPrice.isNull()) (target)->SLPrice=(source)->SLPrice;	\
			if(!(source)->SLTriggerPrice.isNull()) (target)->SLTriggerPrice=(source)->SLTriggerPrice;	\
			if(!(source)->TPPrice.isNull()) (target)->TPPrice=(source)->TPPrice;	\
			if(!(source)->TPTriggerPrice.isNull()) (target)->TPTriggerPrice=(source)->TPTriggerPrice;	\
			if(!(source)->TriggerPrice.isNull()) (target)->TriggerPrice=(source)->TriggerPrice;	\
			if(!(source)->TriggerOrderType.isNull()) (target)->TriggerOrderType=(source)->TriggerOrderType;	\
			if(!(source)->TriggerValue.isNull()) (target)->TriggerValue=(source)->TriggerValue;	\
			if(!(source)->TriggerDetail.isNull()) (target)->TriggerDetail=(source)->TriggerDetail;	\
			if(!(source)->TriggerPriceType.isNull()) (target)->TriggerPriceType=(source)->TriggerPriceType;	\
			if(!(source)->ErrorNo.isNull()) (target)->ErrorNo=(source)->ErrorNo;	\
			if(!(source)->ErrorMsg.isNull()) (target)->ErrorMsg=(source)->ErrorMsg;	\
	}

#endif

#ifndef CopyCloseOrderActionEntity
#define CopyCloseOrderActionEntity(target,source){	\
			CopyTriggerOrderActionEntity(target,source);	\
	}

#endif

#ifndef CopyTrueCloseOrderActionEntity
#define CopyTrueCloseOrderActionEntity(target,source){	\
			CopyTrueTriggerOrderActionEntity(target,source);	\
	}

#endif

#ifndef CopyInputQuoteEntity
#define CopyInputQuoteEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->TradeUnitID=(source)->TradeUnitID;	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->Volume=(source)->Volume;	\
			(target)->BuyOffsetFlag=(source)->BuyOffsetFlag;	\
			(target)->BuyPrice=(source)->BuyPrice;	\
			(target)->SellOffsetFlag=(source)->SellOffsetFlag;	\
			(target)->SellPrice=(source)->SellPrice;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueInputQuoteEntity
#define CopyTrueInputQuoteEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->TradeUnitID.isNull()) (target)->TradeUnitID=(source)->TradeUnitID;	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->BuyOffsetFlag.isNull()) (target)->BuyOffsetFlag=(source)->BuyOffsetFlag;	\
			if(!(source)->BuyPrice.isNull()) (target)->BuyPrice=(source)->BuyPrice;	\
			if(!(source)->SellOffsetFlag.isNull()) (target)->SellOffsetFlag=(source)->SellOffsetFlag;	\
			if(!(source)->SellPrice.isNull()) (target)->SellPrice=(source)->SellPrice;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTradeEntity
#define CopyTradeEntity(target,source){	\
			(target)->TradeID=(source)->TradeID;	\
			(target)->Direction=(source)->Direction;	\
			(target)->OrderID=(source)->OrderID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->PositionID=(source)->PositionID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->OffsetFlag=(source)->OffsetFlag;	\
			(target)->Price=(source)->Price;	\
			(target)->Volume=(source)->Volume;	\
			(target)->DeriveSource=(source)->DeriveSource;	\
			(target)->MatchRole=(source)->MatchRole;	\
			(target)->PriceCurrency=(source)->PriceCurrency;	\
			(target)->ClearCurrency=(source)->ClearCurrency;	\
			(target)->Fee=(source)->Fee;	\
			(target)->FeeCurrency=(source)->FeeCurrency;	\
			(target)->CloseProfit=(source)->CloseProfit;	\
			(target)->Turnover=(source)->Turnover;	\
			(target)->UseMargin=(source)->UseMargin;	\
			(target)->Leverage=(source)->Leverage;	\
			(target)->OrderPrice=(source)->OrderPrice;	\
			(target)->TriggerPrice=(source)->TriggerPrice;	\
			(target)->IsSelfTrade=(source)->IsSelfTrade;	\
			(target)->Remark=(source)->Remark;	\
			(target)->BusinessNo=(source)->BusinessNo;	\
			(target)->OpenPrice=(source)->OpenPrice;	\
			(target)->APPID=(source)->APPID;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->CreateTime=(source)->CreateTime;	\
			(target)->CopyOrderID=(source)->CopyOrderID;	\
			(target)->CopyMemberID=(source)->CopyMemberID;	\
			(target)->CopyProfit=(source)->CopyProfit;	\
			(target)->Position=(source)->Position;	\
			(target)->ReserveProfit=(source)->ReserveProfit;	\
			(target)->ReserveFee=(source)->ReserveFee;	\
			(target)->TradeRemark=(source)->TradeRemark;	\
			(target)->BusinessType=(source)->BusinessType;	\
			(target)->BusinessValue=(source)->BusinessValue;	\
	}

#endif

#ifndef CopyTrueTradeEntity
#define CopyTrueTradeEntity(target,source){	\
			if(!(source)->TradeID.isNull()) (target)->TradeID=(source)->TradeID;	\
			if(!(source)->Direction.isNull()) (target)->Direction=(source)->Direction;	\
			if(!(source)->OrderID.isNull()) (target)->OrderID=(source)->OrderID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->PositionID.isNull()) (target)->PositionID=(source)->PositionID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->OffsetFlag.isNull()) (target)->OffsetFlag=(source)->OffsetFlag;	\
			if(!(source)->Price.isNull()) (target)->Price=(source)->Price;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->DeriveSource.isNull()) (target)->DeriveSource=(source)->DeriveSource;	\
			if(!(source)->MatchRole.isNull()) (target)->MatchRole=(source)->MatchRole;	\
			if(!(source)->PriceCurrency.isNull()) (target)->PriceCurrency=(source)->PriceCurrency;	\
			if(!(source)->ClearCurrency.isNull()) (target)->ClearCurrency=(source)->ClearCurrency;	\
			if(!(source)->Fee.isNull()) (target)->Fee=(source)->Fee;	\
			if(!(source)->FeeCurrency.isNull()) (target)->FeeCurrency=(source)->FeeCurrency;	\
			if(!(source)->CloseProfit.isNull()) (target)->CloseProfit=(source)->CloseProfit;	\
			if(!(source)->Turnover.isNull()) (target)->Turnover=(source)->Turnover;	\
			if(!(source)->UseMargin.isNull()) (target)->UseMargin=(source)->UseMargin;	\
			if(!(source)->Leverage.isNull()) (target)->Leverage=(source)->Leverage;	\
			if(!(source)->OrderPrice.isNull()) (target)->OrderPrice=(source)->OrderPrice;	\
			if(!(source)->TriggerPrice.isNull()) (target)->TriggerPrice=(source)->TriggerPrice;	\
			if(!(source)->IsSelfTrade.isNull()) (target)->IsSelfTrade=(source)->IsSelfTrade;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->BusinessNo.isNull()) (target)->BusinessNo=(source)->BusinessNo;	\
			if(!(source)->OpenPrice.isNull()) (target)->OpenPrice=(source)->OpenPrice;	\
			if(!(source)->APPID.isNull()) (target)->APPID=(source)->APPID;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->CreateTime.isNull()) (target)->CreateTime=(source)->CreateTime;	\
			if(!(source)->CopyOrderID.isNull()) (target)->CopyOrderID=(source)->CopyOrderID;	\
			if(!(source)->CopyMemberID.isNull()) (target)->CopyMemberID=(source)->CopyMemberID;	\
			if(!(source)->CopyProfit.isNull()) (target)->CopyProfit=(source)->CopyProfit;	\
			if(!(source)->Position.isNull()) (target)->Position=(source)->Position;	\
			if(!(source)->ReserveProfit.isNull()) (target)->ReserveProfit=(source)->ReserveProfit;	\
			if(!(source)->ReserveFee.isNull()) (target)->ReserveFee=(source)->ReserveFee;	\
			if(!(source)->TradeRemark.isNull()) (target)->TradeRemark=(source)->TradeRemark;	\
			if(!(source)->BusinessType.isNull()) (target)->BusinessType=(source)->BusinessType;	\
			if(!(source)->BusinessValue.isNull()) (target)->BusinessValue=(source)->BusinessValue;	\
	}

#endif

#ifndef CopyMarketOrderEntity
#define CopyMarketOrderEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Direction=(source)->Direction;	\
			(target)->Price=(source)->Price;	\
			(target)->Volume=(source)->Volume;	\
			(target)->Orders=(source)->Orders;	\
			(target)->BusinessNo=(source)->BusinessNo;	\
			(target)->TrueVolume=(source)->TrueVolume;	\
	}

#endif

#ifndef CopyTrueMarketOrderEntity
#define CopyTrueMarketOrderEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Direction.isNull()) (target)->Direction=(source)->Direction;	\
			if(!(source)->Price.isNull()) (target)->Price=(source)->Price;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->Orders.isNull()) (target)->Orders=(source)->Orders;	\
			if(!(source)->BusinessNo.isNull()) (target)->BusinessNo=(source)->BusinessNo;	\
			if(!(source)->TrueVolume.isNull()) (target)->TrueVolume=(source)->TrueVolume;	\
	}

#endif

#ifndef CopyTickMarketOrderEntity
#define CopyTickMarketOrderEntity(target,source){	\
			CopyMarketOrderEntity(target,source);	\
			(target)->Tick=(source)->Tick;	\
			(target)->SubIndex=(source)->SubIndex;	\
	}

#endif

#ifndef CopyTrueTickMarketOrderEntity
#define CopyTrueTickMarketOrderEntity(target,source){	\
			CopyTrueMarketOrderEntity(target,source);	\
			if(!(source)->Tick.isNull()) (target)->Tick=(source)->Tick;	\
			if(!(source)->SubIndex.isNull()) (target)->SubIndex=(source)->SubIndex;	\
	}

#endif

#ifndef CopyMarketTradeEntity
#define CopyMarketTradeEntity(target,source){	\
			(target)->TradeID=(source)->TradeID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Direction=(source)->Direction;	\
			(target)->Price=(source)->Price;	\
			(target)->Volume=(source)->Volume;	\
			(target)->InsertTime=(source)->InsertTime;	\
	}

#endif

#ifndef CopyTrueMarketTradeEntity
#define CopyTrueMarketTradeEntity(target,source){	\
			if(!(source)->TradeID.isNull()) (target)->TradeID=(source)->TradeID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Direction.isNull()) (target)->Direction=(source)->Direction;	\
			if(!(source)->Price.isNull()) (target)->Price=(source)->Price;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
	}

#endif

#ifndef CopyLineEntity
#define CopyLineEntity(target,source){	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->ClosePrice=(source)->ClosePrice;	\
	}

#endif

#ifndef CopyTrueLineEntity
#define CopyTrueLineEntity(target,source){	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->ClosePrice.isNull()) (target)->ClosePrice=(source)->ClosePrice;	\
	}

#endif

#ifndef CopyKLineBaseEntity
#define CopyKLineBaseEntity(target,source){	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->OpenPrice=(source)->OpenPrice;	\
			(target)->HighestPrice=(source)->HighestPrice;	\
			(target)->LowestPrice=(source)->LowestPrice;	\
			(target)->ClosePrice=(source)->ClosePrice;	\
			(target)->Volume=(source)->Volume;	\
			(target)->Turnover=(source)->Turnover;	\
	}

#endif

#ifndef CopyTrueKLineBaseEntity
#define CopyTrueKLineBaseEntity(target,source){	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->OpenPrice.isNull()) (target)->OpenPrice=(source)->OpenPrice;	\
			if(!(source)->HighestPrice.isNull()) (target)->HighestPrice=(source)->HighestPrice;	\
			if(!(source)->LowestPrice.isNull()) (target)->LowestPrice=(source)->LowestPrice;	\
			if(!(source)->ClosePrice.isNull()) (target)->ClosePrice=(source)->ClosePrice;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->Turnover.isNull()) (target)->Turnover=(source)->Turnover;	\
	}

#endif

#ifndef CopyKLineEntity
#define CopyKLineEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Bar=(source)->Bar;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->OpenPrice=(source)->OpenPrice;	\
			(target)->HighestPrice=(source)->HighestPrice;	\
			(target)->LowestPrice=(source)->LowestPrice;	\
			(target)->ClosePrice=(source)->ClosePrice;	\
			(target)->Volume=(source)->Volume;	\
			(target)->Turnover=(source)->Turnover;	\
	}

#endif

#ifndef CopyTrueKLineEntity
#define CopyTrueKLineEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Bar.isNull()) (target)->Bar=(source)->Bar;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->OpenPrice.isNull()) (target)->OpenPrice=(source)->OpenPrice;	\
			if(!(source)->HighestPrice.isNull()) (target)->HighestPrice=(source)->HighestPrice;	\
			if(!(source)->LowestPrice.isNull()) (target)->LowestPrice=(source)->LowestPrice;	\
			if(!(source)->ClosePrice.isNull()) (target)->ClosePrice=(source)->ClosePrice;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->Turnover.isNull()) (target)->Turnover=(source)->Turnover;	\
	}

#endif

#ifndef CopyPublishKLineEntity
#define CopyPublishKLineEntity(target,source){	\
			CopyKLineEntity(target,source);	\
	}

#endif

#ifndef CopyTruePublishKLineEntity
#define CopyTruePublishKLineEntity(target,source){	\
			CopyTrueKLineEntity(target,source);	\
	}

#endif

#ifndef CopyLastKLineEntity
#define CopyLastKLineEntity(target,source){	\
			CopyKLineEntity(target,source);	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueLastKLineEntity
#define CopyTrueLastKLineEntity(target,source){	\
			CopyTrueKLineEntity(target,source);	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyMarketDataUpdateTimeEntity
#define CopyMarketDataUpdateTimeEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->ProductGroup=(source)->ProductGroup;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueMarketDataUpdateTimeEntity
#define CopyTrueMarketDataUpdateTimeEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->ProductGroup.isNull()) (target)->ProductGroup=(source)->ProductGroup;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyMarketDataPreDataEntity
#define CopyMarketDataPreDataEntity(target,source){	\
			(target)->PreClosePrice=(source)->PreClosePrice;	\
			(target)->PreSettlementPrice=(source)->PreSettlementPrice;	\
			(target)->PreOpenInterest=(source)->PreOpenInterest;	\
			(target)->PrePositionFeeRate=(source)->PrePositionFeeRate;	\
			(target)->ClosePrice=(source)->ClosePrice;	\
			(target)->PositionFeeTime=(source)->PositionFeeTime;	\
			(target)->SettlementTime=(source)->SettlementTime;	\
	}

#endif

#ifndef CopyTrueMarketDataPreDataEntity
#define CopyTrueMarketDataPreDataEntity(target,source){	\
			if(!(source)->PreClosePrice.isNull()) (target)->PreClosePrice=(source)->PreClosePrice;	\
			if(!(source)->PreSettlementPrice.isNull()) (target)->PreSettlementPrice=(source)->PreSettlementPrice;	\
			if(!(source)->PreOpenInterest.isNull()) (target)->PreOpenInterest=(source)->PreOpenInterest;	\
			if(!(source)->PrePositionFeeRate.isNull()) (target)->PrePositionFeeRate=(source)->PrePositionFeeRate;	\
			if(!(source)->ClosePrice.isNull()) (target)->ClosePrice=(source)->ClosePrice;	\
			if(!(source)->PositionFeeTime.isNull()) (target)->PositionFeeTime=(source)->PositionFeeTime;	\
			if(!(source)->SettlementTime.isNull()) (target)->SettlementTime=(source)->SettlementTime;	\
	}

#endif

#ifndef CopyMarketDataLastMatchEntity
#define CopyMarketDataLastMatchEntity(target,source){	\
			(target)->UpperLimitPrice=(source)->UpperLimitPrice;	\
			(target)->LowerLimitPrice=(source)->LowerLimitPrice;	\
			(target)->UnderlyingPrice=(source)->UnderlyingPrice;	\
			(target)->MarkedPrice=(source)->MarkedPrice;	\
			(target)->PositionFeeRate=(source)->PositionFeeRate;	\
			(target)->HighestPrice=(source)->HighestPrice;	\
			(target)->LowestPrice=(source)->LowestPrice;	\
			(target)->LastPrice=(source)->LastPrice;	\
			(target)->Volume=(source)->Volume;	\
			(target)->Turnover=(source)->Turnover;	\
			(target)->OpenInterest=(source)->OpenInterest;	\
			(target)->SettlementPrice=(source)->SettlementPrice;	\
			(target)->OpenPrice=(source)->OpenPrice;	\
	}

#endif

#ifndef CopyTrueMarketDataLastMatchEntity
#define CopyTrueMarketDataLastMatchEntity(target,source){	\
			if(!(source)->UpperLimitPrice.isNull()) (target)->UpperLimitPrice=(source)->UpperLimitPrice;	\
			if(!(source)->LowerLimitPrice.isNull()) (target)->LowerLimitPrice=(source)->LowerLimitPrice;	\
			if(!(source)->UnderlyingPrice.isNull()) (target)->UnderlyingPrice=(source)->UnderlyingPrice;	\
			if(!(source)->MarkedPrice.isNull()) (target)->MarkedPrice=(source)->MarkedPrice;	\
			if(!(source)->PositionFeeRate.isNull()) (target)->PositionFeeRate=(source)->PositionFeeRate;	\
			if(!(source)->HighestPrice.isNull()) (target)->HighestPrice=(source)->HighestPrice;	\
			if(!(source)->LowestPrice.isNull()) (target)->LowestPrice=(source)->LowestPrice;	\
			if(!(source)->LastPrice.isNull()) (target)->LastPrice=(source)->LastPrice;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->Turnover.isNull()) (target)->Turnover=(source)->Turnover;	\
			if(!(source)->OpenInterest.isNull()) (target)->OpenInterest=(source)->OpenInterest;	\
			if(!(source)->SettlementPrice.isNull()) (target)->SettlementPrice=(source)->SettlementPrice;	\
			if(!(source)->OpenPrice.isNull()) (target)->OpenPrice=(source)->OpenPrice;	\
	}

#endif

#ifndef CopyMarketDataOptionsEntity
#define CopyMarketDataOptionsEntity(target,source){	\
			(target)->Delta=(source)->Delta;	\
			(target)->Gamma=(source)->Gamma;	\
			(target)->Vega=(source)->Vega;	\
			(target)->Theta=(source)->Theta;	\
			(target)->Rho=(source)->Rho;	\
			(target)->LastVolatility=(source)->LastVolatility;	\
			(target)->Bid1Volatility=(source)->Bid1Volatility;	\
			(target)->Ask1Volatility=(source)->Ask1Volatility;	\
			(target)->MaskedVolatility=(source)->MaskedVolatility;	\
	}

#endif

#ifndef CopyTrueMarketDataOptionsEntity
#define CopyTrueMarketDataOptionsEntity(target,source){	\
			if(!(source)->Delta.isNull()) (target)->Delta=(source)->Delta;	\
			if(!(source)->Gamma.isNull()) (target)->Gamma=(source)->Gamma;	\
			if(!(source)->Vega.isNull()) (target)->Vega=(source)->Vega;	\
			if(!(source)->Theta.isNull()) (target)->Theta=(source)->Theta;	\
			if(!(source)->Rho.isNull()) (target)->Rho=(source)->Rho;	\
			if(!(source)->LastVolatility.isNull()) (target)->LastVolatility=(source)->LastVolatility;	\
			if(!(source)->Bid1Volatility.isNull()) (target)->Bid1Volatility=(source)->Bid1Volatility;	\
			if(!(source)->Ask1Volatility.isNull()) (target)->Ask1Volatility=(source)->Ask1Volatility;	\
			if(!(source)->MaskedVolatility.isNull()) (target)->MaskedVolatility=(source)->MaskedVolatility;	\
	}

#endif

#ifndef CopyMarketData12345Entity
#define CopyMarketData12345Entity(target,source){	\
			(target)->BidPrice1=(source)->BidPrice1;	\
			(target)->BidVolume1=(source)->BidVolume1;	\
			(target)->AskPrice1=(source)->AskPrice1;	\
			(target)->AskVolume1=(source)->AskVolume1;	\
			(target)->BidPrice2=(source)->BidPrice2;	\
			(target)->BidVolume2=(source)->BidVolume2;	\
			(target)->AskPrice2=(source)->AskPrice2;	\
			(target)->AskVolume2=(source)->AskVolume2;	\
			(target)->BidPrice3=(source)->BidPrice3;	\
			(target)->BidVolume3=(source)->BidVolume3;	\
			(target)->AskPrice3=(source)->AskPrice3;	\
			(target)->AskVolume3=(source)->AskVolume3;	\
			(target)->BidPrice4=(source)->BidPrice4;	\
			(target)->BidVolume4=(source)->BidVolume4;	\
			(target)->AskPrice4=(source)->AskPrice4;	\
			(target)->AskVolume4=(source)->AskVolume4;	\
			(target)->BidPrice5=(source)->BidPrice5;	\
			(target)->BidVolume5=(source)->BidVolume5;	\
			(target)->AskPrice5=(source)->AskPrice5;	\
			(target)->AskVolume5=(source)->AskVolume5;	\
	}

#endif

#ifndef CopyTrueMarketData12345Entity
#define CopyTrueMarketData12345Entity(target,source){	\
			if(!(source)->BidPrice1.isNull()) (target)->BidPrice1=(source)->BidPrice1;	\
			if(!(source)->BidVolume1.isNull()) (target)->BidVolume1=(source)->BidVolume1;	\
			if(!(source)->AskPrice1.isNull()) (target)->AskPrice1=(source)->AskPrice1;	\
			if(!(source)->AskVolume1.isNull()) (target)->AskVolume1=(source)->AskVolume1;	\
			if(!(source)->BidPrice2.isNull()) (target)->BidPrice2=(source)->BidPrice2;	\
			if(!(source)->BidVolume2.isNull()) (target)->BidVolume2=(source)->BidVolume2;	\
			if(!(source)->AskPrice2.isNull()) (target)->AskPrice2=(source)->AskPrice2;	\
			if(!(source)->AskVolume2.isNull()) (target)->AskVolume2=(source)->AskVolume2;	\
			if(!(source)->BidPrice3.isNull()) (target)->BidPrice3=(source)->BidPrice3;	\
			if(!(source)->BidVolume3.isNull()) (target)->BidVolume3=(source)->BidVolume3;	\
			if(!(source)->AskPrice3.isNull()) (target)->AskPrice3=(source)->AskPrice3;	\
			if(!(source)->AskVolume3.isNull()) (target)->AskVolume3=(source)->AskVolume3;	\
			if(!(source)->BidPrice4.isNull()) (target)->BidPrice4=(source)->BidPrice4;	\
			if(!(source)->BidVolume4.isNull()) (target)->BidVolume4=(source)->BidVolume4;	\
			if(!(source)->AskPrice4.isNull()) (target)->AskPrice4=(source)->AskPrice4;	\
			if(!(source)->AskVolume4.isNull()) (target)->AskVolume4=(source)->AskVolume4;	\
			if(!(source)->BidPrice5.isNull()) (target)->BidPrice5=(source)->BidPrice5;	\
			if(!(source)->BidVolume5.isNull()) (target)->BidVolume5=(source)->BidVolume5;	\
			if(!(source)->AskPrice5.isNull()) (target)->AskPrice5=(source)->AskPrice5;	\
			if(!(source)->AskVolume5.isNull()) (target)->AskVolume5=(source)->AskVolume5;	\
	}

#endif

#ifndef CopyMarketData6789TEntity
#define CopyMarketData6789TEntity(target,source){	\
			(target)->BidPrice6=(source)->BidPrice6;	\
			(target)->BidVolume6=(source)->BidVolume6;	\
			(target)->AskPrice6=(source)->AskPrice6;	\
			(target)->AskVolume6=(source)->AskVolume6;	\
			(target)->BidPrice7=(source)->BidPrice7;	\
			(target)->BidVolume7=(source)->BidVolume7;	\
			(target)->AskPrice7=(source)->AskPrice7;	\
			(target)->AskVolume7=(source)->AskVolume7;	\
			(target)->BidPrice8=(source)->BidPrice8;	\
			(target)->BidVolume8=(source)->BidVolume8;	\
			(target)->AskPrice8=(source)->AskPrice8;	\
			(target)->AskVolume8=(source)->AskVolume8;	\
			(target)->BidPrice9=(source)->BidPrice9;	\
			(target)->BidVolume9=(source)->BidVolume9;	\
			(target)->AskPrice9=(source)->AskPrice9;	\
			(target)->AskVolume9=(source)->AskVolume9;	\
			(target)->BidPrice10=(source)->BidPrice10;	\
			(target)->BidVolume10=(source)->BidVolume10;	\
			(target)->AskPrice10=(source)->AskPrice10;	\
			(target)->AskVolume10=(source)->AskVolume10;	\
	}

#endif

#ifndef CopyTrueMarketData6789TEntity
#define CopyTrueMarketData6789TEntity(target,source){	\
			if(!(source)->BidPrice6.isNull()) (target)->BidPrice6=(source)->BidPrice6;	\
			if(!(source)->BidVolume6.isNull()) (target)->BidVolume6=(source)->BidVolume6;	\
			if(!(source)->AskPrice6.isNull()) (target)->AskPrice6=(source)->AskPrice6;	\
			if(!(source)->AskVolume6.isNull()) (target)->AskVolume6=(source)->AskVolume6;	\
			if(!(source)->BidPrice7.isNull()) (target)->BidPrice7=(source)->BidPrice7;	\
			if(!(source)->BidVolume7.isNull()) (target)->BidVolume7=(source)->BidVolume7;	\
			if(!(source)->AskPrice7.isNull()) (target)->AskPrice7=(source)->AskPrice7;	\
			if(!(source)->AskVolume7.isNull()) (target)->AskVolume7=(source)->AskVolume7;	\
			if(!(source)->BidPrice8.isNull()) (target)->BidPrice8=(source)->BidPrice8;	\
			if(!(source)->BidVolume8.isNull()) (target)->BidVolume8=(source)->BidVolume8;	\
			if(!(source)->AskPrice8.isNull()) (target)->AskPrice8=(source)->AskPrice8;	\
			if(!(source)->AskVolume8.isNull()) (target)->AskVolume8=(source)->AskVolume8;	\
			if(!(source)->BidPrice9.isNull()) (target)->BidPrice9=(source)->BidPrice9;	\
			if(!(source)->BidVolume9.isNull()) (target)->BidVolume9=(source)->BidVolume9;	\
			if(!(source)->AskPrice9.isNull()) (target)->AskPrice9=(source)->AskPrice9;	\
			if(!(source)->AskVolume9.isNull()) (target)->AskVolume9=(source)->AskVolume9;	\
			if(!(source)->BidPrice10.isNull()) (target)->BidPrice10=(source)->BidPrice10;	\
			if(!(source)->BidVolume10.isNull()) (target)->BidVolume10=(source)->BidVolume10;	\
			if(!(source)->AskPrice10.isNull()) (target)->AskPrice10=(source)->AskPrice10;	\
			if(!(source)->AskVolume10.isNull()) (target)->AskVolume10=(source)->AskVolume10;	\
	}

#endif

#ifndef CopyMarketDataEntity
#define CopyMarketDataEntity(target,source){	\
			CopyMarketDataUpdateTimeEntity(target,source);	\
			CopyMarketDataPreDataEntity(target,source);	\
			CopyMarketDataLastMatchEntity(target,source);	\
			(target)->IsPubMarketData=(source)->IsPubMarketData;	\
			(target)->InstrumentStatus=(source)->InstrumentStatus;	\
			(target)->Remark=(source)->Remark;	\
			(target)->BusinessNo=(source)->BusinessNo;	\
			(target)->MarkedPriceTotal=(source)->MarkedPriceTotal;	\
			(target)->MarkedPriceWeight=(source)->MarkedPriceWeight;	\
			(target)->L25UpperPrice=(source)->L25UpperPrice;	\
			(target)->L25LowerPrice=(source)->L25LowerPrice;	\
			(target)->L25UpperPrice1=(source)->L25UpperPrice1;	\
			(target)->L25LowerPrice1=(source)->L25LowerPrice1;	\
			(target)->L25UpperPrice2=(source)->L25UpperPrice2;	\
			(target)->L25LowerPrice2=(source)->L25LowerPrice2;	\
			(target)->BidPrice1=(source)->BidPrice1;	\
			(target)->BidVolume1=(source)->BidVolume1;	\
			(target)->AskPrice1=(source)->AskPrice1;	\
			(target)->AskVolume1=(source)->AskVolume1;	\
			(target)->TheoryPrice=(source)->TheoryPrice;	\
			(target)->HighestPrice24=(source)->HighestPrice24;	\
			(target)->LowestPrice24=(source)->LowestPrice24;	\
			(target)->Volume24=(source)->Volume24;	\
			(target)->Turnover24=(source)->Turnover24;	\
			(target)->OpenPrice24=(source)->OpenPrice24;	\
			(target)->Frequency=(source)->Frequency;	\
			(target)->InsertTime=(source)->InsertTime;	\
	}

#endif

#ifndef CopyTrueMarketDataEntity
#define CopyTrueMarketDataEntity(target,source){	\
			CopyTrueMarketDataUpdateTimeEntity(target,source);	\
			CopyTrueMarketDataPreDataEntity(target,source);	\
			CopyTrueMarketDataLastMatchEntity(target,source);	\
			if(!(source)->IsPubMarketData.isNull()) (target)->IsPubMarketData=(source)->IsPubMarketData;	\
			if(!(source)->InstrumentStatus.isNull()) (target)->InstrumentStatus=(source)->InstrumentStatus;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->BusinessNo.isNull()) (target)->BusinessNo=(source)->BusinessNo;	\
			if(!(source)->MarkedPriceTotal.isNull()) (target)->MarkedPriceTotal=(source)->MarkedPriceTotal;	\
			if(!(source)->MarkedPriceWeight.isNull()) (target)->MarkedPriceWeight=(source)->MarkedPriceWeight;	\
			if(!(source)->L25UpperPrice.isNull()) (target)->L25UpperPrice=(source)->L25UpperPrice;	\
			if(!(source)->L25LowerPrice.isNull()) (target)->L25LowerPrice=(source)->L25LowerPrice;	\
			if(!(source)->L25UpperPrice1.isNull()) (target)->L25UpperPrice1=(source)->L25UpperPrice1;	\
			if(!(source)->L25LowerPrice1.isNull()) (target)->L25LowerPrice1=(source)->L25LowerPrice1;	\
			if(!(source)->L25UpperPrice2.isNull()) (target)->L25UpperPrice2=(source)->L25UpperPrice2;	\
			if(!(source)->L25LowerPrice2.isNull()) (target)->L25LowerPrice2=(source)->L25LowerPrice2;	\
			if(!(source)->BidPrice1.isNull()) (target)->BidPrice1=(source)->BidPrice1;	\
			if(!(source)->BidVolume1.isNull()) (target)->BidVolume1=(source)->BidVolume1;	\
			if(!(source)->AskPrice1.isNull()) (target)->AskPrice1=(source)->AskPrice1;	\
			if(!(source)->AskVolume1.isNull()) (target)->AskVolume1=(source)->AskVolume1;	\
			if(!(source)->TheoryPrice.isNull()) (target)->TheoryPrice=(source)->TheoryPrice;	\
			if(!(source)->HighestPrice24.isNull()) (target)->HighestPrice24=(source)->HighestPrice24;	\
			if(!(source)->LowestPrice24.isNull()) (target)->LowestPrice24=(source)->LowestPrice24;	\
			if(!(source)->Volume24.isNull()) (target)->Volume24=(source)->Volume24;	\
			if(!(source)->Turnover24.isNull()) (target)->Turnover24=(source)->Turnover24;	\
			if(!(source)->OpenPrice24.isNull()) (target)->OpenPrice24=(source)->OpenPrice24;	\
			if(!(source)->Frequency.isNull()) (target)->Frequency=(source)->Frequency;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
	}

#endif

#ifndef CopyMarketDataUpdateEntity
#define CopyMarketDataUpdateEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->MarkedPrice=(source)->MarkedPrice;	\
			(target)->TheoryPrice=(source)->TheoryPrice;	\
			(target)->PreSettlementPrice=(source)->PreSettlementPrice;	\
			(target)->SettlementPrice=(source)->SettlementPrice;	\
			(target)->UnderlyingPrice=(source)->UnderlyingPrice;	\
			(target)->PrePositionFeeRate=(source)->PrePositionFeeRate;	\
			(target)->PositionFeeRate=(source)->PositionFeeRate;	\
			(target)->InstrumentStatus=(source)->InstrumentStatus;	\
			(target)->Volume=(source)->Volume;	\
			(target)->Turnover=(source)->Turnover;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueMarketDataUpdateEntity
#define CopyTrueMarketDataUpdateEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->MarkedPrice.isNull()) (target)->MarkedPrice=(source)->MarkedPrice;	\
			if(!(source)->TheoryPrice.isNull()) (target)->TheoryPrice=(source)->TheoryPrice;	\
			if(!(source)->PreSettlementPrice.isNull()) (target)->PreSettlementPrice=(source)->PreSettlementPrice;	\
			if(!(source)->SettlementPrice.isNull()) (target)->SettlementPrice=(source)->SettlementPrice;	\
			if(!(source)->UnderlyingPrice.isNull()) (target)->UnderlyingPrice=(source)->UnderlyingPrice;	\
			if(!(source)->PrePositionFeeRate.isNull()) (target)->PrePositionFeeRate=(source)->PrePositionFeeRate;	\
			if(!(source)->PositionFeeRate.isNull()) (target)->PositionFeeRate=(source)->PositionFeeRate;	\
			if(!(source)->InstrumentStatus.isNull()) (target)->InstrumentStatus=(source)->InstrumentStatus;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->Turnover.isNull()) (target)->Turnover=(source)->Turnover;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyMarketOverViewEntity
#define CopyMarketOverViewEntity(target,source){	\
			CopyMarketDataUpdateTimeEntity(target,source);	\
			CopyMarketDataLastMatchEntity(target,source);	\
			(target)->InstrumentStatus=(source)->InstrumentStatus;	\
			(target)->PrePositionFeeRate=(source)->PrePositionFeeRate;	\
			(target)->PositionFeeTime=(source)->PositionFeeTime;	\
			(target)->BidPrice1=(source)->BidPrice1;	\
			(target)->AskPrice1=(source)->AskPrice1;	\
			(target)->HighestPrice24=(source)->HighestPrice24;	\
			(target)->LowestPrice24=(source)->LowestPrice24;	\
			(target)->Volume24=(source)->Volume24;	\
			(target)->Turnover24=(source)->Turnover24;	\
			(target)->OpenPrice24=(source)->OpenPrice24;	\
	}

#endif

#ifndef CopyTrueMarketOverViewEntity
#define CopyTrueMarketOverViewEntity(target,source){	\
			CopyTrueMarketDataUpdateTimeEntity(target,source);	\
			CopyTrueMarketDataLastMatchEntity(target,source);	\
			if(!(source)->InstrumentStatus.isNull()) (target)->InstrumentStatus=(source)->InstrumentStatus;	\
			if(!(source)->PrePositionFeeRate.isNull()) (target)->PrePositionFeeRate=(source)->PrePositionFeeRate;	\
			if(!(source)->PositionFeeTime.isNull()) (target)->PositionFeeTime=(source)->PositionFeeTime;	\
			if(!(source)->BidPrice1.isNull()) (target)->BidPrice1=(source)->BidPrice1;	\
			if(!(source)->AskPrice1.isNull()) (target)->AskPrice1=(source)->AskPrice1;	\
			if(!(source)->HighestPrice24.isNull()) (target)->HighestPrice24=(source)->HighestPrice24;	\
			if(!(source)->LowestPrice24.isNull()) (target)->LowestPrice24=(source)->LowestPrice24;	\
			if(!(source)->Volume24.isNull()) (target)->Volume24=(source)->Volume24;	\
			if(!(source)->Turnover24.isNull()) (target)->Turnover24=(source)->Turnover24;	\
			if(!(source)->OpenPrice24.isNull()) (target)->OpenPrice24=(source)->OpenPrice24;	\
	}

#endif

#ifndef CopyFiveDepthMarketDataEntity
#define CopyFiveDepthMarketDataEntity(target,source){	\
			CopyMarketDataUpdateTimeEntity(target,source);	\
			CopyMarketData12345Entity(target,source);	\
	}

#endif

#ifndef CopyTrueFiveDepthMarketDataEntity
#define CopyTrueFiveDepthMarketDataEntity(target,source){	\
			CopyTrueMarketDataUpdateTimeEntity(target,source);	\
			CopyTrueMarketData12345Entity(target,source);	\
	}

#endif

#ifndef CopyTenDepthMarketDataEntity
#define CopyTenDepthMarketDataEntity(target,source){	\
			CopyMarketDataUpdateTimeEntity(target,source);	\
			CopyMarketData12345Entity(target,source);	\
			CopyMarketData6789TEntity(target,source);	\
	}

#endif

#ifndef CopyTrueTenDepthMarketDataEntity
#define CopyTrueTenDepthMarketDataEntity(target,source){	\
			CopyTrueMarketDataUpdateTimeEntity(target,source);	\
			CopyTrueMarketData12345Entity(target,source);	\
			CopyTrueMarketData6789TEntity(target,source);	\
	}

#endif

#ifndef CopyOptionsMarketDataEntity
#define CopyOptionsMarketDataEntity(target,source){	\
			CopyMarketDataUpdateTimeEntity(target,source);	\
			CopyMarketDataPreDataEntity(target,source);	\
			CopyMarketDataLastMatchEntity(target,source);	\
			CopyMarketDataOptionsEntity(target,source);	\
	}

#endif

#ifndef CopyTrueOptionsMarketDataEntity
#define CopyTrueOptionsMarketDataEntity(target,source){	\
			CopyTrueMarketDataUpdateTimeEntity(target,source);	\
			CopyTrueMarketDataPreDataEntity(target,source);	\
			CopyTrueMarketDataLastMatchEntity(target,source);	\
			CopyTrueMarketDataOptionsEntity(target,source);	\
	}

#endif

#ifndef CopyCommandEntity
#define CopyCommandEntity(target,source){	\
			(target)->UserID=(source)->UserID;	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->Command=(source)->Command;	\
			(target)->Content=(source)->Content;	\
	}

#endif

#ifndef CopyTrueCommandEntity
#define CopyTrueCommandEntity(target,source){	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->Command.isNull()) (target)->Command=(source)->Command;	\
			if(!(source)->Content.isNull()) (target)->Content=(source)->Content;	\
	}

#endif

#ifndef CopyQryResultEntity
#define CopyQryResultEntity(target,source){	\
			(target)->Key=(source)->Key;	\
	}

#endif

#ifndef CopyTrueQryResultEntity
#define CopyTrueQryResultEntity(target,source){	\
			if(!(source)->Key.isNull()) (target)->Key=(source)->Key;	\
	}

#endif

#ifndef CopyResultEntity
#define CopyResultEntity(target,source){	\
			(target)->Key=(source)->Key;	\
			(target)->Result=(source)->Result;	\
	}

#endif

#ifndef CopyTrueResultEntity
#define CopyTrueResultEntity(target,source){	\
			if(!(source)->Key.isNull()) (target)->Key=(source)->Key;	\
			if(!(source)->Result.isNull()) (target)->Result=(source)->Result;	\
	}

#endif

#ifndef CopyResetLocalIDEntity
#define CopyResetLocalIDEntity(target,source){	\
			(target)->Token=(source)->Token;	\
			(target)->UserID=(source)->UserID;	\
			(target)->APPID=(source)->APPID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->Currency=(source)->Currency;	\
	}

#endif

#ifndef CopyTrueResetLocalIDEntity
#define CopyTrueResetLocalIDEntity(target,source){	\
			if(!(source)->Token.isNull()) (target)->Token=(source)->Token;	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->APPID.isNull()) (target)->APPID=(source)->APPID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
	}

#endif

#ifndef CopyUserLoginEntity
#define CopyUserLoginEntity(target,source){	\
			(target)->UserID=(source)->UserID;	\
			(target)->Password=(source)->Password;	\
			(target)->UserProductID=(source)->UserProductID;	\
			(target)->MacAddress=(source)->MacAddress;	\
			(target)->ClientIPAddress=(source)->ClientIPAddress;	\
			(target)->HDSerialID=(source)->HDSerialID;	\
			(target)->AuthCode=(source)->AuthCode;	\
			(target)->APPID=(source)->APPID;	\
			(target)->Token=(source)->Token;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueUserLoginEntity
#define CopyTrueUserLoginEntity(target,source){	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->Password.isNull()) (target)->Password=(source)->Password;	\
			if(!(source)->UserProductID.isNull()) (target)->UserProductID=(source)->UserProductID;	\
			if(!(source)->MacAddress.isNull()) (target)->MacAddress=(source)->MacAddress;	\
			if(!(source)->ClientIPAddress.isNull()) (target)->ClientIPAddress=(source)->ClientIPAddress;	\
			if(!(source)->HDSerialID.isNull()) (target)->HDSerialID=(source)->HDSerialID;	\
			if(!(source)->AuthCode.isNull()) (target)->AuthCode=(source)->AuthCode;	\
			if(!(source)->APPID.isNull()) (target)->APPID=(source)->APPID;	\
			if(!(source)->Token.isNull()) (target)->Token=(source)->Token;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyUserLogoutEntity
#define CopyUserLogoutEntity(target,source){	\
			(target)->UserID=(source)->UserID;	\
			(target)->APPID=(source)->APPID;	\
			(target)->Token=(source)->Token;	\
	}

#endif

#ifndef CopyTrueUserLogoutEntity
#define CopyTrueUserLogoutEntity(target,source){	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->APPID.isNull()) (target)->APPID=(source)->APPID;	\
			if(!(source)->Token.isNull()) (target)->Token=(source)->Token;	\
	}

#endif

#ifndef CopyCheckTokenEntity
#define CopyCheckTokenEntity(target,source){	\
			(target)->Token=(source)->Token;	\
			(target)->UserID=(source)->UserID;	\
			(target)->MaxLocalID=(source)->MaxLocalID;	\
			(target)->MemberID=(source)->MemberID;	\
	}

#endif

#ifndef CopyTrueCheckTokenEntity
#define CopyTrueCheckTokenEntity(target,source){	\
			if(!(source)->Token.isNull()) (target)->Token=(source)->Token;	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->MaxLocalID.isNull()) (target)->MaxLocalID=(source)->MaxLocalID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
	}

#endif

#ifndef CopyWalletNonceEntity
#define CopyWalletNonceEntity(target,source){	\
			(target)->UserID=(source)->UserID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->Nonce=(source)->Nonce;	\
	}

#endif

#ifndef CopyTrueWalletNonceEntity
#define CopyTrueWalletNonceEntity(target,source){	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->Nonce.isNull()) (target)->Nonce=(source)->Nonce;	\
	}

#endif

#ifndef CopyWalletLoginEntity
#define CopyWalletLoginEntity(target,source){	\
			(target)->UserID=(source)->UserID;	\
			(target)->Signature=(source)->Signature;	\
	}

#endif

#ifndef CopyTrueWalletLoginEntity
#define CopyTrueWalletLoginEntity(target,source){	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->Signature.isNull()) (target)->Signature=(source)->Signature;	\
	}

#endif

#ifndef CopyUserSessionEntity
#define CopyUserSessionEntity(target,source){	\
			CopyUserLoginEntity(target,source);	\
			(target)->APIID=(source)->APIID;	\
			(target)->LoginTime=(source)->LoginTime;	\
			(target)->ExpireTime=(source)->ExpireTime;	\
			(target)->IPAddress=(source)->IPAddress;	\
			(target)->MaxLocalID=(source)->MaxLocalID;	\
			(target)->SessionNo=(source)->SessionNo;	\
			(target)->FrontNo=(source)->FrontNo;	\
			(target)->AccessLimit=(source)->AccessLimit;	\
			(target)->UserType=(source)->UserType;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->LimitAccesses=(source)->LimitAccesses;	\
			(target)->SinkType=(source)->SinkType;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyTrueUserSessionEntity
#define CopyTrueUserSessionEntity(target,source){	\
			CopyTrueUserLoginEntity(target,source);	\
			if(!(source)->APIID.isNull()) (target)->APIID=(source)->APIID;	\
			if(!(source)->LoginTime.isNull()) (target)->LoginTime=(source)->LoginTime;	\
			if(!(source)->ExpireTime.isNull()) (target)->ExpireTime=(source)->ExpireTime;	\
			if(!(source)->IPAddress.isNull()) (target)->IPAddress=(source)->IPAddress;	\
			if(!(source)->MaxLocalID.isNull()) (target)->MaxLocalID=(source)->MaxLocalID;	\
			if(!(source)->SessionNo.isNull()) (target)->SessionNo=(source)->SessionNo;	\
			if(!(source)->FrontNo.isNull()) (target)->FrontNo=(source)->FrontNo;	\
			if(!(source)->AccessLimit.isNull()) (target)->AccessLimit=(source)->AccessLimit;	\
			if(!(source)->UserType.isNull()) (target)->UserType=(source)->UserType;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->LimitAccesses.isNull()) (target)->LimitAccesses=(source)->LimitAccesses;	\
			if(!(source)->SinkType.isNull()) (target)->SinkType=(source)->SinkType;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->UpdateTime.isNull()) (target)->UpdateTime=(source)->UpdateTime;	\
	}

#endif

#ifndef CopyAPIKeyInsertEntity
#define CopyAPIKeyInsertEntity(target,source){	\
			(target)->Name=(source)->Name;	\
			(target)->UserID=(source)->UserID;	\
			(target)->APPID=(source)->APPID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccessKey=(source)->AccessKey;	\
			(target)->SecretKey=(source)->SecretKey;	\
			(target)->Auth=(source)->Auth;	\
			(target)->AccessLimit=(source)->AccessLimit;	\
			(target)->CreateTime=(source)->CreateTime;	\
			(target)->ExpireTime=(source)->ExpireTime;	\
			(target)->IPAddress=(source)->IPAddress;	\
	}

#endif

#ifndef CopyTrueAPIKeyInsertEntity
#define CopyTrueAPIKeyInsertEntity(target,source){	\
			if(!(source)->Name.isNull()) (target)->Name=(source)->Name;	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->APPID.isNull()) (target)->APPID=(source)->APPID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccessKey.isNull()) (target)->AccessKey=(source)->AccessKey;	\
			if(!(source)->SecretKey.isNull()) (target)->SecretKey=(source)->SecretKey;	\
			if(!(source)->Auth.isNull()) (target)->Auth=(source)->Auth;	\
			if(!(source)->AccessLimit.isNull()) (target)->AccessLimit=(source)->AccessLimit;	\
			if(!(source)->CreateTime.isNull()) (target)->CreateTime=(source)->CreateTime;	\
			if(!(source)->ExpireTime.isNull()) (target)->ExpireTime=(source)->ExpireTime;	\
			if(!(source)->IPAddress.isNull()) (target)->IPAddress=(source)->IPAddress;	\
	}

#endif

#ifndef CopyAPIKeyEntity
#define CopyAPIKeyEntity(target,source){	\
			CopyAPIKeyInsertEntity(target,source);	\
			(target)->RemainDays=(source)->RemainDays;	\
			(target)->Status=(source)->Status;	\
	}

#endif

#ifndef CopyTrueAPIKeyEntity
#define CopyTrueAPIKeyEntity(target,source){	\
			CopyTrueAPIKeyInsertEntity(target,source);	\
			if(!(source)->RemainDays.isNull()) (target)->RemainDays=(source)->RemainDays;	\
			if(!(source)->Status.isNull()) (target)->Status=(source)->Status;	\
	}

#endif

#ifndef CopyAPIKeyActionEntity
#define CopyAPIKeyActionEntity(target,source){	\
			(target)->Name=(source)->Name;	\
			(target)->AccessKey=(source)->AccessKey;	\
			(target)->Auth=(source)->Auth;	\
			(target)->AccessLimit=(source)->AccessLimit;	\
			(target)->IPAddress=(source)->IPAddress;	\
			(target)->ExpireTime=(source)->ExpireTime;	\
			(target)->ActionFlag=(source)->ActionFlag;	\
	}

#endif

#ifndef CopyTrueAPIKeyActionEntity
#define CopyTrueAPIKeyActionEntity(target,source){	\
			if(!(source)->Name.isNull()) (target)->Name=(source)->Name;	\
			if(!(source)->AccessKey.isNull()) (target)->AccessKey=(source)->AccessKey;	\
			if(!(source)->Auth.isNull()) (target)->Auth=(source)->Auth;	\
			if(!(source)->AccessLimit.isNull()) (target)->AccessLimit=(source)->AccessLimit;	\
			if(!(source)->IPAddress.isNull()) (target)->IPAddress=(source)->IPAddress;	\
			if(!(source)->ExpireTime.isNull()) (target)->ExpireTime=(source)->ExpireTime;	\
			if(!(source)->ActionFlag.isNull()) (target)->ActionFlag=(source)->ActionFlag;	\
	}

#endif

#ifndef CopyOTokenInsertEntity
#define CopyOTokenInsertEntity(target,source){	\
			(target)->UserID=(source)->UserID;	\
			(target)->APPID=(source)->APPID;	\
			(target)->Token=(source)->Token;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->LoginTime=(source)->LoginTime;	\
			(target)->ExpireTime=(source)->ExpireTime;	\
			(target)->AccessLimit=(source)->AccessLimit;	\
	}

#endif

#ifndef CopyTrueOTokenInsertEntity
#define CopyTrueOTokenInsertEntity(target,source){	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->APPID.isNull()) (target)->APPID=(source)->APPID;	\
			if(!(source)->Token.isNull()) (target)->Token=(source)->Token;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->LoginTime.isNull()) (target)->LoginTime=(source)->LoginTime;	\
			if(!(source)->ExpireTime.isNull()) (target)->ExpireTime=(source)->ExpireTime;	\
			if(!(source)->AccessLimit.isNull()) (target)->AccessLimit=(source)->AccessLimit;	\
	}

#endif

#ifndef CopyOTokenActionEntity
#define CopyOTokenActionEntity(target,source){	\
			(target)->UserID=(source)->UserID;	\
			(target)->APPID=(source)->APPID;	\
			(target)->ActionFlag=(source)->ActionFlag;	\
	}

#endif

#ifndef CopyTrueOTokenActionEntity
#define CopyTrueOTokenActionEntity(target,source){	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->APPID.isNull()) (target)->APPID=(source)->APPID;	\
			if(!(source)->ActionFlag.isNull()) (target)->ActionFlag=(source)->ActionFlag;	\
	}

#endif

#ifndef CopyQryFeeEntity
#define CopyQryFeeEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->MatchRole=(source)->MatchRole;	\
	}

#endif

#ifndef CopyTrueQryFeeEntity
#define CopyTrueQryFeeEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->MatchRole.isNull()) (target)->MatchRole=(source)->MatchRole;	\
	}

#endif

#ifndef CopyQryMarginRateEntity
#define CopyQryMarginRateEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->AccountID=(source)->AccountID;	\
	}

#endif

#ifndef CopyTrueQryMarginRateEntity
#define CopyTrueQryMarginRateEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
	}

#endif

#ifndef CopyClearMarketEntity
#define CopyClearMarketEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
	}

#endif

#ifndef CopyTrueClearMarketEntity
#define CopyTrueClearMarketEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
	}

#endif

#ifndef CopyClearLastKLineEntity
#define CopyClearLastKLineEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Bar=(source)->Bar;	\
	}

#endif

#ifndef CopyTrueClearLastKLineEntity
#define CopyTrueClearLastKLineEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Bar.isNull()) (target)->Bar=(source)->Bar;	\
	}

#endif

#ifndef CopyUserPasswordUpdateEntity
#define CopyUserPasswordUpdateEntity(target,source){	\
			(target)->UserID=(source)->UserID;	\
			(target)->OldPassword=(source)->OldPassword;	\
			(target)->NewPassword=(source)->NewPassword;	\
	}

#endif

#ifndef CopyTrueUserPasswordUpdateEntity
#define CopyTrueUserPasswordUpdateEntity(target,source){	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->OldPassword.isNull()) (target)->OldPassword=(source)->OldPassword;	\
			if(!(source)->NewPassword.isNull()) (target)->NewPassword=(source)->NewPassword;	\
	}

#endif

#ifndef CopyPositionActionEntity
#define CopyPositionActionEntity(target,source){	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->PositionID=(source)->PositionID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->ProductGroup=(source)->ProductGroup;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->TradeUnitID=(source)->TradeUnitID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->PosiDirection=(source)->PosiDirection;	\
			(target)->Amount=(source)->Amount;	\
			(target)->LongLeverage=(source)->LongLeverage;	\
			(target)->ShortLeverage=(source)->ShortLeverage;	\
			(target)->NetLeverage=(source)->NetLeverage;	\
			(target)->ActionType=(source)->ActionType;	\
			(target)->IsCrossMargin=(source)->IsCrossMargin;	\
			(target)->IsAutoAddMargin=(source)->IsAutoAddMargin;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTruePositionActionEntity
#define CopyTruePositionActionEntity(target,source){	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->PositionID.isNull()) (target)->PositionID=(source)->PositionID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->ProductGroup.isNull()) (target)->ProductGroup=(source)->ProductGroup;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->TradeUnitID.isNull()) (target)->TradeUnitID=(source)->TradeUnitID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->PosiDirection.isNull()) (target)->PosiDirection=(source)->PosiDirection;	\
			if(!(source)->Amount.isNull()) (target)->Amount=(source)->Amount;	\
			if(!(source)->LongLeverage.isNull()) (target)->LongLeverage=(source)->LongLeverage;	\
			if(!(source)->ShortLeverage.isNull()) (target)->ShortLeverage=(source)->ShortLeverage;	\
			if(!(source)->NetLeverage.isNull()) (target)->NetLeverage=(source)->NetLeverage;	\
			if(!(source)->ActionType.isNull()) (target)->ActionType=(source)->ActionType;	\
			if(!(source)->IsCrossMargin.isNull()) (target)->IsCrossMargin=(source)->IsCrossMargin;	\
			if(!(source)->IsAutoAddMargin.isNull()) (target)->IsAutoAddMargin=(source)->IsAutoAddMargin;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyQryOrderEntity
#define CopyQryOrderEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->OrderID=(source)->OrderID;	\
	}

#endif

#ifndef CopyTrueQryOrderEntity
#define CopyTrueQryOrderEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->OrderID.isNull()) (target)->OrderID=(source)->OrderID;	\
	}

#endif

#ifndef CopyQryTriggerOrderEntity
#define CopyQryTriggerOrderEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->LocalID=(source)->LocalID;	\
	}

#endif

#ifndef CopyTrueQryTriggerOrderEntity
#define CopyTrueQryTriggerOrderEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
	}

#endif

#ifndef CopyQryLeverageEntity
#define CopyQryLeverageEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->TradeUnitID=(source)->TradeUnitID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->PosiDirection=(source)->PosiDirection;	\
	}

#endif

#ifndef CopyTrueQryLeverageEntity
#define CopyTrueQryLeverageEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->TradeUnitID.isNull()) (target)->TradeUnitID=(source)->TradeUnitID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->PosiDirection.isNull()) (target)->PosiDirection=(source)->PosiDirection;	\
	}

#endif

#ifndef CopyLeverageEntity
#define CopyLeverageEntity(target,source){	\
			CopyQryLeverageEntity(target,source);	\
			(target)->DefaultMaxLeverage=(source)->DefaultMaxLeverage;	\
			(target)->LongLeverage=(source)->LongLeverage;	\
			(target)->LongMaxLeverage=(source)->LongMaxLeverage;	\
			(target)->LongMaxVolume=(source)->LongMaxVolume;	\
			(target)->LongLastVolume=(source)->LongLastVolume;	\
			(target)->LongLevel=(source)->LongLevel;	\
			(target)->LongMaintRatio=(source)->LongMaintRatio;	\
			(target)->ShortLeverage=(source)->ShortLeverage;	\
			(target)->ShortMaxLeverage=(source)->ShortMaxLeverage;	\
			(target)->ShortMaxVolume=(source)->ShortMaxVolume;	\
			(target)->ShortLastVolume=(source)->ShortLastVolume;	\
			(target)->ShortLevel=(source)->ShortLevel;	\
			(target)->ShortMaintRatio=(source)->ShortMaintRatio;	\
			(target)->IsCrossMargin=(source)->IsCrossMargin;	\
	}

#endif

#ifndef CopyTrueLeverageEntity
#define CopyTrueLeverageEntity(target,source){	\
			CopyTrueQryLeverageEntity(target,source);	\
			if(!(source)->DefaultMaxLeverage.isNull()) (target)->DefaultMaxLeverage=(source)->DefaultMaxLeverage;	\
			if(!(source)->LongLeverage.isNull()) (target)->LongLeverage=(source)->LongLeverage;	\
			if(!(source)->LongMaxLeverage.isNull()) (target)->LongMaxLeverage=(source)->LongMaxLeverage;	\
			if(!(source)->LongMaxVolume.isNull()) (target)->LongMaxVolume=(source)->LongMaxVolume;	\
			if(!(source)->LongLastVolume.isNull()) (target)->LongLastVolume=(source)->LongLastVolume;	\
			if(!(source)->LongLevel.isNull()) (target)->LongLevel=(source)->LongLevel;	\
			if(!(source)->LongMaintRatio.isNull()) (target)->LongMaintRatio=(source)->LongMaintRatio;	\
			if(!(source)->ShortLeverage.isNull()) (target)->ShortLeverage=(source)->ShortLeverage;	\
			if(!(source)->ShortMaxLeverage.isNull()) (target)->ShortMaxLeverage=(source)->ShortMaxLeverage;	\
			if(!(source)->ShortMaxVolume.isNull()) (target)->ShortMaxVolume=(source)->ShortMaxVolume;	\
			if(!(source)->ShortLastVolume.isNull()) (target)->ShortLastVolume=(source)->ShortLastVolume;	\
			if(!(source)->ShortLevel.isNull()) (target)->ShortLevel=(source)->ShortLevel;	\
			if(!(source)->ShortMaintRatio.isNull()) (target)->ShortMaintRatio=(source)->ShortMaintRatio;	\
			if(!(source)->IsCrossMargin.isNull()) (target)->IsCrossMargin=(source)->IsCrossMargin;	\
	}

#endif

#ifndef CopyQryAPIKeyEntity
#define CopyQryAPIKeyEntity(target,source){	\
			(target)->UserID=(source)->UserID;	\
	}

#endif

#ifndef CopyTrueQryAPIKeyEntity
#define CopyTrueQryAPIKeyEntity(target,source){	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
	}

#endif

#ifndef CopyQryMarketOrderEntity
#define CopyQryMarketOrderEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Direction=(source)->Direction;	\
			(target)->Depth=(source)->Depth;	\
			(target)->Tick=(source)->Tick;	\
	}

#endif

#ifndef CopyTrueQryMarketOrderEntity
#define CopyTrueQryMarketOrderEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Direction.isNull()) (target)->Direction=(source)->Direction;	\
			if(!(source)->Depth.isNull()) (target)->Depth=(source)->Depth;	\
			if(!(source)->Tick.isNull()) (target)->Tick=(source)->Tick;	\
	}

#endif

#ifndef CopyQryMemberPositionEntity
#define CopyQryMemberPositionEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
	}

#endif

#ifndef CopyTrueQryMemberPositionEntity
#define CopyTrueQryMemberPositionEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
	}

#endif

#ifndef CopyQryPositionEntity
#define CopyQryPositionEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->PositionClass=(source)->PositionClass;	\
	}

#endif

#ifndef CopyTrueQryPositionEntity
#define CopyTrueQryPositionEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->PositionClass.isNull()) (target)->PositionClass=(source)->PositionClass;	\
	}

#endif

#ifndef CopyQryAccountEntity
#define CopyQryAccountEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->Currency=(source)->Currency;	\
	}

#endif

#ifndef CopyTrueQryAccountEntity
#define CopyTrueQryAccountEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
	}

#endif

#ifndef CopyQryInstrumentEntity
#define CopyQryInstrumentEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->ProductGroup=(source)->ProductGroup;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
	}

#endif

#ifndef CopyTrueQryInstrumentEntity
#define CopyTrueQryInstrumentEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->ProductGroup.isNull()) (target)->ProductGroup=(source)->ProductGroup;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
	}

#endif

#ifndef CopyQryCurrencyEntity
#define CopyQryCurrencyEntity(target,source){	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->Currency=(source)->Currency;	\
	}

#endif

#ifndef CopyTrueQryCurrencyEntity
#define CopyTrueQryCurrencyEntity(target,source){	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
	}

#endif

#ifndef CopyQryMarketDataEntity
#define CopyQryMarketDataEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->ProductGroup=(source)->ProductGroup;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->From=(source)->From;	\
			(target)->Limit=(source)->Limit;	\
	}

#endif

#ifndef CopyTrueQryMarketDataEntity
#define CopyTrueQryMarketDataEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->ProductGroup.isNull()) (target)->ProductGroup=(source)->ProductGroup;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->From.isNull()) (target)->From=(source)->From;	\
			if(!(source)->Limit.isNull()) (target)->Limit=(source)->Limit;	\
	}

#endif

#ifndef CopyQryMemberEntity
#define CopyQryMemberEntity(target,source){	\
			(target)->MemberType=(source)->MemberType;	\
			(target)->MemberID=(source)->MemberID;	\
	}

#endif

#ifndef CopyTrueQryMemberEntity
#define CopyTrueQryMemberEntity(target,source){	\
			if(!(source)->MemberType.isNull()) (target)->MemberType=(source)->MemberType;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
	}

#endif

#ifndef CopyQryTradeEntity
#define CopyQryTradeEntity(target,source){	\
			(target)->OrderID=(source)->OrderID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Limit=(source)->Limit;	\
			(target)->Before=(source)->Before;	\
			(target)->After=(source)->After;	\
			(target)->StartTime=(source)->StartTime;	\
			(target)->EndTime=(source)->EndTime;	\
	}

#endif

#ifndef CopyTrueQryTradeEntity
#define CopyTrueQryTradeEntity(target,source){	\
			if(!(source)->OrderID.isNull()) (target)->OrderID=(source)->OrderID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Limit.isNull()) (target)->Limit=(source)->Limit;	\
			if(!(source)->Before.isNull()) (target)->Before=(source)->Before;	\
			if(!(source)->After.isNull()) (target)->After=(source)->After;	\
			if(!(source)->StartTime.isNull()) (target)->StartTime=(source)->StartTime;	\
			if(!(source)->EndTime.isNull()) (target)->EndTime=(source)->EndTime;	\
	}

#endif

#ifndef CopyQryFinishOrderEntity
#define CopyQryFinishOrderEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->OrderID=(source)->OrderID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->OrderStatus=(source)->OrderStatus;	\
			(target)->Limit=(source)->Limit;	\
			(target)->Before=(source)->Before;	\
			(target)->After=(source)->After;	\
			(target)->StartTime=(source)->StartTime;	\
			(target)->EndTime=(source)->EndTime;	\
	}

#endif

#ifndef CopyTrueQryFinishOrderEntity
#define CopyTrueQryFinishOrderEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->OrderID.isNull()) (target)->OrderID=(source)->OrderID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->OrderStatus.isNull()) (target)->OrderStatus=(source)->OrderStatus;	\
			if(!(source)->Limit.isNull()) (target)->Limit=(source)->Limit;	\
			if(!(source)->Before.isNull()) (target)->Before=(source)->Before;	\
			if(!(source)->After.isNull()) (target)->After=(source)->After;	\
			if(!(source)->StartTime.isNull()) (target)->StartTime=(source)->StartTime;	\
			if(!(source)->EndTime.isNull()) (target)->EndTime=(source)->EndTime;	\
	}

#endif

#ifndef CopyQryFinishTriggerOrderEntity
#define CopyQryFinishTriggerOrderEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->OrderID=(source)->OrderID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->TriggerStatus=(source)->TriggerStatus;	\
			(target)->TriggerOrderType=(source)->TriggerOrderType;	\
			(target)->Limit=(source)->Limit;	\
			(target)->Before=(source)->Before;	\
			(target)->After=(source)->After;	\
			(target)->StartTime=(source)->StartTime;	\
			(target)->EndTime=(source)->EndTime;	\
	}

#endif

#ifndef CopyTrueQryFinishTriggerOrderEntity
#define CopyTrueQryFinishTriggerOrderEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->OrderID.isNull()) (target)->OrderID=(source)->OrderID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->TriggerStatus.isNull()) (target)->TriggerStatus=(source)->TriggerStatus;	\
			if(!(source)->TriggerOrderType.isNull()) (target)->TriggerOrderType=(source)->TriggerOrderType;	\
			if(!(source)->Limit.isNull()) (target)->Limit=(source)->Limit;	\
			if(!(source)->Before.isNull()) (target)->Before=(source)->Before;	\
			if(!(source)->After.isNull()) (target)->After=(source)->After;	\
			if(!(source)->StartTime.isNull()) (target)->StartTime=(source)->StartTime;	\
			if(!(source)->EndTime.isNull()) (target)->EndTime=(source)->EndTime;	\
	}

#endif

#ifndef CopyQryWithLeverageEntity
#define CopyQryWithLeverageEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->TradeUnitID=(source)->TradeUnitID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->PosiDirection=(source)->PosiDirection;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->LongLeverage=(source)->LongLeverage;	\
			(target)->ShortLeverage=(source)->ShortLeverage;	\
			(target)->NetLeverage=(source)->NetLeverage;	\
	}

#endif

#ifndef CopyTrueQryWithLeverageEntity
#define CopyTrueQryWithLeverageEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->TradeUnitID.isNull()) (target)->TradeUnitID=(source)->TradeUnitID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->PosiDirection.isNull()) (target)->PosiDirection=(source)->PosiDirection;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->LongLeverage.isNull()) (target)->LongLeverage=(source)->LongLeverage;	\
			if(!(source)->ShortLeverage.isNull()) (target)->ShortLeverage=(source)->ShortLeverage;	\
			if(!(source)->NetLeverage.isNull()) (target)->NetLeverage=(source)->NetLeverage;	\
	}

#endif

#ifndef CopyQryKLineEntity
#define CopyQryKLineEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Bar=(source)->Bar;	\
			(target)->EndTime=(source)->EndTime;	\
			(target)->Limit=(source)->Limit;	\
	}

#endif

#ifndef CopyTrueQryKLineEntity
#define CopyTrueQryKLineEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Bar.isNull()) (target)->Bar=(source)->Bar;	\
			if(!(source)->EndTime.isNull()) (target)->EndTime=(source)->EndTime;	\
			if(!(source)->Limit.isNull()) (target)->Limit=(source)->Limit;	\
	}

#endif

#ifndef CopyQryLineEntity
#define CopyQryLineEntity(target,source){	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Bar=(source)->Bar;	\
			(target)->Limit=(source)->Limit;	\
	}

#endif

#ifndef CopyTrueQryLineEntity
#define CopyTrueQryLineEntity(target,source){	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Bar.isNull()) (target)->Bar=(source)->Bar;	\
			if(!(source)->Limit.isNull()) (target)->Limit=(source)->Limit;	\
	}

#endif

#ifndef CopyQryAccountDetailEntity
#define CopyQryAccountDetailEntity(target,source){	\
			(target)->ProductGroup=(source)->ProductGroup;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->Currency=(source)->Currency;	\
			(target)->SettlementGroup=(source)->SettlementGroup;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Source=(source)->Source;	\
			(target)->Limit=(source)->Limit;	\
			(target)->Before=(source)->Before;	\
			(target)->After=(source)->After;	\
			(target)->StartTime=(source)->StartTime;	\
			(target)->EndTime=(source)->EndTime;	\
	}

#endif

#ifndef CopyTrueQryAccountDetailEntity
#define CopyTrueQryAccountDetailEntity(target,source){	\
			if(!(source)->ProductGroup.isNull()) (target)->ProductGroup=(source)->ProductGroup;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
			if(!(source)->SettlementGroup.isNull()) (target)->SettlementGroup=(source)->SettlementGroup;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Source.isNull()) (target)->Source=(source)->Source;	\
			if(!(source)->Limit.isNull()) (target)->Limit=(source)->Limit;	\
			if(!(source)->Before.isNull()) (target)->Before=(source)->Before;	\
			if(!(source)->After.isNull()) (target)->After=(source)->After;	\
			if(!(source)->StartTime.isNull()) (target)->StartTime=(source)->StartTime;	\
			if(!(source)->EndTime.isNull()) (target)->EndTime=(source)->EndTime;	\
	}

#endif

#ifndef CopyQrySettleDetailEntity
#define CopyQrySettleDetailEntity(target,source){	\
			(target)->ProductGroup=(source)->ProductGroup;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->SettleAction=(source)->SettleAction;	\
	}

#endif

#ifndef CopyTrueQrySettleDetailEntity
#define CopyTrueQrySettleDetailEntity(target,source){	\
			if(!(source)->ProductGroup.isNull()) (target)->ProductGroup=(source)->ProductGroup;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->SettleAction.isNull()) (target)->SettleAction=(source)->SettleAction;	\
	}

#endif

#ifndef CopyOTCTradeInsertEntity
#define CopyOTCTradeInsertEntity(target,source){	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->Price=(source)->Price;	\
			(target)->Volume=(source)->Volume;	\
			(target)->BuyMemberID=(source)->BuyMemberID;	\
			(target)->BuyAccountID=(source)->BuyAccountID;	\
			(target)->BuyTradeUnitID=(source)->BuyTradeUnitID;	\
			(target)->BuyOffsetFlag=(source)->BuyOffsetFlag;	\
			(target)->SellMemberID=(source)->SellMemberID;	\
			(target)->SellAccountID=(source)->SellAccountID;	\
			(target)->SellTradeUnitID=(source)->SellTradeUnitID;	\
			(target)->SellOffsetFlag=(source)->SellOffsetFlag;	\
			(target)->OTCType=(source)->OTCType;	\
			(target)->TakerDirection=(source)->TakerDirection;	\
			(target)->TriggerPrice=(source)->TriggerPrice;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueOTCTradeInsertEntity
#define CopyTrueOTCTradeInsertEntity(target,source){	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->Price.isNull()) (target)->Price=(source)->Price;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->BuyMemberID.isNull()) (target)->BuyMemberID=(source)->BuyMemberID;	\
			if(!(source)->BuyAccountID.isNull()) (target)->BuyAccountID=(source)->BuyAccountID;	\
			if(!(source)->BuyTradeUnitID.isNull()) (target)->BuyTradeUnitID=(source)->BuyTradeUnitID;	\
			if(!(source)->BuyOffsetFlag.isNull()) (target)->BuyOffsetFlag=(source)->BuyOffsetFlag;	\
			if(!(source)->SellMemberID.isNull()) (target)->SellMemberID=(source)->SellMemberID;	\
			if(!(source)->SellAccountID.isNull()) (target)->SellAccountID=(source)->SellAccountID;	\
			if(!(source)->SellTradeUnitID.isNull()) (target)->SellTradeUnitID=(source)->SellTradeUnitID;	\
			if(!(source)->SellOffsetFlag.isNull()) (target)->SellOffsetFlag=(source)->SellOffsetFlag;	\
			if(!(source)->OTCType.isNull()) (target)->OTCType=(source)->OTCType;	\
			if(!(source)->TakerDirection.isNull()) (target)->TakerDirection=(source)->TakerDirection;	\
			if(!(source)->TriggerPrice.isNull()) (target)->TriggerPrice=(source)->TriggerPrice;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyOTCTradeEntity
#define CopyOTCTradeEntity(target,source){	\
			CopyOTCTradeInsertEntity(target,source);	\
			(target)->TradeID=(source)->TradeID;	\
			(target)->TradeTime=(source)->TradeTime;	\
			(target)->InsertTime=(source)->InsertTime;	\
			(target)->DeriveSource=(source)->DeriveSource;	\
			(target)->BuyOrderID=(source)->BuyOrderID;	\
			(target)->SellOrderID=(source)->SellOrderID;	\
	}

#endif

#ifndef CopyTrueOTCTradeEntity
#define CopyTrueOTCTradeEntity(target,source){	\
			CopyTrueOTCTradeInsertEntity(target,source);	\
			if(!(source)->TradeID.isNull()) (target)->TradeID=(source)->TradeID;	\
			if(!(source)->TradeTime.isNull()) (target)->TradeTime=(source)->TradeTime;	\
			if(!(source)->InsertTime.isNull()) (target)->InsertTime=(source)->InsertTime;	\
			if(!(source)->DeriveSource.isNull()) (target)->DeriveSource=(source)->DeriveSource;	\
			if(!(source)->BuyOrderID.isNull()) (target)->BuyOrderID=(source)->BuyOrderID;	\
			if(!(source)->SellOrderID.isNull()) (target)->SellOrderID=(source)->SellOrderID;	\
	}

#endif

#ifndef CopyCommandHeadEntity
#define CopyCommandHeadEntity(target,source){	\
			(target)->IsLast=(source)->IsLast;	\
	}

#endif

#ifndef CopyTrueCommandHeadEntity
#define CopyTrueCommandHeadEntity(target,source){	\
			if(!(source)->IsLast.isNull()) (target)->IsLast=(source)->IsLast;	\
	}

#endif

#ifndef CopyPositionReverseEntity
#define CopyPositionReverseEntity(target,source){	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->PositionID=(source)->PositionID;	\
			(target)->Volume=(source)->Volume;	\
			(target)->Remark=(source)->Remark;	\
			(target)->CopyMemberID=(source)->CopyMemberID;	\
			(target)->APPID=(source)->APPID;	\
	}

#endif

#ifndef CopyTruePositionReverseEntity
#define CopyTruePositionReverseEntity(target,source){	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->PositionID.isNull()) (target)->PositionID=(source)->PositionID;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
			if(!(source)->CopyMemberID.isNull()) (target)->CopyMemberID=(source)->CopyMemberID;	\
			if(!(source)->APPID.isNull()) (target)->APPID=(source)->APPID;	\
	}

#endif

#ifndef CopyInputRobotOrderEntity
#define CopyInputRobotOrderEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->TradeUnitID=(source)->TradeUnitID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->UserID=(source)->UserID;	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID=(source)->InstrumentID;	\
			(target)->OrderPriceType=(source)->OrderPriceType;	\
			(target)->Direction=(source)->Direction;	\
			(target)->OffsetFlag=(source)->OffsetFlag;	\
			(target)->Price=(source)->Price;	\
			(target)->Volume=(source)->Volume;	\
			(target)->VolumeDisplay=(source)->VolumeDisplay;	\
	}

#endif

#ifndef CopyTrueInputRobotOrderEntity
#define CopyTrueInputRobotOrderEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->TradeUnitID.isNull()) (target)->TradeUnitID=(source)->TradeUnitID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->UserID.isNull()) (target)->UserID=(source)->UserID;	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID.isNull()) (target)->InstrumentID=(source)->InstrumentID;	\
			if(!(source)->OrderPriceType.isNull()) (target)->OrderPriceType=(source)->OrderPriceType;	\
			if(!(source)->Direction.isNull()) (target)->Direction=(source)->Direction;	\
			if(!(source)->OffsetFlag.isNull()) (target)->OffsetFlag=(source)->OffsetFlag;	\
			if(!(source)->Price.isNull()) (target)->Price=(source)->Price;	\
			if(!(source)->Volume.isNull()) (target)->Volume=(source)->Volume;	\
			if(!(source)->VolumeDisplay.isNull()) (target)->VolumeDisplay=(source)->VolumeDisplay;	\
	}

#endif

#ifndef CopyRobotOrderEntity
#define CopyRobotOrderEntity(target,source){	\
			CopyInputRobotOrderEntity(target,source);	\
			CopyInnerOrderEntity(target,source);	\
			(target)->LastPriceByInsert=(source)->LastPriceByInsert;	\
			(target)->CreateTime=(source)->CreateTime;	\
	}

#endif

#ifndef CopyTrueRobotOrderEntity
#define CopyTrueRobotOrderEntity(target,source){	\
			CopyTrueInputRobotOrderEntity(target,source);	\
			CopyTrueInnerOrderEntity(target,source);	\
			if(!(source)->LastPriceByInsert.isNull()) (target)->LastPriceByInsert=(source)->LastPriceByInsert;	\
			if(!(source)->CreateTime.isNull()) (target)->CreateTime=(source)->CreateTime;	\
	}

#endif

#ifndef CopyChangeCurrencyEntity
#define CopyChangeCurrencyEntity(target,source){	\
			(target)->MemberID=(source)->MemberID;	\
			(target)->AccountID=(source)->AccountID;	\
			(target)->LocalID=(source)->LocalID;	\
			(target)->ExchangeID=(source)->ExchangeID;	\
			(target)->InstrumentID1=(source)->InstrumentID1;	\
			(target)->InstrumentID2=(source)->InstrumentID2;	\
			(target)->Currency=(source)->Currency;	\
			(target)->Amount=(source)->Amount;	\
			(target)->Remark=(source)->Remark;	\
	}

#endif

#ifndef CopyTrueChangeCurrencyEntity
#define CopyTrueChangeCurrencyEntity(target,source){	\
			if(!(source)->MemberID.isNull()) (target)->MemberID=(source)->MemberID;	\
			if(!(source)->AccountID.isNull()) (target)->AccountID=(source)->AccountID;	\
			if(!(source)->LocalID.isNull()) (target)->LocalID=(source)->LocalID;	\
			if(!(source)->ExchangeID.isNull()) (target)->ExchangeID=(source)->ExchangeID;	\
			if(!(source)->InstrumentID1.isNull()) (target)->InstrumentID1=(source)->InstrumentID1;	\
			if(!(source)->InstrumentID2.isNull()) (target)->InstrumentID2=(source)->InstrumentID2;	\
			if(!(source)->Currency.isNull()) (target)->Currency=(source)->Currency;	\
			if(!(source)->Amount.isNull()) (target)->Amount=(source)->Amount;	\
			if(!(source)->Remark.isNull()) (target)->Remark=(source)->Remark;	\
	}

#endif


#endif

