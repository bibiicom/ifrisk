@echo off
setlocal enabledelayedexpansion
svn update
svn commit --message ""

ssh fastup@43.163.202.31 "./update.sh"
ssh zhonghua@47.254.2.25 "./update.sh"
ssh market@18.141.186.102 "./update.sh"

#pause