@echo off
setlocal enabledelayedexpansion

scp * ec2-user@18.141.186.102:./history/
ssh ec2-user@18.141.186.102 "chmod u+x ./history/*.sh"