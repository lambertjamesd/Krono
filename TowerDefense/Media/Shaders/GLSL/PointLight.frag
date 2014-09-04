#version 420

layout(binding = 0) uniform SceneViewDataPix
{
	mat4 projectionMatrixPix;
	mat4 projectionInvMatrixPix;
	vec4 screenSizePix;
};

layout(binding = 0) uniform sampler2D color;
layout(binding = 1) uniform sampler2D normal;
layout(binding = 2) uniform sampler2D depth;

layout(location = 0) out vec4 outputColor;

in ScreenPositionVertex {
	vec4 rasterPosition;
};

void main()
{
	vec3 textureCoord = rasterPosition.xyz / rasterPosition.w;
	textureCoord.y = 1 - textureCoord.y;

	float screenZ = texture2D(depth, textureCoord.xy).r * 2 - 1;

	if (screenZ > textureCoord.z)
	{
		discard;
	}
	else
	{
		vec2 xyPos = textureCoord.xy * vec2(2, 2) + vec2(-1, -1);
		vec4 viewPos = projectionInvMatrixPix * vec4(xyPos, screenZ, 1);
		viewPos /= viewPos.w;

		vec3 lightDir = vec3(1, 0, 1) - viewPos.xyz;
		float lightDistance = length(lightDir);
		float lightFactor = clamp(dot(lightDir, texture2D(normal, textureCoord.xy).xyz) / lightDistance, 0, 1);
		
		lightFactor *= max(1 - lightDistance, 0) * 10;

		outputColor = vec4(texture2D(color, textureCoord.xy).xyz * lightFactor, 1);
	}
}
