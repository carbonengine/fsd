@echo off
echo Checking out project and filters file
echo Regenerating
SET PYTHONUSERBASE=%~dp0..
..\..\..\..\..\..\shared_tools\python\27\python.exe ..\..\..\carbon\tools\ProjectFileGenerator\ProjectFileGenerator.py -i pyFSD.ccpproj
pause