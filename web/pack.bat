@echo off
setlocal enabledelayedexpansion

cd build

for /d %%i in (*) do (
	cd %%i
	if exist pack.bat (
		echo ============= pack %%i
		call pack.bat
	)	
	cd ..
)

if "%1"=="" (
	set tarname=bin.zip
) else (
	set tarname=%1.zip
)

zip -r !tarname! *\*.exe *\*.dll api\*\*.dll api\*\*.lib api\*\include sql\*.sql
zip -r -q !tarname! api\i5ting\preview\ api\example\
zip -r !tarname! *\*.bat -x *\build.bat -x *\gen.bat -x *\pack.bat 
zip -r !tarname! *.bat -x build.bat -x gen.bat -x pack.bat 