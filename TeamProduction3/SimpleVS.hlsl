#include "SimpleShader.hlsli"
VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD, float3 normal : NORMAL)
{
	VSOutput output;
	output.svpos = mul(proj, mul(view, mul(model, pos)));
	output.uv = uv;
	output.normal = normal;
	return output;
}