#include "TutorialArrow.h"

TutorialArrow::TutorialArrow()
{
}

TutorialArrow::~TutorialArrow()
{
	delete cb;
	delete tex;
}

void TutorialArrow::Initialize()
{
	pos = Vector3();
	angle = Vector3();
	t = 0;
	isDraw = false;
}

void TutorialArrow::LoadAsset(ID3D12Device * device, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cb = new Dx12_CBuffer<CBData>(device, heap, 1);
	MeshData md;
	loader->CreateModelData_Floor(64, 64, md);
	mesh.Create(device, &md);
	tex = loader->LoadTexture("Resources/Texture/", "yajirusi2.png");
}

void TutorialArrow::Update(const Vector3& p,const Vector3 & forward)
{
	const int TRUE_TIME = 30;

	pos = p;
	angle = forward;
	angle = angle.Normalize();
	cb->Map({ Matrix4::RotationY(atan2f(angle.x,angle.z))*Matrix4::Translate(pos),{1,0,0,1} });
	if (t % TRUE_TIME == 0)
	{
		isDraw = (isDraw) ? false : true;
	}

	++t;
}

void TutorialArrow::Draw(ID3D12GraphicsCommandList * cmdList)
{
	if (isDraw)
	{
		cb->Set(cmdList);
		cmdList->SetGraphicsRootDescriptorTable(2, cb->GetHeap()->GetSRVHandleForGPU(tex->GetSRVNumber()));
		mesh.Draw(cmdList);
	}
}
