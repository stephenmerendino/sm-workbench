@echo on

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

C:\VulkanSDK\1.3.268.0\Bin\dxc -spirv -T vs_6_6 -E Main .\infinite-grid.vert -Fo %outputAssetDir%CompiledShaders\infinite-grid.vert.spv -Zi
C:\VulkanSDK\1.3.268.0\Bin\dxc -spirv -T ps_6_6 -E Main .\infinite-grid.frag -Fo %outputAssetDir%CompiledShaders\infinite-grid.frag.spv -Zi
C:\VulkanSDK\1.3.268.0\Bin\dxc -spirv -T vs_6_6 -E Main .\simple-diffuse.vert -Fo %outputAssetDir%CompiledShaders\simple-diffuse.vert.spv -Zi
C:\VulkanSDK\1.3.268.0\Bin\dxc -spirv -T ps_6_6 -E Main .\simple-diffuse.frag -Fo %outputAssetDir%CompiledShaders\simple-diffuse.frag.spv -Zi

popd
