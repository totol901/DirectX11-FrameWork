#pragma once

namespace GameEngine
{
	/** ������ Main �Լ� **/
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pS_cmdline, int iCmdshow);
	/** ������ PROC �Լ� **/
	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
