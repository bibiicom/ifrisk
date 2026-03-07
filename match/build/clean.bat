@echo off
setlocal enabledelayedexpansion

for /d %%i in (*) do (
	cd %%i
	echo clean %%i ...
	call clean.bat
	cd ../
)