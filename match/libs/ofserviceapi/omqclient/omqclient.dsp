# Microsoft Developer Studio Project File - Name="omqclient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=omqclient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "omqclient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "omqclient.mak" CFG="omqclient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "omqclient - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "omqclient - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "omqclient - Win32 Release"

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
# ADD CPP   -DDEBUG_NOT_LOG -nologo -EHsc -MT -O2 -GF -FD   -W3 -DWINDOWS -DMSVC   -I..\..\..\build\src\function\. -I..\..\..\build\src\datastruct\. -I..\..\..\build\src\thread\. -I..\..\..\build\src\logger\. -I..\..\src_network\network\. -I..\..\src_network\network_tcp\. -I..\..\src_network\reactor\. -I..\..\src_protocol\flow\. -I..\..\src_protocol\protocol\. -I..\..\src_protocol\protocol_channel\. -I..\..\src_protocol\protocol_nameserver\. -I..\..\src_protocol\protocol_subscribe\. -I..\..\src_protocol\protocol_link\. -I..\..\src_protocol\protocol_compress\. -I..\..\src_protocol\protocol_ftd\. -I..\..\src_protocol\package\. -I.\. /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib     -nologo /FIXED:NO     /WARN:2
# ADD LINK32   -nologo /FIXED:NO     /WARN:2 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /out:".\../../../build/run/test/tools\omqclient.exe"

!ELSEIF  "$(CFG)" == "omqclient - Win32 Debug"

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
# ADD CPP   -DDEBUG_NOT_LOG -nologo -EHsc -MTd -Od -Gm -Zi -FD -Zm200   -W3 -DDEBUG -DDEBUG_LOG -DWINDOWS -DMSVC   -I..\..\..\build\src\function\. -I..\..\..\build\src\datastruct\. -I..\..\..\build\src\thread\. -I..\..\..\build\src\logger\. -I..\..\src_network\network\. -I..\..\src_network\network_tcp\. -I..\..\src_network\reactor\. -I..\..\src_protocol\flow\. -I..\..\src_protocol\protocol\. -I..\..\src_protocol\protocol_channel\. -I..\..\src_protocol\protocol_nameserver\. -I..\..\src_protocol\protocol_subscribe\. -I..\..\src_protocol\protocol_link\. -I..\..\src_protocol\protocol_compress\. -I..\..\src_protocol\protocol_ftd\. -I..\..\src_protocol\package\. -I.\. /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib     -nologo /FIXED:NO     /DEBUG /WARN:2
# ADD LINK32   -nologo /FIXED:NO     /DEBUG /WARN:2 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib   /out:".\../../../build/run/test/tools\omqclient.exe"

!ENDIF 

# Begin Target

# Name "omqclient - Win32 Release"
# Name "omqclient - Win32 Debug"


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

# Begin Group "thread"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\build\src\thread\Semaphore.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\thread\Thread.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\thread\Mutex.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\thread\Semaphore.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\thread\Thread.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\thread\ThreadSafeMap.h
# End Source File
# End Group

# Begin Group "logger"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\..\build\src\logger\CFilelogService.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\logger\CLogger.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\logger\CProbeLogger.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\logger\CSyslogService.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\logger\monitorIndex.cpp
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\logger\CFilelogService.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\logger\CLogger.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\logger\CProbeLogger.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\logger\CSyslogService.h
# End Source File
# Begin Source File
SOURCE=..\..\..\build\src\logger\monitorIndex.h
# End Source File
# End Group

# Begin Group "network"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_network\network\Channel.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\Client.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\EpollReactor.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\ListenCtrl.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\NetworkFactory.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\PipeEventHandler.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\SelectReactor.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\Server.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\ServiceName.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\SocketInit.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\Channel.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\Client.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\EpollReactor.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\ListenCtrl.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\NetworkFactory.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\PipeEventHandler.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\SelectReactor.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\Server.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\ServiceName.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network\SocketInit.h
# End Source File
# End Group

