#include "ResultEffect.h"

ResultEffect::ResultEffect()
{

}

ResultEffect::~ResultEffect()
{
	delete kingTex;
	for (int i = 0; i < 3; i++)
	{
		delete kingCB[i];
	}
	delete rfTex;
	delete rfCB;
	delete maiTex;
	delete maiCB;

}

void ResultEffect::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	SpriteData sd;
	//王冠
	loader->CreateModelData_Plane(32, 32, sd);
	kingSprite.Create(device, &sd);
	//ランキング
	loader->CreateModelData_Plane(256, 64, sd);
	rfSprite.Create(device, &sd);
	//「枚」
	loader->CreateModelData_Plane(150, 150, sd);
	maiSprite.Create(device, &sd);

	kingTex = loader->LoadTexture("Resources/Texture/", "king.png");
	rfTex = loader->LoadTexture("Resources/Texture/", "runkingFont.png");
	maiTex = loader->LoadTexture("Resources/Texture/", "ui05.png");
	
	rfCB = new Dx12_CBuffer<CBData>(device, heap, 1);
	maiCB = new Dx12_CBuffer<CBData>(device, heap, 1);

	for (int i = 0; i < 3; i++)
	{
		kingCB[i] = new Dx12_CBuffer<CBData>(device, heap, 1);
	}


	for (int i = 0; i < particleCount; i++)
	{
		particle[i].LoadAsset(device, heap, loader);
	}
}

void ResultEffect::Initialize()
{
	kingTexPos = { 10,100,0 };
	rfPos = { 50,10,0 };
	maiPos = { 1000,500,0 };
	for (int i = 0; i < particleCount; i++)
	{
		particle[i].Initialize();
	}
}
void ResultEffect::Emitte(Vector3 pos)
{
	for (int i = 0; i < particleCount; i++)
	{
		particle[i].Emitte(pos);
	}
}
void ResultEffect::Update()
{

	kingCB[0]->Map({ Matrix4::Translate(kingTexPos)*Matrix4::Scale({1,1,1}),{1,1,0,1} });
	kingCB[1]->Map({ Matrix4::Translate(kingTexPos+Vector3(0,64,0))*Matrix4::Scale({1,1,1}),{0.6f,0.6f,0.6f,1} });
	kingCB[2]->Map({ Matrix4::Translate(kingTexPos + Vector3(0,128,0))*Matrix4::Scale({1,1,1}),{0.5f,0.2f,0,1} });

	rfCB->Map({ Matrix4::Translate(rfPos)*Matrix4::Scale({1,1,1}),{1,1,1,1} });
	maiCB->Map({ Matrix4::Translate(maiPos)*Matrix4::Scale({1,1,1}),{0.2f,0.2f,1,1} });
	
	for (int i = 0; i < particleCount; i++)
	{
		particle[i].Update();
	}

}

void ResultEffect::Draw(ID3D12GraphicsCommandList * cmdList)
{

	for (int i = 0; i < 3; i++)
	{
		kingCB[i]->Set(cmdList);
		cmdList->SetGraphicsRootDescriptorTable(2, kingCB[i]->GetHeap()->GetSRVHandleForGPU(kingTex->GetSRVNumber()));
		kingSprite.Draw(cmdList);
	}

	rfCB->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, rfCB->GetHeap()->GetSRVHandleForGPU(rfTex->GetSRVNumber()));
	rfSprite.Draw(cmdList);

	maiCB->Set(cmdList);
	cmdList->SetGraphicsRootDescriptorTable(2, maiCB->GetHeap()->GetSRVHandleForGPU(maiTex->GetSRVNumber()));
	maiSprite.Draw(cmdList);

	for (int i = 0; i < particleCount; i++)
	{
		particle[i].Draw(cmdList);
	}
}

void ResultEffect::SetPosition(const Vector3 & p)
{
	for (int i = 0; i < particleCount; i++)
	{
		particle[i].SetPosition(p);
	}
}





