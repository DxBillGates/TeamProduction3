#pragma once
#include<Windows.h>
class Window
{
private:
	HWND mHwnd;
	WNDCLASSEXA mWndClass;
	MSG mMsg;
	const char* mClassName;
	int mWndWidth;
	int mWndHeight;
public:
	Window(const char* className,int wndWidth,int wndHeight);
	~Window();
	HWND GetHwnd();
	HINSTANCE GetHinstance();
	int GetWndWidth();
	int GetWndHeight();
	float GetAspect();
	void PreviewWindow();
	bool ProcessMessage();
};

