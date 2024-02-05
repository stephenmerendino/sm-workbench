@echo off

set rawAssetDir=%1
set deployDir=%2
set outputAssetDir=%deployDir%\RuntimeAssets\

REM If %deployDir\RuntimeAssets\ does not exist, then create it
if not exist %outputAssetDir% mkdir %outputAssetDir%

REM Copy RawAssets\Textures\ to %deployDir\RuntimeAssets\Textures\
if not exist %outputAssetDir%\Textures\ mkdir %outputAssetDir%\Textures\
copy %rawAssetDir%Textures\ %outputAssetDir%Textures\ /y /d

REM Copy RawAssets\Models\ to %deployDir\RuntimeAssets\Models\

REM Compile all .vert and .frag files to .spv files then copy them to %deployDir\RuntimeAssets\CompiledShaders\






REM *******************
REM OLD

REM @echo on
REM echo %1

REM set projectPath=%~dp0..
REM set shaderPath=%projectPath%\assets\shaders
REM 
REM glslc %shaderPath%\tri.vert -o %shaderPath%\tri-vert.spv
REM glslc %shaderPath%\tri.frag -o %shaderPath%\tri-frag.spv
REM glslc %shaderPath%\simple-color.vert -o %shaderPath%\simple-color-vert.spv
REM glslc %shaderPath%\simple-color.frag -o %shaderPath%\simple-color-frag.spv
REM glslc %shaderPath%\pass-through.vert -o %shaderPath%\pass-through-vert.spv
REM glslc %shaderPath%\post-process.frag -o %shaderPath%\post-process-frag.spv
