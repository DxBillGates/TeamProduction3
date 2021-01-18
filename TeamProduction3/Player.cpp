#include "Player.h"

Keyboard* Player::keyboard = nullptr;
Xinput* Player::ctrler = nullptr;

Player::Player()
	:cb(nullptr), pos(Vector3())
{
}

Player::~Player()
{
	delete cb;
}

void Player::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cb = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
	loader->LoadMeshData("Resources/Model/", "Player", md);
	mesh.Create(pDevice, &md);
}

void Player::Initialize()
{
	pos = Vector3(0, 0, 512);
	vel = Vector3();
	forward = Vector3();
	jumpFlag = false;
	fireValue = 0;
	oldPos = pos;
	isMove = false;
}

void Player::Update()
{
	const float MOVE_SPEED = 1.f;
	const float G = 0.98f;
	const float INCREASE_FIRE_VALUE = 1.f / 60.f;
	const float UPDATE_OLDPOS_DISTANCE = 64;

	if (Vector3::Distance(oldPos, pos) >= UPDATE_OLDPOS_DISTANCE)
	{
		isMove = true;
		oldPos = pos;
	}
	else
	{
		isMove = false;
	}

	Vector3 moveVector = Vector3();
	//ˆÚ“®ˆ—
	if (keyboard->CheakHitKey(Key::W) || ctrler->CheckHitKey(XinputPadKey::XINPUT_UP) || ctrler->GetLStickY() < 0.0f)
	{
		moveVector += forward;
	}
	if (keyboard->CheakHitKey(Key::A) || ctrler->CheckHitKey(XinputPadKey::XINPUT_LEFT) || ctrler->GetLStickX() < 0.0f)
	{
		moveVector -= Vector3::Normalize(Vector3::Cross(Vector3(0, 1, 0), forward));;
	}
	if (keyboard->CheakHitKey(Key::S) || ctrler->CheckHitKey(XinputPadKey::XINPUT_DOWN) || ctrler->GetLStickY() > 0.0f)
	{
		moveVector -= forward;
	}
	if (keyboard->CheakHitKey(Key::D) || ctrler->CheckHitKey(XinputPadKey::XINPUT_RIGHT) || ctrler->GetLStickX() > 0.0f)
	{
		moveVector += Vector3::Normalize(Vector3::Cross(Vector3(0, 1, 0), forward));;
	}

	if (moveVector.Length() > 0)
	{
		fireValue += INCREASE_FIRE_VALUE;
	}
	else
	{
		fireValue -= INCREASE_FIRE_VALUE;
	}

	if (fireValue < 0)
	{
		fireValue = 0;
	}
	else if (fireValue > 1)
	{
		fireValue = 1;
		moveVector *= 5;
	}
	pos += moveVector * MOVE_SPEED;
	cb->Map({ Matrix4::Scale(Vector3(32,32,32)) *Matrix4::RotationY(atan2f(forward.x,forward.z))* Matrix4::Translate(pos),{fireValue,0,0,1} });
}

void Player::Draw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap* heap)
{
	cb->Set(pCmdList);
	//pCmdList->SetGraphicsRootDescriptorTable(2, heap->GetSRVHandleForGPU(md.materialData.texture->GetSRVNumber()));
	mesh.Draw(pCmdList);
}

void Player::SetInputDevice(Keyboard * pKeyboard, Xinput * pCtrler)
{
	keyboard = pKeyboard;
	ctrler = pCtrler;
}

void Player::SetPosition(const Vector3 & p)
{
	pos = p;
}

Vector3 Player::GetPosition()
{
	return pos;
}

void Player::SetForward(const Vector3 & f)
{
	forward = f;
}

float Player::GetFireValue()
{
	return fireValue;
}

void Player::SetFireValue(float fv)
{
	if (fv >= 1)
	{
		fv = 1;
	}
	fireValue = fv;
}

bool Player::GetIsMove()
{
	return isMove;
}

Vector3 Player::GetOldPos()
{
	return oldPos;
}


