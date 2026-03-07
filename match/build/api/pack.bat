@echo off
setlocal enabledelayedexpansion

for %%f in (cplusplus\*Api.dll) do (
    set "capiname=%%f"
)
set "filename=!capiname:.so=!"

:: 执行复制操作
copy /y ..\..\libs\ofserviceapi\1.0\omqcapi\windows_x64\omqcapi.dll "!filename!X.dll"
 
rd /s /q cplusplus\include  1>nul 2>nul
mkdir cplusplus\include 1>nul 2>nul
copy ..\..\src_api\include\*.h cplusplus\include\
del cplusplus\include\*CApi.h
