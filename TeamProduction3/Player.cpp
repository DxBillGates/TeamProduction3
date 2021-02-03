#include "Player.h"

Keyboard* Player::keyboard = nullptr;
Xinput* Player::ctrler = nullptr;

Player::Player()
	:cb(nullptr), pos(Vector3()), CONSTANT_FIRE_VALUE(300)
{
}

Player::~Player()
{
	delete cb;
	delete se;
	delete seData;
	delete hitSE;
	delete hitSEData;
}

void Player::LoadAsset(ID3D12Device * pDevice, Dx12_CBVSRVUAVHeap * heap, LoadContents * loader)
{
	cb = new Dx12_CBuffer<CBData>(pDevice, heap, 1);
	loader->LoadMeshData("Resources/Model/daruma/", "player", md);
	mesh.Create(pDevice, &md);

	seData = new SoundData("Resources/Music/fire.wav");
	se = new Sound(seData);

	hitSEData = new SoundData("Resources/Music/PlayerHit.wav");
	hitSE = new Sound(hitSEData);
}

void Player::Initialize()
{
	pos = Vector3(1024, 0, 1024);
	vel = Vector3();
	forward = Vector3();
	jumpFlag = false;
	redValue = 0;
	fireValue = 0;
	oldPos = pos;
	isMove = false;
	isBuff = false;
	angle = Vector3();
	qLocal = Quaternion(Vector3(0, 0, 1), 0);
	isDraw = true;
	isDamage = false;
	damageTime = 0;
	size = Vector3(64,64,64);
}

