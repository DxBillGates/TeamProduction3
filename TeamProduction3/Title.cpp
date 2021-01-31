#include "Title.h"

Title::Title()
{
}

Title::~Title()
{
	delete tex;
	delete cb;
	delete tex2;
	delete cb2;
}

void Title::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	SpriteData sd;
	loader->CreateModelData_Plane(64 * 12, 64 * 4, sd);
	sprite.Create(device, &sd);
	loader->CreateModelData_Plane(256, 64, sd, true);
	sprite2.Create(device, &sd);
	tex = loader->LoadTexture("Resources/Texture/", "title.png");
	tex2 = loader->LoadTexture("Resources/Texture/", "move3.png");
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
	cb2 = new Dx12_CBuffer<CBData>(device, heap, 1);
}

void Title::Initialize()
{
	pos = Vector3();
	isFade = false;
	endFade = false;
	t = 0;
}

void Title::Update(float size)
{
	if (t >= 1)
	{
		endFade = true;
		isFade = false;
	}
	if (isFade)
	{
		t += 0.016f * 2;
	}
	Vector3 target = Vector3(256, -720, 0);
	pos = Vector3::Lerp(Vector3(256, 128, 0), target, t);
	cb->Map({ Matrix4::Translate(pos),{1,1,1,1} });

	target = Vector3(640, 1440, 0);
	pos = Vector3::Lerp(Vector3(640, 480, 0), target, t);
	cb2->Map({ Matrix4::Scale(Vector3(1,1,1)+Vector3(1,1,1)*size*size) *  Matrix4::Translate(pos),{size,0,0,1} });

}

void Title::Draw(ID3D12GraphicsCommandList * cmdList)
{
	cb->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(tex->GetSRVNumber()));
	sprite.Draw(cmdList);

	cb2->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(tex2->GetSRVNumber()));
	sprite2.Draw(cmdList);
}

void Title::SetPosition(const Vector3 & p)
{
	pos = p;
}

Vector3 Title::GetPosition()
{
	return pos;
}

void Title::SetIsFada(bool b)
{
	isFade = b;
}

bool Title::GetIsFade()
{
	return isFade;
}

bool Title::GetEndIsFade()
{
	return endFade;
}