# Begin Group "network_tcp"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_network\network_tcp\TcpChannel.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network_tcp\TcpClient.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network_tcp\TcpNetworkFactory.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network_tcp\TcpServer.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network_tcp\TcpChannel.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network_tcp\TcpClient.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network_tcp\TcpNetworkFactory.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\network_tcp\TcpServer.h
# End Source File
# End Group

# Begin Group "reactor"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_network\reactor\EventDispatcher.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\EventHandler.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\EventQueue.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\Reactor.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\SimpleReactor.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\TimerHeap.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\EventDispatcher.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\EventHandler.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\EventQueue.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\Reactor.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\SimpleReactor.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\TimerHeap.h
# End Source File
# Begin Source File
SOURCE=..\..\src_network\reactor\TimerQueue.h
# End Source File
# End Group

# Begin Group "flow"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_protocol\flow\CachedFlow.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\CacheList.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\FileFlow.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\FlowReader.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\IndexFlow.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\OneTimeFlow.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\PubFlow.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\CachedFlow.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\CacheList.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\DialogFlow.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\FileFlow.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\Flow.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\FlowReader.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\IndexFlow.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\LockFlow.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\OneTimeFlow.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\PubFlow.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\flow\ReadFlow.h
# End Source File
# End Group

# Begin Group "protocol"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_protocol\protocol\Protocol.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol\Protocol.h
# End Source File
# End Group

# Begin Group "protocol_channel"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_protocol\protocol_channel\ChannelPackage.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_channel\ChannelProtocol.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_channel\Session.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_channel\SessionFactory.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_channel\ChannelPackage.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_channel\ChannelProtocol.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_channel\Session.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_channel\SessionFactory.h
# End Source File
# End Group

# Begin Group "protocol_nameserver"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_protocol\protocol_nameserver\NsPackage.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_nameserver\NsProtocol.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_nameserver\NsSession.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_nameserver\NsSessionFactory.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_nameserver\NsPackage.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_nameserver\NsProtocol.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_nameserver\NsSession.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_nameserver\NsSessionFactory.h
# End Source File
# End Group

# Begin Group "protocol_subscribe"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_protocol\protocol_subscribe\SCPPackage.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_subscribe\SCPPackageDesc.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_subscribe\SCPProtocol.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_subscribe\SCPPackage.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_subscribe\SCPPackageDesc.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_subscribe\SCPProtocol.h
# End Source File
# End Group

# Begin Group "protocol_link"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_protocol\protocol_link\XMPPackage.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_link\XMPProtocol.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_link\XMPSession.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_link\XMPPackage.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_link\XMPProtocol.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_link\XMPSession.h
# End Source File
# End Group

# Begin Group "protocol_compress"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_protocol\protocol_compress\CompressPackage.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_compress\CompressProtocol.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_compress\CompressUtil.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_compress\CompressPackage.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_compress\CompressProtocol.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_compress\CompressUtil.h
# End Source File
# End Group

# Begin Group "protocol_ftd"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_protocol\protocol_ftd\FTDCProtocol.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_ftd\FTDCPubEndPoint.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_ftd\FTDCSession.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_ftd\FTDCSubEndPoint.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_ftd\OrderingQ.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_ftd\FTDCProtocol.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_ftd\FTDCPubEndPoint.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_ftd\FTDCSession.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_ftd\FTDCSubEndPoint.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_ftd\FTDCSubscriber.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\protocol_ftd\OrderingQ.h
# End Source File
# End Group

# Begin Group "package"
# PROP Default_Filter ""
# Begin Source File
SOURCE=..\..\src_protocol\package\FieldSet.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\package\FTDCPackage.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\package\Package.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\package\XTPPackage.cpp
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\package\FieldSet.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\package\FTDCPackage.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\package\Package.h
# End Source File
# Begin Source File
SOURCE=..\..\src_protocol\package\XTPPackage.h
# End Source File
# End Group

# Begin Group "omqclient"
# PROP Default_Filter ""
# Begin Source File
SOURCE=.\omqclient.cpp
# End Source File
# Begin Source File
SOURCE=.\SimpleConnect.cpp
# End Source File
# Begin Source File
SOURCE=.\SimpleConnect.h
# End Source File
# End Group

# End Target
# End Project


