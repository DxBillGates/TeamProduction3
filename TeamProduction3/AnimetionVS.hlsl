#include"AnimetionShader.hlsli"
VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;
	output.svpos = mul(orth, mul(model, pos));
	float2 ancerpoint = float2(ancerAndSize.x / (texSize.x / ancerAndSize.z), ancerAndSize.y / (texSize.y / ancerAndSize.w));
	float2 size = float2(ancerAndSize.z / texSize.x, ancerAndSize.w / texSize.y);
	output.uv = float2(uv.x * size.x + ancerpoint.x, uv.y * size.y + ancerpoint.y);
	return output;
}