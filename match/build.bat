@echo off
setlocal enabledelayedexpansion

call setenv.bat
cd %~dp0\

if not "%1"=="" (
	echo build version is: [%1]
	@rem 将此刻的时间变成编译version的时间
	echo #define INNER_BASE_VERSION "%1" > ..\src_base\function\versionValue.h
	echo #define INNER_DUE_DATE "20990630" >> ..\src_base\function\versionValue.h
)

rem 对static的lib进行版本控制
if exist .\libs\setstatic.bat (
	cd .\libs
	call setstatic.bat 
	cd %~dp0\
)

cd build

for /d %%i in (*) do (
	cd %%i
	if exist makefile.windows (
		echo ============= make %%i
		call nmake -f makefile.windows 1>output 2>&1
	)
	if exist build.bat (
		echo ============= build %%i
		call build.bat
	) else (
		for /d %%i in (*) do (
			cd %%i
			if exist makefile.windows (
				echo ============= make %%i
				call nmake -f makefile.windows 1>output 2>&1
			)
			if exist build.bat (
				echo ============= build %%i
				call build.bat
			)	
			cd ..
		)
	)
	cd ..
)

pause