@echo off
setlocal enabledelayedexpansion

del /q 1.tar.gz

for /f "delims=" %%f in ('dir /b ..\..\remote\*.ini') do (
    set "fullpath=..\..\remote\%%f"
	for /f "tokens=1,2 delims==" %%i in (!fullpath!) do (
        if "%%i"=="build_ip"   set "build_ip=%%j"
        if "%%i"=="build_user" set "build_user=%%j"
        if "%%i"=="release_user"   set "release_user=%%j"
        if "%%i"=="release_ip" set "release_ip=%%j"
    )
	set "project=%%~nf"
)
if "%release_ip%"=="" set "release_ip=%build_ip%"

ssh %release_user%@%release_ip% "cd ./run/trade/;./get1.sh %1"
scp %release_user%@%release_ip%:./run/trade/1.tar.gz ./

rd /q /s snap
rd /q /s flow

tar -xvf 1.tar.gz
rem pause