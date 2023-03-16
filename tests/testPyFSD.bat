SET PYTHONUSERBASE=%~dp0../..
cd ..\
::python %CCP_EVE_PERFORCE_BRANCH_PATH%\updateBinaries.py -p Windows -a x64 client
%CCP_EVE_PERFORCE_BRANCH_PATH%\eve\client\pythonInterpreter.bat %cd%\python\PYFSDTest\test\test_pyFSD.py
pause
