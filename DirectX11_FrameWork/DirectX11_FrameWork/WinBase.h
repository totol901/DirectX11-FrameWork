#pragma once

namespace GameEngine
{
	/** 윈도우 Main 함수 **/
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pS_cmdline, int iCmdshow);
	/** 윈도우 PROC 함수 **/
	LRESULT CALLBACK WndProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
};
