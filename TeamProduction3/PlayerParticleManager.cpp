#include "PlayerParticleManager.h"


PlayerParticleManager::PlayerParticleManager()
{
}

PlayerParticleManager::~PlayerParticleManager()
{

}

void PlayerParticleManager::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	for (int i = 0; i < size; i++)
	{
		particle[i].LoadAsset(pDevice, heap);
	}
}

void PlayerParticleManager::Initialize()
{
	for (int i = 0; i < size; i++)
	{
		particle[i].Initialize();
	}
}

void PlayerParticleManager::Update(float thermo)
{
	int count = (int)(size * thermo);

	for (int i = 0; i < size; i++)
	{
		particle[i].Update();
	}
	for (int i = 0; i < count; i++)
	{
		particle[i].SetLiveFlag(true);
	}
	for (int i = count; i < size; i++)
	{
		particle[i].SetLiveFlag(false);
	}
}

void PlayerParticleManager::Draw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap* heap)
{
	for (int i = 0; i < size; i++)
	{
		particle[i].Draw(pCmdList, heap);
	}
}

void PlayerParticleManager::SetPosition(Vector3 p)
{
	for (int i = 0; i < size; i++)
	{
		particle[i].SetPosition(p);
	}
}

