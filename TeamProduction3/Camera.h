#pragma once
#pragma once
#include"Keyboard.h"
#include "Xinput.h"
#include<DirectXMath.h>
#include "Vector3.h"
#include "Matrix4.h"
struct Perspective
{
	Matrix4 view;
	Matrix4 proj;
};

class Camera
{
private:
	Vector3 pos;
	Vector3 target;
	Vector3 up;
	float yaw;

	static float MAX_SHAKE_TIME;
	static bool isShake;
	static float shakeTime;
public:
	Camera();
	void Initialize();
	void Update(Keyboard* keyboard, Xinput* ctrler,const Vector3& argpos = Vector3());
	void SetPosition(const Vector3& p);
	void SetTarget(const Vector3& t);
	Matrix4 GetViewMatrix();
	Matrix4 GetProjectionMatrix(float fov, float aspect, float nearClip = 1.0f, float farClip = 10000.0f);
	Vector3 GetPosition();
	Vector3 GetTarget();
	Vector3 GetForward();
	static void ScreenShake();
	bool IsShake();
};

