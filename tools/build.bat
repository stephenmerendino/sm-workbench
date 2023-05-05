@echo off

set projectPath=%~dp0..
set buildPath=%projectPath%\build\
set buildConfig=debug

if not [%1]==[] (
    set buildConfig=%1
)

cmake %projectPath% -B %buildPath%
cmake --build %buildPath% --config %buildConfig%
