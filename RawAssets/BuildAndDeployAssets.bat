@echo off

set rawAssetDir=%1
set deployDir=%2
set outputAssetDir=%deployDir%\RuntimeAssets\

REM If %deployDir\RuntimeAssets\ does not exist, then create it
if not exist %outputAssetDir% mkdir %outputAssetDir%

REM Copy RawAssets\Textures\ to %deployDir\RuntimeAssets\Textures\
echo Deploying textures to %outputAssetDir%Textures\
if not exist %outputAssetDir%\Textures\ mkdir %outputAssetDir%\Textures\
xcopy %rawAssetDir%Textures\ %outputAssetDir%Textures\ /s /d

REM Copy RawAssets\Models\ to %deployDir\RuntimeAssets\Models\
echo Deploying models to %outputAssetDir%Models\
if not exist %outputAssetDir%\Models\ mkdir %outputAssetDir%\Models\
xcopy %rawAssetDir%Models\ %outputAssetDir%Models\ /s /d

REM Compile all .vert and .frag files to .spv files then copy them to %deployDir\RuntimeAssets\CompiledShaders\
echo Compiling shaders
if not exist %outputAssetDir%\CompiledShaders\ mkdir %outputAssetDir%\CompiledShaders\
pushd %rawAssetDir%Shaders\
for /r %%f in (*) do (
	echo     Compiling %%~nf%%~xf
	glslc %%f -o %outputAssetDir%CompiledShaders\%%~nf%%~xf.spv
)
popd
