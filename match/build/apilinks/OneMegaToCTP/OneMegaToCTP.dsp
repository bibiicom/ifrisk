# Microsoft Developer Studio Project File - Name="OneMegaToCTP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=OneMegaToCTP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OneMegaToCTP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OneMegaToCTP.mak" CFG="OneMegaToCTP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OneMegaToCTP - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OneMegaToCTP - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OneMegaToCTP - Win32 Release"

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
# ADD BASE CPP   -DDDEBUG_NOT_LOG -DONE_MEGAAPI_IS_LIB -DLIB_ONE_MEGA_API_EXPORT -DMG_ENABLE_OPENSSL=1 -nologo -EHsc -MT -O2 -GF -FD   -W3 -DWINDOWS -DMSVC -DISLIB /c
# ADD CPP   -DDDEBUG_NOT_LOG -DONE_MEGAAPI_IS_LIB -DLIB_ONE_MEGA_API_EXPORT -DMG_ENABLE_OPENSSL=1 -nologo -EHsc -MT -O2 -GF -FD   -W3 -DWINDOWS -DMSVC -DISLIB   -I..\..\..\src_base\datastruct\. -I..\..\..\src_base\function\. -I..\..\..\src_api\include\. -I..\..\..\libs\ctp\6.6.8\include\. -I.\. /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /libpath:..\..\..\libs\ctp\6.6.8\.\windows_x64 thostmduserapi_se.lib   -nologo /dll    
# ADD LINK32   -nologo /dll     kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /libpath:..\..\..\libs\ctp\6.6.8\.\windows_x64 thostmduserapi_se.lib /out:".\.\OneMegaToCTP.dll"  /implib:".\.\OneMegaToCTP.lib"

!ELSEIF  "$(CFG)" == "OneMegaToCTP - Win32 Debug"

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
# ADD BASE CPP   -DDDEBUG_NOT_LOG -DONE_MEGAAPI_IS_LIB -DLIB_ONE_MEGA_API_EXPORT -DMG_ENABLE_OPENSSL=1 -nologo -EHsc -MTd -Od -Gm -Zi -FD -Zm200   -W3 -DDEBUG -DDEBUG_LOG -DWINDOWS -DMSVC -DISLIB  /c
# ADD CPP   -DDDEBUG_NOT_LOG -DONE_MEGAAPI_IS_LIB -DLIB_ONE_MEGA_API_EXPORT -DMG_ENABLE_OPENSSL=1 -nologo -EHsc -MTd -Od -Gm -Zi -FD -Zm200   -W3 -DDEBUG -DDEBUG_LOG -DWINDOWS -DMSVC -DISLIB   -I..\..\..\src_base\datastruct\. -I..\..\..\src_base\function\. -I..\..\..\src_api\include\. -I..\..\..\libs\ctp\6.6.8\include\. -I.\. /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /libpath:..\..\..\libs\ctp\6.6.8\.\windows_x64 thostmduserapi_se.lib   -nologo /dll   /DEBUG
# ADD LINK32   -nologo /dll   /DEBUG kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /libpath:..\..\..\libs\ctp\6.6.8\.\windows_x64 thostmduserapi_se.lib /out:".\.\OneMegaToCTP.dll"  /implib:".\.\OneMegaToCTP.lib"

!ENDIF 

# Begin Target

# Name "OneMegaToCTP - Win32 Release"
# Name "OneMegaToCTP - Win32 Debug"


# Begin Group "datastruct"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\BigDigit.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\CDate.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\CErrorEngine.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\CSequence.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\CSVParser.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\CSVToField.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\FieldDescribe.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\BigDigit.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\CDate.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\CErrorEngine.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\CSequence.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\CSVParser.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\CSVToField.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\FieldDescribe.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\HashMap.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\datastruct\UFDataType.h
# End Source File
# End Group

# Begin Group "function"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\src_base\function\BaseFunction.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\Config.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\platform.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\printinfo.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\profile.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\utility.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\utils.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\version.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\BaseFunction.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\Config.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\ExpEvaluate.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\platform.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\printinfo.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\profile.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\public.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\utility.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\utils.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\version.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\function\versionValue.h
# End Source File
# End Group

# Begin Group "include"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\src_api\include\OneMegaApi.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_api\include\OneMegaCApi.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_api\include\OneMegaDataStruct.h
# End Source File
# End Group

# Begin Group "ctp_include"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\libs\ctp\6.6.8\include\ThostFtdcMdApi.h
# End Source File
# Begin Source File
SOURCE=..\..\..\libs\ctp\6.6.8\include\ThostFtdcUserApiDataType.h
# End Source File
# Begin Source File
SOURCE=..\..\..\libs\ctp\6.6.8\include\ThostFtdcUserApiStruct.h
# End Source File
# End Group

# Begin Group "OneMegaToCTP"
# PROP Default_Filter ""
# Begin Source File
SOURCE=.\Link.cpp
# End Source File
# Begin Source File
SOURCE=.\Link.h
# End Source File
# End Group

# End Target
# End Project


