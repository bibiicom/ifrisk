@echo off
@setlocal enabledelayedexpansion

rem 将此文件放到工程的目录下，并配置好路径

set remote=18.141.186.102
set remote_user=ec2-user

rem ssh -n root@!remote! "cd build;chmod u+x build_remote.svn.sh;./build_remote.svn.sh match"
ssh -n root@!remote! "cd build;chmod u+x build.svn.sh;./build.svn.sh client"

ssh -n !remote_user!@!remote! "cd deploy;./last.sh client"

ssh -n !remote_user!@!remote! "cd deploy;./init.sh 8g"

ssh -n !remote_user!@!remote! "cd run/client; ./stop.sh"

ssh -n !remote_user!@!remote! "cd test/client; ./publish.sh ~/run/client"

ssh -n root@!remote! "setcap 'cap_net_bind_service=+ep' /home/!remote_user!/run/client/nginx/sbin/nginx"

ssh -n !remote_user!@!remote! "cd run/client; ./start.sh"

pause