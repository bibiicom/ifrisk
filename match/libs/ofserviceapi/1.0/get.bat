@echo off
setlocal enabledelayedexpansion

set name=net.last.linux_x64

del !name!.tar.gz

:: 执行远程命令并捕获输出
for /f "delims=" %%i in ('ssh ec2-user@18.141.186.102 "ls -t ./release/net/*.tar.gz | head -n 1"') do (
    set REMOTE_FILE=%%i
    echo Latest file: !REMOTE_FILE!
    scp ec2-user@18.141.186.102:!REMOTE_FILE! ./!name!.tar.gz
)

mkdir !name!
tar -zxvf !name!.tar.gz -C !name!

echo copy !name!\omqsapi\* .\omqsapi\linux_x64\
copy !name!\omqsapi\* .\omqsapi\linux_x64\
echo copy !name!\omqcapi\* .\omqcapi\linux_x64\
copy !name!\omqcapi\* .\omqcapi\linux_x64\		
for /d %%k in (!name!\ofserviceapi\*) do (
	if exist %%k\include (
	echo copy %%k\include\* .\include\
	copy %%k\include\* .\include\
	)
)

pause

rd /s /q !name!

