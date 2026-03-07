@echo off
setlocal enabledelayedexpansion
cd ..
call setenv.bat
cd %~dp0\

for /d %%i in (*) do (
	cd %%i
	if exist setstatic.bat (
		echo ============= setstatic %%i
		call setstatic.bat
	)	
	cd ..
)
