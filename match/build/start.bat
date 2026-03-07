@echo off
setlocal enabledelayedexpansion

for /d %%i in (*) do (
	cd %%i
	for /f "tokens=1,2 delims=_" %%a in ('echo %%i') do (
		if exist %%a.exe (		
			echo start %%a.exe %%b
			start %%a.exe %%b
		)
	)
	cd ..
)