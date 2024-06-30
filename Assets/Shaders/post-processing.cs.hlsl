// need a storage image for input
RWTexture2D<float4> g_mainDrawResult : register(u0, space0);

// need a storage image for output
RWTexture2D<float4> g_postProcessingResult : register(u1, space0);

[numthreads(8, 8, 1)]
void Main(uint3 dispatchThreadId : SV_DispatchThreadID)
{
    uint2 xyPos = dispatchThreadId.xy;
    g_postProcessingResult[xyPos] = g_mainDrawResult[xyPos] * float4(0.0f, 1.0f, 0.0f, 1.0f);
}