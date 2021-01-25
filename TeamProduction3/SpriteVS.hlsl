#include "SpriteShader.hlsli"
VSOutput main(float4 pos : POSITION,float2 uv : TEXCOORD)
{
	VSOutput output;
	output.svpos = mul(orth, mul(model, pos));
	output.uv = uv;
	return output;
}