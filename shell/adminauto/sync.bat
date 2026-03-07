@echo off
setlocal enabledelayedexpansion
svn update
svn commit --message ""

ssh fastup@43.163.202.31 "./stop.sh"
ssh zhonghua@47.254.2.25 "./stop.sh"
ssh market@18.141.186.102 "./stop.sh"

ssh fastup@43.163.202.31 "./update.sh"
ssh zhonghua@47.254.2.25 "./update.sh"
ssh market@18.141.186.102 "./update.sh"

ssh fastup@43.163.202.31 "export TERM=xterm-256color;./start.sh"
ssh zhonghua@47.254.2.25 "export TERM=xterm-256color;./start.sh"
ssh market@18.141.186.102 "export TERM=xterm-256color;./start.sh"

#pause