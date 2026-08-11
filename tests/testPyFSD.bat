@echo off
setlocal

set "SOURCE_DIR=%~dp0.."
set "BUILD_DIR=%SOURCE_DIR%\.cmake-build-x64-windows-debug"

if not exist "%BUILD_DIR%\CMakeFiles\TargetDirectories.txt" (
    cmake --preset x64-windows-debug -S "%SOURCE_DIR%" -B "%BUILD_DIR%"
    if errorlevel 1 exit /b %errorlevel%
)

cmake --build "%BUILD_DIR%" --config Debug --target pyFSD
if errorlevel 1 exit /b %errorlevel%

ctest --test-dir "%BUILD_DIR%" --build-config Debug --output-on-failure -V
SET PYTHONUSERBASE=%~dp0../..
cd ..\
::python %CCP_EVE_PERFORCE_BRANCH_PATH%\updateBinaries.py -p Windows -a x64 client
%CCP_EVE_PERFORCE_BRANCH_PATH%\eve\client\pythonInterpreter.bat %cd%\python\PYFSDTest\test\test_pyFSD.py
pause
