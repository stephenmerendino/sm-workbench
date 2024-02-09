#version 450
#extension GL_ARB_separate_shader_objects : enable

// mesh instance descriptors
layout(set = 3, binding = 0, row_major) uniform MeshInstanceRenderData 
{
	mat4 m_mvp;
} meshInstanceRenderData;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inUv;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragUv;

void main()
{
	gl_Position = vec4(inPos, 1.0f) * meshInstanceRenderData.m_mvp;
	fragColor = inColor;
	fragUv = inUv;
}
