#pragma once
#include<DirectXMath.h>
struct VertexUV_Normal
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
	VertexUV_Normal(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 uv, DirectX::XMFLOAT3 normal) :pos(pos), uv(uv), normal(normal)
	{
	}
};

struct VertexUV
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	VertexUV(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT2 uv):pos(pos),uv(uv)
	{
	}
};

struct Vertex
{
	DirectX::XMFLOAT3 pos;
};
