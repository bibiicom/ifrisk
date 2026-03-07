!!function!!
#include <set>
#include <vector>
#include <string>
using namespace std;

typedef set<string> CPackageSet;
CPackageSet g_setUsePackage;

bool InUse(char *name)
{
	if(g_setUsePackage.find("*")!=g_setUsePackage.end())
		return true;
	return (g_setUsePackage.find(name)!=g_setUsePackage.end());
}

vector<string> Txtsplit(const string& str, string separate_character)
{
    vector<string> vestr;
	int separate_characterLen = separate_character.size();
    int lastPosition = 0;
	int index = -1;
    while (-1 != (index = str.find(separate_character,lastPosition)))
    {

        vestr.push_back(str.substr(lastPosition,index - lastPosition));
        lastPosition = index + separate_characterLen;
    }
    string lastString = str.substr(lastPosition);
    if (!lastString.empty())
	{
        vestr.push_back(lastString);
	}
    return vestr;
}


!!endfunction!!

!!enter api!!
!!travel self!!
!!command g_setUsePackage.insert(@name);!!
!!next!!
!!leave!!
!!enter project!!
!!let apiname=@apiname!!
!!let APINAME=@APINAME!!
!!let apiid=@apiid!!
!!let APIID=@APIID!!
!!leave!!
#include "LinkSwap.h"
#include "public.h"

COFSwapLink::COFSwapLink(COutFogServiceCallback *pCallback):CExLinkApi(pCallback)
{
	memset(&m_OutFogMetaRef, 0, sizeof(TOutFogMetaRef));
	m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
	m_OutFogMetaRef.Direction = OUTFOG_DIRECTION_TO_FRONT;
}

COFSwapLink::~COFSwapLink()
{
}

bool COFSwapLink::ReqField(const TOutFogMetaRef* pOutFogReqRef, const void *pStream)
{
	int ret = 0;
	string errorMsg;
	switch (pOutFogReqRef->MessageNo)
	{
!!enter FTD!!
!!travel packages!!
	!!if InUse(@name)!!
		!!if !strcmp(@usage,"request")!!
	!!let functionname=@name!!
	case TID_!!@name!!: ///!!@comment!!
	{
		!!travel self!!
		C!!@name!!Field field;
		C!!@name!!Field::m_Describe.getSingleField(&field, pStream);
		!!if strcmp(@name,"Link")==0!!
		InitLink(&field, pOutFogReqRef->RequestNo);
		!!else!!
				ret = m_pApi->!!@functionname!!((!!@apiid!!!!@name!!Struct*)&field, pOutFogReqRef->RequestNo);	
		!!endif!!
		errorMsg = "!!@name!!";
		!!next!!
		break;
	}
		!!endif!!
	!!endif!!	
!!next!!
!!leave!!		
	default:
	{
		PRINT_TO_STD("%s,Send Req [%0X] Error! no such method", m_sIdInfo, pOutFogReqRef->MessageNo);
	}
	}
	if (ret != 0)
		PRINT_TO_STD("%s,Send Req [%s] Error! ret=%d", m_sIdInfo, errorMsg.c_str(), ret);
	return true;
}

bool COFSwapLink::InitLink(CLinkField *pLinkField, int nRequestNo)
{
	CExLinkApi::InitLink(pLinkField);
	m_pApi = GetDll(pLinkField->LinkDllLoad);
	if (m_pApi == NULL)
	{
		PRINT_TO_STD("Load %s error", m_sIdInfo, pLinkField->LinkDllLoad.getValue());
		return false;
	}
	m_pApi->SetCallback(this);
	m_pApi->SendInitLink((!!@apiid!!LinkStruct*)pLinkField, nRequestNo);

	PRINT_TO_STD("%s,InitLink,LinkAddress:%s", m_sIdInfo, pLinkField->LinkAddress.getValue());
	m_pApi->SetAddress(pLinkField->LinkAddress.getValue());
	m_pApi->Connect();
	return true;
}

void COFSwapLink::OnConnected()
{
	PRINT_TO_STD("%s,OnConnected:Success!", m_sIdInfo);
	m_bConnect = true;
	Login();
}

