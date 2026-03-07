@echo off
setlocal enabledelayedexpansion

for /d %%i in (*) do (
	cd %%i
	for %%j in (*.exe) do (
		echo stop %%j
		taskkill /f /im %%j
	)
	cd ..
)