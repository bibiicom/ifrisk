@echo off
setlocal enabledelayedexpansion

copy ..\..\libs\ofserviceapi\1.0\omqsapi\windows_x64\*.dll .\
copy ..\..\libs\ofserviceapi\1.0\omqcapi\windows_x64\*.dll .\
copy ..\api\cplusplus\*.dll .\
