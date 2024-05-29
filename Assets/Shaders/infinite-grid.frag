#pragma pack_matrix(row_major)

struct InfiniteGridData
{
	float4x4 m_viewProjection;
	float4x4 m_inverseViewProjection;
    float m_depthFadeDistance;
    float m_majorLineThickness;
    float m_minorLineThickness;
};

ConstantBuffer<InfiniteGridData> g_infiniteGridData : register(b0, space0);

struct PSInput
{
	float4 m_worldPosNear : COLOR0;
	float4 m_worldPosFar : COLOR1;
};

struct PSOutput
{
	float4 m_color : SV_Target0;
    float m_depth : SV_Depth0;
};

static float g_bigLineFrequency = 1.0f;
static float g_bigLineFalloffStart = 0.00f;
static float g_bigLineColor = 0.35f;
static float g_smallLineFrequency = 0.2f;
static float g_smallLineFalloffStart = 0.00f;
static float g_smallLineColor = 0.25f;

bool IsWithinRangeOfMod(float testValue, float modValue, float range)
{
	float remainder = abs(fmod(testValue, modValue));
	return remainder <= range || (modValue - remainder) <= range;
}

float GetFadeout(float3 position, float gridModValue, float lineThickness, float falloffStart)
{
	float xRemainder = abs(fmod(position.x, gridModValue));
	float yRemainder = abs(fmod(position.y, gridModValue));

    xRemainder = min(xRemainder, gridModValue - xRemainder);
    yRemainder = min(yRemainder, gridModValue - yRemainder);

    if (xRemainder <= falloffStart || yRemainder <= falloffStart)
    {
        return 1.0f;
    }

    float smallestRemainder = min(xRemainder, yRemainder);

    float fadeout = 1.0f;
    fadeout = (smallestRemainder - falloffStart) / (lineThickness - falloffStart);
    fadeout *= fadeout;
    return 1.0f - fadeout;
}

bool IsMainAxis(float coordinateValue)
{
	return (coordinateValue <= g_infiniteGridData.m_majorLineThickness) && (coordinateValue >= -g_infiniteGridData.m_majorLineThickness);
}

bool IsBigLine(float3 pos)
{
	if(IsWithinRangeOfMod(pos.x, g_bigLineFrequency, g_infiniteGridData.m_majorLineThickness)) return true;
	if(IsWithinRangeOfMod(pos.y, g_bigLineFrequency, g_infiniteGridData.m_majorLineThickness)) return true;
	return false;
}

bool IsSmallLine(float3 pos)
{
	if(IsWithinRangeOfMod(pos.x, g_smallLineFrequency, g_infiniteGridData.m_minorLineThickness)) return true;
	if(IsWithinRangeOfMod(pos.y, g_smallLineFrequency, g_infiniteGridData.m_minorLineThickness)) return true;
	return false;
}

bool IsPositionOnGrid(float3 pos)
{
    return IsBigLine(pos) || IsSmallLine(pos);
}

bool GetLineColor(float3 intersectPos, out float4 color)
{
/*/
    float scale = 1.0f;
    float2 coord = intersectPos.xy * scale; // use the scale variable to set the distance between the lines

    // sum of absolute values of ddx and ddy of world space position
    // how much is world position changing in x and y coordinates from pixel to pixel
    float2 derivative = fwidth(coord);

    // 
    float2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;

    float line = min(grid.x, grid.y);

    float minimumy = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);

    color = float4(g_bigLineColor, g_bigLineColor, g_bigLineColor, 1.0 - min(line, 1.0));
    // z axis
    if (intersectPos.x > -0.1 * minimumx && intersectPos.x < 0.1 * minimumx)
        color.z = 1.0;
    // x axis
    if (intersectPos.y > -0.1 * minimumy && intersectPos.y < 0.1 * minimumy)
        color.g = 1.0;
    return true;
/*/
    if(!IsPositionOnGrid(intersectPos))
    {
        return false;
    }

	if(IsMainAxis(intersectPos.x) && intersectPos.y >= g_infiniteGridData.m_majorLineThickness)
	{
        float fadeout = GetFadeout(intersectPos, g_bigLineFrequency, g_infiniteGridData.m_majorLineThickness, g_bigLineFalloffStart);
		color = float4(0.0f, 1.0f, 0.0f, 1.0f);
        color.a = fadeout;
		return true;
	}

	if(IsMainAxis(intersectPos.y) && intersectPos.x >= g_infiniteGridData.m_majorLineThickness)
	{
        float fadeout = GetFadeout(intersectPos, g_bigLineFrequency, g_infiniteGridData.m_majorLineThickness, g_bigLineFalloffStart);
		color = float4(1.0f, 0.0f, 0.0f, 1.0f);
        color.a = fadeout;
		return true;
	}

	if(IsBigLine(intersectPos))
	{
        float fadeout = GetFadeout(intersectPos, g_bigLineFrequency, g_infiniteGridData.m_majorLineThickness, g_bigLineFalloffStart);
        color = float4(g_bigLineColor, g_bigLineColor, g_bigLineColor, 1.0f);
        color.a = fadeout;
		return true;
	}

	if(IsSmallLine(intersectPos))
	{
        float fadeout = GetFadeout(intersectPos, g_smallLineFrequency, g_infiniteGridData.m_minorLineThickness, g_smallLineFalloffStart);
        color = float4(g_smallLineColor, g_smallLineColor, g_smallLineColor, 1.0f);
        color.a = fadeout;
		return true;
	}

	return true;
/**/
}

