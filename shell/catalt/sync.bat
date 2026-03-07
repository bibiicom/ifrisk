@echo off
setlocal enabledelayedexpansion

set remote=43.134.74.19
set user=trade

ssh %user%@%remote% "./update.sh"

ssh %user%@%remote% "pm2 restart task"


