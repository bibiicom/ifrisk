@echo off
setlocal enabledelayedexpansion

ssh fastup@43.163.202.31 "rm -rf */*monitor.log"
ssh zhonghua@47.254.2.25 "rm -rf */*monitor.log"
ssh market@18.141.186.102 "rm -rf */*monitor.log"

rem ssh fastup@43.163.202.31 "rm -rf */*monitor.log;./order.sh c"
rem ssh zhonghua@47.254.2.25 "rm -rf */*monitor.log;./order.sh c"
rem ssh market@18.141.186.102 "rm -rf */*monitor.log;./order.sh c"

#pause