PSOutput Main(PSInput IN)
{
    PSOutput OUT;
    //OUT.m_color = float4(1.0f, 0.0f, 0.0f, 1.0f);

    float3 worldPosNear = IN.m_worldPosNear.xyz;
    float3 worldPosFar = IN.m_worldPosFar.xyz;
    float t = (-worldPosNear.z) / (worldPosFar.z - worldPosNear.z);

	if(t > 0)
	{
        float3 intersectPos = worldPosNear.xyz + (t * (worldPosFar.xyz - worldPosNear.xyz));
		if(!GetLineColor(intersectPos, OUT.m_color))
		{
            discard;
		}

        float4 homogenousPos = mul(float4(intersectPos, 1.0f), g_infiniteGridData.m_viewProjection);
        float4 ndcPos = homogenousPos / homogenousPos.w;

        OUT.m_color.a *= 1.0f - (max(ndcPos.z - g_infiniteGridData.m_depthFadeDistance, 0.0f) / (1.0f - g_infiniteGridData.m_depthFadeDistance));
		
        OUT.m_depth = ndcPos.z;
    }
	else
	{
        discard;
	}

    return OUT;
}

//layout(set = 0, binding = 0, row_major) uniform InfiniteGridData
//{
//	mat4x4 m_viewProjection;
//	mat4x4 m_inverseViewProjection;
//    float m_depthFadeDistance;
//    float m_majorLineThickness;
//    float m_minorLineThickness;
//} infiniteGridData;
//
//layout(location = 0) in vec4 worldPosNear;
//layout(location = 1) in vec4 worldPosFar;
//
//layout(location = 0) out vec4 outColor;
//
//float g_bigLineFrequency = 1.0f;
////float g_bigLineThickness = 0.0125f;
//float g_bigLineFalloffStart = 0.00f;
//float g_bigLineColor = 0.35f;
//
//float g_smallLineFrequency = 0.2f;
////float g_smallLineThickness = 0.0075f;
//float g_smallLineFalloffStart = 0.00f;
//float g_smallLineColor = 0.25f;
//
////float g_depthFadeDistance = 0.9975f;
//
//bool IsWithinRangeOfMod(float testValue, float modValue, float range)
//{
//	float remainder = abs(mod(testValue, modValue));
//	return remainder <= range || (modValue - remainder) <= range;
//}
//
//float GetFadeout(vec3 position, float gridModValue, float lineThickness, float falloffStart)
//{
//	float xRemainder = abs(mod(position.x, gridModValue));
//	float yRemainder = abs(mod(position.y, gridModValue));
//    float4x4 m_viewProjection;
//    float4x4 m_inverseViewProjection;
//
//    xRemainder = min(xRemainder, gridModValue - xRemainder);
//    yRemainder = min(yRemainder, gridModValue - yRemainder);
//
//    if (xRemainder <= falloffStart || yRemainder <= falloffStart)
//    {
//        return 1.0f;
//    }
//
//    float smallestRemainder = min(xRemainder, yRemainder);
//
//    float fadeout = 1.0f;
//    fadeout = (smallestRemainder - falloffStart) / (lineThickness - falloffStart);
//    fadeout *= fadeout;
//    return 1.0f - fadeout;
//}
//
//bool IsMainAxis(float coordinateValue)
//{
//	return (coordinateValue <= infiniteGridData.m_majorLineThickness) && (coordinateValue >= -infiniteGridData.m_majorLineThickness);
//}
//
//bool IsBigLine(vec3 pos)
//{
//	if(IsWithinRangeOfMod(pos.x, g_bigLineFrequency, infiniteGridData.m_majorLineThickness)) return true;
//	if(IsWithinRangeOfMod(pos.y, g_bigLineFrequency, infiniteGridData.m_majorLineThickness)) return true;
//	return false;
//}
//
//bool IsSmallLine(vec3 pos)
//{
//	if(IsWithinRangeOfMod(pos.x, g_smallLineFrequency, infiniteGridData.m_minorLineThickness)) return true;
//	if(IsWithinRangeOfMod(pos.y, g_smallLineFrequency, infiniteGridData.m_minorLineThickness)) return true;
//	return false;
//}
//
//bool IsPositionOnGrid(vec3 pos)
//{
//    return IsBigLine(pos) || IsSmallLine(pos);
//}
//
//bool GetLineColor(vec3 intersectPos, out vec4 color)
//{
///*/
//    float scale = 1.0f;
//    vec2 coord = intersectPos.xy * scale; // use the scale variable to set the distance between the lines
//
//    // sum of absolute values of ddx and ddy of world space position
//    // how much is world position changing in x and y coordinates from pixel to pixel
//    vec2 derivative = fwidth(coord);
//
//    // 
//    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
//
//    float line = min(grid.x, grid.y);
//
//    float minimumy = min(derivative.y, 1);
//    float minimumx = min(derivative.x, 1);
//
//    color = vec4(g_bigLineColor, g_bigLineColor, g_bigLineColor, 1.0 - min(line, 1.0));
//    // z axis
//    if (intersectPos.x > -0.1 * minimumx && intersectPos.x < 0.1 * minimumx)
//        color.z = 1.0;
//    // x axis
//    if (intersectPos.y > -0.1 * minimumy && intersectPos.y < 0.1 * minimumy)
//        color.g = 1.0;
//    return true;
///*/
//    if(!IsPositionOnGrid(intersectPos))
//    {
//        return false;
//    }
//
//	color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
//
//	if(IsMainAxis(intersectPos.x) && intersectPos.y >= infiniteGridData.m_majorLineThickness)
//	{
//        float fadeout = GetFadeout(intersectPos, g_bigLineFrequency, infiniteGridData.m_majorLineThickness, g_bigLineFalloffStart);
//		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
//        color.a = fadeout;
//		return true;
//	}
//
//	if(IsMainAxis(intersectPos.y) && intersectPos.x >= infiniteGridData.m_majorLineThickness)
//	{
//        float fadeout = GetFadeout(intersectPos, g_bigLineFrequency, infiniteGridData.m_majorLineThickness, g_bigLineFalloffStart);
//		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
//        color.a = fadeout;
//		return true;
//	}
//
//	if(IsBigLine(intersectPos))
//	{
//        float fadeout = GetFadeout(intersectPos, g_bigLineFrequency, infiniteGridData.m_majorLineThickness, g_bigLineFalloffStart);
//        color = vec4(g_bigLineColor, g_bigLineColor, g_bigLineColor, 1.0f);
//        color.a = fadeout;
//		return true;
//	}
//
//	if(IsSmallLine(intersectPos))
//	{
//        float fadeout = GetFadeout(intersectPos, g_smallLineFrequency, infiniteGridData.m_minorLineThickness, g_smallLineFalloffStart);
//        color = vec4(g_smallLineColor, g_smallLineColor, g_smallLineColor, 1.0f);
//        color.a = fadeout;
//		return true;
//	}
//
//	return true;
///**/
//}
//
//void main()
//{
//    float t = -worldPosNear.z / (worldPosFar.z - worldPosNear.z);
//	if(t > 0)
//	{
//        vec3 intersectPos = worldPosNear.xyz + (t * (worldPosFar.xyz - worldPosNear.xyz));
//		if(!GetLineColor(intersectPos, outColor))
//		{
//            discard;
//		}
//
//        vec4 homogenousPos = vec4(intersectPos, 1.0f) * infiniteGridData.m_viewProjection;
//        vec4 ndcPos = homogenousPos / homogenousPos.w;
//
//        outColor.a *= 1.0f - (max(ndcPos.z - infiniteGridData.m_depthFadeDistance, 0.0f) / (1.0f - infiniteGridData.m_depthFadeDistance));
//		
//        gl_FragDepth = ndcPos.z;
//    }
//	else
//	{
//        discard;
//	}
//}
