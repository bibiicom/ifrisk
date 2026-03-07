@echo off
setlocal enabledelayedexpansion

for %%i in (*.tar.gz) do (
	mkdir %%~ni
	tar -zxvf %%i -C %%~ni
)

for %%i in (*.zip) do (
	mkdir %%~ni
	unzip -o %%i -d %%~ni
)

for %%i in (windows_x64,linux_x64,hpunix_x64,macos_x64) do (
	for /d %%j in (*%%i*) do (
		echo copy %%j\omqsapi\* .\omqsapi\%%i\
		copy %%j\omqsapi\* .\omqsapi\%%i\
		echo copy %%j\omqcapi\* .\omqcapi\%%i\
		copy %%j\omqcapi\* .\omqcapi\%%i\		
		for /d %%k in (%%j\ofserviceapi\*) do (
			if exist %%k\include (
			echo copy %%k\include\* .\include\
			copy %%k\include\* .\include\
			)
		)
	)
)

pause

for %%i in (*.tar.gz) do (
	rd /s /q %%~ni
)

for %%i in (*.zip) do (
	rd /s /q %%~ni
)

pause