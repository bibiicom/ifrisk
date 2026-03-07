@echo off
setlocal enabledelayedexpansion

del /q *.log
del /q .\dump\*
del /q *.con

for /d %%i in (dump_snap\dump.*0000) do (
	rd /q /s %%i
)

for /d %%i in (dump_snap\Inc_*) do (
	rd /q /s %%i
)


del /q flow\*
