@echo off
setlocal enabledelayedexpansion
call setenv.bat

for /d %%i in (*) do (
	cd %%i
	if exist list.xml (
		echo ============= gen makefile %%i
		del makefile* *.dsw *.dsp 1>nul 2>nul
		call genprj list.xml all 1>nul 2>nul
	)
	if exist gen.bat (
		echo ============= gen %%i
		call gen.bat
	) else (
		for /d %%i in (*) do (
			cd %%i
			if exist list.xml (
				echo ============= gen makefile %%i
				del makefile* *.dsw *.dsp 1>nul 2>nul
				call genprj list.xml all 1>nul 2>nul
			)
			if exist gen.bat (
				echo ============= gen %%i
				call gen.bat
			)
			cd ..
		)
	)
	cd ..
)
