@echo off
setlocal enabledelayedexpansion

set remote=43.134.74.19
set user=trade

ssh %user%@%remote% "./update.sh"

ssh %user%@%remote% "cd lark;npm i"

ssh %user%@%remote% "cd lark;npm run build"

ssh %user%@%remote% "pm2 restart lark"


