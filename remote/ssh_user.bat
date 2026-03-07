@echo off
@setlocal enabledelayedexpansion

for /f "delims=" %%f in ('dir /b *.ini') do (
    for /f "tokens=1,2 delims==" %%i in (%%f) do (
        if "%%i"=="build_ip"   set "build_ip=%%j"
        if "%%i"=="release_user"   set "release_user=%%j"
        if "%%i"=="release_ip" set "release_ip=%%j"
    )
	set "project=%%~nf"
)
if "%release_ip%"=="" set "release_ip=%build_ip%"

ssh -t %release_user%@%release_ip%
