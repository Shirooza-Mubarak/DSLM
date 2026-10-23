@echo off
setlocal
cd /d "%~dp0"
set "PATH=%~dp0;%~dp0ScannerFiles;%~dp0StageFiles;%~dp0CameraFiles;%PATH%"
"%~dp0x64\Release\MyDSLM.exe"