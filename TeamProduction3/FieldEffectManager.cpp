#include "FieldEffectManager.h"

FieldEffectManager::FieldEffectManager()
{
	fePuddleList.resize(64);
	feFireWallList.resize(64);
}

FieldEffectManager::~FieldEffectManager()
{
}

void FieldEffectManager::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	FieldEffectPuddle::StaticLoadAsset(device, loader);
	FieldEffectFireWall::StaticLoadAsset(device, loader);
	for (auto& fe : fePuddleList)
	{
		fe.LoadAsset(device, heap);
	}
	for (auto& fe : feFireWallList)
	{
		fe.LoadAsset(device, heap,loader);
	}
}

void FieldEffectManager::Initialize()
{
	for (auto& fe : fePuddleList)
	{
		fe.Initialize();
	}
	for (auto& fe : feFireWallList)
	{
		fe.Initialize();
	}
}

void FieldEffectManager::Update()
{
	for (auto& fe : fePuddleList)
	{
		fe.Update();
	}
	for (auto& fe : feFireWallList)
	{
		fe.Update();
	}
}

void FieldEffectManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (auto& fe : fePuddleList)
	{
		fe.Draw(cmdList);
	}
	for (auto& fe : feFireWallList)
	{
		fe.Draw(cmdList);
	}
}

void FieldEffectManager::CreatePuddle(const Vector3 & pos)
{
	for (auto& fe : fePuddleList)
	{
		bool getLiveFlag = fe.GetLiveFlag();
		if (!getLiveFlag)
		{
			fe.Initialize();
			fe.Create(pos);
			break;
		}
	}
}

void FieldEffectManager::CreateFireWall(const Vector3 & pos)
{
	for (auto& fe : feFireWallList)
	{
		bool getLiveFlag = fe.GetLiveFlag();
		if (!getLiveFlag)
		{
			fe.Initialize();
			fe.Create(pos);
			break;
		}
	}
}

std::vector<FieldEffectPuddle>* FieldEffectManager::GetPuddleList()
{
	return &fePuddleList;
}

std::vector<FieldEffectFireWall>* FieldEffectManager::GetFireWallList()
{
	return &feFireWallList;
}
