@echo off
setlocal enabledelayedexpansion
call setenv.bat

call gentpl ..\..\xmls\entity 1>nul

for /d %%i in (*) do (
	cd %%i
	echo cd %%i
	if exist gen.bat (
		echo ============= gen %%i
		call gen.bat
	)
	cd ..
)

for /d %%i in (cplusplus,linkswap,dataswap*) do (
	cd %%i
	echo ============= gen makefile %%i
	del makefile* *.dsw *.dsp 1>nul 2>nul
	call genprj list.xml all 1>nul 2>nul
	cd ..
)
