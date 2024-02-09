#version 450
#extension GL_ARB_separate_shader_objects : enable

// frame descriptor
layout(set = 1, binding = 0, row_major) uniform FrameRenderData 
{
    float m_elapsedTimeSeconds;
	float m_deltaTimeSeconds;
} frameRenderData;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inUv;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragUv;

void main()
{
	gl_Position = vec4(inPos, 1.0f);
	fragColor = inColor;
	fragUv = inUv;
}
