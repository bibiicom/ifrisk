# Microsoft Developer Studio Project File - Name="OneMegaToSina" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=OneMegaToSina - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OneMegaToSina.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OneMegaToSina.mak" CFG="OneMegaToSina - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OneMegaToSina - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OneMegaToSina - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OneMegaToSina - Win32 Release"

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
# ADD BASE CPP   -DDDEBUG_NOT_LOG -DONE_MEGAAPI_IS_LIB -DLIB_ONE_MEGA_API_EXPORT -DMG_ENABLE_OPENSSL=0 -nologo -EHsc -MT -O2 -GF -FD   -W3 -DWINDOWS -DMSVC -DISLIB /c
# ADD CPP   -DDDEBUG_NOT_LOG -DONE_MEGAAPI_IS_LIB -DLIB_ONE_MEGA_API_EXPORT -DMG_ENABLE_OPENSSL=0 -nologo -EHsc -MT -O2 -GF -FD   -W3 -DWINDOWS -DMSVC -DISLIB   -I..\..\..\src_base\datastruct\. -I..\..\..\src_base\function\. -I..\..\..\src_base\mongoose79\. -I..\..\..\src_base\tool\. -I..\..\..\src_base\jsoncpp\. -I..\..\..\src_base\thread\. -I..\..\..\src_api\include\. -I.\. /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib     -nologo /dll    
# ADD LINK32   -nologo /dll     kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /out:".\.\OneMegaToSina.dll"  /implib:".\.\OneMegaToSina.lib"

!ELSEIF  "$(CFG)" == "OneMegaToSina - Win32 Debug"

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
# ADD BASE CPP   -DDDEBUG_NOT_LOG -DONE_MEGAAPI_IS_LIB -DLIB_ONE_MEGA_API_EXPORT -DMG_ENABLE_OPENSSL=0 -nologo -EHsc -MTd -Od -Gm -Zi -FD -Zm200   -W3 -DDEBUG -DDEBUG_LOG -DWINDOWS -DMSVC -DISLIB  /c
# ADD CPP   -DDDEBUG_NOT_LOG -DONE_MEGAAPI_IS_LIB -DLIB_ONE_MEGA_API_EXPORT -DMG_ENABLE_OPENSSL=0 -nologo -EHsc -MTd -Od -Gm -Zi -FD -Zm200   -W3 -DDEBUG -DDEBUG_LOG -DWINDOWS -DMSVC -DISLIB   -I..\..\..\src_base\datastruct\. -I..\..\..\src_base\function\. -I..\..\..\src_base\mongoose79\. -I..\..\..\src_base\tool\. -I..\..\..\src_base\jsoncpp\. -I..\..\..\src_base\thread\. -I..\..\..\src_api\include\. -I.\. /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib     -nologo /dll   /DEBUG
# ADD LINK32   -nologo /dll   /DEBUG kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /out:".\.\OneMegaToSina.dll"  /implib:".\.\OneMegaToSina.lib"

!ENDIF 

# Begin Target

# Name "OneMegaToSina - Win32 Release"
# Name "OneMegaToSina - Win32 Debug"


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

# Begin Group "mongoose79"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\src_base\mongoose79\HttpClient.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\mongoose79\SMTPClient.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\mongoose79\WSClient.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\mongoose79\mongoose.c
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\mongoose79\HttpClient.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\mongoose79\mongoose.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\mongoose79\SMTPClient.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\mongoose79\WSClient.h
# End Source File
# End Group

# Begin Group "tool"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\src_base\tool\tool.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\tool\tool.h
# End Source File
# End Group

# Begin Group "jsoncpp"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\src_base\jsoncpp\json_reader.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\jsoncpp\json_value.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\jsoncpp\json_writer.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\jsoncpp\json_batchallocator.h
# End Source File
# End Group

# Begin Group "thread"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\src_base\thread\Semaphore.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\thread\Thread.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\thread\Mutex.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\thread\Semaphore.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\thread\Thread.h
# End Source File
# Begin Source File
SOURCE=..\..\..\src_base\thread\ThreadSafeMap.h
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

# Begin Group "OneMegaToSina"
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


