@echo off
setlocal enabledelayedexpansion

del /q *.log
del /q flow\*.*log
del /q flow\*.flow
del /q flow\*.txt
rd /q /s snap
mkdir snap
mkdir snap
