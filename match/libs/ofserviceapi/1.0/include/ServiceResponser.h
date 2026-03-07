/////////////////////////////////////////////////////////////////////////
///CExchangeResponser.h
/// 定义了回调接口的几种使用方式，这个是给交易所系统使用的
///@history 
///徐忠华 创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef _Responser_exchange_H_
#define _Responser_exchange_H_

#include "OutFogServiceApi.h"
#include "FieldDescribe.h"
#include "BaseFunction.h"

//#ifndef CopyHeadMeta
//#define CopyHeadMeta(target,source)	\
//	(target)->MessageNo=(source)->MessageNo;	\
//	(target)->Type=(source)->Type;	\
//	(target)->Chain=(source)->Chain;	\
//	(target)->FrontNo=(source)->FrontNo;	\
//	(target)->SessionNo=(source)->SessionNo;	\
//	(target)->RequestNo=(source)->RequestNo;	\
//	(target)->SubjectNo=(source)->SubjectNo; \
//	(target)->SequenceNo=(source)->SequenceNo;	\
//	(target)->GlobalNo=(source)->GlobalNo;		\
//	(target)->BusinessNo=(source)->BusinessNo;		\
//	(target)->Direction=(source)->Direction;		\
//	memcpy((target)->Remark,(source)->Remark,128);
//#endif

//不使用逐字段拷贝而使用memcpy的原因是为了加快运行速度
//FieldNo和IsReplay不能拷贝，这个是api独有的，传输中不需要
//由于字节对齐原因，这里不能使用sizeof(TOutFogMetaRef)，而使用人工计算的大小
#ifndef CopyHeadMeta
#define CopyHeadMeta(target,source)	\
	memcpy((target),(source),176);
#endif

//发送一个报文
//--发送回报，可以是response/public/private/subject
//---CXTPResponser.SendResponse(**,OUTFOG_CHAIN_SINGLE);

//发送一串报文
//--第一个一定要有报文，可以是response/public/private/subject
//---CXTPResponser.SendToPublic(**,OUTFOG_CHAIN_FIRST);
//--如果发送的中间报文出现打包不够的情况，会自动重新打包
//---CXTPResponser.SendNext(**,OUTFOG_CHAIN_CONTINUE);
//---CXTPResponser.SendNext(**,OUTFOG_CHAIN_LAST);

class CServiceResponser
{
public:
	CServiceResponser(COutFogServiceCallback *pServiceCallback)
	{
		m_pServiceCallback = pServiceCallback;
		memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	}

	//回复请求，用户对话和查询
	virtual bool SendResponse(int nMessageNo, void* pField, CFieldDescribe* pFieldDescribe, TSessionRef* pSession, char cChain = OUTFOG_CHAIN_SINGLE, int nErrorNo = 0, const char* sErrorMsg = "Success")
	{
		//没有指定前置，无法回复消息
		if (pSession->frontID == INT_32_MAX)
		{
			m_OutFogMetaRef.RequestNo = -1; //后续的报文同样无法发出
			return false;
		}
			
		//没有准备报文，无法打包
		if (cChain == OUTFOG_CHAIN_LAST || cChain == OUTFOG_CHAIN_CONTINUE)
		{
			m_OutFogMetaRef.RequestNo = -1; //后续的报文同样无法发出
			return false;
		}			
		//如果nRequestNo小于零，就不要回复了，做市账户减少数量使用		
		if (pSession->requestID < 0)
		{
			m_OutFogMetaRef.RequestNo = pSession->requestID; //后续的报文同样无法发出
			if (nErrorNo == 0)
			{
				return false;
			}			
			else
			{
				if (pSession->requestID == -1)
					return false;
			}
		}			

		m_OutFogMetaRef.FrontNo = pSession->frontID;
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
		m_OutFogMetaRef.MessageNo = nMessageNo;
		m_OutFogMetaRef.SessionNo = pSession->sessionID;
		m_OutFogMetaRef.RequestNo = pSession->requestID;
		m_OutFogMetaRef.Chain = cChain;

		m_OutFogMetaRef.SubjectNo = nErrorNo;
		//sprintf(m_OutFogMetaRef.SubIndexName, sErrorMsg);
		//性能考虑
		//这里只能拷贝124个字节，要不然会影响其他字段
		strncpy(m_OutFogMetaRef.Remark, sErrorMsg, 120);
		m_OutFogMetaRef.Reserve[0] = pSession->zip + '0';
		if (pField != NULL)
		{
			m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
			m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
			pFieldDescribe->setSingleField(m_pStreamBuf, pField);
			m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
		}
		else
		{
			m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
		}
		return true;
	};

