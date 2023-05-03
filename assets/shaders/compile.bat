@echo off

pushd Shaders

glslc tri.vert -o tri-vert.spv
glslc tri.frag -o tri-frag.spv
glslc simple-color.vert -o simple-color-vert.spv
glslc simple-color.frag -o simple-color-frag.spv

IF NOT EXIST "..\..\Run_x64\" (
	mkdir "..\..\Run_x64"
)

IF NOT EXIST "..\..\Run_x64\Shaders\" (
	mkdir "..\..\Run_x64\Shaders\"
)

xcopy *.spv "..\..\Run_x64\Shaders\" /y

IF NOT EXIST "..\..\Run_Win32\" (
	mkdir "..\..\Run_Win32"
)

IF NOT EXIST "..\..\Run_Win32\Shaders\" (
	mkdir "..\..\Run_Win32\Shaders\"
)

xcopy *.spv "..\..\Run_Win32\Shaders\" /y