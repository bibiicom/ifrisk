@echo off
setlocal enabledelayedexpansion

del /q *.log
del /q dump\*

for /d %%i in (snap/inc*) do (
	echo rm snap\%%i
	rd /q /s snap\%%i
)

del /q flow\*.nlog
del /q flow\*.txt
del /q flow\output*flow
del /q flow\*flow
del /q flow\*.tlog
del /q flow\*.log

rd /q /s snap\snap.54581500000
del /q flow\inputall.54581500000.flow