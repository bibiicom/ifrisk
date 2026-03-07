# Microsoft Developer Studio Project File - Name="marketmaker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=marketmaker - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "marketmaker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "marketmaker.mak" CFG="marketmaker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "marketmaker - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "marketmaker - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "marketmaker - Win32 Release"

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
# ADD CPP   -DDEBUG_NOT_LOG -nologo -EHsc -MT -O2 -GF -FD   -W3 -DWINDOWS -DMSVC   -I..\..\src_base\crypt\. -I..\..\src_base\datastruct\. -I..\..\src_base\function\. -I..\..\src_base\logger\. -I..\..\src_base\netstruct\. -I..\..\src_base\field\. -I..\..\libs\ofserviceapi\1.0\include\. -I..\..\libs\ofserviceapi\ofapibase\. -I..\..\src\marketmaker\. /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib     -nologo /FIXED:NO     /WARN:2
# ADD LINK32   -nologo /FIXED:NO     /WARN:2 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /out:".\\marketmaker.exe"

!ELSEIF  "$(CFG)" == "marketmaker - Win32 Debug"

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
# ADD CPP   -DDEBUG_NOT_LOG -nologo -EHsc -MTd -Od -Gm -Zi -FD -Zm200   -W3 -DDEBUG -DDEBUG_LOG -DWINDOWS -DMSVC   -I..\..\src_base\crypt\. -I..\..\src_base\datastruct\. -I..\..\src_base\function\. -I..\..\src_base\logger\. -I..\..\src_base\netstruct\. -I..\..\src_base\field\. -I..\..\libs\ofserviceapi\1.0\include\. -I..\..\libs\ofserviceapi\ofapibase\. -I..\..\src\marketmaker\. /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib     -nologo /FIXED:NO     /DEBUG /WARN:2
# ADD LINK32   -nologo /FIXED:NO     /DEBUG /WARN:2 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /out:".\\marketmaker.exe"

!ENDIF 

# Begin Target

# Name "marketmaker - Win32 Release"
# Name "marketmaker - Win32 Debug"


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

# Begin Group "marketmaker"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src\marketmaker\FastWorker.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\marketmaker\IndexWorker.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\marketmaker\Main.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\marketmaker\MarketWorker.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src\marketmaker\FastWorker.h
# End Source File
# Begin Source File
SOURCE=..\..\src\marketmaker\IndexWorker.h
# End Source File
# Begin Source File
SOURCE=..\..\src\marketmaker\Main.h
# End Source File
# Begin Source File
SOURCE=..\..\src\marketmaker\MarketWorker.h
# End Source File
# Begin Source File
SOURCE=..\..\src\marketmaker\WorkerInterface.h
# End Source File
# End Group

# End Target
# End Project


