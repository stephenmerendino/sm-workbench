#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragUv;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = fragColor;
	outColor.xyz = pow(outColor.xyz, vec3(1.0f / 2.2f));
}
