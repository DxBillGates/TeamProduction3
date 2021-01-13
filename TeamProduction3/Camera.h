#pragma once
#pragma once
#include"Keyboard.h"
#include<DirectXMath.h>
struct Perspective
{
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
};

class Camera
{
private:
	Keyboard* keyboard;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 up;
	float yaw;
	float pitch;
	float turnSpeed;
	float moveSpeed;
public:
	Camera();
	void Initialize();
	void Update();
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix(float fov, float aspect, float nearClip = 1.0f, float farClip = 1000.0f);
	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetTurnSpeed(float value);
	void SetMoveSpeed(float value);
	void SetKeyboard(Keyboard* pKeyboard);
};

