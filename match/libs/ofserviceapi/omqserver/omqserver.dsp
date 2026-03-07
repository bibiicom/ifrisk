# Microsoft Developer Studio Project File - Name="omqserver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=omqserver - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "omqserver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "omqserver.mak" CFG="omqserver - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "omqserver - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "omqserver - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "omqserver - Win32 Release"

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
# ADD CPP   -DDEBUG_NOT_LOG -nologo -EHsc -MT -O2 -GF -FD   -W3 -DWINDOWS -DMSVC   -I..\..\..\build\src\function\. -I..\..\..\build\src\datastruct\. -I..\..\..\build\libs\ofserviceapi\1.0\include\. -I..\..\..\build\libs\ofserviceapi\ofapibase\. /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib     -nologo /FIXED:NO     /WARN:2
# ADD LINK32   -nologo /FIXED:NO     /WARN:2 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /out:".\../../../build/run/test/tools\omqserver.exe"

!ELSEIF  "$(CFG)" == "omqserver - Win32 Debug"

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
# ADD CPP   -DDEBUG_NOT_LOG -nologo -EHsc -MTd -Od -Gm -Zi -FD -Zm200   -W3 -DDEBUG -DDEBUG_LOG -DWINDOWS -DMSVC   -I..\..\..\build\src\function\. -I..\..\..\build\src\datastruct\. -I..\..\..\build\libs\ofserviceapi\1.0\include\. -I..\..\..\build\libs\ofserviceapi\ofapibase\. /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib     -nologo /FIXED:NO     /DEBUG /WARN:2
# ADD LINK32   -nologo /FIXED:NO     /DEBUG /WARN:2 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /out:".\../../../build/run/test/tools\omqserver.exe"

!ENDIF 

# Begin Target

# Name "omqserver - Win32 Release"
# Name "omqserver - Win32 Debug"


# Begin Group "function"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\build\src\function\BaseFunction.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\Config.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\platform.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\printinfo.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\profile.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\utility.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\utils.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\version.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\BaseFunction.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\Config.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\ExpEvaluate.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\platform.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\printinfo.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\profile.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\public.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\utility.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\utils.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\version.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\function\versionValue.h
# End Source File
# End Group

# Begin Group "datastruct"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\BigDigit.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\CDate.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\CErrorEngine.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\CSequence.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\CSVParser.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\CSVToField.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\FieldDescribe.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\BigDigit.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\CDate.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\CErrorEngine.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\CSequence.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\CSVParser.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\CSVToField.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\FieldDescribe.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\HashMap.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\datastruct\UFDataType.h
# End Source File
# End Group

# Begin Group "ofserviceapi_include"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\build\libs\ofserviceapi\1.0\include\OutFogServiceApi.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\libs\ofserviceapi\1.0\include\OutFogServiceApiStruct.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\libs\ofserviceapi\1.0\include\OutFogServiceCApi.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\libs\ofserviceapi\1.0\include\ServiceResponser.h
# End Source File
# End Group

# Begin Group "ofapibase"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\build\libs\ofserviceapi\ofapibase\OutFogServiceApiWrapper.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\libs\ofserviceapi\ofapibase\OutFogService.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\libs\ofserviceapi\ofapibase\OutFogServiceApiWrapper.h
# End Source File
# End Group

# Begin Group "omqserver"
# PROP Default_Filter ""
# Begin Source File
SOURCE=.\omqserver.cpp
# End Source File
# End Group

# End Target
# End Project


