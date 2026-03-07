@echo off
setlocal enabledelayedexpansion

:: 1. 获取第一个 *.ini.example 文件作为 project
for %%f in (*.ini.example) do (
    set "project=%%~nf"
)
rem echo !project!

:: 2. 枚举 !project!.* 文件，收集后缀
echo 可选后缀：
set /a i=0
for /f "delims=" %%f in ('dir /b "%project%.*"') do (
    set "fname=%%f"
    :: 排除 project 本身和 project.example
    if /i not "!fname!"=="%project%" if /i not "!fname!"=="%project%.example" (
        :: 去掉前缀 project. 只保留后缀
        set /a i+=1
        set "suffix=!fname:%project%.=!"
        echo !i!. !suffix!
    )
)

:: 3. 显示后缀给用户选择
set /p choice=请输入选择的后缀标号:

:: 遍历标号，复制选中的文件
set /a i=0
for /f "delims=" %%f in ('dir /b "%project%.*"') do (
    set "fname=%%f"
    if /i not "!fname!"=="%project%" if /i not "!fname!"=="%project%.example" (
        set /a i+=1
        if !i! equ !choice! (
            set "suffix=!fname:%project%.=!"
            echo "copy !project!.!suffix! to !project!"
			copy /v /y "!project!.!suffix!" "!project!"
        )
    )
)

pause

endlocal