	virtual bool SendRspPub(int nMessageNo, void* pField, CFieldDescribe* pFieldDescribe, int dwSubjectID, int nZip, TSessionRef* pSession, const char* sIndex, char cChain = OUTFOG_CHAIN_SINGLE)
	{
		//没有指定前置，无法回复消息
		if (pSession->frontID == INT_32_MAX)
		{
			m_OutFogMetaRef.RequestNo = -1; //后续的报文同样无法发出
			return false;
		}

		//没有准备报文，无法打包
		if (cChain == OUTFOG_CHAIN_LAST || cChain == OUTFOG_CHAIN_CONTINUE)
		{
			m_OutFogMetaRef.RequestNo = -1; //后续的报文同样无法发出
			return false;
		}

		m_OutFogMetaRef.FrontNo = pSession->frontID;
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_RSPPUB;
		m_OutFogMetaRef.MessageNo = nMessageNo;
		m_OutFogMetaRef.SessionNo = pSession->sessionID;
		m_OutFogMetaRef.Chain = cChain;
		m_OutFogMetaRef.SubjectNo = dwSubjectID;
		m_OutFogMetaRef.Reserve[0] = pSession->zip + '0';
		m_OutFogMetaRef.Reserve[1] = nZip + '0';
		strncpy(m_OutFogMetaRef.Remark, sIndex, 120);

		if (pField != NULL)
		{
			m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
			m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
			pFieldDescribe->setSingleField(m_pStreamBuf, pField);
			m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
		}
		else
		{
			m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
		}
		return true;
	};

	//向其他主题发送数据
	virtual void SendToSubjectID(int nMessageNo, void* pField, CFieldDescribe* pFieldDescribe, int dwSubjectID, int nZip, UF_INT8 nBusinessNo = UF_INT8_MAX, char cChain = OUTFOG_CHAIN_SINGLE)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
		m_OutFogMetaRef.MessageNo = nMessageNo;
		m_OutFogMetaRef.SubjectNo = dwSubjectID;
		m_OutFogMetaRef.BusinessNo = nBusinessNo;
		m_OutFogMetaRef.Chain = cChain;
		m_OutFogMetaRef.Reserve[0] = nZip + '0';

