struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer cbuff0 : register(b0)
{
	matrix orth;
}

cbuffer cbuff1 : register(b1)
{
	matrix model;
	float4 ancerAndSize;
	float4 texSize;
	float4 color;
}