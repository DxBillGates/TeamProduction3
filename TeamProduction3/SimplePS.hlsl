#include "SimpleShader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float3 lightdir = float3(0, -1, 0);
	lightdir = normalize(lightdir);
	float3 lightcolor = float3(1, 1, 1);
	float3 diffuse = float3(0.5f, 0.5f, 0.5f);
	float4 texcolor = tex.Sample(smp, input.uv);
	return float4(dot(-lightdir, input.normal) * diffuse.xyz * lightcolor + 0.5f, 1.0f) * color * texcolor;
}