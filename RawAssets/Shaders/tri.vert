#version 450
#extension GL_ARB_separate_shader_objects : enable

// frame descriptors
layout(set = 1, binding = 0, row_major) uniform FrameRenderData 
{
    float time;
	float delta_time_seconds;
} frame_render_data;

// mesh instance descriptors
layout(set = 3, binding = 0, row_major) uniform MeshInstanceRenderData 
{
	mat4 mvp;
} mesh_instance_render_data;

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_uv;

layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec2 frag_uv;

void main()
{
	gl_Position = vec4(in_pos, 1.0f) * mesh_instance_render_data.mvp;
	frag_color = in_color;
	frag_uv = in_uv;
}
