/////////////////////////////////////////////////////////////////////////
///@system OutFogServiceApiWrapper
///OutFogServiceApiWrapper.h
///定义了C++实现C的api的一个简单的脚手架
///@history 
///20190909	徐忠华	创建该文件
/////////////////////////////////////////////////////////////////////////

#include "OutFogServiceApiWrapper.h"

void _OutFogRspField(void* pInstance,TOutFogMetaRef* pOutFogRspRef,void *pStream)
{
	((COutFogServiceApi*)pInstance)->getServiceCallback()->RspField(pOutFogRspRef,pStream);
}

