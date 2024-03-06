#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0, row_major) uniform InfiniteGridData
{
	mat4x4 m_viewProjection;
	mat4x4 m_inverseViewProjection;
} infiniteGridData;

vec4 g_ndcPositions[6] = 
{ 
	vec4(-1.0f, -1.0f, 0.5f, 1.0f),
	vec4(-1.0f, 1.0f, 0.5f, 1.0f),
	vec4(1.0f, 1.0f, 0.5f, 1.0f),
	vec4(-1.0f, -1.0f, 0.5f, 1.0f),
	vec4(1.0f, 1.0f, 0.5f, 1.0f),
	vec4(1.0f, -1.0f, 0.5f, 1.0f)
};

layout(location = 0) out vec4 worldPosNear;
layout(location = 1) out vec4 worldPosFar;

void main()
{
    vec4 ndcPos = g_ndcPositions[gl_VertexIndex];
	
	worldPosNear = vec4(ndcPos.x, ndcPos.y, 0.0f, 1.0f) * infiniteGridData.m_inverseViewProjection;
	worldPosNear.xyz /= worldPosNear.w;

	worldPosFar = vec4(ndcPos.x, ndcPos.y, 1.0f, 1.0f) * infiniteGridData.m_inverseViewProjection;
	worldPosFar.xyz /= worldPosFar.w;

	gl_Position = ndcPos;
}
