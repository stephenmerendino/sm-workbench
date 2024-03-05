#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 worldPosNear;
layout(location = 1) in vec4 worldPosFar;

layout(location = 0) out vec4 outColor;

float g_lineWholeNumberThickness = 0.025f;
float g_lineWholeNumberFullColor = 0.01f;

float g_fractionLines = 0.20f;
float g_lineFractionalNumberThickness = 0.01f;
float g_lineFractionalNumberFullColor = 0.005f;

float g_lineWholeNumberColor = 0.35f;
float g_lineFractionalNumberColor = 0.25f;

float GetDistanceFromThickLine(vec3 pos)
{
	float xDist = min(fract(pos.x), 1.0f - fract(pos.x)); 
	float yDist = min(fract(pos.y), 1.0f - fract(pos.y)); 
	return min(xDist, yDist);
}

float GetDistanceFromThinLine(vec3 pos)
{
    return 0.0f;
}

bool IsMainAxis(float coordinateValue)
{
	return (coordinateValue <= g_lineWholeNumberThickness) && (coordinateValue >= -g_lineWholeNumberThickness);
}

bool IsThickLine(vec3 pos)
{
	if( fract(pos.x) <= g_lineWholeNumberThickness || 1.0f - fract(pos.x) <= g_lineWholeNumberThickness) return true;
	if( fract(pos.y) <= g_lineWholeNumberThickness || 1.0f - fract(pos.y) <= g_lineWholeNumberThickness) return true;
	return false;
}

bool IsWithinRangeOfMod(float testValue, float modValue, float range)
{
	float remainder = abs(modf(testValue, modValue));
	return (remainder <= range) && ((remainder - modValue) >= range);
}

bool IsThinLine(vec3 pos)
{
	if(IsWithinRangeOfMod(pos.x, g_fractionLines, g_lineFractionalNumberThickness)) return true;
	if(IsWithinRangeOfMod(pos.y, g_fractionLines, g_lineFractionalNumberThickness)) return true;
	return false;
}

bool IsPositionOnGrid(vec3 pos)
{
	if( fract(pos.x) <= g_lineWholeNumberThickness || 1.0f - fract(pos.x) <= g_lineWholeNumberThickness) return true;
	if( fract(pos.y) <= g_lineWholeNumberThickness || 1.0f - fract(pos.y) <= g_lineWholeNumberThickness) return true;
	if(IsThinLine(pos)) return true;
	//if( fract(pos.x) <= g_lineFractionalNumberThickness || 1.0f - fract(pos.x) <= g_lineFractionalNumberThickness) return true;
	//if( fract(pos.y) <= g_lineFractionalNumberThickness || 1.0f - fract(pos.y) <= g_lineFractionalNumberThickness) return true;
	return false;
}

bool GetLineColor(vec3 intersectPos, out vec4 color)
{
    if(!IsPositionOnGrid(intersectPos))
    {
        return false;
    }

	color = vec4(1.0f, 0.0f, 1.0f, 1.0f);

	if(IsMainAxis(intersectPos.x) && intersectPos.y >= g_lineWholeNumberThickness)
	{
		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
		return true;
	}

	if(IsMainAxis(intersectPos.y) && intersectPos.x >= g_lineWholeNumberThickness)
	{
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		return true;
	}

	if(IsThickLine(intersectPos))
	{
        color = vec4(g_lineWholeNumberColor, g_lineWholeNumberColor, g_lineWholeNumberColor, 1.0f);
		return true;
	}

	if(IsThinLine(intersectPos))
	{
        //color = vec4(g_lineFractionalNumberColor, g_lineFractionalNumberColor, g_lineFractionalNumberColor, 1.0f);
        color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
		return true;
	}

	return true;
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
	}
	else
	{
        discard;
	}
}
