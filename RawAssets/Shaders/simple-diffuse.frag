#version 450
#extension GL_ARB_separate_shader_objects : enable

// global descriptors
layout(set = 0, binding = 0) uniform sampler linearSampler2d; 

// material descriptors
layout(set = 2, binding = 0) uniform texture2D diffuse;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragUv;

layout(location = 0) out vec4 outColor;

void main()
{
    vec4 color = texture(sampler2D(diffuse, linearSampler2d), fragUv) * fragColor;
	color.xyz = pow(color.xyz, vec3(1.0f / 2.2f));
	outColor = color; 
}
