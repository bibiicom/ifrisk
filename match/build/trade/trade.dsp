# Microsoft Developer Studio Project File - Name="trade" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=trade - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "trade.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "trade.mak" CFG="trade - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "trade - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "trade - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "trade - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP   -DDEBUG_NOT_LOG -nologo -EHsc -MT -O2 -GF -FD   -W3 -DWINDOWS -DMSVC /c
# ADD CPP   -DDEBUG_NOT_LOG -nologo -EHsc -MT -O2 -GF -FD   -W3 -DWINDOWS -DMSVC   -I..\..\src_base\function\. -I..\..\src_base\logger\. -I..\..\src_base\crypt\. -I..\..\src_base\datastruct\. -I..\..\src_base\netstruct\. -I..\..\src_base\field\. -I..\..\src_base\mdb\mdbbase\. -I..\..\src_base\mdb\mdbstruct\. -I..\..\src_base\mdb\mdbchange\. -I..\..\libs\ofserviceapi\1.0\include\. -I..\..\libs\ofserviceapi\ofapibase\. -I..\..\src\service\. -I..\..\src\service\serviceQuery\. -I..\..\src\service\serviceLogin\. -I..\..\src\service\serviceSettlement\. -I..\..\src\service\serviceTrade\. -I..\..\src\service\serviceTrade\wedge\. -I..\..\src\service\serviceTrade\wedge\wedgeMatch\. -I..\..\src\service\serviceTrade\wedge\wedgeRisk\. -I..\..\src\service\serviceTrade\wedge\wedgeControl\. -I..\..\src\trade\. /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib     -nologo /FIXED:NO     /WARN:2
# ADD LINK32   -nologo /FIXED:NO     /WARN:2 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /out:".\./\trade.exe"

!ELSEIF  "$(CFG)" == "trade - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP   -DDEBUG_NOT_LOG -nologo -EHsc -MTd -Od -Gm -Zi -FD -Zm200   -W3 -DDEBUG -DDEBUG_LOG -DWINDOWS -DMSVC  /c
# ADD CPP   -DDEBUG_NOT_LOG -nologo -EHsc -MTd -Od -Gm -Zi -FD -Zm200   -W3 -DDEBUG -DDEBUG_LOG -DWINDOWS -DMSVC   -I..\..\src_base\function\. -I..\..\src_base\logger\. -I..\..\src_base\crypt\. -I..\..\src_base\datastruct\. -I..\..\src_base\netstruct\. -I..\..\src_base\field\. -I..\..\src_base\mdb\mdbbase\. -I..\..\src_base\mdb\mdbstruct\. -I..\..\src_base\mdb\mdbchange\. -I..\..\libs\ofserviceapi\1.0\include\. -I..\..\libs\ofserviceapi\ofapibase\. -I..\..\src\service\. -I..\..\src\service\serviceQuery\. -I..\..\src\service\serviceLogin\. -I..\..\src\service\serviceSettlement\. -I..\..\src\service\serviceTrade\. -I..\..\src\service\serviceTrade\wedge\. -I..\..\src\service\serviceTrade\wedge\wedgeMatch\. -I..\..\src\service\serviceTrade\wedge\wedgeRisk\. -I..\..\src\service\serviceTrade\wedge\wedgeControl\. -I..\..\src\trade\. /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib     -nologo /FIXED:NO     /DEBUG /WARN:2
# ADD LINK32   -nologo /FIXED:NO     /DEBUG /WARN:2 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /out:".\./\trade.exe"

!ENDIF 

# Begin Target

# Name "trade - Win32 Release"
# Name "trade - Win32 Debug"


# Begin Group "function"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_base\function\BaseFunction.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\Config.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\platform.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\printinfo.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\profile.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\utility.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\utils.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\version.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\BaseFunction.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\Config.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\ExpEvaluate.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\platform.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\printinfo.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\profile.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\public.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\utility.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\utils.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\version.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\function\versionValue.h
# End Source File
# End Group

# Begin Group "logger"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_base\logger\CFilelogService.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\logger\CLogger.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\logger\CProbeLogger.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\logger\CSyslogService.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\logger\monitorIndex.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\logger\CFilelogService.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\logger\CLogger.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\logger\CProbeLogger.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\logger\CSyslogService.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\logger\monitorIndex.h
# End Source File
# End Group

# Begin Group "crypt"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_base\crypt\cryptUtility.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\crypt\Des.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\crypt\hexEncode.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\crypt\md5Handler.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\crypt\cryptAll.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\crypt\cryptUtility.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\crypt\Des.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\crypt\DesEncryptAlgorithm.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\crypt\hexEncode.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\crypt\md5Handler.h
# End Source File
# End Group

# Begin Group "datastruct"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_base\datastruct\BigDigit.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\CDate.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\CErrorEngine.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\CSequence.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\CSVParser.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\CSVToField.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\FieldDescribe.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\BigDigit.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\CDate.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\CErrorEngine.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\CSequence.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\CSVParser.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\CSVToField.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\FieldDescribe.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\HashMap.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\datastruct\UFDataType.h
# End Source File
# End Group

# Begin Group "netstruct"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_base\netstruct\errorDefine.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\netstruct\TableDefine.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\netstruct\CopyField.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\netstruct\DataType.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\netstruct\errorDefine.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\netstruct\TableDefine.h
# End Source File
# End Group

