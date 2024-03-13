#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0, row_major) uniform InfiniteGridData
{
	mat4x4 m_viewProjection;
	mat4x4 m_inverseViewProjection;
} infiniteGridData;

layout(location = 0) in vec4 worldPosNear;
layout(location = 1) in vec4 worldPosFar;

layout(location = 0) out vec4 outColor;

float g_bigLineFrequency = 1.0f;
float g_bigLineThickness = 0.0125f;
float g_bigLineFalloffStart = 0.00f;
float g_bigLineColor = 0.35f;

float g_smallLineFrequency = 0.2f;
float g_smallLineThickness = 0.0075f;
float g_smallLineFalloffStart = 0.00f;
float g_smallLineColor = 0.25f;

float g_depthFadeDistance = 0.9975f;

bool IsWithinRangeOfMod(float testValue, float modValue, float range)
{
	float remainder = abs(mod(testValue, modValue));
	return remainder <= range || (modValue - remainder) <= range;
}

float GetFadeout(vec3 position, float gridModValue, float lineThickness, float falloffStart)
{
	float xRemainder = abs(mod(position.x, gridModValue));
	float yRemainder = abs(mod(position.y, gridModValue));

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
	return (coordinateValue <= g_bigLineThickness) && (coordinateValue >= -g_bigLineThickness);
}

bool IsBigLine(vec3 pos)
{
	if(IsWithinRangeOfMod(pos.x, g_bigLineFrequency, g_bigLineThickness)) return true;
	if(IsWithinRangeOfMod(pos.y, g_bigLineFrequency, g_bigLineThickness)) return true;
	return false;
}

bool IsSmallLine(vec3 pos)
{
	if(IsWithinRangeOfMod(pos.x, g_smallLineFrequency, g_smallLineThickness)) return true;
	if(IsWithinRangeOfMod(pos.y, g_smallLineFrequency, g_smallLineThickness)) return true;
	return false;
}

bool IsPositionOnGrid(vec3 pos)
{
    return IsBigLine(pos) || IsSmallLine(pos);
}

bool GetLineColor(vec3 intersectPos, out vec4 color)
{
/*/
    float scale = 1.0f;
    vec2 coord = intersectPos.xy * scale; // use the scale variable to set the distance between the lines

    // sum of absolute values of ddx and ddy of world space position
    // how much is world position changing in x and y coordinates from pixel to pixel
    vec2 derivative = fwidth(coord);

    // 
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;

    float line = min(grid.x, grid.y);

    float minimumy = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);

    color = vec4(g_bigLineColor, g_bigLineColor, g_bigLineColor, 1.0 - min(line, 1.0));
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

	color = vec4(1.0f, 0.0f, 1.0f, 1.0f);

	if(IsMainAxis(intersectPos.x) && intersectPos.y >= g_bigLineThickness)
	{
        float fadeout = GetFadeout(intersectPos, g_bigLineFrequency, g_bigLineThickness, g_bigLineFalloffStart);
		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
        color.a = fadeout;
		return true;
	}

	if(IsMainAxis(intersectPos.y) && intersectPos.x >= g_bigLineThickness)
	{
        float fadeout = GetFadeout(intersectPos, g_bigLineFrequency, g_bigLineThickness, g_bigLineFalloffStart);
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
        color.a = fadeout;
		return true;
	}

	if(IsBigLine(intersectPos))
	{
        float fadeout = GetFadeout(intersectPos, g_bigLineFrequency, g_bigLineThickness, g_bigLineFalloffStart);
        color = vec4(g_bigLineColor, g_bigLineColor, g_bigLineColor, 1.0f);
        color.a = fadeout;
		return true;
	}

	if(IsSmallLine(intersectPos))
	{
        float fadeout = GetFadeout(intersectPos, g_smallLineFrequency, g_smallLineThickness, g_smallLineFalloffStart);
        color = vec4(g_smallLineColor, g_smallLineColor, g_smallLineColor, 1.0f);
        color.a = fadeout;
		return true;
	}

	return true;
/**/
}

void main()
{
    float t = -worldPosNear.z / (worldPosFar.z - worldPosNear.z);
	if(t > 0)
	{
        vec3 intersectPos = worldPosNear.xyz + (t * (worldPosFar.xyz - worldPosNear.xyz));
		if(!GetLineColor(intersectPos, outColor))
		{
            discard;
		}

        vec4 homogenousPos = vec4(intersectPos, 1.0f) * infiniteGridData.m_viewProjection;
        vec4 ndcPos = homogenousPos / homogenousPos.w;

        outColor.a *= 1.0f - (max(ndcPos.z - g_depthFadeDistance, 0.0f) / (1.0f - g_depthFadeDistance));
		
        gl_FragDepth = ndcPos.z;
    }
	else
	{
        discard;
	}
}
