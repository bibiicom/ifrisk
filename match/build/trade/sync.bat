@echo off
setlocal enabledelayedexpansion

rem svn commit --message ""

scp redis*.sh db*.sh centos@173.231.224.54:./run/trade/
ssh centos@173.231.224.54 "chmod u+x ./run/trade/*.sh
