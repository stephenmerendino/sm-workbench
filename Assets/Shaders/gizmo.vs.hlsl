#pragma pack_matrix(row_major)

struct vs_input_t
{
	[[vk::location(0)]] float3 pos : POSITION0;
	[[vk::location(1)]] float2 uv : UV0;
	[[vk::location(2)]] float3 color : COLOR0;
};

struct vs_output_t
{
	float4 pos : SV_POSITION;
	[[vk::location(0)]] float2 uv : UV0;
	[[vk::location(1)]] float3 color : COLOR0;
};

struct mesh_instance_render_data_t 
{
	float4x4 mvp;
};

[[vk::binding(0, 0)]]
ConstantBuffer<mesh_instance_render_data_t> g_mesh_instance_render_data : register(b0, space0);

vs_output_t main(vs_input_t input)
{
	vs_output_t vert_output;
	vert_output.pos = mul(float4(input.pos, 1.0f), g_mesh_instance_render_data.mvp);
	vert_output.color = input.color;
	vert_output.uv = input.uv;
	return vert_output;
}