# Begin Group "field"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_base\field\FieldData.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\field\FieldMap.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\field\FieldData.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\field\FieldMap.h
# End Source File
# End Group

# Begin Group "mdbbase"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdb.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdbAction.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdbIter.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdbLog.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdbSearch.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdbStruct.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdbTable.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdb.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdbAction.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdbIter.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdbLog.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdbStruct.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbbase\mdbTable.h
# End Source File
# End Group

# Begin Group "mdbstruct"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\Allocator.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CAVLTree.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CBaseObject.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CDatabaseDataFactory.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CDatabaseResource.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CDoubleHash.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CFixMem.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CHashIndex.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CTransaction.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\Allocator.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CAVLTree.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CBaseObject.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CDatabaseDataFactory.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CDatabaseResource.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CDoubleHash.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CFixMem.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CHashIndex.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CResource.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\CTransaction.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\databaseData.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbstruct\MemoryStack.h
# End Source File
# End Group

# Begin Group "mdbchange"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbchange\AlterService.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbchange\AlterServiceInterface.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbchange\TableTrigger.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbchange\AlterService.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbchange\AlterServiceInterface.h
# End Source File
# Begin Source File
SOURCE=..\..\src_base\mdb\mdbchange\TableTrigger.h
# End Source File
# End Group

# Begin Group "ofserviceapi_include"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\libs\ofserviceapi\1.0\include\OutFogServiceApi.h
# End Source File
# Begin Source File
SOURCE=..\..\libs\ofserviceapi\1.0\include\OutFogServiceApiStruct.h
# End Source File
# Begin Source File
SOURCE=..\..\libs\ofserviceapi\1.0\include\OutFogServiceCApi.h
# End Source File
# Begin Source File
SOURCE=..\..\libs\ofserviceapi\1.0\include\ServiceResponser.h
# End Source File
# End Group

# Begin Group "ofapibase"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\libs\ofserviceapi\ofapibase\OutFogServiceApiWrapper.cpp
# End Source File
# Begin Source File
SOURCE=..\..\libs\ofserviceapi\ofapibase\OutFogService.h
# End Source File
# Begin Source File
SOURCE=..\..\libs\ofserviceapi\ofapibase\OutFogServiceApiWrapper.h
# End Source File
# End Group

# Begin Group "service"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src\service\ServiceShare.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\tradingTools.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\ServiceInterface.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\ServiceShare.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\tradingTools.h
# End Source File
# End Group

# Begin Group "serviceQuery"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src\service\serviceQuery\QueryServiceInterface.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceQuery\QueryServiceInterface.h
# End Source File
# End Group

# Begin Group "serviceLogin"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src\service\serviceLogin\LoginService.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceLogin\LoginServiceInterface.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceLogin\LoginService.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceLogin\LoginServiceInterface.h
# End Source File
# End Group

# Begin Group "serviceSettlement"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src\service\serviceSettlement\CSettlementService.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceSettlement\SettlementServiceInterface.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceSettlement\CSettlementService.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceSettlement\SettlementServiceInterface.h
# End Source File
# End Group

# Begin Group "serviceTrade"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\CPositionChangeList.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\CTradeService.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\TradeServiceInterface.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\CPositionChangeList.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\CTradeService.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\TradeServiceInterface.h
# End Source File
# End Group

# Begin Group "wedge"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\CWedge.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\CWedgeConfig.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\CWedgeGroup.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\CWedge.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\CWedgeConfig.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\CWedgeGroup.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\Wedge.h
# End Source File
# End Group

# Begin Group "wedgeMatch"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\genTrade.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\KlineTriggers.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\marketDataWedges.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\matchWedges.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\OrderBook.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\TriggerOrderWedges.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\genTrade.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\KlineTriggers.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\marketDataWedges.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\matchWedges.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\OrderBook.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeMatch\TriggerOrderWedges.h
# End Source File
# End Group

# Begin Group "wedgeRisk"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeRisk\accountAlgorithms.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeRisk\accountWedges.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeRisk\positionControlers.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeRisk\positionLimitWedges.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeRisk\accountAlgorithms.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeRisk\accountWedges.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeRisk\positionControlers.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeRisk\positionLimitWedges.h
# End Source File
# End Group

# Begin Group "wedgeControl"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeControl\baseWedges.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeControl\orderTypeWedges.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeControl\priceControlWedges.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeControl\baseWedges.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeControl\orderTypeWedges.h
# End Source File
# Begin Source File
SOURCE=..\..\src\service\serviceTrade\wedge\wedgeControl\priceControlWedges.h
# End Source File
# End Group

# Begin Group "trade"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src\trade\BaseEngine.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\trade\Engine.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\trade\Exchange.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\trade\MyAlterService.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\trade\MyAlterServiceInterface.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\trade\TradeEngine.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\trade\BaseEngine.h
# End Source File
# Begin Source File
SOURCE=..\..\src\trade\Engine.h
# End Source File
# Begin Source File
SOURCE=..\..\src\trade\MyAlterService.h
# End Source File
# Begin Source File
SOURCE=..\..\src\trade\MyAlterServiceInterface.h
# End Source File
# Begin Source File
SOURCE=..\..\src\trade\MyResponser.h
# End Source File
# Begin Source File
SOURCE=..\..\src\trade\TradeEngine.h
# End Source File
# End Group

# End Target
# End Project


