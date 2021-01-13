#include "SimpleShader.hlsli"
float4 main(VSOutput input) : SV_TARGET
{
	float3 lightdir = float3(0, 0, 1);
	lightdir = normalize(lightdir);
	float3 lightcolor = float3(1, 1, 1);
	float3 diffuse = float3(0.5f, 0.5f, 0.5f);
	return float4(dot(-lightdir, input.normal) * diffuse.xyz * lightcolor + 0.3f, 1.0f) * color;
}