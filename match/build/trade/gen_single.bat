@echo off
setlocal enabledelayedexpansion

cd ..\..\xmls\
call gen.bat

cd %~dp0
call genprj list.xml

pause
