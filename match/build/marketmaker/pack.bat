@echo off
setlocal enabledelayedexpansion

copy ..\..\libs\ofserviceapi\1.0\omqsapi\windows_x64\*.dll .\
copy ..\..\libs\ofserviceapi\1.0\omqcapi\windows_x64\*.dll .\
copy ..\..\libs\ctp\6.6.8\windows_x64\*.dll .\

copy ..\api\cplusplus\*.dll .\

for /d %%i in (..\apilinks\*) do (
	copy %%i\*.dll .\
)

