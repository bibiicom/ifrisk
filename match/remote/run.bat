@echo off
@setlocal enabledelayedexpansion

echo 是否要继续执行操作？(Y/N)
choice /c YN /n /m "确认请输入 Y, 取消请输入 N :"

if errorlevel 2 (
    echo 用户取消操作
    exit /b
)


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

ssh -n %release_user%@%release_ip% "git config --global credential.helper store"
ssh -t %release_user%@%release_ip% "git clone https://github.com/scardexcom/batch.git"
ssh -n %release_user%@%release_ip% "mv batch/* ./; mv batch/.git* ./;rm -rf batch"
ssh -n %release_user%@%release_ip% "chmod u+x *.sh; ./update.sh"

ssh -n %release_user%@%release_ip% "cd deploy; cp list.run.all list.run; cp network.ini.example network.ini;cp nginx.conf.9527 nginx.conf"

ssh -n %release_user%@%release_ip% "cat bashrc.example >> .bashrc"


pause
