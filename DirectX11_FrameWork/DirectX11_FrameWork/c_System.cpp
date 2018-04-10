#include "stdafx.h"
#include "c_System.h"

namespace
{
	c_System* g_System = NULL; //proc 함수 접근을 위한 시스템 클래스 포인터
};

c_System::c_System()
	:m_Input(NULL),
	m_Graphics(NULL)
{
	g_System = this;
}

c_System::~c_System()
{ 
}

bool c_System::Initialize()
{
	int screenWidth, screenHeight; 
	bool result; 
	
	// 함수에 높이와 너비를 전달하기 전에 변수를 0으로 초기화한다. 
	screenWidth = 0; screenHeight = 0; 

	// 윈도우즈 api를 사용하여 초기화한다. 
	InitializeWindows(screenWidth, screenHeight); 

	// input 객체를 생성합니다. 이 객체는 유저로부터 들어오는 키보드 입력을 처리하기 이해 사용합니다. 
	m_Input = new c_Input; 
	if(!m_Input) 
	{ 
		return false; 
	}
	// Input 객체를 초기화합니다. 
	m_Input->Initialize(); 

	// graphics 객체를 생성합니다. 이 객체는 이 어플리케이션의 모든 그래픽 요소를 그리는 일을 합니다. 
	m_Graphics = new c_Grapic; 
	if(!m_Graphics) 
	{ 
		return false; 
	}
	// graphics 객체를 초기화합니다. 
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd); 
	if(!result) 
	{ 
		return false; 
	} 
	
	return true;
}

void c_System::Shutdown()
{
	// 그래픽 obj 릴리즈  
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		SAFE_DELETE(m_Graphics);
	}

	// 인풋 obj 릴리즈  
	if (m_Input)
	{
		SAFE_DELETE(m_Input);
	}

	//윈도우 종료
	ShutdownWindows();

	return;
}

void c_System::Run()
{
	MSG msg;
	bool done, result;
	// 메시지 0으로 
	ZeroMemory(&msg, sizeof(MSG));
	// 루프를 위한 변수  
	done = false;

	while (!done)
	{
		//윈도우 메시지 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//종료 메시지가 들어오면 종료   
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// 아닌경우 프래임 돌려줌   
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

void c_System::SetWindowedMode()
{
	e_windowStyle = E_WINDOW_STYLE::E_WINDOW;
	SetWindowLong(m_hwnd, GWL_STYLE, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	ShowWindow(m_hwnd, SW_SHOW);
}

void c_System::SetWindowedFullScreenMode()
{
	e_windowStyle = E_WINDOW_STYLE::E_WINDOW_FULL_SCREEN;
	SetWindowLong(m_hwnd, GWL_STYLE, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP);
	ShowWindow(m_hwnd, SW_SHOW);
}

bool c_System::Frame()
{
	bool result;
	//종료키 눌렀나 채크  
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//그래픽 obj에서 프래임 돌려줌
	result = m_Graphics->Frame();
	//채크하여 종료 판별
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK c_System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		//키보드가 눌렸는지 채크 
		case WM_KEYDOWN:
		{
			//키가 눌렸다면 처리
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}
		//키보드가 때졋는지 채크   
		case WM_KEYUP:
		{
			//키가 때졋다면 처리
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}
		//다른 메시지 채크
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

bool c_System::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// 프로세스의 instance 받아옴 
	m_hinstance = GetModuleHandle(NULL);

	// 어플리케이션 이름 
	m_applicationName = L"DirectX11 Engine";

	//윈도우 클래스 setup
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;  
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	//윈도우 클래스 등록
	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// 전체 창 모드를 위해 Display세팅 받아옴   
	ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings);

	//Display세팅을 이용하여 스크린 크기 구해줌 
	screenWidth = dmScreenSettings.dmPelsWidth;
	screenHeight = dmScreenSettings.dmPelsHeight;

	//전체 창 모드 or 창모드 
	if (e_windowStyle == E_WINDOW_STYLE::E_WINDOW_FULL_SCREEN 
		|| e_windowStyle == E_WINDOW_STYLE::E_FULL_SCREEN
		)
	{
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Display세팅 변경 
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//윈도우 포지션 바꿔줌   
		posX = posY = 0;

		// 윈도우 만든다.
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName.c_str(),
			m_applicationName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	}
	//창모드
	else
	{
		// 창모드 800x600   
		screenWidth = 800;
		screenHeight = 600;

		// 스크린 중앙에 위치시킴 
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		// 윈도우 만든다.
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName.c_str(),
			m_applicationName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	}

	// 윈도우창 보여주고 Focus 해줌  
	ShowWindow(m_hwnd, SW_SHOW);
	//WM_PAINT 메시지 발생 및 클라이언트 갱신
	UpdateWindow(m_hwnd);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 커서 숨김
	ShowCursor(false);

	return true;
}

void c_System::ShutdownWindows()
{
	// 커서 보여줌
	ShowCursor(true);

	// 디스플레이 상태 원래대로  
	if (e_windowStyle == E_WINDOW_STYLE::E_WINDOW_FULL_SCREEN
		|| e_windowStyle == E_WINDOW_STYLE::E_FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// 윈도우 파괴  
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 어플리케이션 인스턴스 제거
	UnregisterClass(m_applicationName.c_str(), m_hinstance);
	m_hinstance = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
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
		// 다른 종류의 메시지 처리
		default:
		{
			return g_System->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
