
@echo off

cls

set pauseOpt=nopause

call CleanAll.bat noCheck

if ErrorLevel == 1 (
    goto fail
)

@echo off
call CheckInstall.bat CMAKE

if ErrorLevel == 1 (
    goto fail
)

if exist "%CMAKE_PATH%/cmake.exe" (
"%CMAKE_PATH%/cmake" -G "Visual Studio 15 2017 Win64"
goto generated
)

cmake -G "Visual Studio 15 2017 Win64"

:generated

if NOT %pauseOpt% == nopause (
    pause
)

Simulation.sln

goto end:

:fail
    echo.
    echo Rebuild FAILED
    goto end

:end