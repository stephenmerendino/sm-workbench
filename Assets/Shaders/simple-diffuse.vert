#pragma pack_matrix(row_major)

struct VsInput
{
	[[vk::location(0)]] float3 m_pos : POSITION0;
	[[vk::location(1)]] float4 m_color : COLOR0;
	[[vk::location(2)]] float2 m_uv : UV0;
};

struct VsOutput
{
	float4 m_pos : SV_POSITION;
	[[vk::location(0)]] float4 m_color : COLOR0;
	[[vk::location(1)]] float2 m_uv : UV0;
};

struct FrameRenderData
{
	float m_elapsedTimeSeconds;
	float m_deltaTimeSeconds;
};

struct MeshInstanceRenderData
{
	float4x4 m_mvp;
};

ConstantBuffer<FrameRenderData> g_frameRenderData : register(b0, space1);
ConstantBuffer<MeshInstanceRenderData> g_meshInstanceRenderData : register(b0, space3);

VsOutput Main(VsInput input)
{
	VsOutput vertOutput;
	vertOutput.m_pos = mul(float4(input.m_pos, 1.0f), g_meshInstanceRenderData.m_mvp);
	vertOutput.m_color = input.m_color;
	vertOutput.m_uv = input.m_uv;
	return vertOutput;
}

//#version 450
//#extension GL_ARB_separate_shader_objects : enable
//
//// frame descriptors
//layout(set = 1, binding = 0, row_major) uniform FrameRenderData 
//{
//  float m_elapsedTimeSeconds;
//	float m_deltaTimeSeconds;
//} frameRenderData;
//
//// mesh instance descriptors
//layout(set = 3, binding = 0, row_major) uniform MeshInstanceRenderData 
//{
//	mat4 m_mvp;
//} meshInstanceRenderData;
//
//layout(location = 0) in vec3 inPos;
//layout(location = 1) in vec4 inColor;
//layout(location = 2) in vec2 inUv;
//
//layout(location = 0) out vec4 fragColor;
//layout(location = 1) out vec2 fragUv;
//
//void main()
//{
//	gl_Position = vec4(inPos, 1.0f) * meshInstanceRenderData.m_mvp;
//	fragColor = inColor;
//	fragUv = inUv;
//}
