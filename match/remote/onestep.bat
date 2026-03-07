@echo off
@setlocal enabledelayedexpansion

for /f "delims=" %%f in ('dir /b *.ini') do (
    for /f "tokens=1,2 delims==" %%i in (%%f) do (
        if "%%i"=="build_ip"   set "build_ip=%%j"
        if "%%i"=="build_user" set "build_user=%%j"
        if "%%i"=="release_user"   set "release_user=%%j"
        if "%%i"=="release_ip" set "release_ip=%%j"
    )
	set "project=%%~nf"
)
if "%release_ip%"=="" set "release_ip=%build_ip%"

cd ../
git commit -a -m "a"
git push

ssh -n %build_user%@%build_ip% "chmod u+x build.sh;./build.sh %project%"

ssh -n %release_user%@%release_ip% "cd deploy;./last.sh"

ssh -n %release_user%@%release_ip% "cd deploy;./init.sh 8g"

ssh -n %build_user%@%build_ip% "setcap 'cap_net_bind_service=+ep' /home/%release_user%/run/client/nginx/sbin/nginx"

ssh -n %release_user%@%release_ip% "cd deploy;./onestep.sh Y"

pause