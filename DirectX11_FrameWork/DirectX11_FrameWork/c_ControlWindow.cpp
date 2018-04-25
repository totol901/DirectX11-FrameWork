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

	//메인 윈도우 클래스 속성(attribute) setup
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

	//메인 윈도우 클래스 등록
	if (!RegisterClassEx(&wc))
	{
		assert(0 && TEXT("윈도우 클래스 레지스터 초기화 오류"));
		return false;
	}

	// 전체 창 모드를 위해 Display세팅 받아옴   
	ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings);

	//Display세팅을 이용하여 스크린 크기 구해줌 
	iScreenWidth = dmScreenSettings.dmPelsWidth;
	iScreenHeight = dmScreenSettings.dmPelsHeight;

	//전체 창 모드 or 창모드 
	if (eWindowModeStyle == E_WINDOW_MODE_STYLE::E_WINDOW_FULL_SCREEN
		|| eWindowModeStyle == E_WINDOW_MODE_STYLE::E_FULL_SCREEN
		)
	{
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Display세팅 변경 
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//윈도우 포지션 바꿔줌   
		posX = posY = 0;

		//윈도우 만들어 줌
		m_hContorlWindowHandle = CreateWindowEx(WS_EX_APPWINDOW, wS_ApplicationName.c_str(),
			wS_ApplicationName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, iScreenWidth, iScreenHeight, NULL, NULL, hInstance, NULL);
	}
	//창모드
	else
	{
		// 창모드 800x600   
		iScreenWidth = 800;
		iScreenHeight = 600;

		// 스크린 중앙에 위치시킴 
		posX = (GetSystemMetrics(SM_CXSCREEN) - iScreenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - iScreenHeight) / 2;

		// 윈도우 만든다.
		m_hContorlWindowHandle = CreateWindowEx(WS_EX_APPWINDOW, wS_ApplicationName.c_str(),
			wS_ApplicationName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU,
			posX, posY, iScreenWidth, iScreenHeight, NULL, NULL, hInstance, NULL);
	}

	m_Hdc = GetDC(m_hContorlWindowHandle);

	// 윈도우창 보여주고 Focus 해줌  
	ShowWindow(m_hContorlWindowHandle, SW_SHOW);
	//WM_PAINT 메시지 발생 및 클라이언트 갱신
	UpdateWindow(m_hContorlWindowHandle);
	SetForegroundWindow(m_hContorlWindowHandle);
	SetFocus(m_hContorlWindowHandle);

	// 커서 숨김
	ShowCursor(false);

	return true;
}

//------------------------------------------------------------------------------------
void GameEngine::c_ControlWindow::DestroyControlWindow()
{
	// 커서 보여줌
	ShowCursor(true);

	// 디스플레이 상태 원래대로  
	if (m_eWindowModeStyle == E_WINDOW_MODE_STYLE::E_WINDOW_FULL_SCREEN
		|| m_eWindowModeStyle == E_WINDOW_MODE_STYLE::E_FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// DC 릴리즈
	ReleaseDC(m_hContorlWindowHandle, m_Hdc);
	
	// 어플리케이션 인스턴스 제거
	UnregisterClass(m_wS_ApplicationName.c_str(), m_hInstance);

	// 윈도우 파괴  
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
		//키보드가 눌렸는지 채크 
		case WM_KEYDOWN:
		{
			//키가 눌렸다면 처리
			InputManager::GetSingleton()->KeyDown((unsigned int)wparam);
			return 0;
		}
		//키보드가 때졋는지 채크   
		case WM_KEYUP:
		{
			//키가 때졋다면 처리
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
