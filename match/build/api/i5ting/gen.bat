rem @echo off
setlocal enabledelayedexpansion
call setenv.bat

rem npm install -g i5ting_toc

iconv.exe -c -f GBK -t UTF-8 api.md > api.u.md
rd /s /q preview 1>nul 2>nul
call i5ting_toc -f api.u.md
copy example\* preview\ 
