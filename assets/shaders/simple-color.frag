#version 450
#extension GL_ARB_separate_shader_objects : enable

// globals descriptors
layout(set = 0, binding = 0) uniform sampler linear_sampler_2d; 

// material descriptors
layout(set = 2, binding = 0) uniform texture2D diffuse;

layout(location = 0) in vec4 frag_color;
layout(location = 1) in vec2 frag_uv;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = frag_color;
}
