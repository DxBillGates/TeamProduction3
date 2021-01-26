#include "FieldEffectFireWall.h"

Dx12_Mesh FieldEffectFireWall::mesh = Dx12_Mesh();

FieldEffectFireWall::FieldEffectFireWall()
{
}

FieldEffectFireWall::~FieldEffectFireWall()
{
	delete cb;
}

void FieldEffectFireWall::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap)
{
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
}

void FieldEffectFireWall::Initialize()
{
	pos = Vector3();
	size = Vector3();
	reviveFlag = false;
	liveFlag = false;
	reviveT = 0;
	alpha = 0;
	cb->Map({ Matrix4::Scale(size)*Matrix4::Translate(pos),{1,0,0,alpha} });
}

void FieldEffectFireWall::Update()
{
	const float REVIVE_TIME = 30;
	const float LIVE_TIME = 256;
	const Vector3 MAX_SIZE = Vector3(32, 32, 32);
	if (reviveFlag)
	{
		if (reviveT < 1)
		{
			reviveT += (1.0f / REVIVE_TIME);
		}
		if (reviveT >= 1)
		{
			reviveT = 1;
			reviveFlag = false;
			liveFlag = true;
		}
		alpha = reviveT;
	}
	else
	{
		alpha -= (1.0f / LIVE_TIME);
	}
	size = Vector3(MAX_SIZE) * alpha;
	cb->Map({ Matrix4::Scale(size)*Matrix4::Translate(pos),{1,0,0,alpha} });
	if (alpha <= 0.0f)
	{
		Initialize();
	}
}

void FieldEffectFireWall::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cb->Set(cmdList);
	mesh.Draw(cmdList);
}

void FieldEffectFireWall::StaticLoadAsset(ID3D12Device * device, LoadContents * loader)
{
	MeshData md;
	loader->LoadMeshData("Resources/Model/", "cube", md);
	mesh.Create(device, &md);
}
