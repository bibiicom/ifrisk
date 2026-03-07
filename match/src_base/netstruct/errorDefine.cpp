/////////////////////////////////////////////////////////////////////////
///errorDefine.cpp
///定义了错误引擎初始化的函数
/////////////////////////////////////////////////////////////////////////

#include "errorDefine.h"

void defineErrors(CErrorEngine *pEngine)
{
	pEngine->registerErrorType(ERROR_NONE,(char *)"None");
	pEngine->registerErrorType(ERROR_SystemNotReady,(char *)"SystemNotReady");
	pEngine->registerErrorType(ERROR_RECORD_NOT_FOUND,(char *)"RecordNotFound");
	pEngine->registerErrorType(ERROR_RECORD_EXIST,(char *)"RecordExist");
	pEngine->registerErrorType(ERROR_INVALID_ACTION,(char *)"InvalidAction");
	pEngine->registerErrorType(ERROR_INVALID_VALUE,(char *)"InvalidValue");
	pEngine->registerErrorType(ERROR_InvalidStatus,(char *)"InvalidStatus");
	pEngine->registerErrorType(ERROR_INVALID_SESSION,(char *)"InvalidSession");
	pEngine->registerErrorType(ERROR_INSTRUMENT_NOT_FOUND,(char *)"InstrumentNotFound");
	pEngine->registerErrorType(ERROR_MEMBER_NOT_FOUND,(char *)"MemberNotFound");
	pEngine->registerErrorType(ERROR_MARKETDATA_NOT_FOUND,(char *)"MarketDataNotFound");
	pEngine->registerErrorType(ERROR_BAD_FIELD,(char *)"BadField");
	pEngine->registerErrorType(ERROR_DUPLICATE_ACTION,(char *)"DuplicateAction");
	pEngine->registerErrorType(ERROR_MARKET_ORDER_CAN_NOT_QUEUE,(char *)"MarketOrderCanNotQueue");
	pEngine->registerErrorType(ERROR_GTD_ORDER_EXPIRED,(char *)"GtdOrderExpired");
	pEngine->registerErrorType(ERROR_MV_ORDER_EXCEED_VOLUMN,(char *)"MvOrderExceedVolumn");
	pEngine->registerErrorType(ERROR_ORDER_EXIST,(char *)"OrderExist");
	pEngine->registerErrorType(ERROR_ORDER_NOT_FOUND,(char *)"OrderNotFound");
	pEngine->registerErrorType(ERROR_QUOTE_NOT_FOUND,(char *)"QuoteNotFound");
	pEngine->registerErrorType(ERROR_INVALID_INSTRUMENT_STATUS,(char *)"InvalidInstrumentStatus");
	pEngine->registerErrorType(ERROR_INVALID_INSTRUMENT_STATUS_SHIFT,(char *)"InvalidInstrumentStatusShift");
	pEngine->registerErrorType(ERROR_NOT_ENOUGH_VOLUME_TO_MODIFY,(char *)"NotEnoughVolumeToModify");
	pEngine->registerErrorType(ERROR_NOT_ENOUGH_POSITION_TO_CLOSE,(char *)"NotEnoughPositionToClose");
	pEngine->registerErrorType(ERROR_POSITION_LIMIT_EXCEED,(char *)"PositionLimit");
	pEngine->registerErrorType(ERROR_AccountLessThanZeroAfterClose,(char *)"AccountLessThanZeroAfterClose");
	pEngine->registerErrorType(ERROR_MEMBER_POSITION_LIMIT_EXCEED,(char *)"MemberPositionLimit");
	pEngine->registerErrorType(ERROR_AccountNotFound,(char *)"AccountNotFound");
	pEngine->registerErrorType(ERROR_INSUFFICIENT_MONEY,(char *)"InsufficientMoney");
	pEngine->registerErrorType(ERROR_INVALID_VOLUME,(char *)"InvalidVolume");
	pEngine->registerErrorType(ERROR_VOLUME_NOT_ON_TICK,(char *)"VolumeNotOnTick");
	pEngine->registerErrorType(ERROR_PRICE_NOT_ON_TICK,(char *)"PriceNotOnTick");
	pEngine->registerErrorType(ERROR_PriceOutOfUpperLimit,(char *)"PriceOutOfUpperLimit");
	pEngine->registerErrorType(ERROR_PriceOutOfLowerLimit,(char *)"PriceOutOfLowerLimit");
	pEngine->registerErrorType(ERROR_NO_TRADING_RIGHT,(char *)"NoTradingRight");
	pEngine->registerErrorType(ERROR_CLOSE_ONLY,(char *)"CloseOnly");
	pEngine->registerErrorType(ERROR_USER_NOT_LOGIN,(char *)"UserNotLogin");
	pEngine->registerErrorType(ERROR_ONLY_TRADE_MANAGER_CAN_DO_THIS,(char *)"OnlyTradeManagerCanDoThis");
	pEngine->registerErrorType(ERROR_USER_MISMATCH,(char *)"UserMismatch");
	pEngine->registerErrorType(ERROR_USER_RELOGIN,(char *)"UserRelogin");
	pEngine->registerErrorType(ERROR_INVALID_USER_OR_PASSWORD,(char *)"InvalidUserOrPassword");
	pEngine->registerErrorType(ERROR_USER_NOT_ACTIVE,(char *)"UserNotActive");
	pEngine->registerErrorType(ERROR_INVALID_LOGIN_IPADDRESS,(char *)"InvalidLoginIpaddress");
	pEngine->registerErrorType(ERROR_CAN_NOT_OPERATE_DERIVED_ORDER,(char *)"CanNotOperateDerivedOrder");
	pEngine->registerErrorType(ERROR_ORDER_HAD_SUSPENDED,(char *)"OrderHadSuspended");
	pEngine->registerErrorType(ERROR_ORDER_HAD_ACTIVED,(char *)"OrderHadActived");
	pEngine->registerErrorType(ERROR_GTD_ORDER_DATE_MISSED,(char *)"GtdOrderDateMissed");
	pEngine->registerErrorType(ERROR_UnSupportOrderType,(char *)"UnSupportOrderType");
	pEngine->registerErrorType(ERROR_USER_NO_PERMISSION,(char *)"UserNoPermission");
	pEngine->registerErrorType(ERROR_USER_NOT_FOUND,(char *)"UserNotFound");
	pEngine->registerErrorType(ERROR_CanNotActionForOtherUser,(char *)"CanNotActionForOtherUser");
	pEngine->registerErrorType(ERROR_NotEnoughMargin,(char *)"NotEnoughMargin");
	pEngine->registerErrorType(ERROR_COMBINATION_ON_SINGLE_INSTRUMENT,(char *)"CombinationOnSingleInstrument");
	pEngine->registerErrorType(ERROR_OTC_TYPE_ERROR,(char *)"OtcTypeError");
	pEngine->registerErrorType(ERROR_NOT_ENOUGH_LEVERAGE,(char *)"NotEnoughLeverage");
	pEngine->registerErrorType(ERROR_PRICE_MUST_GREATER_THAN_ZERO,(char *)"PriceMustGreaterThanZero");
	pEngine->registerErrorType(ERROR_INVALID_APIKEY,(char *)"InvalidAPIKey");
	pEngine->registerErrorType(ERROR_APIKEY_EXPIRED,(char *)"APIKeyExpired");
	pEngine->registerErrorType(ERROR_APIKEY_LIMIT_EXCEED,(char *)"APIKeyLimitExceed");
	pEngine->registerErrorType(ERROR_KEY_IS_NULL,(char *)"KeyIsNull");
	pEngine->registerErrorType(ERROR_CAN_NOT_FIND_MARGIN_RATE,(char *)"CanNotFoundMarginRate");
	pEngine->registerErrorType(ERROR_DUPLICATE_APIKEY,(char *)"DuplicateAPIKey");
	pEngine->registerErrorType(ERROR_NO_LIMITPRICE,(char *)"NoLimitPrice");
	pEngine->registerErrorType(ERROR_EXCEED_MAX_QUERY_COUNT_PER_SECOND,(char *)"ExceedMaxQueryCountPerSecond");
	pEngine->registerErrorType(ERROR_ORDER_LIMIT_EXCEED,(char *)"OrderLimitExceed");
	pEngine->registerErrorType(ERROR_NOT_ENOUGH_OPENORDERS,(char *)"NotEnoughOpenorders");
	pEngine->registerErrorType(ERROR_SESSION_NOT_FOUND,(char *)"SessionNotFound");
	pEngine->registerErrorType(ERROR_PRICE_OUT_OF_ASKPRICE1,(char *)"PriceOutOfAskPrice1");
	pEngine->registerErrorType(ERROR_PRICE_OUT_OF_BIDPRICE1,(char *)"PriceOutOfBidPrice1");
	pEngine->registerErrorType(ERROR_POSITION_EXIST,(char *)"PositionExist");
	pEngine->registerErrorType(ERROR_UnderlyingPriceError,(char *)"UnderlyingPriceError");
	pEngine->registerErrorType(ERROR_RecordAnalysisError,(char *)"RecordAnalysisError");
	pEngine->registerErrorType(ERROR_DuplicateRecord,(char *)"DuplicateRecord");
	pEngine->registerErrorType(ERROR_OutOfMaxVolume,(char *)"OutOfMaxVolume");
	pEngine->registerErrorType(ERROR_LessThanMinVolume,(char *)"LessThanMinVolume");
	pEngine->registerErrorType(ERROR_PositionLessThanMinVolume,(char *)"PositionLessThanMinVolume");
	pEngine->registerErrorType(ERROR_TRADE_FORBIDDEN,(char *)"TradeForbidden");
	pEngine->registerErrorType(ERROR_CAN_NOT_FIND_FEE,(char *)"CanNotFoundFee");
	pEngine->registerErrorType(ERROR_POSITION_COUNT_LIMIT_EXCEED,(char *)"PositionCountLimitExceed");
	pEngine->registerErrorType(ERROR_LEVERAGE_TOO_HIGH,(char *)"LeverageTooHigh");
	pEngine->registerErrorType(ERROR_POSITION_UNDER_DEFICIT,(char *)"PositionUnderDeficit");
	pEngine->registerErrorType(ERROR_CAN_NOT_CHANGE_POSTYPE,(char *)"CanNotChangePosType");
}

