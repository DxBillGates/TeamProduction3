#pragma once
#include<d3d12.h>
#include"Dx12_CBVSRVUAVHeap.h"
#include"COMRelease.h"
template<typename T>
class Dx12_CBuffer
{
private:
	Dx12_CBVSRVUAVHeap* pHeap;
	int rangeNumber;
	int cbvNumber;				//ヒープの何番目にあるか
	ID3D12Resource* buffer;		//定数バッファ
public:
	Dx12_CBuffer();
	Dx12_CBuffer(ID3D12Device* pDevice, Dx12_CBVSRVUAVHeap* heap, int rangeNumber);		//param1 ID3D12Device* ,param2 cbvHeap
	~Dx12_CBuffer();
	void Map(const T& mappinData);
	int GetCbvNumber();
	int GetRangeNumber();
	void Set(ID3D12GraphicsCommandList* pCmdList);
	Dx12_CBVSRVUAVHeap* GetHeap();

};

template<typename T>
inline Dx12_CBuffer<T>::Dx12_CBuffer()
{
}

template<typename T>
inline Dx12_CBuffer<T>::Dx12_CBuffer(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap* heap, int rangeNumber)
{
	pHeap = heap;
	cbvNumber = heap->GetNextCbvDescriptorNumber();
	this->rangeNumber = rangeNumber;
	//定数バッファの設定
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Width = (sizeof(T) + 0xff)&~0xff;
	HRESULT result = pDevice->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&buffer));
	//定数バッファビューの設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = buffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)resDesc.Width;
	//ヒープにビューを生成
	heap->CreateCBV(buffer);
}

template<typename T>
inline Dx12_CBuffer<T>::~Dx12_CBuffer()
{
	COM_RELEASE(buffer);
}

template<typename T>
inline void Dx12_CBuffer<T>::Map(const T& mappinData)
{
	//定数バッファのデータを書き換える
	T* cbMap = nullptr;
	buffer->Map(0, nullptr, (void**)&cbMap);
	*cbMap = mappinData;
	buffer->Unmap(0, nullptr);
}


template<typename T>
inline int Dx12_CBuffer<T>::GetCbvNumber()
{
	return cbvNumber;
}

template<typename T>
inline int Dx12_CBuffer<T>::GetRangeNumber()
{
	return rangeNumber;
}

template<typename T>
inline void Dx12_CBuffer<T>::Set(ID3D12GraphicsCommandList * pCmdList)
{
	pHeap->Set();
	pCmdList->SetGraphicsRootDescriptorTable(rangeNumber, pHeap->GetCBVHandleForGPU(cbvNumber));
}

template<typename T>
inline Dx12_CBVSRVUAVHeap * Dx12_CBuffer<T>::GetHeap()
{
	return pHeap;
}

