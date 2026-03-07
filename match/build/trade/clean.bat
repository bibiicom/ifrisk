@echo off
setlocal enabledelayedexpansion

del /q *.log

rd /q /s snap
mkdir snap

del /q flow\*.*log
del /q flow\*.txt
del /q flow\*.flow
rem del /q 1.tar.gz *.ini