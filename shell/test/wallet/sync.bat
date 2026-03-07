@echo off
setlocal enabledelayedexpansion

call npm install

for /f "delims=" %%f in ('dir /b *.ini') do (
    for /f "tokens=1,2 delims==" %%i in (%%f) do (
        if "%%i"=="remote"   set "remote=%%j"
        if "%%i"=="user" set "user=%%j"
    )
)

scp *.sh %user%@%remote%:./run/wallet/

ssh %user%@%remote% "stop.sh wallet Y"

for /d %%i in (arb) do (
	cd %%i
	call pkg withdraw.js --targets node18-linux-x64 --output withdraw
	call pkg deposit.js --targets node18-linux-x64 --output deposit
	scp deposit %user%@%remote%:./run/wallet/%%i/
	scp withdraw %user%@%remote%:./run/wallet/%%i/
	ssh %user%@%remote% "chmod u+x ./run/wallet/%%i/deposit ./run/wallet/%%i/withdraw"
	cd ..
)

ssh %user%@%remote% "start.sh wallet"

