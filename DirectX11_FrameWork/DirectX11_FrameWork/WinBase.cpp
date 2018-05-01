#include "stdafx.h"
#include "WinBase.h"
#include "GlobalManager.h"
#include "c_System.h"

using namespace GameEngine;

//------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pS_cmdline, int iCmdshow)
{
#ifdef _DEBUG
	//����� ��� -> �޸� �� äũ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	bool result = false;

	//���� �ý��� ����
	c_System g_cSystem;

	//���� �ý��� �ʱ�ȭ
	if (g_cSystem.Initialize(hInstance, TEXT("Game Engine - Version 0.1b")))
	{
		//�ʱ�ȭ �Ϸ�� ����
		g_cSystem.Run();
	}

	//���� �ý��� ����
	g_cSystem.Shutdown();

	return 0;
}

//------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//������ �ı� �޽��� �Գ� Ȯ��   
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// ������ �ݱ� �޽��� �Գ� Ȯ��
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		
	}

	// �ٸ� ������ �޽��� ó��
	return GlobalManager::GetContorlWindow().MessageHandler(hWnd, uMsg, wParam, lParam);
}

