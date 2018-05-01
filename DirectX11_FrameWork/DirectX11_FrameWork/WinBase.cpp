#include "stdafx.h"
#include "WinBase.h"
#include "GlobalManager.h"
#include "c_System.h"

using namespace GameEngine;

//------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pS_cmdline, int iCmdshow)
{
#ifdef _DEBUG
	//디버그 모드 -> 메모리 릭 채크
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	bool result = false;

	//엔진 시스템 생성
	c_System g_cSystem;

	//엔진 시스템 초기화
	if (g_cSystem.Initialize(hInstance, TEXT("Game Engine - Version 0.1b")))
	{
		//초기화 완료시 실행
		g_cSystem.Run();
	}

	//엔진 시스템 종료
	g_cSystem.Shutdown();

	return 0;
}

//------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//윈도우 파괴 메시지 왔나 확인   
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// 윈도우 닫기 메시지 왔나 확인
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		
	}

	// 다른 종류의 메시지 처리
	return GlobalManager::GetContorlWindow().MessageHandler(hWnd, uMsg, wParam, lParam);
}