void COFSwapLink::OnDisconnected(int nReason)
{
	PRINT_TO_STD("%s,OnDisConnected,Reason=[%d]", m_sIdInfo, nReason);
	m_bConnect = false;
	m_bLogin = false;
	m_DisconnectNum++;
	//	SendLoginStatus(SS_Logout);
	//	SendDisConnetMessageNotify();
}

void COFSwapLink::Login()
{
	if (m_LinkField.LinkUserID.isNull() && m_LinkField.LinkPassword.isNull())
	{
		PRINT_TO_STD("%s,NoNeedToLogin!", m_sIdInfo);
		m_OutFogMetaRef.Chain = OUTFOG_CHAIN_SINGLE;
		m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
		m_OutFogMetaRef.MessageNo = TID_RecvUserLogin;
		m_OutFogMetaRef.RequestNo = 0;
		m_OutFogMetaRef.SubjectNo = 0;
		strcpy(m_OutFogMetaRef.Remark, "Success");
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);
		return;
	}
	!!@apiid!!UserLoginStruct reqUserLogin;
	memset(&reqUserLogin, 0, sizeof(reqUserLogin));
	strncpy(reqUserLogin.UserID, m_LinkField.LinkUserID.getValue(), sizeof(reqUserLogin.UserID));
	strncpy(reqUserLogin.Password, m_LinkField.LinkPassword.getValue(), sizeof(reqUserLogin.Password));
	strcpy(reqUserLogin.UserProductID, ProductInfo);
	vector<string> Configs = Txtsplit(m_LinkField.Config.getValue(), ",");
	for (int i = 0; i < Configs.size(); i++)
	{
		vector<string> eachConfigs = Txtsplit(Configs[i], ":");
		if (eachConfigs.size()==2 && eachConfigs[0] == "APPID")
			strncpy(reqUserLogin.APPID, eachConfigs[1].c_str(), sizeof(reqUserLogin.APPID));
	}
	
	int ret = m_pApi->SendUserLogin(&reqUserLogin, 0);
	if (ret != 0)
		PRINT_TO_STD("%s,Login:Fail!,ret=%d", m_sIdInfo, ret);
	else
		PRINT_TO_STD("%s,Login:Success!", m_sIdInfo);
}
!!enter FTD!!
!!travel packages!!
!!let package_name=@name!!
!!if InUse(@name)!!
	!!if !strcmp(@usage,"response") !!

