// TableDefine.cpp
// 所需内存表定义
//////////////////////////////////////////////////////////////////////
#include "TableDefine.h"

TTinitTableInfo g_InitTableInfo[] = 
{
	{"ServiceConfig",&CServiceConfigField::m_Describe},	
	{"Currency",&CCurrencyField::m_Describe},	
	{"MarketData",&CMarketDataField::m_Describe},	
	{"Instrument",&CInstrumentField::m_Describe},	
	{"CurrentTime",&CCurrentTimeField::m_Describe},	
	{"Member",&CMemberField::m_Describe},	
	{"UserSession",&CUserSessionField::m_Describe},	
	{"LastKLine",&CLastKLineField::m_Describe},	
	{"CFDRate",&CCFDRateField::m_Describe},	
	{"TradingRight",&CTradingRightField::m_Describe},	
	{"MarginRate",&CMarginRateField::m_Describe},	
	{"Fee",&CFeeField::m_Describe},	
	{"Liquidity",&CLiquidityField::m_Describe},	
	{"Account",&CAccountField::m_Describe},	
	{"MemberPosition",&CMemberPositionField::m_Describe},	
	{"Position",&CPositionField::m_Describe},	
	{"Order",&COrderField::m_Describe},	
	{"TriggerOrder",&CTriggerOrderField::m_Describe},	
	{"PositionOrder",&CPositionOrderField::m_Describe},	
	{NULL,NULL}				//一定要有这一行，表示结束
};

TTinitTableInfo g_AllTableInfo[] = 
{
	{"ServiceConfig",&CServiceConfigField::m_Describe},
	{"Currency",&CCurrencyField::m_Describe},
	{"MarketData",&CMarketDataField::m_Describe},
	{"Instrument",&CInstrumentField::m_Describe},
	{"CurrentTime",&CCurrentTimeField::m_Describe},
	{"Member",&CMemberField::m_Describe},
	{"UserSession",&CUserSessionField::m_Describe},
	{"LastKLine",&CLastKLineField::m_Describe},
	{"CFDRate",&CCFDRateField::m_Describe},
	{"TradingRight",&CTradingRightField::m_Describe},
	{"MarginRate",&CMarginRateField::m_Describe},
	{"Fee",&CFeeField::m_Describe},
	{"Liquidity",&CLiquidityField::m_Describe},
	{"Account",&CAccountField::m_Describe},
	{"MemberPosition",&CMemberPositionField::m_Describe},
	{"Position",&CPositionField::m_Describe},
	{"Order",&COrderField::m_Describe},
	{"TriggerOrder",&CTriggerOrderField::m_Describe},
	{"PositionOrder",&CPositionOrderField::m_Describe},
	{NULL,NULL}				//一定要有这一行，表示结束
};

bool LoadTable(const char *pszCSVFileName, CFieldDescribe *pFieldDesc, CTinitTableCallback* pTinitTableCallback)
{
	PRINT_TO_STD("Init LoadTable %s", pszCSVFileName);

	CFieldAnalysis record;

	FILE *fp = mfopen(pszCSVFileName, "rb", false);
	if (fp == NULL)
	{
		PRINT_TO_STD(":Can't open csv file  ...NextFile");
		return true;
	}

	char bufRead[MAX_LINE_LEGTH];
	if (fgets(bufRead, MAX_LINE_LEGTH, fp) == NULL)
	{
		PRINT_TO_STD(":Missing csv file header  ...Exit");
		fclose(fp);
		return false;
	}

	if (!record.AnalysisFieldName(bufRead))
	{
		PRINT_TO_STD(":Anlysis csv file header error  ...Exit");
		fclose(fp);
		return false;
	}

	char bufStruct[MAX_LINE_LEGTH];
	bool bRet = true;
	int iLine = 1;

	while (fgets(bufRead, MAX_LINE_LEGTH, fp) != NULL)
	{
		if (!record.AnalysisFieldContent(bufRead))
		{
			PRINT_TO_STD("Anlysis csv file content error  ...Exit");
			bRet = false;
			break;
		}
		
		record.TransToStruct(pFieldDesc, bufStruct);
		pTinitTableCallback->SendTable(pFieldDesc, bufStruct);
		iLine++;
	}

	PRINT_TO_STD(" :%d  ...OK", iLine - 1);

	fclose(fp);
	return bRet;
}
