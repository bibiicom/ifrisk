@echo off
@setlocal enabledelayedexpansion

for /f "delims=" %%f in ('dir /b *.ini') do (
    for /f "tokens=1,2 delims==" %%i in (%%f) do (
        if "%%i"=="build_ip"   set "build_ip=%%j"
        if "%%i"=="build_user" set "build_user=%%j"
    )
)

scp *.sh %build_user%@%build_ip%:./
scp *.ini %build_user%@%build_ip%:./

ssh %build_user%@%build_ip% "chmod u+x *.sh"

pause