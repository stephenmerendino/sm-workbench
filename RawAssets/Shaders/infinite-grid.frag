#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 worldPosNear;
layout(location = 1) in vec4 worldPosFar;

layout(location = 0) out vec4 outColor;

float g_lineWholeNumberThickness = 0.025f;
float g_lineWholeNumberFullColor = 0.01f;

float g_fractionLines = 0.20f;
float g_lineFractionalNumberThickness = 0.01f;
float g_lineFractionalNumberFullColor = 0.05f;

float g_lineWholeNumberColor = 0.35f;
float g_lineFractionalNumberColor = 0.25f;

bool IsCloseEnough(float value)
{
	if(mod(value, 1.0f) <= g_lineWholeNumberThickness) return true;
	//if(mod(value, g_fractionLines) <= g_lineFractionalNumberThickness) return true;
	return false;
}

vec4 GetLineColor(vec3 intersectPos)
{
	float wholeNumberLineX = mod(intersectPos.x, 1.0f);
	float wholeNumberLineY = mod(intersectPos.y, 1.0f);

	float wholeNumberLineValue = min(wholeNumberLineX, wholeNumberLineY);
	if(wholeNumberLineValue < g_lineWholeNumberThickness)
	{
        //float fadeout = clamp(1.0f - abs((wholeNumberLineValue - g_lineWholeNumberFullColor) / (g_lineWholeNumberThickness - g_lineWholeNumberFullColor)), 0.0f, 1.0f);
		vec4 lineColor = vec4(g_lineWholeNumberColor, g_lineWholeNumberColor, g_lineWholeNumberColor, 1.0f);
		//lineColor.xyz *= fadeout;

		if(intersectPos.x > 0 && intersectPos.y >= 0 && intersectPos.x < g_lineWholeNumberThickness)
		{
            lineColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		if(intersectPos.x > 0 && intersectPos.y >= 0 && intersectPos.y < g_lineWholeNumberThickness)
		{
            lineColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
		}

		return lineColor;
	}

	//float fractionalNumberLineX = mod(intersectPos.x, g_fractionLines);
	//float fractionalNumberLineY = mod(intersectPos.y, g_fractionLines);

	//float fractionalNumberLineValue = min(fractionalNumberLineX, fractionalNumberLineY);
	//if(fractionalNumberLineValue < g_lineFractionalNumberThickness)
	//{
    //    return vec4(g_lineFractionalNumberColor, g_lineFractionalNumberColor, g_lineFractionalNumberColor, 1.0f);
	//}

	return vec4(1.0f, 0.0f, 1.0f, 1.0f);
}

void main()
{
    float t = -worldPosNear.z / (worldPosFar.z - worldPosNear.z);
	if(t > 0)
	{
        vec3 intersectPos = worldPosNear.xyz + (t * (worldPosFar.xyz - worldPosNear.xyz));
		if(!IsCloseEnough(intersectPos.x) && !IsCloseEnough(intersectPos.y))
		{
            discard;
		}

        outColor = GetLineColor(intersectPos);
	}
	else
	{
        discard;
	}
}
