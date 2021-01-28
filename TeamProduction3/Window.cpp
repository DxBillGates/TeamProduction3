#include "Window.h"

LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

Window::Window(const char * className, int wndWidth, int wndHeight) :
	mWndClass({}), mMsg({}), mClassName(className), mWndWidth(wndWidth), mWndHeight(wndHeight)
{
	// ウィンドウの設定
	mWndClass.cbSize = sizeof(WNDCLASSEX);
	mWndClass.lpfnWndProc = (WNDPROC)WinProc;
	mWndClass.lpszClassName = mClassName;
	mWndClass.hInstance = GetModuleHandle(nullptr);
	mWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	// Windowの登録
	RegisterClassEx(&mWndClass);

	// Windowの生成
	RECT rect = { 0,0,mWndWidth,mWndHeight };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	mHwnd = CreateWindow(mWndClass.lpszClassName,
		mWndClass.lpszClassName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr,
		nullptr,
		mWndClass.hInstance,
		nullptr);

}

Window::~Window()
{
	UnregisterClass(mWndClass.lpszClassName, mWndClass.hInstance);
}

HWND Window::GetHwnd()
{
	return mHwnd;
}

HINSTANCE Window::GetHinstance()
{
	return mWndClass.hInstance;
}

int Window::GetWndWidth()
{
	return mWndWidth;
}

int Window::GetWndHeight()
{
	return mWndHeight;
}

float Window::GetAspect()
{
	return (float)mWndWidth / (float)mWndHeight;
}

void Window::PreviewWindow()
{
	ShowWindow(mHwnd, SW_SHOW);
}

bool Window::ProcessMessage()
{
	if (PeekMessage(&mMsg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&mMsg);
		DispatchMessage(&mMsg);
	}
	if (mMsg.message == WM_QUIT)
	{
		return false;
	}
	return true;
}
