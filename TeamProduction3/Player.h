#pragma once
#include "Dx12_CBuffer.h"
#include "Dx12_Mesh.h"
#include "Keyboard.h"
#include "Xinput.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Sound.h"

class Player
{
private:
	struct CBData
	{
		Matrix4 mat;
		DirectX::XMFLOAT4 color;
	};
private:
	static Keyboard* keyboard;
	static Xinput* ctrler;
	Dx12_Mesh mesh;
	MeshData md;
	Dx12_CBuffer<CBData>* cb;

	Vector3 pos;
	Vector3 vel;
	Vector3 forward;
	bool jumpFlag;
	float redValue;
	Vector3 oldPos;
	bool isMove;
	bool isBuff;
	float fireValue;
	const float CONSTANT_FIRE_VALUE;
	Vector3 angle;

	Quaternion qLocal;

	Sound* se;
	SoundData* seData;

	Sound* hitSE;
	SoundData* hitSEData;

	bool isDamage;
	bool isDraw;
	int damageTime;

	Vector3 size;
public:
	Player();
	~Player();
	void LoadAsset(ID3D12Device* pDevice,Dx12_CBVSRVUAVHeap* heap,LoadContents* loader);
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* pCmdList,Dx12_CBVSRVUAVHeap* heap);
	static void SetInputDevice(Keyboard* pKeyboard,Xinput* pCtrler);
	void SetPosition(const Vector3& p);
	Vector3 GetPosition();
	void SetForward(const Vector3& f);
	float GetFireValue();
	void SetFireValue(float fv);
	bool GetIsMove();
	Vector3 GetOldPos();
	void Respawn();
	float GetRedValue();
	void SetRedValue(float s);
	bool GetIsOverCFireValue();

	bool GetIsDamage();
	void SetIsDamage(bool b);

	Vector3 GetSize();
};

