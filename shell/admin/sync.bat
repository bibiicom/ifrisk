@echo off
setlocal enabledelayedexpansion

rem scp * ec2-user@13.213.55.10:./run/tools/
rem ssh ec2-user@13.213.55.10 "chmod u+x ./run/tools/*.sh"

scp * ec2-user@18.141.186.102:./admin/
ssh ec2-user@18.141.186.102 "chmod u+x ./run/tools/*.sh"
