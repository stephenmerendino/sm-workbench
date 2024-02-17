#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 worldPosNear;
layout(location = 1) in vec4 worldPosFar;

layout(location = 0) out vec4 outColor;

bool IsCloseEnough(float value)
{
    return value < 0.01f && value > -0.01f;
}

void main()
{
    float t = -worldPosNear.z / (worldPosFar.z - worldPosNear.z);
	if(t > 0)
	{
        vec3 intersectPos = worldPosNear.xyz + (t * (worldPosFar.xyz - worldPosNear.xyz));
		if(IsCloseEnough(intersectPos.x) || IsCloseEnough(intersectPos.y))
		{
            outColor = vec4(0.35f, 0.35f, 0.35f, 1.0f);
		}
		else
		{
            discard;
		}
	}
	else
	{
        discard;
	}
}
