#version 450
#extension GL_ARB_separate_shader_objects : enable

// global descriptors
layout(set = 0, binding = 0) uniform sampler linearSampler2d; 

// frame descriptors
layout(set = 1, binding = 0, row_major) uniform FrameRenderData 
{
    float m_elapsedTimeSeconds;
	float m_deltaTimeSeconds;
} frameRenderData;

// post process descriptors
layout(set = 2, binding = 0) uniform texture2D sceneColor;
layout(set = 2, binding = 1) uniform texture2D sceneDepth;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragUv;

layout(location = 0) out vec4 outColor;

float remap(float val, float oldMin, float oldMax, float newMin, float newMax)
{
    return (((val - oldMin) / (oldMax - oldMin)) * (newMax - newMin)) + newMin;
}

void main()
{
    vec4 color = texture(sampler2D(sceneColor, linearSampler2d), fragUv) * fragColor;
	outColor = color; 

    //vec4 color = texture(sampler2D(scene_depth, linear_sampler_2d), frag_uv) * frag_color;
    //color *= vec4(0.0f, 1.0f, 0.0f, 1.0f);
    //float depth_remapped = remap(color.r, 0.95f, 1.0f, 0.0f, 1.0f);
    //depth_remapped = clamp(depth_remapped, 0.0f, 1.0f);
	//out_color = vec4(depth_remapped, depth_remapped, depth_remapped, 1.0f); 
}
