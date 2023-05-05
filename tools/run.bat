@echo off

set projectPath=%~dp0..
set buildConfig=debug

if not [%1]==[] (
    set buildConfig=%1
)

set exePath=%projectPath%\run\%buildConfig%\

START /D %exePath% %exePath%\sm-workbench.exe
