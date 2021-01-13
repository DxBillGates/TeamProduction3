#include "keyboard.h"

Keyboard::Keyboard(HWND hwnd, HINSTANCE hInstance)
{
	HRESULT result;
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&input, nullptr);
	result = input->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	for (int i = 0; i < sizeof(checkOldKey) / sizeof(checkOldKey[0]); i++)
	{
		checkOldKey[i] = false;
	}
}

Keyboard::~Keyboard()
{
	keyboard->Release();
	keyboard = nullptr;
	input->Release();
	input = nullptr;
}

void Keyboard::Update()
{
	HRESULT result;
	keyboard->Acquire();
	for (int i = 0; i < sizeof(checkNowKey) / sizeof(checkNowKey[0]); i++)
	{
		if (checkNowKey[i])
		{
			checkOldKey[i] = true;
		}
		else
		{
			checkOldKey[i] = false;
		}
	}
	result = keyboard->GetDeviceState(sizeof(checkNowKey), checkNowKey);
}

bool Keyboard::KeyPressTrigger(Key key)
{
	if (!checkOldKey[(UINT)key])
	{
		if (checkNowKey[(UINT)key])
		{
			return true;
		}
	}
	return false;
}

bool Keyboard::KeyReleaseTrigger(Key key)
{
	if (checkOldKey[(UINT)key])
	{
		if (!checkNowKey[(UINT)key])
		{
			return true;
		}
	}
	return false;
}

bool Keyboard::CheakHitKey(Key key)
{
	if (checkNowKey[(UINT)key])
	{
		return true;
	}
	return false;
}

bool Keyboard::CheakHitKeyAll()
{
	for (int i = 0; i < sizeof(checkOldKey) / sizeof(checkOldKey[0]); i++)
	{
		if (checkOldKey[i])
		{
			return true;
		}
	}
	return false;
}

