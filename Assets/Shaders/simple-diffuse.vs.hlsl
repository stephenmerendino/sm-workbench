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