void COFSwapLink::!!@name!!(!!travel self!!!!if @pumpid!=0!!, !!endif!!const !!@apiid!!!!@name!!Struct *p!!@name!!!!next!!, int nErrorNo,const char* pErrorMsg, int nRequestNo, char cChain)
{		
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.Type = OUTFOG_TYPE_RESPONSE;
	m_OutFogMetaRef.MessageNo = TID_!!@package_name!!;
	m_OutFogMetaRef.RequestNo = nRequestNo;
	strncpy(m_OutFogMetaRef.Remark, pErrorMsg, 61);	//61是omq报文里面的容量决定
	
		!!travel self!!	
	if (p!!@name!! != NULL)
	{
		if(nErrorNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nErrorNo;
			m_OutFogMetaRef.FieldNo = -C!!@name!!Field::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nErrorNo;
			m_OutFogMetaRef.FieldNo = C!!@name!!Field::m_Describe.m_FieldID;	
		}
		C!!@name!!Field::m_Describe.setSingleField(pStreamBuf, p!!@name!!);
		m_OutFogMetaRef.FieldNo = C!!@name!!Field::m_Describe.m_FieldID;
		m_OutFogMetaRef.Length = C!!@name!!Field::m_Describe.m_nStreamSize;
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
		!!next!!
	if (!!travel self!!!!if @pumpid!=0!! && !!endif!!p!!@name!! == NULL!!next!!)
		m_pServiceCallback->RspField(&m_OutFogMetaRef, NULL);	
}
	!!elseif !strcmp(@usage,"Push")!!

void COFSwapLink::!!@name!!(!!travel self!!!!if @pumpid!=0!!, !!endif!!const !!@apiid!!!!@name!!Struct *p!!@name!!!!next!!, int nTopicNo, const char* FilterValue, double nSequenceNo, double nBusinessNo, char cChain)
{
	m_OutFogMetaRef.Type = OUTFOG_TYPE_PUBLISH;
	m_OutFogMetaRef.MessageNo = TID_!!@name!!;
	m_OutFogMetaRef.Chain = cChain;
	m_OutFogMetaRef.BusinessNo = nBusinessNo;
		!!travel self!!
	if (p!!@name!! != NULL)
	{
		C!!@name!!Field::m_Describe.setSingleField(pStreamBuf, p!!@name!!);
		if(nTopicNo < 0)
		{
			m_OutFogMetaRef.SubjectNo = -nTopicNo;
			m_OutFogMetaRef.FieldNo = -C!!@name!!Field::m_Describe.m_FieldID;
		}
		else
		{
			m_OutFogMetaRef.SubjectNo = nTopicNo;
			m_OutFogMetaRef.FieldNo = C!!@name!!Field::m_Describe.m_FieldID;
		}
		m_OutFogMetaRef.Length = C!!@name!!Field::m_Describe.m_nStreamSize;
		m_OutFogMetaRef.Remark[0] = '\0';
		memcpy(m_OutFogMetaRef.SubIndexName,FilterValue,sizeof(m_OutFogMetaRef.SubIndexName));
		m_pServiceCallback->RspField(&m_OutFogMetaRef, pStreamBuf);
	}
		!!next!!
}
	!!endif!!
!!endif!!
!!next!!
!!leave!!

C!!@apiname!!!!@apiid!!Api* COFSwapLink::GetDll(const char* pName)
{
	C!!@apiname!!!!@apiid!!Api *pDll = NULL;
	char ServiceDllName[128];
#ifdef WINDOWS
	sprintf(ServiceDllName, "%s.dll", pName);
	HINSTANCE hDll = LoadLibrary(ServiceDllName);
	if (hDll != NULL)
	{
		_Create!!@apiname!!!!@apiid!!Api* MyCreateServiceApi = (_Create!!@apiname!!!!@apiid!!Api*)GetProcAddress(hDll, "Create!!@apiname!!!!@apiid!!Api");
		if (MyCreateServiceApi != NULL)
		{
			pDll = MyCreateServiceApi();
			if (pDll == NULL)
			{
				PRINT_TO_STD("error: %s->Create!!@apiname!!!!@apiid!!Api return NULL",ServiceDllName);
				return NULL;
			}
			else
			{
				PRINT_TO_STD("Load Library:%s", ServiceDllName);
			}
		}
		else
		{
			int errorid = GetLastError();
			PRINT_TO_STD("error: Can not GetProcAddress:%s->Create!!@apiname!!!!@apiid!!Api errorid:%d", ServiceDllName, errorid);
			return NULL;
		}
	}
	else
	{
		int errorid = GetLastError();
		PRINT_TO_STD("error: Can not LoadLibrary:%s errorid:%d", ServiceDllName, errorid);
		return NULL;
	}
#else
	sprintf(ServiceDllName, "lib%s.so", pName);
	void *handle = dlopen(ServiceDllName, RTLD_NOW);
	if (handle)
	{
		_Create!!@apiname!!!!@apiid!!Api* MyCreateServiceApi = (_Create!!@apiname!!!!@apiid!!Api*)dlsym(handle, "Create!!@apiname!!!!@apiid!!Api");
		if (MyCreateServiceApi != NULL)
		{
			pDll = MyCreateServiceApi();
			if (pDll == NULL)
			{
				PRINT_TO_STD("error: %s->Create!!@apiname!!!!@apiid!!Api return NULL",ServiceDllName);
				return NULL;
			}
			else
			{
				PRINT_TO_STD("Load Library:%s", ServiceDllName);
			}
		}
		else
		{
			PRINT_TO_STD("error: Can not dlsym:%s->Create!!@apiname!!!!@apiid!!Api msg:%s",ServiceDllName, dlerror());
			return NULL;
		}
	}
	else
	{
		PRINT_TO_STD("error: Can not dlopen:%s msg:%s", ServiceDllName, dlerror());
		return NULL;
	}
#endif
	return pDll;
}

extern "C" COutFogServiceApi* CreateOutFogServiceApi(COutFogServiceCallback *pCallback, int nServiceID)
{
	return new COFSwapLink(pCallback);
}
