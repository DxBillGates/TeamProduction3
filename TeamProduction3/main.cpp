#include"Application.h"
#include<Windows.h>
//DEBUG_MODE�Ȃ�R���\�[�����\���ł���main�֐����G���g���[�|�C���g
#ifdef _DEBUG
#include <crtdbg.h>
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
//RELEASE_MODE�Ȃ�WinMain�֐����G���g���[�|�C���g
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