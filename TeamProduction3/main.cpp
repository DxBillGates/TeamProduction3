#include"Application.h"
#include<Windows.h>
//DEBUG_MODEならコンソールも表示できるmain関数がエントリーポイント
#ifdef _DEBUG
#include <crtdbg.h>
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
//RELEASE_MODEならWinMain関数がエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif
	Application app;
	if (!app.Run())
	{
		return -1;
	}
	return 0;
}