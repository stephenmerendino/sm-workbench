@echo off

set projectPath=%~dp0..
set shaderPath=%projectPath%\assets\shaders

glslc %shaderPath%\tri.vert -o %shaderPath%\tri-vert.spv
glslc %shaderPath%\tri.frag -o %shaderPath%\tri-frag.spv
glslc %shaderPath%\simple-color.vert -o %shaderPath%\simple-color-vert.spv
glslc %shaderPath%\simple-color.frag -o %shaderPath%\simple-color-frag.spv
