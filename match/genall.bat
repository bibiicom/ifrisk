@echo off
setlocal enabledelayedexpansion
call setenv.bat

if not exist src_base\function\versionValue.h (
	set /p version=<version
	set builddate=%date:~0,4%%date:~5,2%%date:~8,2%
	set buildversion=!version!_!builddate!
	echo #define INNER_BASE_VERSION "!buildversion!" > src_base\function\versionValue.h
	echo #define INNER_DUE_DATE "20240630" >> src_base\function\versionValue.h
)

rem 对static的lib进行版本控制
if exist libs\setstatic.bat (
	cd libs
	call setstatic.bat 
	cd %~dp0\
)

cd xmls
echo ============= gen xmls
call gen.bat
cd ..

for /d %%i in (build*) do (
	cd %%i
	if exist gen.bat (
		echo ============= gen %%i
		call gen.bat
	)
	cd ..
)

pause