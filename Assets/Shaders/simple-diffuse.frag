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

//#version 450
//#extension GL_ARB_separate_shader_objects : enable
//
//// global descriptors
//layout(set = 0, binding = 0) uniform sampler linearSampler2d; 
//
//// material descriptors
//layout(set = 2, binding = 0) uniform texture2D diffuse;
//
//layout(location = 0) in vec4 fragColor;
//layout(location = 1) in vec2 fragUv;
//
//layout(location = 0) out vec4 outColor;
//
//void main()
//{
//    vec4 color = texture(sampler2D(diffuse, linearSampler2d), fragUv) * fragColor;
//	color.xyz = pow(color.xyz, vec3(1.0f / 2.2f));
//	outColor = color; 
//}
