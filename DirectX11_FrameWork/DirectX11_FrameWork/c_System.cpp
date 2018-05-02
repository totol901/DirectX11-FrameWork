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
	// �Լ��� ���̿� �ʺ� �����ϱ� ���� ������ 0���� �ʱ�ȭ�Ѵ�. 
	int screenWidth, screenHeight = 0; 
	bool result = false;

	// ������ ��Ʈ�� Ŭ���� �̿��Ͽ� ������ �ʱ�ȭ 
	GlobalManager::GetContorlWindow().CreateControlWindow(hInstance,
		wS_ApplicationName, E_WINDOW_MODE_STYLE::E_WINDOW,
		screenWidth, screenHeight);
	
	// input�Ŵ��� �̱��� ��ü�� �����մϴ�.
	if(!InputManager::GetSingleton())
	{ 
		MessageBox(0, L"InputManager ���� ����", 0, 0);
		
		return false; 
	}
	// Input ��ü�� �ʱ�ȭ�մϴ�. 
	InputManager::GetSingleton()->Initialize();

	// graphics ��ü�� �����մϴ�. �� ��ü�� �� ���ø����̼��� ��� �׷��� ��Ҹ� �׸��� ���� �մϴ�. 
	m_pGraphics = new c_Grapic;
	if(!m_pGraphics)
	{ 
		MessageBox(0, L"m_pGraphics �����Ҵ� ����", 0, 0);
		return false; 
	}
	// graphics ��ü�� �ʱ�ȭ�մϴ�. 
	result = m_pGraphics->Initialize(screenWidth, screenHeight);
	if(!result) 
	{
		MessageBox(0, L"m_pGraphics �ʱ�ȭ ����", 0, 0);
		return false; 
	} 
	
	return true;
}

//------------------------------------------------------------------------------------
void c_System::Shutdown()
{
	// �׷��� obj ������  
	if (m_pGraphics)
	{
		m_pGraphics->Shutdown();
		SAFE_DELETE(m_pGraphics);
	}

	// ��ǲ �̱��� ������  
	if (InputManager::GetSingleton())
	{
		InputManager::GetSingleton()->DestroySingleton();
	}

	//������ ��Ʈ�� Ŭ���� �̿��Ͽ� ������ ����
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

//------------------------------------------------------------------------------------
bool c_System::Frame()
{
	bool result = false;
	//����Ű ������ äũ  
	if (InputManager::GetSingleton()->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//�׷��� obj���� ������ ������
	result = m_pGraphics->Frame();
	//äũ�Ͽ� ���� �Ǻ�
	if (!result)
	{
		return false;
	}

	return true;
}
