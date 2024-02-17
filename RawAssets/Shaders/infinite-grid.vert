#version 450
#extension GL_ARB_separate_shader_objects : enable

vec3 g_ndcPositions[6] = 
{ 
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f)
};

void main()
{
	gl_Position = vec4(g_ndcPositions[gl_VertexIndex], 1.0f);
}
