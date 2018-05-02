#include "stdafx.h"
#include "c_System.h"
#include "GlobalManager.h"

using namespace GameEngine;

//------------------------------------------------------------------------------------
c_System::c_System()
	:m_pGraphics(nullptr)
{
}

//------------------------------------------------------------------------------------
c_System::~c_System()
{ 
}

//------------------------------------------------------------------------------------
bool c_System::Initialize(HINSTANCE hInstance, const wstring wS_ApplicationName)
{
	// 함수에 높이와 너비를 전달하기 전에 변수를 0으로 초기화한다. 
	int screenWidth, screenHeight = 0; 
	bool result = false;

	// 윈도우 컨트롤 클래스 이용하여 윈도우 초기화 
	GlobalManager::GetContorlWindow().CreateControlWindow(hInstance,
		wS_ApplicationName, E_WINDOW_MODE_STYLE::E_WINDOW,
		screenWidth, screenHeight);
	
	// input매니저 싱글턴 객체를 생성합니다.
	if(!InputManager::GetSingleton())
	{ 
		MessageBox(0, L"InputManager 생성 오류", 0, 0);
		
		return false; 
	}
	// Input 객체를 초기화합니다. 
	InputManager::GetSingleton()->Initialize();

	// graphics 객체를 생성합니다. 이 객체는 이 어플리케이션의 모든 그래픽 요소를 그리는 일을 합니다. 
	m_pGraphics = new c_Grapic;
	if(!m_pGraphics)
	{ 
		MessageBox(0, L"m_pGraphics 동적할당 오류", 0, 0);
		return false; 
	}
	// graphics 객체를 초기화합니다. 
	result = m_pGraphics->Initialize(screenWidth, screenHeight);
	if(!result) 
	{
		MessageBox(0, L"m_pGraphics 초기화 오류", 0, 0);
		return false; 
	} 
	
	return true;
}

//------------------------------------------------------------------------------------
void c_System::Shutdown()
{
	// 그래픽 obj 릴리즈  
	if (m_pGraphics)
	{
		m_pGraphics->Shutdown();
		SAFE_DELETE(m_pGraphics);
	}

	// 인풋 싱글턴 릴리즈  
	if (InputManager::GetSingleton())
	{
		InputManager::GetSingleton()->DestroySingleton();
	}

	//윈도우 컨트롤 클래스 이용하여 윈도우 종료
	GlobalManager::GetContorlWindow().DestroyControlWindow();

	return;
}

//------------------------------------------------------------------------------------
void c_System::Run()
{
	MSG msg;
	bool done, result = false;
	ZeroMemory(&msg, sizeof(MSG)); 

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

//------------------------------------------------------------------------------------
bool c_System::Frame()
{
	bool result = false;
	//종료키 눌렀나 채크  
	if (InputManager::GetSingleton()->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//그래픽 obj에서 프래임 돌려줌
	result = m_pGraphics->Frame();
	//채크하여 종료 판별
	if (!result)
	{
		return false;
	}

	return true;
}
