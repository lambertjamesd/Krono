
float3 WorldPosition(float4x4 projectionViewInvMatrix, float3 normalizedPos)
{
#ifdef OPENGL
	normalizedPos.z = normalizedPos.z * 2 - 1;
#endif
	float4 result = mul(projectionViewInvMatrix, float4(normalizedPos, 1.0));
	result /= result.w;
	return result.xyz;
}

float3 NormalizedPosition(float4x4 projectionViewMatrix, float3 worldPosition)
{
	float4 result = mul(projectionViewMatrix, float4(worldPosition, 1.0));
	result /= result.w;
#ifdef OPENGL
	result.z = (result.z + 1) * 0.5;
#endif
	return result.xyz;
}

float2 TexToNorm(float2 texturePos)
{
#ifdef OPENGL
	return texturePos.xy * float2(2.0, 2.0) - float2(1.0, 1.0);
#else
	return texturePos.xy * float2(2.0, -2.0) - float2(1.0, -1.0);
#endif
}

float2 NormToTex(float2 normXY)
{
#ifdef OPENGL
	return (normXY + float2(1.0, 1.0)) * float2(0.5, 0.5);
#else
	return (normXY + float2(1.0, -1.0)) * float2(0.5, -0.5);
#endif
}