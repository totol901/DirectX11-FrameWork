#include "stdafx.h"
#include "c_ControlWindow.h"
#include "InputManager.h"

using namespace GameEngine;

//------------------------------------------------------------------------------------
c_ControlWindow::c_ControlWindow()
{
}

//------------------------------------------------------------------------------------
c_ControlWindow::~c_ControlWindow()
{
}

//------------------------------------------------------------------------------------
bool GameEngine::c_ControlWindow::CreateControlWindow(HINSTANCE hInstance, wstring wS_ApplicationName, E_WINDOW_MODE_STYLE eWindowModeStyle, int &iScreenWidth, int &iScreenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	m_hInstance = hInstance;
	m_wS_ApplicationName = eWindowModeStyle;
	m_eWindowModeStyle = eWindowModeStyle;

	//���� ������ Ŭ���� �Ӽ�(attribute) setup
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = wS_ApplicationName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	//���� ������ Ŭ���� ���
	if (!RegisterClassEx(&wc))
	{
		assert(0 && TEXT("������ Ŭ���� �������� �ʱ�ȭ ����"));
		return false;
	}

	// ��ü â ��带 ���� Display���� �޾ƿ�   
	ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings);

	//Display������ �̿��Ͽ� ��ũ�� ũ�� ������ 
	iScreenWidth = dmScreenSettings.dmPelsWidth;
	iScreenHeight = dmScreenSettings.dmPelsHeight;

	//��ü â ��� or â��� 
	if (eWindowModeStyle == E_WINDOW_MODE_STYLE::E_WINDOW_FULL_SCREEN
		|| eWindowModeStyle == E_WINDOW_MODE_STYLE::E_FULL_SCREEN
		)
	{
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Display���� ���� 
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//������ ������ �ٲ���   
		posX = posY = 0;

		//������ ����� ��
		m_hContorlWindowHandle = CreateWindowEx(WS_EX_APPWINDOW, wS_ApplicationName.c_str(),
			wS_ApplicationName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, iScreenWidth, iScreenHeight, NULL, NULL, hInstance, NULL);
	}
	//â���
	else
	{
		// â��� 800x600   
		iScreenWidth = 800;
		iScreenHeight = 600;

		// ��ũ�� �߾ӿ� ��ġ��Ŵ 
		posX = (GetSystemMetrics(SM_CXSCREEN) - iScreenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - iScreenHeight) / 2;

		// ������ �����.
		m_hContorlWindowHandle = CreateWindowEx(WS_EX_APPWINDOW, wS_ApplicationName.c_str(),
			wS_ApplicationName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU,
			posX, posY, iScreenWidth, iScreenHeight, NULL, NULL, hInstance, NULL);
	}

	m_Hdc = GetDC(m_hContorlWindowHandle);

	// ������â �����ְ� Focus ����  
	ShowWindow(m_hContorlWindowHandle, SW_SHOW);
	//WM_PAINT �޽��� �߻� �� Ŭ���̾�Ʈ ����
	UpdateWindow(m_hContorlWindowHandle);
	SetForegroundWindow(m_hContorlWindowHandle);
	SetFocus(m_hContorlWindowHandle);

	// Ŀ�� ����
	ShowCursor(false);

	return true;
}

//------------------------------------------------------------------------------------
void GameEngine::c_ControlWindow::DestroyControlWindow()
{
	// Ŀ�� ������
	ShowCursor(true);

	// ���÷��� ���� �������  
	if (m_eWindowModeStyle == E_WINDOW_MODE_STYLE::E_WINDOW_FULL_SCREEN
		|| m_eWindowModeStyle == E_WINDOW_MODE_STYLE::E_FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// DC ������
	ReleaseDC(m_hContorlWindowHandle, m_Hdc);
	
	// ���ø����̼� �ν��Ͻ� ����
	UnregisterClass(m_wS_ApplicationName.c_str(), m_hInstance);

	// ������ �ı�  
	DestroyWindow(m_hContorlWindowHandle);
}

//------------------------------------------------------------------------------------
void c_ControlWindow::SetWindowedMode()
{
	m_eWindowModeStyle = E_WINDOW_MODE_STYLE::E_WINDOW;
	SetWindowLong(m_hContorlWindowHandle, GWL_STYLE, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	ShowWindow(m_hContorlWindowHandle, SW_SHOW);
}

//------------------------------------------------------------------------------------
void c_ControlWindow::SetWindowedFullScreenMode()
{
	m_eWindowModeStyle = E_WINDOW_MODE_STYLE::E_WINDOW_FULL_SCREEN;
	SetWindowLong(m_hContorlWindowHandle, GWL_STYLE, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP);
	ShowWindow(m_hContorlWindowHandle, SW_SHOW);
}

//------------------------------------------------------------------------------------
LRESULT GameEngine::c_ControlWindow::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		//Ű���尡 ���ȴ��� äũ 
		case WM_KEYDOWN:
		{
			//Ű�� ���ȴٸ� ó��
			InputManager::GetSingleton()->KeyDown((unsigned int)wparam);
			return 0;
		}
		//Ű���尡 �������� äũ   
		case WM_KEYUP:
		{
			//Ű�� �����ٸ� ó��
			InputManager::GetSingleton()->KeyUp((unsigned int)wparam);
			return 0;
		}

	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

//------------------------------------------------------------------------------------
const HWND &c_ControlWindow::GetControlWindowHandle()
{
	return m_hContorlWindowHandle;
}

//------------------------------------------------------------------------------------
const E_WINDOW_MODE_STYLE &c_ControlWindow::GetWindowModeStyle()
{
	return m_eWindowModeStyle;
}
