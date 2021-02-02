#include "Camera.h"
#include "Quaternion.h"
#include <random>

float Camera::MAX_SHAKE_TIME = 0.3f;
float Camera::shakeTime = 0;
bool Camera::isShake = false;

Camera::Camera()
	:pos(Vector3(0, 0, -1)), target(Vector3(0, 0, 0)), up(Vector3(0, 1, 0)),yaw(0)
{
}

void Camera::Initialize()
{
	pos = Vector3(0, 0, -1);
	target = Vector3(0, 0, 0);
	up = Vector3(0, 1, 0);
	yaw = 0;
	isShake = false;
	shakeTime = 0;
}

void Camera::Update(Keyboard * keyboard, Xinput * ctrler, const Vector3& argpos)
{
	const float Y = 1024;           //ƒJƒƒ‰‚Ì‚‚³
	const float R = 512;			//’Ž‹“_‚Æ‚Ì‹——£
	const float SHAKE_RANGE = 450.f;//‰æ–Ê—h‚ê’l
	pos += argpos - target;
	target = argpos;

	//ƒJƒƒ‰‚Ì‰ñ“]
	//yaw += (keyboard->CheakHitKey(Key::RIGHT) || ctrler->GetRStickX() > 0) ? S : 0;
	//yaw += (keyboard->CheakHitKey(Key::LEFT) || ctrler->GetRStickX() < 0) ? -S : 0;
	Vector3 v;
	v.x = R * sinf(DirectX::XM_2PI / 360 * yaw)+target.x;
	v.y = Y;
	v.z = R * cosf(DirectX::XM_2PI / 360 * yaw)+target.z;
	pos = v;

	if (isShake)
	{
		Vector3 vec;
		vec.x = (float)std::rand() / RAND_MAX * SHAKE_RANGE - SHAKE_RANGE / 2.0f;
		vec.z = (float)std::rand() / RAND_MAX * SHAKE_RANGE - SHAKE_RANGE / 2.0f;
		pos += vec;
		target += vec;

		if (shakeTime < 0)
		{
			isShake = false;
			shakeTime = 0;
		}
		shakeTime -= 0.016f;
	}
}

void Camera::SetPosition(const Vector3 & p)
{
	pos = p;
}

void Camera::SetTarget(const Vector3 & t)
{
	target = t;
}

Matrix4 Camera::GetViewMatrix()
{
	Matrix4 result;
	Vector3 xAxis, yAxis, zAxis;
	zAxis = Vector3::Normalize(target - pos);
	xAxis = Vector3::Normalize(Vector3::Cross(up, zAxis));
	yAxis = Vector3::Cross(zAxis, xAxis);
	Vector3 negpos = Vector3(-pos.x, -pos.y, -pos.z);
	result =
	{
		xAxis.x,xAxis.y,xAxis.z,0,
		yAxis.x,yAxis.y,yAxis.z,0,
		zAxis.x,zAxis.y,zAxis.z,0,
		0,0,0,1,
	};
	result = Matrix4::Transpose(result);
	result.m[3][0] = Vector3::Dot(xAxis,negpos);
	result.m[3][1] = Vector3::Dot(yAxis,negpos);
	result.m[3][2] = Vector3::Dot(zAxis,negpos);
	result.m[3][3] = 1;
	return result;
}

Matrix4 Camera::GetProjectionMatrix(float fov, float aspect, float nearClip, float farClip)
{
	Matrix4 result;
	float tanTheta = tanf(fov / 2);
	float x, y, width, height;
	x = 1.0f / tanTheta;
	y = 1.0f / tanTheta * aspect;
	width = 1/(farClip - nearClip)*farClip;
	height = -nearClip / (farClip - nearClip)*farClip;
	result =
	{
		x,0,0,0,
		0,y,0,0,
		0,0,width,1,
		0,0,height,0
	};
	return result;
}

Vector3 Camera::GetPosition()
{
	return pos;
}

Vector3 Camera::GetTarget()
{
	return target;
}

Vector3 Camera::GetForward()
{
	Vector3 result = Vector3::Normalize(target - Vector3(pos.x,0,pos.z));
	return result;
}

void Camera::ScreenShake()
{
	if (!isShake)
	{
		isShake = true;
		shakeTime = MAX_SHAKE_TIME;
	}
	else
	{
		shakeTime = MAX_SHAKE_TIME;
	}
}

bool Camera::IsShake()
{
	return isShake;
}
