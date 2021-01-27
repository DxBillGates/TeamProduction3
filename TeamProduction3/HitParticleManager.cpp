#include "HitParticleManager.h"


HitParticleManager::HitParticleManager()
{
}

HitParticleManager::~HitParticleManager()
{

}

void HitParticleManager::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	for (int i = 0; i < size; i++)
	{
		particle[i].LoadAsset(pDevice, heap);
	}
}

void HitParticleManager::Initialize()
{
	for (int i = 0; i < size; i++)
	{
		particle[i].Initialize();
	}
}
void HitParticleManager::Emitte(Vector3 pos)
{
	for (int i = 0; i < size; i++)
	{
		particle[i].Emitte(pos);
	}
}

void HitParticleManager::Update()
{
	for (int i = 0; i < size; i++)
	{
		particle[i].Update();
	}
}

void HitParticleManager::Draw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap* heap)
{
	for (int i = 0; i < size; i++)
	{
		particle[i].Draw(pCmdList, heap);
	}
}

