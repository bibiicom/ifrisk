@echo off
setlocal enabledelayedexpansion
fnm use 18
call pkg withdraw.js --targets node18-linux-x64 --output withdraw
call pkg deposit.js --targets node18-linux-x64 --output deposit



