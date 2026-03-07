@echo off
setlocal enabledelayedexpansion

rem svn commit --message ""

scp kline.sh centos@173.231.224.54:./run/sinkdb1/
ssh centos@173.231.224.54 "chmod u+x ./run/sinkdb1/*.sh
