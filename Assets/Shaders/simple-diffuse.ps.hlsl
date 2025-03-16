#pragma pack_matrix(row_major)

SamplerState g_linearSampler2D : register(s0, space0);
Texture2D<float4> g_diffuse : register(t0, space2);

struct PsInput
{
	[[vk::location(0)]] float2 m_uv : UV0;
	[[vk::location(1)]] float3 m_color : COLOR0;
};

struct PSOutput
{
	float4 m_color : SV_TARGET0;
};

PSOutput Main(PsInput IN)
{
    PSOutput OUT;
    float4 diffuseColor = g_diffuse.Sample(g_linearSampler2D, IN.m_uv);
	OUT.m_color = diffuseColor * float4(IN.m_color, 1.0f);
	OUT.m_color.xyz = pow(OUT.m_color.xyz, 1.0f / 2.2f);
	return OUT;
}
