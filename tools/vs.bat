@echo off

set projectPath=%~dp0..
set sln=%projectPath%\build\sm-workbench.sln

start devenv.exe %sln%
