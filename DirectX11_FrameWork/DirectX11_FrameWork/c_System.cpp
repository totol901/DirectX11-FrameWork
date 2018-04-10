#include "stdafx.h"
#include "c_System.h"

namespace
{
	c_System* g_System = NULL; //proc �Լ� ������ ���� �ý��� Ŭ���� ������
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
	
	// �Լ��� ���̿� �ʺ� �����ϱ� ���� ������ 0���� �ʱ�ȭ�Ѵ�. 
	screenWidth = 0; screenHeight = 0; 

	// �������� api�� ����Ͽ� �ʱ�ȭ�Ѵ�. 
	InitializeWindows(screenWidth, screenHeight); 

	// input ��ü�� �����մϴ�. �� ��ü�� �����κ��� ������ Ű���� �Է��� ó���ϱ� ���� ����մϴ�. 
	m_Input = new c_Input; 
	if(!m_Input) 
	{ 
		return false; 
	}
	// Input ��ü�� �ʱ�ȭ�մϴ�. 
	m_Input->Initialize(); 

	// graphics ��ü�� �����մϴ�. �� ��ü�� �� ���ø����̼��� ��� �׷��� ��Ҹ� �׸��� ���� �մϴ�. 
	m_Graphics = new c_Grapic; 
	if(!m_Graphics) 
	{ 
		return false; 
	}
	// graphics ��ü�� �ʱ�ȭ�մϴ�. 
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd); 
	if(!result) 
	{ 
		return false; 
	} 
	
	return true;
}

void c_System::Shutdown()
{
	// �׷��� obj ������  
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		SAFE_DELETE(m_Graphics);
	}

	// ��ǲ obj ������  
	if (m_Input)
	{
		SAFE_DELETE(m_Input);
	}

	//������ ����
	ShutdownWindows();

	return;
}

void c_System::Run()
{
	MSG msg;
	bool done, result;
	// �޽��� 0���� 
	ZeroMemory(&msg, sizeof(MSG));
	// ������ ���� ����  
	done = false;

	while (!done)
	{
		//������ �޽��� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//���� �޽����� ������ ����   
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// �ƴѰ�� ������ ������   
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
	//����Ű ������ äũ  
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//�׷��� obj���� ������ ������
	result = m_Graphics->Frame();
	//äũ�Ͽ� ���� �Ǻ�
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
		//Ű���尡 ���ȴ��� äũ 
		case WM_KEYDOWN:
		{
			//Ű�� ���ȴٸ� ó��
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}
		//Ű���尡 �������� äũ   
		case WM_KEYUP:
		{
			//Ű�� �����ٸ� ó��
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}
		//�ٸ� �޽��� äũ
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

	// ���μ����� instance �޾ƿ� 
	m_hinstance = GetModuleHandle(NULL);

	// ���ø����̼� �̸� 
	m_applicationName = L"DirectX11 Engine";

	//������ Ŭ���� setup
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

	//������ Ŭ���� ���
	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// ��ü â ��带 ���� Display���� �޾ƿ�   
	ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings);

	//Display������ �̿��Ͽ� ��ũ�� ũ�� ������ 
	screenWidth = dmScreenSettings.dmPelsWidth;
	screenHeight = dmScreenSettings.dmPelsHeight;

	//��ü â ��� or â��� 
	if (e_windowStyle == E_WINDOW_STYLE::E_WINDOW_FULL_SCREEN 
		|| e_windowStyle == E_WINDOW_STYLE::E_FULL_SCREEN
		)
	{
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Display���� ���� 
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//������ ������ �ٲ���   
		posX = posY = 0;

		// ������ �����.
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName.c_str(),
			m_applicationName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	}
	//â���
	else
	{
		// â��� 800x600   
		screenWidth = 800;
		screenHeight = 600;

		// ��ũ�� �߾ӿ� ��ġ��Ŵ 
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		// ������ �����.
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName.c_str(),
			m_applicationName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	}

	// ������â �����ְ� Focus ����  
	ShowWindow(m_hwnd, SW_SHOW);
	//WM_PAINT �޽��� �߻� �� Ŭ���̾�Ʈ ����
	UpdateWindow(m_hwnd);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Ŀ�� ����
	ShowCursor(false);

	return true;
}

void c_System::ShutdownWindows()
{
	// Ŀ�� ������
	ShowCursor(true);

	// ���÷��� ���� �������  
	if (e_windowStyle == E_WINDOW_STYLE::E_WINDOW_FULL_SCREEN
		|| e_windowStyle == E_WINDOW_STYLE::E_FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// ������ �ı�  
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���ø����̼� �ν��Ͻ� ����
	UnregisterClass(m_applicationName.c_str(), m_hinstance);
	m_hinstance = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
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
		// �ٸ� ������ �޽��� ó��
		default:
		{
			return g_System->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