		if (pField == NULL)
		{
			m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
		}
		else
		{
			m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
			m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
			pFieldDescribe->setSingleField(m_pStreamBuf, pField);
			m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
		}
	};

	virtual void PrepareResponse(int nMessageNo, int nErrorID, char* sErrorMsg, TSessionRef* pSession)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
		m_OutFogMetaRef.MessageNo = nMessageNo;
		m_OutFogMetaRef.FrontNo = pSession->frontID;
		m_OutFogMetaRef.SessionNo = pSession->sessionID;
		m_OutFogMetaRef.RequestNo = pSession->requestID;
		m_OutFogMetaRef.Reserve[0] = pSession->zip + '0';
		m_OutFogMetaRef.SubjectNo = nErrorID;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_FIRST;
		//sprintf(m_OutFogMetaRef.SubIndexName, sErrorMsg);
		strcpy(m_OutFogMetaRef.Remark, sErrorMsg);
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
	};

	//继续发送上一个TID的Field,如果cChain==OUTFOG_CHAIN_LAST,全部发送
	virtual void SendNext(void *pField, CFieldDescribe *pFieldDescribe, char cChain = OUTFOG_CHAIN_CONTINUE)
	{
		if (m_OutFogMetaRef.RequestNo < 0)
			return;
		m_OutFogMetaRef.Chain = cChain;
		if (pField == NULL)
		{
			m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
		}
		else
		{
			m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
			m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
			pFieldDescribe->setSingleField(m_pStreamBuf, pField);
			m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
		}
	};

	virtual void SendNoNext(void *pField, CFieldDescribe *pFieldDescribe, char cChain = OUTFOG_CHAIN_CONTINUE)
	{
		if (m_OutFogMetaRef.RequestNo < 0)
			return;
		m_OutFogMetaRef.Chain = cChain;
		if (pField == NULL)
		{
			m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
		}
		else
		{
			m_OutFogMetaRef.FieldNo = -pFieldDescribe->m_FieldID;
			m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
			pFieldDescribe->setSingleField(m_pStreamBuf, pField);
			m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
		}
	};

	//订阅指定的流水，前提是先注册过(public、private、user不用注册)
	//virtual void SubSubjectID(int nLocalID, int dwSubjectID, UF_INT8 nSequenceNo, const char* SubIndexName, const char* UserID, int nFrontID, int nSessionID, UF_INT8 nBusinessNo = UF_INT8_MAX, int nRequestNo = 1)
	virtual void SubSubjectID(int nMessageNo, int dwSubjectID, UF_INT8 nSequenceNo, const char* SubIndexName, TSessionRef* pSession, int nZip, UF_INT8 nBusinessNo, int nRequestNo)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_SUBTOPIC;
		m_OutFogMetaRef.SubjectNo = dwSubjectID;
		m_OutFogMetaRef.SequenceNo = nSequenceNo;
		m_OutFogMetaRef.BusinessNo = nBusinessNo;
		m_OutFogMetaRef.MessageNo = nMessageNo;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		m_OutFogMetaRef.FrontNo = pSession->frontID;
		m_OutFogMetaRef.SessionNo = pSession->sessionID;
		m_OutFogMetaRef.RequestNo = nRequestNo;
		strcpy(m_OutFogMetaRef.SubIndexName, SubIndexName);
		m_OutFogMetaRef.Reserve[0] = nZip + '0';
		//strcpy(m_OutFogMetaRef.UserID, UserID);
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
	};

	virtual void AddApiKey(const char* sApiKey, const char* sSecretKey, const char* sUserID, const UF_INT8 iExpireTime, const int iAccessLimit)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_ADDAPIKEY;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		strcpy(m_OutFogMetaRef.Remark, sApiKey);
		sprintf(m_pStreamBuf, "%s,%s", sUserID, sSecretKey);
		m_OutFogMetaRef.BusinessNo = iExpireTime;
		m_OutFogMetaRef.RequestNo = iAccessLimit;
		m_OutFogMetaRef.FieldNo = 0;
		m_OutFogMetaRef.Length = 512;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
	};

	virtual void DelSession(int nFrontID, int nSessionID)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_DELSESSION;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		m_OutFogMetaRef.FrontNo = nFrontID;
		m_OutFogMetaRef.SessionNo = nSessionID;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
	};

	virtual void DelApiKey(const char* sApiKey)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_DELAPIKEY;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		strcpy(m_OutFogMetaRef.Remark, sApiKey);
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
	};

	virtual void AddApiKeyAllowIP(const char* sApiKey, const char* sAllowIP)
	{
		vector<string> IPs = Txtsplit(sAllowIP, ";");
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_ADDAPIKEYALLOWIP;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		m_OutFogMetaRef.FieldNo = 0;
		m_OutFogMetaRef.Length = 512;
		for (vector<string>::iterator it = IPs.begin(); it != IPs.end(); it++)
		{
			strcpy(m_OutFogMetaRef.Remark, sApiKey);
			strcpy(m_pStreamBuf, (*it).c_str());
			m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
		}
	};

	virtual void DelApiKeyAllowIP(const char* sApiKey, const char* sAllowIP)
	{
		vector<string> IPs = Txtsplit(sAllowIP, ";");
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_DELAPIKEYALLOWIP;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		m_OutFogMetaRef.FieldNo = 0;
		m_OutFogMetaRef.Length = 512;
		for (vector<string>::iterator it = IPs.begin(); it != IPs.end(); it++)
		{
			strcpy(m_OutFogMetaRef.Remark, sApiKey);
			strcpy(m_pStreamBuf, (*it).c_str());
			m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
		}
	};

	virtual void UnSubSubjectID(int dwSubjectID, const char* SubIndexName, TSessionRef* pSession)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_UNSUBTOPIC;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		m_OutFogMetaRef.FrontNo = pSession->frontID;
		m_OutFogMetaRef.SessionNo = pSession->sessionID;
		m_OutFogMetaRef.SubjectNo = dwSubjectID;
		strcpy(m_OutFogMetaRef.SubIndexName, SubIndexName);
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
	};

	virtual void UnSubSubjectIDAll(TSessionRef* pSession)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_UNSUBTOPIC;
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		m_OutFogMetaRef.FrontNo = pSession->frontID;
		m_OutFogMetaRef.SessionNo = pSession->sessionID;
		m_OutFogMetaRef.SubjectNo = INT_MAX;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
	};

	//向报盘机发送
	virtual void InitLink(const char* sLinkDll, int nLinkID, int nTID, void *pField, CFieldDescribe *pFieldDescribe, int dwSubjectID, char cChain = OUTFOG_CHAIN_SINGLE)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_OFFER;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_INITLINK;
		strncpy(m_OutFogMetaRef.SubIndexName, sLinkDll, sizeof(m_OutFogMetaRef.SubIndexName));
		m_OutFogMetaRef.GlobalNo = nLinkID;
		m_OutFogMetaRef.MessageNo = nTID;
		m_OutFogMetaRef.SubjectNo = dwSubjectID;
		m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
		m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
		m_OutFogMetaRef.Chain = cChain;
		pFieldDescribe->setSingleField(m_pStreamBuf, pField);
		m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
	};

	virtual void StartLink(int nLinkType, int nLinkID, int nTID, void *pField, CFieldDescribe *pFieldDescribe, int dwSubjectID, char cChain = OUTFOG_CHAIN_SINGLE)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_OFFER;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_START;
		m_OutFogMetaRef.GlobalNo = nLinkID;
		m_OutFogMetaRef.MessageNo = nTID;
		m_OutFogMetaRef.SubjectNo = dwSubjectID;
		m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
		m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
		m_OutFogMetaRef.Chain = cChain;
		pFieldDescribe->setSingleField(m_pStreamBuf, pField);
		m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
	};

	//向报盘机发送
	virtual void SendToLink(int nTID, void *pField, CFieldDescribe *pFieldDescribe, int dwSubjectID, char cChain = OUTFOG_CHAIN_SINGLE)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_OFFER;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
		m_OutFogMetaRef.MessageNo = nTID;
		m_OutFogMetaRef.SubjectNo = dwSubjectID;
		m_OutFogMetaRef.FieldNo = pFieldDescribe->m_FieldID;
		m_OutFogMetaRef.Length = pFieldDescribe->m_nStreamSize;
		m_OutFogMetaRef.Chain = cChain;
		pFieldDescribe->setSingleField(m_pStreamBuf, pField);
		m_pServiceCallback->RspField(&m_OutFogMetaRef, m_pStreamBuf);
	};

	//告知核心从哪里开始反演：只有调用这个接口之后，核心才开始工作
	virtual void InitKernel(UF_INT8 dwSequenceNo)
	{
		m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_NULL;
		//m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_KERNEL;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_INIT;
		m_OutFogMetaRef.SequenceNo = dwSequenceNo;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
	};

	//获取callback
	virtual COutFogServiceCallback* GetCallback()
	{
		return m_pServiceCallback;
	};
	//获取MetaRef
	virtual TOutFogMetaRef* GetMetaRef()
	{
		return &m_OutFogMetaRef;
	};
	//获取StreamBuf
	virtual char* GetStreamBuf()
	{
		return m_pStreamBuf;
	};
	//准备MetaRef
	virtual bool PrepareMetaRef(CFieldDescribe *pFieldDescribe, void* pData, void* pOldData, char Type)
	{
		memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
		return true;
	};
	virtual bool setdumpNo(UF_INT8 nSequenceNo)
	{
		return true;
	}

protected:
	TOutFogMetaRef m_OutFogMetaRef;
	COutFogServiceCallback *m_pServiceCallback;
	char m_pStreamBuf[OUTFOG_MAX_PACKAGE_SIZE];
};

#endif

