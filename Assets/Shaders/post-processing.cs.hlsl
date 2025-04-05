// need a storage image for input
[[vk::binding(0, 0)]]
RWTexture2D<float4> g_main_draw_result : register(u0, space0);

// need a storage image for output
[[vk::binding(1, 0)]]
RWTexture2D<float4> g_post_processing_result : register(u1, space0);

[numthreads(8, 8, 1)]
void main(uint3 dispatch_thread_id : SV_DispatchThreadID)
{
    uint2 xy_pos = dispatch_thread_id.xy;
    g_post_processing_result[xy_pos] = g_main_draw_result[xy_pos] * float4(1.0f, 1.0f, 1.0f, 1.0f);
}