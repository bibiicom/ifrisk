@echo off
setlocal enabledelayedexpansion

del /q 1.tar.gz
set ipaddress=119.45.191.216
set user=xzh

ssh !user!@!ipaddress! "rm -rf ./run/risk/1.tar.gz"
ssh !user!@!ipaddress! "cd ./run/risk; tar cvf 1.tar ./flow/inputall.* ./*.ini; gzip 1.tar"
scp !user!@!ipaddress!:./run/risk/1.tar.gz ./

rd /q /s flow
tar -xvf 1.tar.gz
pause