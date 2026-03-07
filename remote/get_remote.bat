@echo off
@setlocal enabledelayedexpansion

for /f "delims=" %%f in ('dir /b *.ini') do (
    for /f "tokens=1,2 delims==" %%i in (%%f) do (
        if "%%i"=="build_ip"   set "build_ip=%%j"
        if "%%i"=="build_user" set "build_user=%%j"
    )
	set "project=%%~nf"
)

rmdir /s /q "%project%"
del *.tar.gz

for /f "usebackq delims=" %%x in (`ssh -n %build_user%@%build_ip% "ls -t ./release/%project%/*.tar.gz | head -n 1"`) do (
    set "release=%%x"
)

echo Latest release: %release%
scp %build_user%@%build_ip%:%release% .
mkdir "%project%"

for /f "delims=" %i in ('dir /b *.tar.gz') do tar -xvf "%i" -C "%project%"

pause