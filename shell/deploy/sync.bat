@echo off
@setlocal enabledelayedexpansion

rem 将此文件放到工程的目录下，并配置好路径

set remote=18.141.186.102
set remote_user=ec2-user

scp -r nginx.conf !remote_user!@!remote!:./deploy/

pause