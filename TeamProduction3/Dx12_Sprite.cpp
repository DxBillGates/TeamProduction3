#include "Dx12_Sprite.h"
#include "COMRelease.h"

Dx12_Sprite::Dx12_Sprite()
	:vBuff(nullptr), iBuff(nullptr), vbView({}), ibView({})
{
}

Dx12_Sprite::~Dx12_Sprite()
{
	COM_RELEASE(vBuff);
	COM_RELEASE(iBuff);
}

void Dx12_Sprite::Create(ID3D12Device * device, SpriteData * spriteData)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Width = sizeof(VertexUV)* spriteData->vertices.size();
	device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vBuff));
	vbView.BufferLocation = vBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = (UINT)resDesc.Width;
	vbView.StrideInBytes = sizeof(VertexUV);
	resDesc.Width = sizeof(unsigned short)*spriteData->indices.size();
	device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&iBuff));
	ibView.BufferLocation = iBuff->GetGPUVirtualAddress();
	ibView.SizeInBytes = (UINT)resDesc.Width;
	ibView.Format = DXGI_FORMAT_R16_UINT;

	VertexUV* vbMap = nullptr;
	unsigned short* ibMap = nullptr;
	vBuff->Map(0, nullptr, (void**)&vbMap);
	iBuff->Map(0, nullptr, (void**)&ibMap);
	for (int i = 0; i < (int)spriteData->vertices.size(); ++i)
	{
		vbMap[i] = spriteData->vertices[i];
	}
	for (int i = 0; i < (int)spriteData->indices.size(); ++i)
	{
		ibMap[i] = spriteData->indices[i];
	}
	vBuff->Unmap(0, nullptr);
	iBuff->Unmap(0, nullptr);
}

void Dx12_Sprite::Create(ID3D12Device * device, std::vector<VertexUV>* vertices, std::vector<unsigned short>* indices)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Width = sizeof(VertexUV)*vertices->size();
	device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vBuff));
	vbView.BufferLocation = vBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = (UINT)resDesc.Width;
	vbView.StrideInBytes = sizeof(VertexUV);
	resDesc.Width = sizeof(unsigned short)*indices->size();
	device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&iBuff));
	ibView.BufferLocation = iBuff->GetGPUVirtualAddress();
	ibView.SizeInBytes = (UINT)resDesc.Width;
	ibView.Format = DXGI_FORMAT_R16_UINT;

	VertexUV* vbMap = nullptr;
	unsigned short* ibMap = nullptr;
	vBuff->Map(0, nullptr, (void**)&vbMap);
	iBuff->Map(0, nullptr, (void**)&ibMap);
	for (int i = 0; i < (int)vertices->size(); ++i)
	{
		vbMap[i] = (*vertices)[i];
	}
	for (int i = 0; i < (int)indices->size(); ++i)
	{
		ibMap[i] = (*indices)[i];
	}
	vBuff->Unmap(0, nullptr);
	iBuff->Unmap(0, nullptr);
}

void Dx12_Sprite::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cmdList->IASetIndexBuffer(&ibView);
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	cmdList->DrawIndexedInstanced(GetIndicesCount(), 1, 0, 0, 0);
}

ID3D12Resource * Dx12_Sprite::GetVBuff()
{
	return vBuff;
}

ID3D12Resource * Dx12_Sprite::GetIBuff()
{
	return iBuff;
}

D3D12_VERTEX_BUFFER_VIEW * Dx12_Sprite::GetVBView()
{
	return &vbView;
}

D3D12_INDEX_BUFFER_VIEW * Dx12_Sprite::GetIBView()
{
	return &ibView;
}

UINT Dx12_Sprite::GetIndicesCount()
{
	return ibView.SizeInBytes / sizeof(unsigned short);
}
