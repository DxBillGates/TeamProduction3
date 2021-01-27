#include "FireParticleManager.h"


FireParticleManager::FireParticleManager()
{
}

FireParticleManager::~FireParticleManager()
{
	
}

void FireParticleManager::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	for (int i = 0; i < size; i++)
	{
		particle[i].LoadAsset(pDevice, heap);
	}
}

void FireParticleManager::Initialize()
{
	for (int i = 0; i < size; i++)
	{
		particle[i].Initialize();
	}
}

void FireParticleManager::Update()
{
	for (int i = 0; i < size; i++)
	{
		particle[i].Update();
	}
}

void FireParticleManager::Draw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap* heap)
{
	for (int i = 0; i < size; i++)
	{
		particle[i].Draw(pCmdList,heap);
	}
}

void FireParticleManager::SetLiveFlag(bool b)
{
}

void FireParticleManager::SetPosition(Vector3 p)
{
	for (int i = 0; i < size; i++)
	{
		particle[i].SetPosition(p);
	}
}

