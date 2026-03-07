//////////////////////////////////////////////////////////////////////////
// 文件: ExLinkApi.h
// 功能: 定义了连接交易所的一个通用的实现，方便其他报盘机的编写
//       同时可用于模拟一个假的报盘机，用于测试核心的性能
//////////////////////////////////////////////////////////////////////////

#ifndef _ExLinkApi_H
#define _ExLinkApi_H

#include "FieldData.h"
#include "BaseFunction.h"
#include "printinfo.h"
#include "OutFogServiceApi.h"


#if _MSC_VER > 1000
#pragma once
#endif

//数据流类型

#define	DIALOG_FLOW  	1           //对话流
#define	PRIVATE_FLOW  	2           //私有流
#define	PUBLIC_FLOW     3           //公共流
#define	QUERY_FLOW		4			//查询

#define DefaultLocalIDSuffix "0000"
#define LocalIDSuffixLength 4

#define	ProductInfo  "OUTFOG_v1.0"	//产品信息
#define OUTFOG_MAX_PACKAGE_SIZEDD  1024


//登陆状态,增加正在登陆时,连接线程不需要发起登录请求
enum LoginStatus
{
	NOT_LOGIN =0,
	LOGGING   =1,
	LOGIN_OK  =2,
};

typedef set<string> SetQdamClientID;//投资者容器

struct TInst
{
	char  ExchangeID[10];
	char  InstrumentID[31];
	
	bool operator < (const TInst &right) const
	{
		if(strcmp(ExchangeID,right.ExchangeID) < 0)
		{
			return true;
		}
		if(strcmp(ExchangeID,right.ExchangeID) > 0)
		{
			return false;
		}
		if(strcmp(InstrumentID,right.InstrumentID) < 0)
		{
			return true;
		}
		return false;
	}
};

class CExLinkApi:public COutFogServiceApi
{
public:
	CExLinkApi(COutFogServiceCallback *pCallback);
	virtual ~CExLinkApi(){};

	void InitLink(CLinkField *pLink);
	void setLinkField(CUserLoginField *pField);
	bool setPassword(CLinkField * pLinkField);

	bool m_bLogin;//是否登录成功

protected:

	int  m_iReqSeqNo;
	CLinkField m_LinkField;		
	bool m_bConnect;//是否连接上

	char m_sIdInfo[512];
	char pStreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
	TOutFogMetaRef m_OutFogMetaRef;

	int m_DisconnectNum;//连接断开的次数
	bool m_bRevRtn;//是否接受流水，如果日期不符，或者不需要接受流水，接受的流水信息不保存到流文件，不向qtrade发送
	bool m_bCheckLinked;//交易所API不能断线重连时，LinkMange负责重连
	int m_nLinkNo;
};

#endif
