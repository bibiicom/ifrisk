rem @echo off
setlocal enabledelayedexpansion
copy .\example\* .\preview\
scp -r preview centos@173.231.224.54:/home/centos/website/