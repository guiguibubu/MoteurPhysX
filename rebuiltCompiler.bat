
@echo off

cls

set openWhenFinish=%1

call CleanAll.bat noCheck

set pauseOpt=nopause

@echo off
call CheckInstall.bat CMAKE

if ErrorLevel == 1 (
    goto fail
)


SET bDirVar=compiler

echo Removing Old Build Directory

rd %bDirVar% /Q /S

mkdir %bDirVar%

cd %bDirVar%

if not defined CMAKE_GENERATOR (
    set CMAKE_GENERATOR="Visual Studio 16 2019"
) 

if exist "%CMAKE_PATH%/cmake.exe" (
"%CMAKE_PATH%/cmake" .. -G %CMAKE_GENERATOR%
goto generated
)

cmake .. -G %CMAKE_GENERATOR%

:generated

cd ..

if NOT %pauseOpt% == nopause (
    pause
)

if "%openWhenFinish%"=="-o" (
	.\%bDirVar%\Simulation.sln
)

goto end:

:fail
    echo.
    echo Rebuild FAILED
    goto end

:end

:RetrievePath
    IF EXIST "TeamInfo.team" (
        for /f "delims== tokens=1,2" %%G in (TeamInfo.team) do set %%G=%%H
    )
    exit /B 0