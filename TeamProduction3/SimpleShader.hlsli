struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

cbuffer cbuff0 : register(b0)
{
	matrix view;
	matrix proj;
}

cbuffer cbuff1 : register(b1)
{
	matrix model;
	float4 color;
}