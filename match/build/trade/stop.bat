@echo off
setlocal enabledelayedexpansion

echo stop trade.exe
curl http://127.0.0.1:8090/StopEngine
curl http://127.0.0.1:8090/DumpMDB

taskkill /f /im trade.exe
