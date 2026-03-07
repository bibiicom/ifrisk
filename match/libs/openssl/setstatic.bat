@echo off
setlocal enabledelayedexpansion
cd ..\..\
call setenv.bat
cd %~dp0\

rem 对static的lib进行版本控制
rd /s /q 1.1.0h\static\windows_x64 1>nul 2>nul
if exist 1.1.0h\static\windows_%cc_version%_x64 (
	xcopy 1.1.0h\static\windows_%cc_version%_x64 1.1.0h\static\windows_x64 /s /e /i /y
) else (
	xcopy 1.1.0h\static\windows_x64.gen 1.1.0h\static\windows_x64 /s /e /i /y
)

rd /s /q 1.1.0h\static\linux_x64 1>nul 2>nul
xcopy 1.1.0h\static\linux_x64.gen 1.1.0h\static\linux_x64 /s /e /i /y

