#include "FieldEffectPuddle.h"
MeshData FieldEffectPuddle::md = MeshData();
Dx12_Mesh FieldEffectPuddle::mesh = Dx12_Mesh();

FieldEffectPuddle::FieldEffectPuddle() :FieldEffect()
{
}

FieldEffectPuddle::~FieldEffectPuddle()
{
	delete cb;
	delete se;
	delete seData;
}

void FieldEffectPuddle::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap)
{
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
	seData = new SoundData("Resources/Music/jouhatu.wav");
	se = new Sound(seData);
}

void FieldEffectPuddle::Initialize()
{
	pos = Vector3();
	size = Vector3();
	reviveFlag = false;
	liveFlag = false;
	reviveT = 0;
	alpha = 0;
	cb->Map({ Matrix4::Scale(size / 2)*Matrix4::Translate(pos),{0,0.5f,1,1} });
}

void FieldEffectPuddle::Update()
{
	const float REVIVE_TIME = 1;			//生成時間(秒)
	const float MAX_SIZE = 128;				//マックスサイズ
	const float DECREASE_ALPHA = 0.001f;	//自然になくなる値
	if (reviveFlag)
	{
		if (reviveT < 1)
		{
			reviveT += (0.016f / REVIVE_TIME);
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
		alpha -= DECREASE_ALPHA;
	}
	size = Vector3(MAX_SIZE, 1, MAX_SIZE) * alpha;
	cb->Map({ Matrix4::Scale(size)*Matrix4::Translate(pos),{0,0.5f,1,1} });
	if (alpha <= 0)
	{
		Initialize();
	}
}

void FieldEffectPuddle::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cb->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(md.materialData.texture->GetSRVNumber()));
	mesh.Draw(cmdList);
}

void FieldEffectPuddle::StaticLoadAsset(ID3D12Device * device, LoadContents * loader)
{
	loader->LoadMeshData("Resources/Model/mizu/", "mizu", md);
	mesh.Create(device, &md);
}

Sound * FieldEffectPuddle::GetSE()
{
	return se;
}
