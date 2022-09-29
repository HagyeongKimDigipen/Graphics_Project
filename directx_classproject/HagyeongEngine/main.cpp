/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#include "Win32OS.h"
#include "window.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	Window window;
	window.CreateAndShowWindow(L"Hello", 800, 600);
	while(!window.ShouldQuit())
	{
		window.update();
		window.renderFrame();
		window.PollEvents();
		// update logic
		// draw stuff
	}
	
	return 0;
}

