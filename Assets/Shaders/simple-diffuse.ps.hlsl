#pragma pack_matrix(row_major)

SamplerState g_linearSampler2D : register(s0, space0);
Texture2D<float4> g_diffuse : register(t0, space2);

struct PsInput
{
    float4 m_color : COLOR0;
    float2 m_uv : TEXCOORD0;
};

struct PSOutput
{
	float4 m_color : SV_TARGET0;
};

PSOutput Main(PsInput IN)
{
    PSOutput OUT;
    float4 diffuseColor = g_diffuse.Sample(g_linearSampler2D, IN.m_uv);
	OUT.m_color = diffuseColor * IN.m_color;
	OUT.m_color.xyz = pow(OUT.m_color.xyz, 1.0f / 2.2f);
	return OUT;
}
