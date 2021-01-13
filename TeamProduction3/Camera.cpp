#include "Camera.h"
#include <stdio.h>

Camera::Camera()
	:
	keyboard(nullptr),
	position({ 0,0,0 }),
	direction({ 0,0,1 }),
	up({ 0,1,0 }),
	yaw(0),
	pitch(0),
	turnSpeed(1),
	moveSpeed(1)
{
}

void Camera::Initialize()
{
	position = { 0,0,0 };
	direction = { 0,0,1 };
	up = { 0,1,0 };
}

void Camera::Update()
{
	DirectX::XMFLOAT3 move = { 0,0,0 };

	if (keyboard->CheakHitKey(Key::W))move.z -= 10.0f;
	if (keyboard->CheakHitKey(Key::S))move.z += 10.0f;
	if (keyboard->CheakHitKey(Key::D))move.x -= 10.0f;
	if (keyboard->CheakHitKey(Key::A))move.x += 10.0f;
	if (keyboard->CheakHitKey(Key::E))position.y += 10.0f;
	if (keyboard->CheakHitKey(Key::Q))position.y -= 10.0f;

	if (keyboard->CheakHitKey(Key::UP))pitch += 0.05f;
	if (keyboard->CheakHitKey(Key::DOWN))pitch -= 0.05f;
	if (keyboard->CheakHitKey(Key::RIGHT))yaw += 0.05f;
	if (keyboard->CheakHitKey(Key::LEFT))yaw -= 0.05f;

	pitch = min(pitch, DirectX::XM_PIDIV4);
	pitch = max(-DirectX::XM_PIDIV4, pitch);

	float x = move.x * -cosf(yaw) - move.z * sinf(yaw);
	float z = move.x * sinf(yaw) - move.z * cosf(yaw);
	position.x += x * moveSpeed;
	position.z += z * moveSpeed;

	float r = cosf(pitch);
	direction.x = r * sinf(yaw);
	direction.y = sinf(pitch);
	direction.z = r * cosf(yaw);
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&direction), DirectX::XMLoadFloat3(&up));
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(float fov, float aspect, float nearClip, float farClip)
{
	return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspect, nearClip, farClip);
}

void Camera::SetPosition(DirectX::XMFLOAT3 pos)
{
	position = pos;
}

void Camera::SetTurnSpeed(float value)
{
	turnSpeed = value;
}

void Camera::SetMoveSpeed(float value)
{
	moveSpeed = value;
}

void Camera::SetKeyboard(Keyboard * pKeyboard)
{
	keyboard = pKeyboard;
}
