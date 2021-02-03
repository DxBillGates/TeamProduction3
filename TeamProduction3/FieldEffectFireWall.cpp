#include "FieldEffectFireWall.h"

Dx12_Mesh FieldEffectFireWall::mesh = Dx12_Mesh();

FieldEffectFireWall::FieldEffectFireWall()
{
}

FieldEffectFireWall::~FieldEffectFireWall()
{
	delete cb;
}

void FieldEffectFireWall::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents* loader)
{
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
	fpManager.LoadAsset(device, heap, loader);
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
	fpManager.Initialize();
}

void FieldEffectFireWall::Update()
{
	const float REVIVE_TIME = 30;					//何フレームで蘇生させるか
	const float LIVE_TIME = 100;					//何フレーム生存させるか
	const Vector3 MAX_SIZE = Vector3(32, 32, 32);	//最大サイズ
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
		if (liveFlag)
			alpha -= (1.0f / LIVE_TIME);
	}
	size = Vector3(MAX_SIZE) * alpha;
	//printf("%f,%f,%f\n", size.x, size.y, alpha);
	cb->Map({ Matrix4::Scale(size)*Matrix4::Translate(pos),{1,0,0,alpha} });
	if (liveFlag)
	{
		fpManager.SetPosition(pos);
		fpManager.Update();
	}
	if (alpha <= 0.0f && liveFlag)
	{
		pos = Vector3();
		size = Vector3();
		reviveFlag = false;
		liveFlag = false;
		reviveT = 0;
		alpha = 0;
	}

}

void FieldEffectFireWall::Draw(ID3D12GraphicsCommandList * cmdList)
{
	//cb->Set(cmdList);
	//mesh.Draw(cmdList);

	if (liveFlag)
	{
		fpManager.Draw(cmdList, nullptr);
	}
}

void FieldEffectFireWall::StaticLoadAsset(ID3D12Device * device, LoadContents * loader)
{
	MeshData md;
	loader->LoadMeshData("Resources/Model/", "cube", md);
	mesh.Create(device, &md);
}
