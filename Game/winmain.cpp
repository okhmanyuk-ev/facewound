
#include "winmain.h"

CCore core;








int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	core.Init(hInstance, lpCmdLine);

	core.Run();
	
	return 0;

}