void Player::Update()
{
	const float MAX_VELOCITY_LENGTH = 2;
	const float FIRE_MOVE_VALUE = 6;
	const float MOVE_SPEED = 2.7f;
	const float G = 0.98f;
	const float INCREASE_FIRE_VALUE = 0.75f / 60.f;
	const float UPDATE_OLDPOS_DISTANCE = 64;
	const float MAX_FIRE_VALUE = 600;
	float f = 0.01f;
	const int DAMAGE_TIME = 10;
	const int MAX_DAMAGE_TIME = 30;


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
		angle.x += -f;
	}
	if (keyboard->CheakHitKey(Key::A) || ctrler->CheckHitKey(XinputPadKey::XINPUT_LEFT) || ctrler->GetLStickX() < 0.0f)
	{
		moveVector -= Vector3::Normalize(Vector3::Cross(Vector3(0, 1, 0), forward));;
		angle.z += -f;
	}
	if (keyboard->CheakHitKey(Key::S) || ctrler->CheckHitKey(XinputPadKey::XINPUT_DOWN) || ctrler->GetLStickY() > 0.0f)
	{
		moveVector -= forward;
		angle.x += f;
	}
	if (keyboard->CheakHitKey(Key::D) || ctrler->CheckHitKey(XinputPadKey::XINPUT_RIGHT) || ctrler->GetLStickX() > 0.0f)
	{
		moveVector += Vector3::Normalize(Vector3::Cross(Vector3(0, 1, 0), forward));;
		angle.z += f;
	}

	if (keyboard->CheakHitKey(Key::X))
	{
		angle.x = 0.1f;
	}
	if (keyboard->CheakHitKey(Key::Y))
	{
		angle.y = 0.1f;
	}
	if (keyboard->CheakHitKey(Key::Z))
	{
		angle.z = 0.1f;
	}

	if (moveVector.Length() > 0)
	{
		fireValue += 1;
		//if(fireValue >= CONSTANT_FIRE_VALUE)
		redValue += INCREASE_FIRE_VALUE;
	}
	else
	{
		fireValue -= 1;
		//if (fireValue < CONSTANT_FIRE_VALUE)
		redValue -= INCREASE_FIRE_VALUE;
	}

	if (fireValue >= MAX_FIRE_VALUE)
	{
		//Initialize();
	}
	if (fireValue <= 0)
	{
		fireValue = 0;
	}
	if (fireValue >= CONSTANT_FIRE_VALUE)
	{
		isBuff = true;
	}

	if (redValue < 0)
	{
		redValue = 0;
	}
	else if (redValue > 1)
	{
		se->Start();
		redValue = 1;
		moveVector *= FIRE_MOVE_VALUE;
	}
	vel += moveVector.Normalize();
	//printf("%f\n", vel.Length());
	if (moveVector.Length() == 0)
	{
		Vector3 v = vel;
		vel -= v.Normalize() / 10;
		if (vel.Length() < 1)
		{
			vel = Vector3();
		}
	}
	if (vel.Length() > MAX_VELOCITY_LENGTH)
	{
		vel = vel.Normalize() * MAX_VELOCITY_LENGTH;
	}
	pos += vel + moveVector;

	const Vector3 UNITX = Vector3(1, 0, 0), UNITY = Vector3(0, 1, 0), UNITZ = Vector3(0, 0, 1);

	Vector3 vSideAxis = Quaternion::GetAxis(Quaternion(UNITX, qLocal));
	Vector3 vUpAxis = Quaternion::GetAxis(Quaternion(UNITY, qLocal));
	Vector3 vForwardAxis = Quaternion::GetAxis(Quaternion(UNITZ, qLocal));

	Vector3 v = moveVector.Normalize() / 10;
	Quaternion qRoll = Quaternion(vUpAxis, v.y);
	Quaternion qPitch = Quaternion(vSideAxis, v.z);
	Quaternion qYow = Quaternion(vForwardAxis, v.x);

	qLocal = qRoll * qLocal;
	qLocal = qPitch * qLocal;
	qLocal = qYow * qLocal;

	//printf("%f,%f,%f,%f\n", qLocal.x, qLocal.y, qLocal.z, qLocal.w);
	Matrix4 mR = Quaternion::Rotation(qLocal);
	//Vector3 v = moveVector.Normalize()+angle;
	//Matrix4 mR = Matrix4::RotationZ(-v.x) * Matrix4::RotationX(v.z) * Matrix4::RotationY(v.y);
	//cb->Map({ Matrix4::Scale(Vector3(32,32,32)) * mR * Matrix4::Translate(pos),{redValue,1,1,1} });
	cb->Map({ Matrix4::Scale(size) * mR * Matrix4::Translate(pos),{1,1,1,1} });

	if (isDamage)
	{
		if (damageTime % DAMAGE_TIME == 0)
		{
			isDraw = (isDraw) ? false : true;
		}
		if (damageTime >= MAX_DAMAGE_TIME)
		{
			isDamage = false;
			isDraw = true;
			damageTime = 0;
		}
		++damageTime;
	}
}

void Player::Draw(ID3D12GraphicsCommandList * pCmdList, Dx12_CBVSRVUAVHeap* heap)
{
	if (isDraw)
	{
		cb->Set(pCmdList);
		pCmdList->SetGraphicsRootDescriptorTable(2, heap->GetSRVHandleForGPU(md.materialData.texture->GetSRVNumber()));
		mesh.Draw(pCmdList);
	}
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
	return redValue;
}

void Player::SetFireValue(float fv)
{
	if (fv >= 1)
	{
		fv = 1;
	}
	redValue = fv;
}

bool Player::GetIsMove()
{
	return isMove;
}

Vector3 Player::GetOldPos()
{
	return oldPos;
}

void Player::Respawn()
{
	Initialize();
}

float Player::GetRedValue()
{
	return redValue;
}

void Player::SetRedValue(float s)
{
	redValue = s;
}

bool Player::GetIsOverCFireValue()
{
	if (fireValue >= CONSTANT_FIRE_VALUE)
	{
		return true;
	}
	return false;
}

bool Player::GetIsDamage()
{
	return isDamage;
}

void Player::SetIsDamage(bool b)
{
	hitSE->Start();
	isDamage = b;
}

Vector3 Player::GetSize()
{
	return size;
}


