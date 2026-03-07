@echo off
@setlocal enabledelayedexpansion

rem 将此文件放到工程的目录下，并配置好路径

for /f "tokens=1,2 delims==" %%i in (..\net.ini) do (
	if "%%i"=="remote_user"   set "remote_user=%%j"
	if "%%i"=="remote_ip"   set "remote_ip=%%j"
)

call npm install
call npm run build

cd build

del build.zip

zip -r build.zip *

cd ..

ssh -n !remote_user!@!remote_ip! "cd ./run/client;./stop.sh"

ssh -n !remote_user!@!remote_ip! "rm -rf ./run/client/nginx/html/*"

ssh -n !remote_user!@!remote_ip! "cd ./deploy;./init.sh 8g"

scp -r build/build.zip !remote_user!@!remote_ip!:./run/client/nginx/html/

ssh -n !remote_user!@!remote_ip! "cd ./run/client/nginx/html/; unzip build.zip; rm -rf build.zip"

ssh -n root@!remote_ip! "setcap 'cap_net_bind_service=+ep' /home/!remote_user!/run/client/nginx/sbin/nginx"
 
ssh -n !remote_user!@!remote_ip! "cd ./run/client; ./start.sh"

pause