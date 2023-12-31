#include "PreCompiledHeaders.h"
#include "Application.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
	App theApp = App();
    if (!theApp.Initialize())
        return 0;

    return theApp.Run();
}

LRESULT CALLBACK WindowProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return App::GetApp()->ProcessMessage(hwnd, msg, wParam, lParam);
}
