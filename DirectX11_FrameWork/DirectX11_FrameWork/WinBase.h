#pragma once

namespace GameEngine
{
	/** ������ Main �Լ� **/
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pS_cmdline, int iCmdshow);
	/** ������ PROC �Լ� **/
	LRESULT CALLBACK WndProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
};
