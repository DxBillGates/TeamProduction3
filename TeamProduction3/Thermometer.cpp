#include "Thermometer.h"


Thermometer::Thermometer()
	:cb(nullptr), position(Vector3())
{
}

Thermometer::~Thermometer()
{
	delete cb;
}

void Thermometer::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cb = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
	loader->LoadMeshData("Resources/Model/", "cube", md);
	mesh.Create(pDevice, &md);
}

void Thermometer::Initialize()
{
	position = {};
	scale = { 10,10,10 };
	liveFlag = true;
	color = {};
	direction = {};
}

void Thermometer::Update(Vector3 pos,float thermol,Vector3 cameraPos)
{
	position = pos;
	billBoradPos = cameraPos;
#pragma region ÉJÉâÅ[

	if (thermol < 0.3f)
		color = { 0,0,1 };
	if (thermol > 0.3f)
		color = { 0,1,0 };
	if (thermol > 0.6f)
		color = { 1,1,0 };
	if (thermol > 0.9f)
		color = { 1,0,0 };

#pragma endregion

	Vector3 dir = billBoradPos - position;
	direction = dir.Normalize();
		scale.x = thermol * 100;
		if (thermol < 0.0f)
		{
			scale.x = 0;
		}
	
	/*if (scale.x < 0)
	{
		liveFlag = false;
	}
	else
	{
		liveFlag = true;
	}*/
	cb->Map({ Matrix4::Scale(scale) *Matrix4::RotationY(atan2f(direction.x,direction.z))* Matrix4::Translate(pos),{color.x,color.y,color.z,1} });
}

void Thermometer::Draw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap* heap)
{
	cb->Set(pCmdList);
	pCmdList->SetGraphicsRootDescriptorTable(2, heap->GetSRVHandleForGPU(md.materialData.texture->GetSRVNumber()));
	if (liveFlag)
		mesh.Draw(pCmdList);
}



Vector3 Thermometer::GetPosition()
{
	return position;
}
