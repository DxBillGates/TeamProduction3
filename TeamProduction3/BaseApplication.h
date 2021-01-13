#pragma once
#include"Xinput.h"
class BaseApplication
{
public:
	virtual void LoadAsset() = 0;
	virtual bool Initialize() = 0;
	virtual bool Run() = 0;
	virtual bool Update() = 0;
	virtual void Draw() = 0;
public:
	BaseApplication() {}
	virtual ~BaseApplication() {}
};

