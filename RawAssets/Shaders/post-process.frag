#version 450
#extension GL_ARB_separate_shader_objects : enable

// global descriptors
layout(set = 0, binding = 0) uniform sampler linear_sampler_2d; 

// frame descriptors
layout(set = 1, binding = 0, row_major) uniform FrameRenderData 
{
    float time;
	float delta_time_seconds;
} frame_render_data;

// post process descriptors
layout(set = 2, binding = 0) uniform texture2D scene_color;
layout(set = 2, binding = 1) uniform texture2D scene_depth;

layout(location = 0) in vec4 frag_color;
layout(location = 1) in vec2 frag_uv;

layout(location = 0) out vec4 out_color;

float remap(float val, float old_min, float old_max, float new_min, float new_max)
{
    return (((val - old_min) / (old_max - old_min)) * (new_max - new_min)) + new_min;
}

void main()
{
    vec4 color = texture(sampler2D(scene_color, linear_sampler_2d), frag_uv) * frag_color;
	out_color = color; 

    //vec4 color = texture(sampler2D(scene_depth, linear_sampler_2d), frag_uv) * frag_color;
    //color *= vec4(0.0f, 1.0f, 0.0f, 1.0f);
    //float depth_remapped = remap(color.r, 0.95f, 1.0f, 0.0f, 1.0f);
    //depth_remapped = clamp(depth_remapped, 0.0f, 1.0f);
	//out_color = vec4(depth_remapped, depth_remapped, depth_remapped, 1.0f); 
}
