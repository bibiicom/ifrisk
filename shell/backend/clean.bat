@echo off
setlocal enabledelayedexpansion

set remote=43.134.74.19
set user=trade

ssh %user%@%remote% "rm -rf ./run/client/nginx/his1h/*"
ssh %user%@%remote% "rm -rf ./run/client/nginx/hisw/*"
